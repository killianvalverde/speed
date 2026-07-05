/* speed - Generic C++ library.
 * Copyright (C) 2015-2026 Killian Valverde.
 *
 * This file is part of speed.
 *
 * speed is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * speed is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with speed. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file operations.cpp
 * @brief Core operations implementation for the system::terminal submodule.
 * @author Killian Valverde
 * @date 2017-10-18
 */

#include "../../../platform/platform.hpp"
#include "../../operations.hpp"

#include <cerrno>
#include <iostream>

#include <io.h>

#include "../../../errors/errors.hpp"

namespace speed::system::terminal {

enum class color_target : std::uint8_t
{
    FOREGROUND,
    BACKGROUND
};

static constexpr WORD foreground_bits[] =
{
    0,
    0,

    0,
    FOREGROUND_RED,
    FOREGROUND_GREEN,
    FOREGROUND_RED | FOREGROUND_GREEN,
    FOREGROUND_BLUE,
    FOREGROUND_RED | FOREGROUND_BLUE,
    FOREGROUND_GREEN | FOREGROUND_BLUE,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,

    FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_INTENSITY,
    FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
};

static constexpr WORD background_bits[] =
{
    0,
    0,

    0,
    BACKGROUND_RED,
    BACKGROUND_GREEN,
    BACKGROUND_RED | BACKGROUND_GREEN,
    BACKGROUND_BLUE,
    BACKGROUND_RED | BACKGROUND_BLUE,
    BACKGROUND_GREEN | BACKGROUND_BLUE,
    BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,

    BACKGROUND_INTENSITY,
    BACKGROUND_RED | BACKGROUND_INTENSITY,
    BACKGROUND_GREEN | BACKGROUND_INTENSITY,
    BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY,
    BACKGROUND_BLUE | BACKGROUND_INTENSITY,
    BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
    BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
    BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY
};

static_assert(
    std::size(foreground_bits) ==
    static_cast<std::size_t>(color::BRIGHT_WHITE) + 1);

static_assert(
    std::size(background_bits) ==
    static_cast<std::size_t>(color::BRIGHT_WHITE) + 1);

static WORD get_color_bits(color colr, color_target target) noexcept
{
    const auto index = static_cast<std::size_t>(colr);

    if (index >= std::size(foreground_bits))
    {
        return 0;
    }

    return target == color_target::FOREGROUND
        ? foreground_bits[index]
        : background_bits[index];
}

static WORD get_color_mask(color_target target) noexcept
{
    switch (target)
    {
    case color_target::FOREGROUND:
        return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

    case color_target::BACKGROUND:
        return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
    }

    return 0;
}

static bool get_current_attributes(
    HANDLE console_handle,
    WORD& attributes,
    std::error_code* err_code
) noexcept
{
    CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
    if (!GetConsoleScreenBufferInfo(console_handle, &console_screen_buffer_info))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        return false;
    }

    attributes = console_screen_buffer_info.wAttributes;
    return true;
}

static HANDLE get_handle_from_ostream(const void* os_address) noexcept
{
    HANDLE console_handle = nullptr;

    if (os_address == &std::cout || os_address == &std::wcout)
    {
        console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    else if (os_address == &std::cerr || os_address == &std::wcerr ||
        os_address == &std::clog || os_address == &std::wclog)
    {
        console_handle = GetStdHandle(STD_ERROR_HANDLE);
    }

    return console_handle;
}

static bool set_console_color(
    HANDLE console_handle,
    color_target target,
    color new_color,
    std::error_code* err_code
) noexcept
{
    struct cached_attributes
    {
        HANDLE handle;
        WORD attributes;
    };

    static cached_attributes foreground_cache[2];
    static cached_attributes background_cache[2];
    static std::size_t foreground_cache_size = 0;
    static std::size_t background_cache_size = 0;

    cached_attributes* cache = target == color_target::FOREGROUND
        ? foreground_cache
        : background_cache;

    std::size_t& cache_size = target == color_target::FOREGROUND
        ? foreground_cache_size
        : background_cache_size;

    DWORD unused_mode;
    WORD default_attributes;
    WORD new_attributes;
    bool found_cached_attributes = false;

    if (console_handle == nullptr || console_handle == INVALID_HANDLE_VALUE)
    {
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }
    if (!GetConsoleMode(console_handle, &unused_mode))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        return false;
    }

    for (std::size_t i = 0; i < cache_size; ++i)
    {
        if (cache[i].handle == console_handle)
        {
            default_attributes = cache[i].attributes;
            found_cached_attributes = true;
            break;
        }
    }

    const WORD color_mask = get_color_mask(target);

    if (!found_cached_attributes)
    {
        if (!get_current_attributes(console_handle, default_attributes, err_code))
        {
            return false;
        }

        default_attributes &= color_mask;

        if (cache_size < 2)
        {
            cache[cache_size].handle = console_handle;
            cache[cache_size].attributes = default_attributes;
            ++cache_size;
        }
    }

    if (!get_current_attributes(console_handle, new_attributes, err_code))
    {
        return false;
    }

    new_attributes &= static_cast<WORD>(~color_mask);

    switch (new_color)
    {
    case color::NIL:
        return true;

    case color::DEFAULT:
        new_attributes |= default_attributes;
        break;

    default:
        new_attributes |= get_color_bits(new_color, target);
        break;
    }

    if (!SetConsoleTextAttribute(console_handle, new_attributes))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        return false;
    }

    return true;
}

bool flush_input_terminal(FILE* input_stream, std::error_code* err_code) noexcept
{
    const int fd = _fileno(input_stream);

    if (fd == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    const intptr_t osfh = _get_osfhandle(fd);

    if (osfh == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    auto handle = reinterpret_cast<HANDLE>(osfh);

    if (!FlushConsoleInputBuffer(handle))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        return false;
    }

    return true;
}

bool flush_output_terminal(FILE* output_stream, std::error_code* err_code) noexcept
{
    if (fflush(output_stream) != 0)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    return true;
}

bool set_background_color(std::ostream& os, color new_color, std::error_code* err_code) noexcept
{
    return set_console_color(get_handle_from_ostream(&os), color_target::BACKGROUND,
        new_color, err_code);
}

bool set_background_color(std::wostream& wos, color new_color, std::error_code* err_code) noexcept
{
    return set_console_color(get_handle_from_ostream(&wos), color_target::BACKGROUND,
        new_color, err_code);
}

bool set_foreground_color(std::ostream& os, color new_color, std::error_code* err_code) noexcept
{
    return set_console_color(get_handle_from_ostream(&os), color_target::FOREGROUND,
        new_color, err_code);
}

bool set_foreground_color(std::wostream& wos, color new_color, std::error_code* err_code) noexcept
{
    return set_console_color(get_handle_from_ostream(&wos), color_target::FOREGROUND,
        new_color, err_code);
}

bool wait_for_key(bool flush_input, std::error_code* err_code) noexcept
{
    if (flush_input && !flush_input_terminal(stdin, err_code))
    {
        return false;
    }

    HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);
    if (input_handle == nullptr || input_handle == INVALID_HANDLE_VALUE)
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        return false;
    }

    while (true)
    {
        DWORD wait_result = WaitForSingleObject(input_handle, INFINITE);
        if (wait_result == WAIT_FAILED)
        {
            errors::assign_system_error_code(GetLastError(), err_code);
            return false;
        }

        constexpr DWORD BATCH_SIZE = 32;
        INPUT_RECORD input_records[BATCH_SIZE];
        DWORD events_read = 0;

        if (!ReadConsoleInput(input_handle, input_records, BATCH_SIZE, &events_read))
        {
            errors::assign_system_error_code(GetLastError(), err_code);
            return false;
        }

        for (DWORD i = 0; i < events_read; ++i)
        {
            if (input_records[i].EventType == KEY_EVENT &&
                input_records[i].Event.KeyEvent.bKeyDown)
            {
                return true;
            }
        }
    }
}

}

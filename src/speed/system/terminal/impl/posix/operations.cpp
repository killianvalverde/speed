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

#include <cstring>
#include <iostream>

#include <termios.h>

#include "../../../errors/errors.hpp"

namespace speed::system::terminal {

enum class color_target : std::uint8_t
{
    FOREGROUND,
    BACKGROUND
};

static constexpr const char* foreground_sequences[] = {
    nullptr, "\033[39m", "\033[30m", "\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m",
    "\033[36m", "\033[37m", "\033[90m", "\033[91m", "\033[92m", "\033[93m", "\033[94m", "\033[95m",
    "\033[96m", "\033[97m"
};

static constexpr const char* background_sequences[] = {
    nullptr, "\033[49m", "\033[40m", "\033[41m", "\033[42m", "\033[43m", "\033[44m", "\033[45m",
    "\033[46m", "\033[47m", "\033[100m", "\033[101m", "\033[102m", "\033[103m", "\033[104m",
    "\033[105m", "\033[106m", "\033[107m"
};

static_assert(
    std::size(foreground_sequences) ==
    static_cast<std::size_t>(color::BRIGHT_WHITE) + 1);

static_assert(
    std::size(background_sequences) ==
    static_cast<std::size_t>(color::BRIGHT_WHITE) + 1);

static const char* get_color_sequence(color colr, color_target target) noexcept
{
    const auto index = static_cast<std::size_t>(colr);

    if (index >= std::size(foreground_sequences))
    {
        return nullptr;
    }

    return target == color_target::FOREGROUND
        ? foreground_sequences[index]
        : background_sequences[index];
}

static int get_fd_from_ostream(const void* os_address) noexcept
{
    if (os_address == &std::cout || os_address == &std::wcout)
    {
        return STDOUT_FILENO;
    }

    if (os_address == &std::cerr || os_address == &std::wcerr ||
        os_address == &std::clog || os_address == &std::wclog)
    {
        return STDERR_FILENO;
    }

    return -1;
}

template<typename CharT, typename TraitsT>
static bool set_color(
    std::basic_ostream<CharT, TraitsT>& os,
    color_target target,
    color new_color,
    std::error_code* err_code
) noexcept
{
    const int fd = get_fd_from_ostream(&os);
    if (fd == -1)
    {
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }
    if (isatty(fd) == 0)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    const char* sequence = get_color_sequence(new_color, target);
    if (sequence == nullptr)
    {
        return true;
    }

    if constexpr (std::same_as<CharT, char>)
    {
        os << sequence;
    }
    else
    {
        while (*sequence != '\0')
        {
            os.put(static_cast<CharT>(*sequence));
            ++sequence;
        }
    }

    if (!os.good())
    {
        errors::assign_errc(std::errc::io_error, err_code);
        return false;
    }

    return true;
}

bool flush_input_terminal(FILE* input_stream, std::error_code* err_code) noexcept
{
    if (tcflush(fileno(input_stream), TCIFLUSH) == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
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

bool set_foreground_color(std::ostream& os, color new_color, std::error_code* err_code) noexcept
{
    return set_color(os, color_target::FOREGROUND, new_color, err_code);
}

bool set_foreground_color(std::wostream& wos, color new_color, std::error_code* err_code) noexcept
{
    return set_color(wos, color_target::FOREGROUND, new_color, err_code);
}

bool set_background_color(std::ostream& os, color new_color, std::error_code* err_code) noexcept
{
    return set_color(os, color_target::BACKGROUND, new_color, err_code);
}

bool set_background_color(std::wostream& wos, color new_color, std::error_code* err_code) noexcept
{
    return set_color(wos, color_target::BACKGROUND, new_color, err_code);
}

bool wait_for_key(bool flush_input_term, std::error_code* err_code) noexcept
{
    termios old_termios;
    termios new_termios;
    int stdin_fd;
    int saved_errno = 0;
    bool success = true;
    char unused_char;

    if (stdin_fd = fileno(stdin); stdin_fd == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    if (tcgetattr(stdin_fd, &old_termios) == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    new_termios = old_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);

    if (flush_input_term && !flush_input_terminal(stdin, err_code))
    {
        return false;
    }

    if (tcsetattr(stdin_fd, TCSANOW, &new_termios) == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    while (read(stdin_fd, &unused_char, 1) == -1)
    {
        if (errno != EINTR)
        {
            saved_errno = errno;
            success = false;
            break;
        }
    }

    if (tcsetattr(stdin_fd, TCSANOW, &old_termios) == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    if (!success)
    {
        errors::assign_errno_error_code(saved_errno, err_code);
        return false;
    }

    return true;
}

}

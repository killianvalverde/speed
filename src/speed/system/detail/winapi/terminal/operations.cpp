/* speed - Generic C++ library.
 * Copyright (C) 2015-2024 Killian Valverde.
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
 * @file        operations.cpp
 * @brief       terminal operations source.
 * @author      Killian Valverde
 * @date        2017/10/18
 */

#include "../../../compatibility/compatibility.hpp"
#ifdef SPEED_WINAPI

#include <conio.h>
#include <io.h>

#include "operations.hpp"

namespace speed::system::detail::winapi::terminal {

bool get_current_text_attribute(HANDLE console_handl, WORD* text_attr) noexcept
{
    CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_inf;
    
    if (console_handl == INVALID_HANDLE_VALUE ||
        !::GetConsoleScreenBufferInfo(console_handl, &console_screen_buffer_inf))
    {
        return false;
    }
    
    *text_attr = console_screen_buffer_inf.wAttributes;
    return true;
}

bool flush_input_terminal(::FILE* input_strm, std::error_code* err_code) noexcept
{
    if (!::FlushConsoleInputBuffer((HANDLE)::_get_osfhandle(_fileno(input_strm))))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}

bool flush_output_terminal(::FILE* output_strm, std::error_code* err_code) noexcept
{
    if (::fflush(output_strm))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}

bool kbhit(
        const char* mess,
        bool flush_input_term,
        std::error_code* err_code
) noexcept
{
    HANDLE input_handl;
    INPUT_RECORD input_rec;
    DWORD res;
    DWORD event_red;
    bool key_dwn = false;

    if (mess != nullptr)
    {
        std::printf("%s", mess);
    }

    if (flush_input_term)
    {
        if (!flush_input_terminal(stdin, err_code))
        {
            return false;
        }
    }
    
    if ((input_handl = ::GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int) GetLastError(), err_code);
        return false;
    }

    while (!key_dwn)
    {
        if ((res = ::WaitForSingleObject(input_handl, INFINITE)) == WAIT_FAILED)
        {
            system::errors::assign_system_error_code((int) GetLastError(), err_code);
            return false;
        }
        
        if (res == WAIT_OBJECT_0 &&
            ReadConsoleInput(input_handl, &input_rec, 1, &event_red) &&
            event_red == 1 &&
            input_rec.EventType == KEY_EVENT &&
            input_rec.Event.KeyEvent.bKeyDown)
        {
            key_dwn = true;
        }
    }
    
    return true;
}

bool set_foreground_text_attribute(
        ::FILE* terminal_strm,
        system::terminal::text_attribute text_attr
) noexcept
{
    static bool first_cll = true;
    static WORD default_text_attr;
    
    HANDLE console_handl;
    DWORD mode;
    WORD new_text_attr;
    
    console_handl = (HANDLE)::_get_osfhandle(_fileno(terminal_strm));
    if (console_handl == INVALID_HANDLE_VALUE ||
        !console_handl ||
        !::GetConsoleMode(console_handl, &mode))
    {
        return false;
    }
    
    if (first_cll)
    {
        if (!get_current_text_attribute(console_handl, &default_text_attr))
        {
            return false;
        }
        
        first_cll = false;
    }
    
    if (!get_current_text_attribute(console_handl, &new_text_attr))
    {
        return false;
    }
    
    new_text_attr &= 0xF0;

    switch (text_attr)
    {
    case system::terminal::text_attribute::DEFAULT:
        new_text_attr = default_text_attr;
        break;
    case system::terminal::text_attribute::BLACK:
        new_text_attr |= 0;
        break;
    case system::terminal::text_attribute::RED:
        new_text_attr |= FOREGROUND_RED;
        break;
    case system::terminal::text_attribute::GREEN:
        new_text_attr |= FOREGROUND_GREEN;
        break;
    case system::terminal::text_attribute::BROWN:
        new_text_attr |= FOREGROUND_RED | FOREGROUND_GREEN;
        break;
    case system::terminal::text_attribute::BLUE:
        new_text_attr |= FOREGROUND_BLUE;
        break;
    case system::terminal::text_attribute::PURPLE:
        new_text_attr |= FOREGROUND_RED | FOREGROUND_BLUE;
        break;
    case system::terminal::text_attribute::CYAN:
        new_text_attr |= FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    case system::terminal::text_attribute::LIGHT_GRAY:
        new_text_attr |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    case system::terminal::text_attribute::GRAY:
        new_text_attr |= FOREGROUND_INTENSITY;
        break;
    case system::terminal::text_attribute::LIGHT_RED:
        new_text_attr |= FOREGROUND_RED | FOREGROUND_INTENSITY;
        break;
    case system::terminal::text_attribute::LIGHT_GREEN:
        new_text_attr |= FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        break;
    case system::terminal::text_attribute::YELLOW:
        new_text_attr |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        break;
    case system::terminal::text_attribute::LIGHT_BLUE:
        new_text_attr |= FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case system::terminal::text_attribute::LIGHT_PURPLE:
        new_text_attr |= FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case system::terminal::text_attribute::LIGHT_CYAN:
        new_text_attr |= FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case system::terminal::text_attribute::WHITE:
        new_text_attr |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case system::terminal::text_attribute::NIL:
    default:
        return true;
    }

    if (!::SetConsoleTextAttribute(console_handl, new_text_attr))
    {
        return false;
    }

    return true;
}

}

#endif

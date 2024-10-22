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
 * @file        speed/system/api/winapi/terminal/operations.cpp
 * @brief       terminal operations source.
 * @author      Killian Valverde
 * @date        2017/10/18
 */

#include "../../../compatibility/compatibility.hpp"
#ifdef SPEED_WINAPI

#include <conio.h>
#include <io.h>

#include <cstring>

#include "operations.hpp"


namespace speed::system::api::winapi::terminal {


bool flush_input_terminal(::FILE* input_strm, std::error_code* err_code) noexcept
{
    if (!::FlushConsoleInputBuffer((HANDLE)::_get_osfhandle(_fileno(input_strm))))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}


bool flush_output_terminal(::FILE* output_strm, std::error_code* err_code) noexcept
{
    if (::fflush(output_strm))
    {
        assign_system_error_code((int)GetLastError(), err_code);
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
        assign_system_error_code((int) GetLastError(), err_code);
        return false;
    }

    while (!key_dwn)
    {
        if ((res = ::WaitForSingleObject(input_handl, INFINITE)) == WAIT_FAILED)
        {
            assign_system_error_code((int) GetLastError(), err_code);
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


bool set_text_attribute(
        ::FILE* terminal_strm,
        text_attribute txt_attribute
) noexcept
{
    DWORD mode;
    WORD colr;
    HANDLE console_handl = (HANDLE)_get_osfhandle(_fileno(terminal_strm));

    if (console_handl == INVALID_HANDLE_VALUE || !console_handl ||
        !GetConsoleMode(console_handl, &mode))
    {
        return false;
    }

    switch (txt_attribute)
    {
    case text_attribute::DEFAULT:
        colr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    case text_attribute::BLACK:
        colr = 0;
        break;
    case text_attribute::RED:
        colr = FOREGROUND_RED;
        break;
    case text_attribute::GREEN:
        colr = FOREGROUND_GREEN;
        break;
    case text_attribute::BROWN:
        colr = FOREGROUND_RED | FOREGROUND_GREEN;
        break;
    case text_attribute::BLUE:
        colr = FOREGROUND_BLUE;
        break;
    case text_attribute::PURPLE:
        colr = FOREGROUND_RED | FOREGROUND_BLUE;
        break;
    case text_attribute::CYAN:
        colr = FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    case text_attribute::LIGHT_GRAY:
        colr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    case text_attribute::DARK_GRAY:
        colr = FOREGROUND_INTENSITY;
        break;
    case text_attribute::LIGHT_RED:
        colr = FOREGROUND_RED | FOREGROUND_INTENSITY;
        break;
    case text_attribute::LIGHT_GREEN:
        colr = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        break;
    case text_attribute::YELLOW:
        colr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        break;
    case text_attribute::LIGHT_BLUE:
        colr = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case text_attribute::LIGHT_PURPLE:
        colr = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case text_attribute::LIGHT_CYAN:
        colr = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case text_attribute::WHITE:
        colr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case text_attribute::NIL:
    default:
        return true;
    }

    if (!SetConsoleTextAttribute(console_handl, colr))
    {
        return false;
    }

    return true;
}


}


#endif

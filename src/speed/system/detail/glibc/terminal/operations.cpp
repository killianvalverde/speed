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
#ifdef SPEED_GLIBC

#include "operations.hpp"

#include <termios.h>
#include <cstring>

namespace speed::system::detail::glibc::terminal {

bool flush_input_terminal(::FILE* input_strm, std::error_code* err_code) noexcept
{
    if (::tcflush(::fileno(input_strm), TCIFLUSH) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}

bool flush_output_terminal(::FILE* output_strm, std::error_code* err_code) noexcept
{
    if (::tcflush(::fileno(output_strm), TCOFLUSH) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
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
    ::termios oldt;
    ::termios newt;
    int stdout_fd;
    int stdin_fd;
    char buf;
    
    if ((stdout_fd = ::fileno(stdout)) == -1)
    {
        system::errors::assign_generic_error_code(errno, err_code);
        return false;
    }
    
    if ((stdin_fd = ::fileno(stdin)) == -1)
    {
        system::errors::assign_generic_error_code(errno, err_code);
        return false;
    }
    
    if (mess != nullptr)
    {
        if (::write(stdout_fd, mess, ::strlen(mess) * sizeof(char)) == -1)
        {
            system::errors::assign_system_error_code(errno, err_code);
            return false;
        }
    }
    
    if (::tcgetattr(stdin_fd, &oldt) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    
    if (flush_input_term)
    {
        if (!flush_input_terminal(stdin, err_code))
        {
            return false;
        }
    }
    
    if (::tcsetattr(stdin_fd, TCSANOW, &newt) == -1 ||
        ::read(stdin_fd, &buf, 1) == -1 ||
        ::tcsetattr(stdin_fd, TCSANOW, &oldt) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}

bool set_foreground_text_attribute(
        ::FILE* terminal_strm,
        system::terminal::text_attribute text_attr
) noexcept
{
    int res = 0;
    
    switch (text_attr)
    {
    case system::terminal::text_attribute::DEFAULT:
        res = ::fprintf(terminal_strm, "\033[0m");
        break;
    case system::terminal::text_attribute::BLACK:
        res = ::fprintf(terminal_strm, "\033[0;30m");
        break;
    case system::terminal::text_attribute::RED:
        res = ::fprintf(terminal_strm, "\033[0;31m");
        break;
    case system::terminal::text_attribute::GREEN:
        res = ::fprintf(terminal_strm, "\033[0;32m");
        break;
    case system::terminal::text_attribute::BROWN:
        res = ::fprintf(terminal_strm, "\033[0;33m");
        break;
    case system::terminal::text_attribute::BLUE:
        res = ::fprintf(terminal_strm, "\033[0;34m");
        break;
    case system::terminal::text_attribute::PURPLE:
        res = ::fprintf(terminal_strm, "\033[0;35m");
        break;
    case system::terminal::text_attribute::CYAN:
        res = ::fprintf(terminal_strm, "\033[0;36m");
        break;
    case system::terminal::text_attribute::LIGHT_GRAY:
        res = ::fprintf(terminal_strm, "\033[0;37m");
        break;
    case system::terminal::text_attribute::GRAY:
        res = ::fprintf(terminal_strm, "\033[1;30m");
        break;
    case system::terminal::text_attribute::LIGHT_RED:
        res = ::fprintf(terminal_strm, "\033[1;31m");
        break;
    case system::terminal::text_attribute::LIGHT_GREEN:
        res = ::fprintf(terminal_strm, "\033[1;32m");
        break;
    case system::terminal::text_attribute::YELLOW:
        res = ::fprintf(terminal_strm, "\033[1;33m");
        break;
    case system::terminal::text_attribute::LIGHT_BLUE:
        res = ::fprintf(terminal_strm, "\033[1;34m");
        break;
    case system::terminal::text_attribute::LIGHT_PURPLE:
        res = ::fprintf(terminal_strm, "\033[1;35m");
        break;
    case system::terminal::text_attribute::LIGHT_CYAN:
        res = ::fprintf(terminal_strm, "\033[1;36m");
        break;
    case system::terminal::text_attribute::WHITE:
        res = ::fprintf(terminal_strm, "\033[1;37m");
        break;
    case system::terminal::text_attribute::NIL:
    default:
        break;
    }
    
    return (res >= 0);
}

}

#endif

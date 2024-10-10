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
 * @file        speed/system/api/glibc/terminal/operations.cpp
 * @brief       terminal operations source.
 * @author      Killian Valverde
 * @date        2017/10/18
 */

#include "../../../compatibility/compatibility.hpp"
#ifdef SPEED_GLIBC

#include <termios.h>
#include <cstring>

#include "operations.hpp"


namespace speed::system::api::glibc::terminal {


bool flush_input_terminal(int fd, std::error_code* err_code) noexcept
{
    if (::tcflush(fd, TCIFLUSH) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}


bool flush_output_terminal(int fd, std::error_code* err_code) noexcept
{
    if (::tcflush(fd, TCOFLUSH) == -1)
    {
        assign_system_error_code(errno, err_code);
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
        assign_generic_error_code(errno, err_code);
        return false;
    }
    
    if ((stdin_fd = ::fileno(stdin)) == -1)
    {
        assign_generic_error_code(errno, err_code);
        return false;
    }
    
    if (mess != nullptr)
    {
        if (::write(stdout_fd, mess, ::strlen(mess) * sizeof(char)) == -1)
        {
            assign_system_error_code(errno, err_code);
            return false;
        }
    }
    
    if (::tcgetattr(stdin_fd, &oldt) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    
    if (flush_input_term)
    {
        if (!flush_input_terminal(stdin_fd, err_code))
        {
            return false;
        }
    }
    
    if (::tcsetattr(stdin_fd, TCSANOW, &newt) == -1 ||
        ::read(stdin_fd, &buf, 1) == -1 ||
        ::tcsetattr(stdin_fd, TCSANOW, &oldt) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}


bool set_text_attribute(
        ::FILE* strm,
        text_attribute txt_attribute
) noexcept
{
    int res = 0;
    
    switch (txt_attribute)
    {
    case text_attribute::DEFAULT:
        res = ::fprintf(strm, "\033[0m");
        break;
    case text_attribute::BLACK:
        res = ::fprintf(strm, "\033[0;30m");
        break;
    case text_attribute::RED:
        res = ::fprintf(strm, "\033[0;31m");
        break;
    case text_attribute::GREEN:
        res = ::fprintf(strm, "\033[0;32m");
        break;
    case text_attribute::BROWN:
        res = ::fprintf(strm, "\033[0;33m");
        break;
    case text_attribute::BLUE:
        res = ::fprintf(strm, "\033[0;34m");
        break;
    case text_attribute::PURPLE:
        res = ::fprintf(strm, "\033[0;35m");
        break;
    case text_attribute::CYAN:
        res = ::fprintf(strm, "\033[0;36m");
        break;
    case text_attribute::LIGHT_GRAY:
        res = ::fprintf(strm, "\033[0;37m");
        break;
    case text_attribute::DARK_GRAY:
        res = ::fprintf(strm, "\033[1;30m");
        break;
    case text_attribute::LIGHT_RED:
        res = ::fprintf(strm, "\033[1;31m");
        break;
    case text_attribute::LIGHT_GREEN:
        res = ::fprintf(strm, "\033[1;32m");
        break;
    case text_attribute::YELLOW:
        res = ::fprintf(strm, "\033[1;33m");
        break;
    case text_attribute::LIGHT_BLUE:
        res = ::fprintf(strm, "\033[1;34m");
        break;
    case text_attribute::LIGHT_PURPLE:
        res = ::fprintf(strm, "\033[1;35m");
        break;
    case text_attribute::LIGHT_CYAN:
        res = ::fprintf(strm, "\033[1;36m");
        break;
    case text_attribute::WHITE:
        res = ::fprintf(strm, "\033[1;37m");
        break;

    case text_attribute::NIL:
        break;
    }
    
    return (res >= 0);
}


}


#endif

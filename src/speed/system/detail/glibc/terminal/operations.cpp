/* speed - Generic C++ library.
 * Copyright (C) 2015-2025 Killian Valverde.
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

#include "../../../platform/platform.hpp"
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
        std::ostream& os,
        system::terminal::text_attribute text_attr
) noexcept
{
    bool is_stdout_terminl = (::isatty(fileno(stdout)) != 0);
    bool is_stderr_terminl = (::isatty(fileno(stderr)) != 0);
    
    if ((&os == &std::cout && !is_stdout_terminl) ||
        (&os == &std::cerr && !is_stderr_terminl))
    {
        return false;
    }
    
    try
    {
        switch (text_attr)
        {
        case system::terminal::text_attribute::DEFAULT:
            os << "\033[0m";
            break;
        case system::terminal::text_attribute::BLACK:
            os << "\033[0;30m";
            break;
        case system::terminal::text_attribute::RED:
            os << "\033[0;31m";
            break;
        case system::terminal::text_attribute::GREEN:
            os << "\033[0;32m";
            break;
        case system::terminal::text_attribute::BROWN:
            os << "\033[0;33m";
            break;
        case system::terminal::text_attribute::BLUE:
            os << "\033[0;34m";
            break;
        case system::terminal::text_attribute::PURPLE:
            os << "\033[0;35m";
            break;
        case system::terminal::text_attribute::CYAN:
            os << "\033[0;36m";
            break;
        case system::terminal::text_attribute::LIGHT_GRAY:
            os << "\033[0;37m";
            break;
        case system::terminal::text_attribute::GRAY:
            os << "\033[1;30m";
            break;
        case system::terminal::text_attribute::LIGHT_RED:
            os << "\033[1;31m";
            break;
        case system::terminal::text_attribute::LIGHT_GREEN:
            os << "\033[1;32m";
            break;
        case system::terminal::text_attribute::YELLOW:
            os << "\033[1;33m";
            break;
        case system::terminal::text_attribute::LIGHT_BLUE:
            os << "\033[1;34m";
            break;
        case system::terminal::text_attribute::LIGHT_PURPLE:
            os << "\033[1;35m";
            break;
        case system::terminal::text_attribute::LIGHT_CYAN:
            os << "\033[1;36m";
            break;
        case system::terminal::text_attribute::WHITE:
            os << "\033[1;37m";
            break;
        case system::terminal::text_attribute::NIL:
        default:
            return true;
        }
        
        return os.good();
    }
    catch (...)
    {
        return false;
    }
}

bool set_foreground_text_attribute(
        std::wostream& wos,
        system::terminal::text_attribute text_attr
) noexcept
{
    bool is_stdout_terminl = (::isatty(fileno(stdout)) != 0);
    bool is_stderr_terminl = (::isatty(fileno(stderr)) != 0);
    
    if ((&wos == &std::wcout && !is_stdout_terminl) ||
        (&wos == &std::wcerr && !is_stderr_terminl))
    {
        return false;
    }
    
    try
    {
        switch (text_attr)
        {
        case system::terminal::text_attribute::DEFAULT:
            wos << L"\033[0m";
            break;
        case system::terminal::text_attribute::BLACK:
            wos << L"\033[0;30m";
            break;
        case system::terminal::text_attribute::RED:
            wos << L"\033[0;31m";
            break;
        case system::terminal::text_attribute::GREEN:
            wos << L"\033[0;32m";
            break;
        case system::terminal::text_attribute::BROWN:
            wos << L"\033[0;33m";
            break;
        case system::terminal::text_attribute::BLUE:
            wos << L"\033[0;34m";
            break;
        case system::terminal::text_attribute::PURPLE:
            wos << L"\033[0;35m";
            break;
        case system::terminal::text_attribute::CYAN:
            wos << L"\033[0;36m";
            break;
        case system::terminal::text_attribute::LIGHT_GRAY:
            wos << L"\033[0;37m";
            break;
        case system::terminal::text_attribute::GRAY:
            wos << L"\033[1;30m";
            break;
        case system::terminal::text_attribute::LIGHT_RED:
            wos << L"\033[1;31m";
            break;
        case system::terminal::text_attribute::LIGHT_GREEN:
            wos << L"\033[1;32m";
            break;
        case system::terminal::text_attribute::YELLOW:
            wos << L"\033[1;33m";
            break;
        case system::terminal::text_attribute::LIGHT_BLUE:
            wos << L"\033[1;34m";
            break;
        case system::terminal::text_attribute::LIGHT_PURPLE:
            wos << L"\033[1;35m";
            break;
        case system::terminal::text_attribute::LIGHT_CYAN:
            wos << L"\033[1;36m";
            break;
        case system::terminal::text_attribute::WHITE:
            wos << L"\033[1;37m";
            break;
        case system::terminal::text_attribute::NIL:
        default:
            return true;
        }
        
        return wos.good();
    }
    catch (...)
    {
        return false;
    }
}

}

#endif

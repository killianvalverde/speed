/* speed - Generic C++ library.
 * Copyright (C) 2015-2023 Killian Valverde.
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
 * @file        speed/system/exception/system_exception.hpp
 * @brief       system_exception class header.
 * @author      Killian Green
 * @date        2017/10/18 - 23:06
 */

#ifndef SPEED_SYSTEM_EXCEPTION_SYSTEM_EXCEPTION_HPP
#define SPEED_SYSTEM_EXCEPTION_SYSTEM_EXCEPTION_HPP

#include "../../exception.hpp"


namespace speed::system::exception {


/**
 * @brief       Class that represents the base of system exceptions.
 */
class system_exception : public speed::exception::exception_base
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "system exception";
    }
};


/**
 * @brief       Class that represents a system not suported exception.
 */
class system_not_suported_exception : public system_exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "system not suported exception";
    }
};


}


#endif

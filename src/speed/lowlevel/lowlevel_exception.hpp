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
 * @file        speed/lowlevel/lowlevel_exception.hpp
 * @brief       lowlevel_exception main header.
 * @author      Killian Green
 * @date        2017/10/20 - 23:18
 */

#ifndef SPEED_LOWLEVEL_LOWLEVEL_EXCEPTION_HPP
#define SPEED_LOWLEVEL_LOWLEVEL_EXCEPTION_HPP

#include "../exception.hpp"


namespace speed::lowlevel {


/**
 * @brief       Base class used to throw exceptions when a lowlevel operation fails.
 */
class lowlevel_exception : public speed::exception::exception_base
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "lowlevel exception";
    }
};


/**
 * @brief       Class used to throw exceptions when a overflow error occurs.
 */
class overflow_exception : public lowlevel_exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "overflow exception";
    }
};


}


#endif

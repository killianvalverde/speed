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
 * @file        speed/lowlevel/exception.hpp
 * @brief       lowlevel exception main header.
 * @author      Killian Valverde
 * @date        2017/10/20
 */

#ifndef SPEED_LOWLEVEL_EXCEPTION_HPP
#define SPEED_LOWLEVEL_EXCEPTION_HPP

#include "../exceptions/exceptions.hpp"


namespace speed::lowlevel {


/**
 * @brief       Base class used to throw exceptions when a lowlevel operation fails.
 */
class exception : public speed::exceptions::exception_base
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "lowlevel exception";
    }
};


/**
 * @brief       Class used to throw exceptions when a overflow error occurs.
 */
class overflow_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "overflow exception";
    }
};


}


#endif

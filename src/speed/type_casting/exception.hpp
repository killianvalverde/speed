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
 * @file        exception.hpp
 * @brief       exception main header.
 * @author      Killian Valverde
 * @date        2016/08/19
 */

#ifndef SPEED_TYPE_CASTING_EXCEPTION_HPP
#define SPEED_TYPE_CASTING_EXCEPTION_HPP

#include "../exceptions/exceptions.hpp"

namespace speed::type_casting {

/**
 * @brief       Base class used to throw exceptions when a type casting fails.
 */
class exception : public speed::exceptions::exception_base
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "type casting exception";
    }
};

/**
 * @brief       Class used to throw exceptions when somme undefined error has occured.
 */
class conversion_fails_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "conversion fails exception";
    }
};

/**
 * @brief       Class used to throw exceptions when a source value cannot be interpreted as target.
 */
class invalid_syntax_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "invalid syntax exception";
    }
};

/**
 * @brief       Class used to throw exceptions when an overflow range error occur.
 */
class overflow_range_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "overflow range exception";
    }
};

/**
 * @brief       Class used to throw exceptions when an underflow range error occur.
 */
class underflow_range_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "underflow range exception";
    }
};

/**
 * @brief       Class used to throw exceptions when an underflow range error occur.
 */
class unknown_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "unknown exception";
    }
};

}

#endif

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
 * @file        speed/type_casting/error_codes.hpp
 * @brief       error_codes main header.
 * @author      Killian Valverde
 * @date        2024/05/09
 */

#ifndef SPEED_TYPE_CASTING_ERROR_CODES_HPP
#define SPEED_TYPE_CASTING_ERROR_CODES_HPP

#include <cstdint>


namespace speed::type_casting {


/**
 * @brief       Error codes used by the casting functions.
 */
enum class error_codes : std::uint8_t
{
    /** Null error code. */
    NIL = 0x0,

    /** Arithmetic error codes begin. */
    ARITHMETIC_CODES_BEGINS = 0x1,

    /** Arithmetic conversion fails. */
    ARITHMETIC_CONVERSION_FAILS = 0x1,

    /** Arithmetic invalid syntax. */
    ARITHMETIC_INVALID_SYNTAX = 0x2,

    /** Arithmetic overflow range error. */
    ARITHMETIC_OVERFLOW_RANGE = 0x3,

    /** Arithmetic underflow fange error. */
    ARITHMETIC_UNDERFLOW_RANGE = 0x4,

    /** Arithmetic error codes end. */
    ARITHMETIC_CODES_ENDS = 0x4,

    /** System error codes begin. */
    SYSTEM_CODES_BEGINS = 0x5,

    /** Filesystem error codes begin. */
    FILESYSTEM_CODES_BEGINS = 0x5,

    /** Filesystem invalid path. */
    FILESYSTEM_INVALID_PATH = 0x6,

    /** Filesystem invalid path. */
    FILESYSTEM_CODES_ENDS = 0x6,

    /** System error codes end. */
    SYSTEM_CODES_ENDS = 0x6,

    /** Other error code. */
    OTHER = 0x7,
};


}


#endif

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
 * @file        error_conditions.hpp
 * @brief       error_conditions main header.
 * @author      Killian Valverde
 * @date        2024/04/16
 */

#ifndef SPEED_TYPE_CASTING_ERROR_CONDITIONS_HPP
#define SPEED_TYPE_CASTING_ERROR_CONDITIONS_HPP

#include <system_error>

#include "../errors/errors.hpp"

namespace speed::type_casting {

/**
 * @brief       The type casting error condition enum.
 */
enum class error_conditions : std::uint8_t
{
    /** Null condition. */
    NIL = 0x0,

    /** Arithmetic error. */
    ARITHMETIC_ERROR = 0x1,

    /** System error. */
    SYSTEM_ERROR = 0x2,

    /** Other error. */
    OTHER = 0x3
};

}

namespace std {
template<> struct is_error_condition_enum<speed::type_casting::error_conditions>
        : public std::true_type
{
};
}

#endif

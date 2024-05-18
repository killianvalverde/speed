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
 * @file        speed/type_casting/error_category.cpp
 * @brief       error_category source file.
 * @author      Killian Valverde
 * @date        2024/04/16
 */

#include "error_category.hpp"
#include "error_codes.hpp"


namespace speed::type_casting {


std::error_condition error_category::default_error_condition(int val) const noexcept
{
    if (val == static_cast<int>(error_codes::NIL))
    {
        return {error_conditions::NIL};
    }
    else if (val >= static_cast<int>(error_codes::ARITHMETIC_CODES_BEGINS) &&
             val <= static_cast<int>(error_codes::ARITHMETIC_CODES_ENDS))
    {
        return {error_conditions::ARITHMETIC_ERROR};
    }
    else if (val >= static_cast<int>(error_codes::SYSTEM_CODES_BEGINS) &&
             val <= static_cast<int>(error_codes::SYSTEM_CODES_ENDS))
    {
        return {error_conditions::SYSTEM_ERROR};
    }
    else if (val == static_cast<int>(error_codes::OTHER))
    {
        return {error_conditions::OTHER};
    }
    
    return {error_conditions::NIL};
}


std::string error_category::message(int val) const
{
    switch (static_cast<error_codes>(val))
    {
        case error_codes::NIL:
            return "No error";

        case error_codes::ARITHMETIC_CONVERSION_FAILS:
            return "Conversion fails";

        case error_codes::ARITHMETIC_INVALID_SYNTAX:
            return "Invalid syntax";

        case error_codes::ARITHMETIC_OVERFLOW_RANGE:
            return "Overflow range";

        case error_codes::ARITHMETIC_UNDERFLOW_RANGE:
            return "Underflow range";

        default:
            return "Unknown error";
    }
}


/** @cond */
namespace __private {
error_category error_category_obj;
}
/** @endcond */


}

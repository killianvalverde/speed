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
 * @file        error_category.cpp
 * @brief       error_category source file.
 * @author      Killian Valverde
 * @date        2024/04/16
 */

#include "error_category.hpp"

namespace speed::type_casting {

std::error_condition error_category::default_error_condition(int val) const noexcept
{
    if (val == static_cast<int>(detail::error_code_enum::NIL))
    {
        return {errc::NIL};
    }
    else if (val >= static_cast<int>(detail::error_code_enum::ARITHMETIC_CODES_BEGINS) &&
             val <= static_cast<int>(detail::error_code_enum::ARITHMETIC_CODES_ENDS))
    {
        return {errc::ARITHMETIC_ERROR};
    }
    else if (val >= static_cast<int>(detail::error_code_enum::SYSTEM_CODES_BEGINS) &&
             val <= static_cast<int>(detail::error_code_enum::SYSTEM_CODES_ENDS))
    {
        return {errc::SYSTEM_ERROR};
    }
    else if (val >= static_cast<int>(detail::error_code_enum::OTHERS_BEGINS))
    {
        return {errc::OTHER};
    }
    
    return {errc::NIL};
}

std::string error_category::message(int val) const
{
    switch (static_cast<detail::error_code_enum>(val))
    {
        case detail::error_code_enum::NIL:
            return "No error";

        case detail::error_code_enum::ARITHMETIC_CONVERSION_FAILS:
            return "Conversion fails";

        case detail::error_code_enum::ARITHMETIC_INVALID_SYNTAX:
            return "Invalid syntax";

        case detail::error_code_enum::ARITHMETIC_OVERFLOW_RANGE:
            return "Overflow range";

        case detail::error_code_enum::ARITHMETIC_UNDERFLOW_RANGE:
            return "Underflow range";

        case detail::error_code_enum::FILESYSTEM_INVALID_PATH:
            return "Invalid path";

        case detail::error_code_enum::RANGE_ERROR:
            return "Invalid syntax";

        case detail::error_code_enum::OTHER:
            return "Invalid syntax";

        default:
            return "Unknown error";
    }
}

/** @cond */
namespace detail {
error_category error_category_obj;
}
/** @endcond */

}

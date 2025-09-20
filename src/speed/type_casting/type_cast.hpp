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
 * @file        type_cast.hpp
 * @brief       type_cast main header.
 * @author      Killian Valverde
 * @date        2016/08/19
 */

#ifndef SPEED_TYPE_CASTING_TYPE_CAST_HPP
#define SPEED_TYPE_CASTING_TYPE_CAST_HPP

#include "detail/forward_declarations.hpp"
#include "error_category.hpp"
#include "exception.hpp"
#include "try_type_cast.hpp"

namespace speed::type_casting {

/**
 * @brief       Safely casts a value from one type to another, throwing exceptions on failure.
 * @param       arg : The input value to be cast.
 * @return      The successfully converted value of type `TargetT`.
 */
template<typename TargetT, typename SourceT>
TargetT type_cast(const SourceT& arg)
{
    std::error_code err_code;
    TargetT res;
    
    if (!try_type_cast(arg, res, &err_code))
    {
        switch (static_cast<detail::error_code_value>(err_code.value()))
        {
        case detail::error_code_value::ARITHMETIC_CONVERSION_FAILS:
            throw conversion_fails_exception();

        case detail::error_code_value::ARITHMETIC_INVALID_SYNTAX:
            throw invalid_syntax_exception();

        case detail::error_code_value::ARITHMETIC_OVERFLOW_RANGE:
            throw overflow_range_exception();

        case detail::error_code_value::ARITHMETIC_UNDERFLOW_RANGE:
            throw underflow_range_exception();

        default:
            throw unknown_exception();
        }
    }

    return res;
}

/**
 * @brief       Safely casts a value from one type to another with a fallback default value.
 * @param       arg : The input value to be cast.
 * @param       default_val : The value to return if the cast fails.
 * @return      The successfully converted value of type `TargetT` or `default_val` if the
 *              cast fails.
 */
template<typename TargetT, typename SourceT, typename DefaultValueT>
TargetT type_cast(const SourceT& arg, DefaultValueT&& default_val) noexcept
{
    try
    {
        TargetT res;
        
        if (try_type_cast(arg, res))
        {
            return res;
        }
    }
    catch (...)
    {
    }

    return std::forward<DefaultValueT>(default_val);
}

}

#endif

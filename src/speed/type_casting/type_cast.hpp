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
 * @file        speed/type_casting/type_cast.hpp
 * @brief       type_cast main header.
 * @author      Killian Valverde
 * @date        2016/08/19
 */

#ifndef SPEED_TYPE_CASTING_TYPE_CAST_HPP
#define SPEED_TYPE_CASTING_TYPE_CAST_HPP

#include "error_category.hpp"
#include "exception.hpp"
#include "try_type_cast.hpp"


namespace speed::type_casting {


/**
 * @brief       Convert the source to the target.
 * @param       arg : The value to convert.
 * @return      If function was successful the new value is returned, otherwise and exception is 
 *              throw.
 */
template<typename TpTarget, typename TpSource>
TpTarget type_cast(const TpSource& arg)
{
    std::error_code err_code;
    TpTarget res;
    
    if (!try_type_cast(arg, &res, &err_code))
    {
        switch (static_cast<error_codes>(err_code.value()))
        {
            case error_codes::ARITHMETIC_CONVERSION_FAILS:
                throw conversion_fails_exception();

            case error_codes::ARITHMETIC_INVALID_SYNTAX:
                throw invalid_syntax_exception();

            case error_codes::ARITHMETIC_OVERFLOW_RANGE:
                throw overflow_range_exception();

            case error_codes::ARITHMETIC_UNDERFLOW_RANGE:
                throw underflow_range_exception();

            default:
                throw unknown_exception();
        }
    }

    return res;
}


/**
 * @brief       Convert the source to the target.
 * @param       arg : The value to convert.
 * @param       default_val : The value returned if the conversion fails.
 * @return      If function was successful the new value is returned, otherwise the default value
 *              is returned.
 */
template<typename TpTarget, typename TpSource, typename TpDefaultValue>
TpTarget type_cast(const TpSource& arg, TpDefaultValue&& default_val) noexcept
{
    try
    {
        TpTarget res;
        
        if (try_type_cast(arg, &res))
        {
            return res;
        }
    }
    catch (...)
    {
    }

    return std::forward<TpDefaultValue>(default_val);
}


}


#endif

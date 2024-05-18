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
 * @file        speed/type_casting/try_cast_c_string_to_integral_unsigned.hpp
 * @brief       try_cast_c_string_to_integral_unsigned main header.
 * @author      Killian Valverde
 * @date        2016/08/19
 */

#ifndef SPEED_TYPE_CASTING_TRY_CAST_C_STRING_TO_INTEGRAL_UNSIGNED_HPP
#define SPEED_TYPE_CASTING_TRY_CAST_C_STRING_TO_INTEGRAL_UNSIGNED_HPP

#include <cctype>
#include <limits>
#include <type_traits>

#include "../type_traits/type_traits.hpp"
#include "error_codes.hpp"


namespace speed::type_casting {


/** @cond */
namespace __private {


template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                std::is_integral<TpTarget>::value &&
                std::is_unsigned<TpTarget>::value,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    using unsigned_type = TpTarget;
    using char_type = std::remove_const_t<std::remove_pointer_t<std::decay_t<TpSource>>>;
    using string_pointer_type = const char_type*;
    
    constexpr unsigned_type max_unsigned = std::numeric_limits<unsigned_type>::max();
    
    string_pointer_type str = arg;
    unsigned_type res_bldr = 0;
    char_type sgn;
    char_type digt;
    
    if (str == nullptr)
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_CONVERSION_FAILS),
            err_code);

        return false;
    }
    
    while (isspace(*str))
    {
        ++str;
    }
    
    sgn = *str;
    if (sgn == '+')
    {
        ++str;
    }
    
    if (*str == '\0')
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_INVALID_SYNTAX),
            err_code);

        return false;
    }
    
    while ((digt = *str) != '\0')
    {
        if (!isdigit(digt))
        {
            assign_type_casting_error_code(
                static_cast<int>(error_codes::ARITHMETIC_INVALID_SYNTAX),
                err_code);

            return false;
        }
        
        digt -= '0';
        
        if ((res_bldr > max_unsigned / 10) ||
            (res_bldr == max_unsigned / 10 && digt > (max_unsigned % 10)))
        {
            assign_type_casting_error_code(
                static_cast<int>(error_codes::ARITHMETIC_OVERFLOW_RANGE),
                err_code);

            return false;
        }

        res_bldr = res_bldr * 10 + digt;
        ++str;
    }

    *res = res_bldr;
    
    return true;
}


} /* __private */
/** @endcond */


}


#endif

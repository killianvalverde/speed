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
 * @file        speed/type_casting/c_string_to_integral_signed.hpp
 * @brief       c_string_to_integral_signed main header.
 * @author      Killian Valverde
 * @date        2016/08/19 - 17:35
 */

#ifndef SPEED_TYPE_CASTING_C_STRING_TO_INTEGRAL_SIGNED_HPP
#define SPEED_TYPE_CASTING_C_STRING_TO_INTEGRAL_SIGNED_HPP

#include <cctype>
#include <limits>
#include <type_traits>

#include "../type_traits.hpp"
#include "type_casting_exception.hpp"


namespace speed::type_casting {


/**
 * @brief       Convert a C string into a integral signed type.
 * @param       arg : The value to convert.
 * @return      The C string converted into a integral signed type.
 * @throw       speed::type_casting::conversion_fails_exception : If the conversion fail an
 *              exception is thrown.
 * @throw       speed::type_casting::invalid_syntax_exception : If the string has an invalid syntax
 *              an exception is thrown.
 * @throw       speed::type_casting::overflow_range_exception : If there is an overflow in the
 *              conversion an exception is thrown.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                std::is_integral<TpTarget>::value &&
                std::is_signed<TpTarget>::value,
        TpTarget
>
type_cast(const TpSource& arg)
{
    using signed_type = TpTarget;
    using char_type = std::remove_const_t<std::remove_pointer_t<std::decay_t<TpSource>>>;
    using string_pointer_type = const char_type*;
    
    constexpr signed_type min_signed = std::numeric_limits<signed_type>::min();
    constexpr signed_type max_signed = std::numeric_limits<signed_type>::max();
    
    string_pointer_type str = arg;
    signed_type res = 0;
    char_type sgn;
    char_type digt;
    
    if (str == nullptr)
    {
        throw conversion_fails_exception();
    }
    
    while (isspace(*str))
    {
        str++;
    }
    
    sgn = *str;
    if (sgn == '-' || sgn == '+')
    {
        str++;
    }
    
    if (*str == '\0')
    {
        throw invalid_syntax_exception();
    }
    
    while ((digt = *str) != '\0')
    {
        if (!isdigit(digt))
        {
            throw invalid_syntax_exception();
        }
        
        digt -= '0';
        
        if ((res < min_signed / 10) ||
            (res == min_signed / 10 && digt > -(min_signed % 10)))
        {
            throw overflow_range_exception();
        }
        
        res = res * 10 - digt;
        str++;
    }
    
    if (sgn != '-')
    {
        if (res < -max_signed)
        {
            throw overflow_range_exception();
        }
        
        res *= -1;
    }
    
    return res;
}
    
    
}


#endif

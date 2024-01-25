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
 * @file        speed/type_casting/c_string_to_floating_point.hpp
 * @brief       c_string_to_floating_point main header.
 * @author      Killian Valverde
 * @date        2016/08/19 - 18:02
 */

#ifndef SPEED_TYPE_CASTING_C_STRING_TO_FLOATING_POINT_HPP
#define SPEED_TYPE_CASTING_C_STRING_TO_FLOATING_POINT_HPP

#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <type_traits>

#include "../type_traits.hpp"
#include "type_casting_exception.hpp"


namespace speed::type_casting {


/**
 * @brief       Convert a C string into a float.
 * @param       arg : The value to convert.
 * @return      The C string converted into a float.
 * @throw       speed::type_casting::conversion_fails_exception : If the conversion fail an
 *              exception is thrown.
 * @throw       speed::type_casting::overflow_range_exception : If there is an overflow in the
 *              conversion an exception is thrown.
 * @throw       speed::type_casting::underflow_range_exception : If there is an underflow in the
 *              conversion an exception is thrown.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                speed::type_traits::is_char<
                        std::remove_pointer_t<
                        std::decay_t<
                        typename std::remove_extent<TpSource>::type>>>::value &&
                speed::type_traits::is_float<TpTarget>::value,
        TpTarget
>
type_cast(const TpSource& arg)
{
    using floating_point_type = TpTarget;
    
    errno = 0;
    char* endptr;
    floating_point_type res = strtof(arg, &endptr);
    
    if (endptr == arg || *endptr != '\0')
    {
        throw conversion_fails_exception();
    }
    
    if (res == HUGE_VALF && errno == ERANGE)
    {
        throw overflow_range_exception();
    }
    
    if (errno == ERANGE)
    {
        throw underflow_range_exception();
    }
    
    return res;
}


/**
 * @brief       Convert a C string into a double.
 * @param       arg : The value to convert.
 * @return      The C string converted into a double.
 * @throw       speed::type_casting::conversion_fails_exception : If the conversion fail an
 *              exception is thrown.
 * @throw       speed::type_casting::overflow_range_exception : If there is an overflow in the
 *              conversion an exception is thrown.
 * @throw       speed::type_casting::underflow_range_exception : If there is an underflow in the
 *              conversion an exception is thrown.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                speed::type_traits::is_char<
                       std::remove_pointer_t<
                       std::decay_t<
                       typename std::remove_extent<TpSource>::type>>>::value &&
                speed::type_traits::is_double<TpTarget>::value,
        TpTarget
>
type_cast(const TpSource& arg)
{
    using floating_point_type = TpTarget;
    
    errno = 0;
    char* endptr;
    floating_point_type res = strtod(arg, &endptr);
    
    if (endptr == arg || *endptr != '\0')
    {
        throw conversion_fails_exception();
    }
    
    if (res == HUGE_VAL && errno == ERANGE)
    {
        throw overflow_range_exception();
    }
    
    if (errno == ERANGE)
    {
        throw underflow_range_exception();
    }
    
    return res;
}


/**
 * @brief       Convert a C string into a long double.
 * @param       arg : The value to convert.
 * @return      The C string converted into a long double.
 * @throw       speed::type_casting::conversion_fails_exception : If the conversion fail an
 *              exception is thrown.
 * @throw       speed::type_casting::overflow_range_exception : If there is an overflow in the
 *              conversion an exception is thrown.
 * @throw       speed::type_casting::underflow_range_exception : If there is an underflow in the
 *              conversion an exception is thrown.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                speed::type_traits::is_char<
                        std::remove_pointer_t<
                        std::decay_t<
                        typename std::remove_extent<TpSource>::type>>>::value &&
                speed::type_traits::is_long_double<TpTarget>::value,
        TpTarget
>
type_cast(const TpSource& arg)
{
    using floating_point_type = TpTarget;
    
    errno = 0;
    char* endptr;
    floating_point_type res = strtold(arg, &endptr);
    
    if (endptr == arg || *endptr != '\0')
    {
        throw conversion_fails_exception();
    }
    
    if (res == HUGE_VALL && errno == ERANGE)
    {
        throw overflow_range_exception();
    }
    
    if (errno == ERANGE)
    {
        throw underflow_range_exception();
    }
    
    return res;
}


/**
 * @brief       Convert a string into a float.
 * @param       arg : The value to convert.
 * @return      The string converted into a float.
 * @throw       speed::type_casting::conversion_fails_exception : If the conversion fail an
 *              exception is thrown.
 * @throw       speed::type_casting::overflow_range_exception : If there is an overflow in the
 *              conversion an exception is thrown.
 * @throw       speed::type_casting::underflow_range_exception : If there is an underflow in the
 *              conversion an exception is thrown.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                !speed::type_traits::is_char<
                        std::remove_pointer_t<
                        std::decay_t<
                        typename std::remove_extent<TpSource>::type>>>::value &&
                std::is_floating_point<TpTarget>::value,
        TpTarget
>
type_cast(const TpSource& arg)
{
    return type_cast<TpTarget>(type_cast<std::string>(arg).c_str());
}


}


#endif

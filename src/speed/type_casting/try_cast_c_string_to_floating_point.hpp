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
 * @file        speed/type_casting/try_cast_c_string_to_floating_point.hpp
 * @brief       try_cast_c_string_to_floating_point main header.
 * @author      Killian Valverde
 * @date        2016/08/19
 */

#ifndef SPEED_TYPE_CASTING_TRY_CAST_C_STRING_TO_FLOATING_POINT_HPP
#define SPEED_TYPE_CASTING_TRY_CAST_C_STRING_TO_FLOATING_POINT_HPP

#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <type_traits>

#include "../type_traits/type_traits.hpp"
#include "error_codes.hpp"


namespace speed::type_casting {


/** @cond */
namespace __private {


// TODO: The conditions for the generation are kinda weird.
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                speed::type_traits::is_char<
                        std::remove_pointer_t<
                        std::decay_t<
                        typename std::remove_extent<TpSource>::type>>>::value &&
                speed::type_traits::is_float<TpTarget>::value,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    using floating_point_type = TpTarget;
    
    errno = 0;
    char* endptr;
    floating_point_type res_bldr;

    res_bldr = strtof(arg, &endptr);
    
    if (endptr == arg || *endptr != '\0')
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_CONVERSION_FAILS),
            err_code);

        return false;
    }
    
    if (res_bldr == HUGE_VALF && errno == ERANGE)
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_OVERFLOW_RANGE),
            err_code);
            
        return false;
    }
    
    if (errno == ERANGE)
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_UNDERFLOW_RANGE),
            err_code);
            
        return false;
    }

    *res = res_bldr;
    
    return true;
}


template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                speed::type_traits::is_char<
                       std::remove_pointer_t<
                       std::decay_t<
                       typename std::remove_extent<TpSource>::type>>>::value &&
                speed::type_traits::is_double<TpTarget>::value,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    using floating_point_type = TpTarget;
    
    errno = 0;
    char* endptr;
    floating_point_type res_bldr;

    res_bldr = strtod(arg, &endptr);
    
    if (endptr == arg || *endptr != '\0')
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_CONVERSION_FAILS),
            err_code);

        return false;
    }
    
    if (res_bldr == HUGE_VAL && errno == ERANGE)
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_OVERFLOW_RANGE),
            err_code);
            
        return false;
    }
    
    if (errno == ERANGE)
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_UNDERFLOW_RANGE),
            err_code);
            
        return false;
    }

    *res = res_bldr;
    
    return true;
}


template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                speed::type_traits::is_char<
                        std::remove_pointer_t<
                        std::decay_t<
                        typename std::remove_extent<TpSource>::type>>>::value &&
                speed::type_traits::is_long_double<TpTarget>::value,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    using floating_point_type = TpTarget;
    
    errno = 0;
    char* endptr;
    floating_point_type res_bldr;

    res_bldr = strtold(arg, &endptr);
    
    if (endptr == arg || *endptr != '\0')
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_CONVERSION_FAILS),
            err_code);

        return false;
    }
    
    if (res_bldr == HUGE_VALL && errno == ERANGE)
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_OVERFLOW_RANGE),
            err_code);
            
        return false;
    }
    
    if (errno == ERANGE)
    {
        assign_type_casting_error_code(
            static_cast<int>(error_codes::ARITHMETIC_UNDERFLOW_RANGE),
            err_code);
            
        return false;
    }

    *res = res_bldr;
    
    return true;
}


} /* __private */
/** @endcond */


}


#endif

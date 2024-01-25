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
 * @file        speed/type_casting/c_string_to_basic_string.hpp
 * @brief       c_string_to_basic_string main header.
 * @author      Killian Valverde
 * @date        2016/08/19 - 15:39
 */

#ifndef SPEED_TYPE_CASTING_C_STRING_TO_BASIC_STRING_HPP
#define SPEED_TYPE_CASTING_C_STRING_TO_BASIC_STRING_HPP

#include <string>
#include <type_traits>

#include "../stringutils.hpp"
#include "../type_traits.hpp"
#include "type_casting_exception.hpp"


namespace speed::type_casting {


/**
 * @brief       Convert a C string into a std::basic_string.
 * @param       arg : The value to convert.
 * @return      The C string converted into a std::basic_string.
 * @throw       speed::type_casting::type_casting_exception : If the conversion fail an exception is
 *              thrown.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_basic_string<TpTarget>::value &&
                speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value,
        TpTarget
>
type_cast(const TpSource& arg)
{
    try
    {
        TpTarget str;
        std::size_t len = speed::stringutils::strlen(arg);
    
        str.assign(arg, arg + len);
    
        return str;
    }
    catch (...)
    {
        throw conversion_fails_exception();
    }
}


}


#endif

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
 * @file        speed/type_casting/basic_string_to_type.hpp
 * @brief       basic_string_to_type main header.
 * @author      Killian Valverde
 * @date        2016/08/23 - 20:41
 */

#ifndef SPEED_TYPE_CASTING_BASIC_STRING_TO_TYPE_HPP
#define SPEED_TYPE_CASTING_BASIC_STRING_TO_TYPE_HPP

#include <type_traits>

#include "type_casting_exception.hpp"
#include "c_string_to_basic_string.hpp"
#include "c_string_to_floating_point.hpp"
#include "c_string_to_integral_signed.hpp"
#include "c_string_to_integral_unsigned.hpp"
#include "c_string_to_path.hpp"


namespace speed::type_casting {


/**
 * @brief       Convert a std::basic_string into the target type.
 * @param       arg : The value to convert.
 * @return      The std::basic_string converted into the target.
 * @throw       speed::type_casting::type_casting_exception : If the conversion fail an exception
 *              is thrown.
 */
template<typename TpTarget, typename TpChar, typename TpCharTraits, typename TpCharAlloc>
inline TpTarget type_cast(const std::basic_string<TpChar, TpCharTraits, TpCharAlloc>& arg)
{
    return type_cast<TpTarget>(arg.c_str());
}


}


#endif


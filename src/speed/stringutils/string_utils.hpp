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
 * @file        speed/stringutils/string_utils.hpp
 * @brief       string_utils functions header.
 * @author      Killian Valverde
 * @date        2017/12/24
 */

#ifndef SPEED_STRINGUTILS_STRING_UTILS_HPP
#define SPEED_STRINGUTILS_STRING_UTILS_HPP

#include <string>
#include <type_traits>
#include <vector>

#include "../type_traits/type_traits.hpp"
#include "c_string_utils.hpp"


namespace speed::stringutils {


/**
 * @brief       Returns the length of a C string str.
 * @param       str : A C string.
 * @return      The length of the C string.
 */
template<typename TpChar>
[[nodiscard]] inline std::enable_if_t<
        speed::type_traits::is_character<TpChar>::value,
        std::size_t
>
get_string_length(const TpChar* str) noexcept
{   
    return speed::stringutils::strlen(str);
}


/**
 * @brief       Returns the length of a string.
 * @param       str : A string.
 * @return      The length of the string.
 */
template<typename TpString>
[[nodiscard]] inline typename TpString::size_type get_string_length(const TpString& str) noexcept
{
    return str.length();
}


}


#endif

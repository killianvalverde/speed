/* speed - Generic C++ library.
 * Copyright (C) 2015-2026 Killian Valverde.
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
 * @file        tstring.hpp
 * @brief       tstring header.
 * @author      Killian Valverde
 * @date        2026/03/15
*/

#ifndef SPEED_STRINGUTILS_TSTRING_HPP
#define SPEED_STRINGUTILS_TSTRING_HPP

#include <string>

#include "../type_traits/type_traits.hpp"

namespace speed::stringutils {

/** Platform-dependent string literal helper. */
#ifdef _WIN32
#define SPEED_T(x) L##x
#else
#define SPEED_T(x) x
#endif

/** Platform character type. */
using tchar_t =
#ifdef _WIN32
    wchar_t;
#else
    char;
#endif

/** Alias for `std::basic_string` using `tchar_t` as the character type. */
template<
        typename CharTraitsT = std::char_traits<tchar_t>,
        typename AllocatorT = std::allocator<tchar_t>
>
using basic_tstring = std::basic_string<tchar_t, CharTraitsT, AllocatorT>;

/** Equivalent to `basic_tstring` with `std::char_traits<tchar_t>` and `std::allocator<tchar_t>`. */
using tstring = basic_tstring<std::char_traits<tchar_t>, std::allocator<tchar_t>>;

/**
 * @brief       Selects the string whose character type matches the specified CharT.
 * @param       str1 : Reference to the first string.
 * @param       str2 : Reference to the second string.
 * @return      Reference to the string whose character type matches CharT.
 */
template<typename CharT, typename StringT1, typename StringT2>
requires (
        std::is_same_v<CharT, type_traits::character_type_of_t<StringT1>> ||
        std::is_same_v<CharT, type_traits::character_type_of_t<StringT2>>
)
constexpr auto& select_string(StringT1& str1, StringT2& str2)
{
    using char_type_1 = type_traits::character_type_of_t<StringT1>;

    if constexpr (std::is_same_v<CharT, char_type_1>)
    {
        return str1;
    }
    else
    {
        return str2;
    }
}

}

#endif
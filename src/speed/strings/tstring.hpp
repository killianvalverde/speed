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
 * @file tstring.hpp
 * @brief Platform-independent character and string type definitions.
 * @author Killian Valverde
 * @date 2026-03-15
*/

#pragma once

#include <string>

#include "../type_traits/type_traits.hpp"

namespace speed::strings {

/** Creates a string literal using the native character type of the current platform. */
#ifdef _WIN32
#define SPEED_T(x) L##x
#else
#define SPEED_T(x) x
#endif

/** Native character type of the current platform. */
using tchar_t =
#ifdef _WIN32
wchar_t;
#else
char;
#endif

/** Platform-independent string type. */
template<
    typename CharTraitsT = std::char_traits<tchar_t>,
    typename AllocatorT = std::allocator<tchar_t>>
using basic_tstring = std::basic_string<tchar_t, CharTraitsT, AllocatorT>;

/** Alias for `basic_tstring` using the default character traits and allocator. */
using tstring = basic_tstring<std::char_traits<tchar_t>, std::allocator<tchar_t>>;

/**
 * @brief Selects one of two strings based on a character type.
 *
 * Returns a reference to the string whose character type matches `CharT`.
 *
 * @param str1 The first string.
 * @param str2 The second string.
 * @return A reference to the string whose character type is `CharT`.
 */
template<typename CharT, typename StringT1, typename StringT2>
    requires (std::is_same_v<CharT, type_traits::character_type_of_t<StringT1>> ||
        std::is_same_v<CharT, type_traits::character_type_of_t<StringT2>>)
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

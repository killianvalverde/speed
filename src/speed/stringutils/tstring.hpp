/* speed - Generic C++ library.
 * Copyright (C) 2015-2025 Killian Valverde.
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

namespace speed::stringutils {

#ifdef _WIN32
#define SPD_T(x) L##x
#else
#define SPD_T(x) x
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

}

#endif
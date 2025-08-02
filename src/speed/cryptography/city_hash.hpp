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
 * @file        city_hash.hpp
 * @brief       city_hash functions header.
 * @author      Killian Valverde
 * @date        2024/11/01
 */

#ifndef SPEED_CRYPTOGRAPHY_CITY_HASH_HPP
#define SPEED_CRYPTOGRAPHY_CITY_HASH_HPP

#include <cstdint>

#include "../stringutils/stringutils.hpp"
#include "../type_traits/type_traits.hpp"

namespace speed::cryptography {

/**
 * @brief       Compute the city-hash of a specific value.
 * @param       ptr : Pointer to the value to hash.
 * @param       sz : Size of the value to hash.
 * @return      The hash value.
 */
std::uint64_t city_hash_64(const void* ptr, std::size_t sz);

/**
 * @brief       Compute the city-hash of a specific cstr.
 * @param       str : Cstr to hash.
 * @return      The hash value.
 */
template<typename StringT>
inline std::uint64_t city_hash_64(const StringT& str)
{
    using string_view_type = type_traits::string_view_of_t<StringT>;
    string_view_type strv = str;
    
    return city_hash_64(strv.data(), strv.size() * sizeof(string_view_type::value_type));
}

}

#endif

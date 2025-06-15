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
 * @file        concepts.hpp
 * @brief       stringutils concepts header.
 * @author      Killian Valverde
 * @date        2025/06/11
 */

#ifndef SPEED_STRINGUTILS_CONCEPTS_HPP
#define SPEED_STRINGUTILS_CONCEPTS_HPP

#include <string>

namespace speed::stringutils {

template<typename T>
concept StringLike = requires(T str) {
    { str.data() } -> std::convertible_to<const typename T::value_type*>;
    { str.size() } -> std::convertible_to<std::size_t>;
    typename T::traits_type;
};

}

#endif

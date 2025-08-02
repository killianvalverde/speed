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
 * @file        type_traits.hpp
 * @brief       type_traits main header.
 * @author      Killian Valverde
 * @date        2025/05/18
 */

#ifndef SPEED_ARGPARSE_DETAIL_TYPE_TRAITS_HPP
#define SPEED_ARGPARSE_DETAIL_TYPE_TRAITS_HPP

#include <type_traits>

#include "../../type_traits/type_traits.hpp"

namespace speed::argparse::detail {

/** @cond */
template <typename T>
struct is_std_array : std::false_type {};

template <typename T, std::size_t N>
struct is_std_array<std::array<T, N>> : std::true_type {};

template <typename T>
inline constexpr bool is_std_array_v = is_std_array<T>::value;

template<typename T>
struct is_supported_container_helper : std::false_type {};

template<typename T, std::size_t N>
struct is_supported_container_helper<std::array<T, N>> : std::true_type {};

template<typename... Ts>
struct is_supported_container_helper<std::vector<Ts...>> : std::true_type {};

template<typename... Ts>
struct is_supported_container_helper<std::deque<Ts...>> : std::true_type {};

template<typename... Ts>
struct is_supported_container_helper<std::queue<Ts...>> : std::true_type {};

template<typename... Ts>
struct is_supported_container_helper<std::priority_queue<Ts...>> : std::true_type {};

template<typename... Ts>
struct is_supported_container_helper<std::stack<Ts...>> : std::true_type {};

template<typename... Ts>
struct is_supported_container_helper<std::forward_list<Ts...>> : std::true_type {};

template<typename... Ts>
struct is_supported_container_helper<std::list<Ts...>> : std::true_type {};

template<typename... Ts>
struct is_supported_container_helper<std::set<Ts...>> : std::true_type {};

template<typename... Ts>
struct is_supported_container_helper<std::unordered_set<Ts...>> : std::true_type {};

template<typename... Ts>
struct is_supported_container_helper<std::unordered_multiset<Ts...>> : std::true_type {};

template<typename... Ts>
struct is_supported_container_helper<std::pair<Ts...>> : std::true_type {};

template<typename... Ts>
struct is_supported_container_helper<std::tuple<Ts...>> : std::true_type {};

template<typename T>
struct is_nested_supported_container_helper : std::false_type {};

template<typename T, std::size_t N>
struct is_nested_supported_container_helper<std::array<T, N>> : std::true_type {};

template<typename... Ts>
struct is_nested_supported_container_helper<std::vector<Ts...>> : std::true_type {};

template<typename... Ts>
struct is_nested_supported_container_helper<std::deque<Ts...>> : std::true_type {};

template<typename... Ts>
struct is_nested_supported_container_helper<std::queue<Ts...>> : std::true_type {};

template<typename... Ts>
struct is_nested_supported_container_helper<std::stack<Ts...>> : std::true_type {};

template<typename... Ts>
struct is_nested_supported_container_helper<std::forward_list<Ts...>> : std::true_type {};

template<typename... Ts>
struct is_nested_supported_container_helper<std::list<Ts...>> : std::true_type {};

template<typename T>
concept has_value_type = requires { typename T::value_type; };

template<typename T>
struct is_supported_container : public is_supported_container_helper<
        typename std::remove_cv<T>::type>::type
{
};

template<typename T>
constexpr bool is_supported_container_v = is_supported_container<T>::value;

template<typename T>
struct is_nested_supported_container : std::false_type {};

template<typename T>
requires detail::has_value_type<T>
struct is_nested_supported_container<T> : std::bool_constant<
        detail::is_nested_supported_container_helper<std::remove_cv_t<T>>::value &&
        is_supported_container_v<typename T::value_type>>
{
};

template<typename T>
constexpr bool is_nested_supported_container_v = is_nested_supported_container<T>::value;
/** @endcond */

}

#endif

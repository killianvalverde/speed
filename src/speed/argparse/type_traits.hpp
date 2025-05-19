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
 * @file        type_traits.hpp
 * @brief       type_traits main header.
 * @author      Killian Valverde
 * @date        2025/05/18
 */

#ifndef SPEED_ARGPARSE_TYPE_TRAITS_HPP
#define SPEED_ARGPARSE_TYPE_TRAITS_HPP

#include <type_traits>

#include "../type_traits/type_traits.hpp"

namespace speed::argparse {

/** @cond */
namespace __private {

template<typename T>
struct __is_supported_container_helper : std::false_type {};

template<typename T, std::size_t N>
struct __is_supported_container_helper<std::array<T, N>> : std::true_type {};

template<typename... Args>
struct __is_supported_container_helper<std::vector<Args...>> : std::true_type {};

template<typename... Args>
struct __is_supported_container_helper<std::deque<Args...>> : std::true_type {};

template<typename... Args>
struct __is_supported_container_helper<std::queue<Args...>> : std::true_type {};

template<typename... Args>
struct __is_supported_container_helper<std::priority_queue<Args...>> : std::true_type {};

template<typename... Args>
struct __is_supported_container_helper<std::stack<Args...>> : std::true_type {};

template<typename... Args>
struct __is_supported_container_helper<std::forward_list<Args...>> : std::true_type {};

template<typename... Args>
struct __is_supported_container_helper<std::list<Args...>> : std::true_type {};

template<typename... Args>
struct __is_supported_container_helper<std::set<Args...>> : std::true_type {};

template<typename... Args>
struct __is_supported_container_helper<std::unordered_set<Args...>> : std::true_type {};

template<typename... Args>
struct __is_supported_container_helper<std::unordered_multiset<Args...>> : std::true_type {};

template<typename... Args>
struct __is_supported_container_helper<std::pair<Args...>> : std::true_type {};

template<typename... Args>
struct __is_supported_container_helper<std::tuple<Args...>> : std::true_type {};

template<typename T>
struct __is_nested_supported_container_helper : std::false_type {};

template<typename T, std::size_t N>
struct __is_nested_supported_container_helper<std::array<T, N>> : std::true_type {};

template<typename... Args>
struct __is_nested_supported_container_helper<std::vector<Args...>> : std::true_type {};

template<typename... Args>
struct __is_nested_supported_container_helper<std::deque<Args...>> : std::true_type {};

template<typename... Args>
struct __is_nested_supported_container_helper<std::queue<Args...>> : std::true_type {};

template<typename... Args>
struct __is_nested_supported_container_helper<std::stack<Args...>> : std::true_type {};

template<typename... Args>
struct __is_nested_supported_container_helper<std::forward_list<Args...>> : std::true_type {};

template<typename... Args>
struct __is_nested_supported_container_helper<std::list<Args...>> : std::true_type {};

template<typename T>
concept __has_value_type = requires { typename T::value_type; };

}
/** @endcond */

/**
 * @brief       Trait class that identifies whether Tp is a supported container type.
 */
template<typename Tp>
struct is_supported_container : public __private::__is_supported_container_helper<
        typename std::remove_cv<Tp>::type>::type
{
};

/**
 * @brief       Trait class that identifies whether Tp is a supported container type.
 */
template<typename Tp>
constexpr bool is_supported_container_v = is_supported_container<Tp>::value;

/**
 * @brief       Trait class that identifies whether Tp is a nested supported container type.
 */
template<typename Tp>
struct is_nested_supported_container : std::false_type {};

/**
 * @brief       Trait class that identifies whether Tp is a nested supported container type.
 */
template<typename Tp>
requires __private::__has_value_type<Tp>
struct is_nested_supported_container<Tp> : std::bool_constant<
        __private::__is_nested_supported_container_helper<std::remove_cv_t<Tp>>::value &&
        is_supported_container_v<typename Tp::value_type>>
{
};

/**
 * @brief       Trait class that identifies whether Tp is a nested supported container type.
 */
template<typename Tp>
constexpr bool is_nested_supported_container_v = is_nested_supported_container<Tp>::value;

}

#endif

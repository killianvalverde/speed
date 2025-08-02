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
 * @file        operations.hpp
 * @brief       detail operations functions header.
 * @author      Killian Valverde
 * @date        2025/07/20
 */

#ifndef SPEED_TYPE_TRAITS_DETAIL_OPERATIONS_HPP
#define SPEED_TYPE_TRAITS_DETAIL_OPERATIONS_HPP

#include <string>
#include <string_view>
#include <type_traits>

#include "forward_declarations.hpp"

/** @cond */
namespace speed::type_traits::detail {

template <typename T>
struct allocator_of_helper
{
};

template <typename CharT, typename CharTraitsT, typename AllocatorT>
struct allocator_of_helper<std::basic_string<CharT, CharTraitsT, AllocatorT>>
{
    using type = AllocatorT;
};

template <typename CharT, typename CharTraitsT>
struct allocator_of_helper<std::basic_string_view<CharT, CharTraitsT>>
{
    using type = std::allocator<CharT>;
};

template <typename CharT>
struct allocator_of_helper<CharT*>
{
    using type = std::allocator<std::remove_cv_t<CharT>>;
};

template <typename T>
struct character_traits_of_helper
{
};

template <typename CharT, typename CharTraitsT, typename AllocatorT>
struct character_traits_of_helper<std::basic_string<CharT, CharTraitsT, AllocatorT>>
{
    using type = CharTraitsT;
};

template <typename CharT, typename CharTraitsT>
struct character_traits_of_helper<std::basic_string_view<CharT, CharTraitsT>>
{
    using type = CharTraitsT;
};

template <typename CharT>
struct character_traits_of_helper<CharT*>
{
    using type = std::enable_if_t<
            is_character<CharT>::value,
            std::char_traits<std::remove_cv_t<CharT>>>;
};

template <typename T>
struct character_type_of_helper
{
};

template <typename CharT, typename CharTraitsT, typename AllocatorT>
struct character_type_of_helper<std::basic_string<CharT, CharTraitsT, AllocatorT>>
{
    using type = CharT;
};

template <typename CharT, typename CharTraitsT>
struct character_type_of_helper<std::basic_string_view<CharT, CharTraitsT>>
{
    using type = CharT;
};

template <typename CharT>
struct character_type_of_helper<CharT*>
{
    using type = std::enable_if_t<is_character<CharT>::value, std::remove_cv_t<CharT>>;
};

template<typename...>
struct logical_or;

template<>
struct logical_or<> : public std::false_type {};

template<typename T>
struct logical_or<T> : public T {};

template<typename T1, typename T2>
struct logical_or<T1, T2> : public std::conditional<T1::value, T1, T2>::type {};

template<typename T1, typename T2, typename T3, typename... TN>
struct logical_or<T1, T2, T3, TN...>
        : public std::conditional<T1::value, T1, logical_or<T2, T3, TN...>>::type {};

template<typename...>
struct logical_and;

template<>
struct logical_and<> : public std::true_type {};

template<typename T>
struct logical_and<T> : public T {};

template<typename T1, typename T2>
struct logical_and<T1, T2> : public std::conditional<T1::value, T2, T1>::type {};

template<typename T1, typename T2, typename T3, typename... TN>
struct logical_and<T1, T2, T3, TN...>
        : public std::conditional<T1::value, logical_and<T2, T3, TN...>, T1>::type {};

template<typename>
struct is_character_helper : public std::false_type {};

template<>
struct is_character_helper<char> : public std::true_type {};

template<>
struct is_character_helper<signed char> : public std::true_type {};

template<>
struct is_character_helper<unsigned char> : public std::true_type {};

template<>
struct is_character_helper<wchar_t> : public std::true_type {};

template<>
struct is_character_helper<char8_t> : std::true_type {};

template<>
struct is_character_helper<char16_t> : public std::true_type {};

template<>
struct is_character_helper<char32_t> : public std::true_type {};

template<typename T, bool IsEnum>
struct underlying_type_of_helper
{
    using type = T;
};

template<typename T>
struct underlying_type_of_helper<T, true>
{
    using type = std::underlying_type_t<T>;
};

}
/** @endcond */

#endif

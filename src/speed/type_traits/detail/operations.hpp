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
 * @file operations.hpp
 * @brief Internal helper operations for the type_traits module.
 * @author Killian Valverde
 * @date 2025-07-20
 */

#pragma once

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

template<typename T, bool>
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

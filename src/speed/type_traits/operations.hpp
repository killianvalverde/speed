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
 * @brief Core operations for the type_traits module.
 * @author Killian Valverde
 * @date 2016-08-05
 */

#pragma once

#include <string_view>
#include <type_traits>

#include "detail/forward_declarations.hpp"
#include "detail/operations.hpp"

namespace speed::type_traits {

/**
 * @brief Extracts the allocator type associated with a given type.
 *
 * @tparam T The type from which to extract the allocator.
 */
template<typename T>
struct allocator_of
{
    using type = detail::allocator_of_helper<std::decay_t<T>>::type;
};

/**
 * @brief Alias for allocator_of<T>::type.
 *
 * @tparam T The type from which to extract the allocator.
 */
template<typename T>
using allocator_of_t = allocator_of<T>::type;

/**
 * @brief Selects the appropriate CRTP base type depending on a sentinel.
 *
 * If ActualT equals SentinelT, the base is instantiated with DerivedT,
 * otherwise with ActualT directly.
 *
 * @tparam BaseT Base template.
 * @tparam DerivedT Derived template used in CRTP.
 * @tparam ActualT Actual type provided.
 * @tparam SentinelT Sentinel type used for comparison.
 * @tparam ParameterTs Additional template parameters.
 */
template<
    template<typename...> class BaseT,
    template<typename...> class DerivedT,
    typename ActualT,
    typename SentinelT,
    typename... ParameterTs
>
using basic_crtp_base_t = std::conditional_t<
    std::is_same_v<ActualT, SentinelT>,
    BaseT<ParameterTs..., DerivedT<ParameterTs..., ActualT>>,
    BaseT<ParameterTs..., ActualT>
>;

/**
 * @brief Selects the appropriate self type for CRTP patterns.
 *
 * If ActualT equals SentinelT, resolves to SelfT instantiated with parameters,
 * otherwise resolves to ActualT.
 *
 * @tparam SelfT Template representing the CRTP self type.
 * @tparam ActualT Actual type provided.
 * @tparam SentinelT Sentinel type used for comparison.
 * @tparam ParameterTs Additional template parameters.
 */
template<
    template<typename...> class SelfT,
    typename ActualT,
    typename SentinelT,
    typename... ParameterTs
>
using basic_crtp_self_t = std::conditional_t<
    std::is_same_v<ActualT, SentinelT>,
    SelfT<ParameterTs...>,
    ActualT
>;

/**
 * @brief Extracts the character traits type associated with a given type.
 *
 * @tparam T The type from which to extract character traits.
 */
template<typename T>
struct character_traits_of
{
    using type = detail::character_traits_of_helper<std::decay_t<T>>::type;
};

/**
 * @brief Alias for character_traits_of<T>::type.
 *
 * @tparam T The type from which to extract character traits.
 */
template<typename T>
using character_traits_of_t = character_traits_of<T>::type;

/**
 * @brief Extracts the character type associated with a given type.
 *
 * @tparam T The type from which to extract the character type.
 */
template<typename T>
struct character_type_of
{
    using type = detail::character_type_of_helper<std::decay_t<T>>::type;
};

/**
 * @brief Alias for character_type_of<T>::type.
 *
 * @tparam T The type from which to extract the character type.
 */
template<typename T>
using character_type_of_t = character_type_of<T>::type;

/**
 * @brief Checks whether a type is a character type.
 *
 * @tparam T The type to check.
 */
template<typename T>
struct is_character : std::disjunction<
    std::is_same<std::remove_cv_t<T>, char>,
    std::is_same<std::remove_cv_t<T>, unsigned char>,
    std::is_same<std::remove_cv_t<T>, signed char>,
    std::is_same<std::remove_cv_t<T>, wchar_t>,
    std::is_same<std::remove_cv_t<T>, char8_t>,
    std::is_same<std::remove_cv_t<T>, char16_t>,
    std::is_same<std::remove_cv_t<T>, char32_t>
> {};

/**
 * @brief Convenience variable template for is_character.
 */
template<typename T>
constexpr bool is_character_v = is_character<T>::value;

/**
 * @brief Checks whether a type is a pointer to a character type.
 *
 * @tparam T The type to check.
 */
template<typename T>
struct is_character_pointer : std::conjunction<
    std::is_pointer<T>,
    is_character<std::remove_pointer_t<T>>
> {};

/**
 * @brief Convenience variable template for is_character_pointer.
 */
template<typename T>
constexpr bool is_character_pointer_v = is_character_pointer<T>::value;

/**
 * @brief Produces a std::basic_string_view type associated with a given type.
 *
 * The character type and traits are deduced using corresponding traits.
 *
 * @tparam T The source type.
 */
template<typename T>
struct string_view_of
{
    using type = std::basic_string_view<character_type_of_t<T>, character_traits_of_t<T>>;
};

/**
 * @brief Alias for string_view_of<T>::type.
 *
 * @tparam T The source type.
 */
template<typename T>
using string_view_of_t = string_view_of<T>::type;

/**
 * @brief Extracts the underlying type of a type.
 *
 * If T is an enum, resolves to its underlying type,
 * otherwise resolves to T itself.
 *
 * @tparam T The type to inspect.
 */
template<typename T>
struct underlying_type_of
{
    using type = detail::underlying_type_of_helper<T, std::is_enum_v<T>>::type;
};

/**
 * @brief Alias for underlying_type_of<T>::type.
 *
 * @tparam T The type to inspect.
 */
template<typename T>
using underlying_type_of_t = underlying_type_of<T>::type;

}

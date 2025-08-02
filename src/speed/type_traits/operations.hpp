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
 * @brief       operations functions header.
 * @author      Killian Valverde
 * @date        2016/08/05
 */

#ifndef SPEED_TYPE_TRAITS_OPERATIONS_HPP
#define SPEED_TYPE_TRAITS_OPERATIONS_HPP

#include <string>
#include <string_view>
#include <type_traits>

#include "detail/forward_declarations.hpp"
#include "detail/operations.hpp"

namespace speed::type_traits {

/**
 * @brief       Type trait to deduce the allocator type used by a given type.
 */
template<typename T>
struct allocator_of
{
    /** The allocator type associated with `T`. */
    using type = typename detail::allocator_of_helper<std::decay_t<T>>::type;
};

/**
 * @brief       Helper alias to extract the allocator type of a given type.
 */
template<typename T>
using allocator_of_t = typename allocator_of<T>::type;

/**
 * @brief       Alias that simplifies to inherit from the base class in the context of a CRTP
 *              pattern.
 */
template<
        template<typename...> class BaseT,
        template<typename...> class DerivedT,
        typename ActualT,
        typename ConditionT,
        typename... ParameterTs
>
using basic_crtp_base = typename std::conditional<
        std::is_same_v<ActualT, ConditionT>,
        BaseT<ParameterTs..., DerivedT<ParameterTs..., ActualT>>,
        BaseT<ParameterTs..., ActualT>
>::type;

/**
 * @brief       Alias that simplifies getting access to the self type in a CRTP context.
 */
template<
        template<typename...> class SelfT,
        typename ActualT,
        typename ConditionT,
        typename... ParameterTs
>
using basic_crtp_self = typename std::conditional<
        std::is_same_v<ActualT, ConditionT>,
        SelfT<ParameterTs...>,
        ActualT
>::type;

/**
 * @brief       Trait class that try to obtains the character traits of any kind of String.
 */
template<typename T>
struct character_traits_of
{
    /** The character traits type associated with `T`. */
    using type = typename detail::character_traits_of_helper<std::decay_t<T>>::type;
};

/**
 * @brief       Trait class that try to obtains the character traits of any kind of String.
 */
template<typename T>
using character_traits_of_t = typename character_traits_of<T>::type;

/**
 * @brief       Trait class that try to obtains the character type of any kind of String.
 */
template<typename T>
struct character_type_of
{
    /** The character type associated with `T`. */
    using type = typename detail::character_type_of_helper<std::decay_t<T>>::type;
};

/**
 * @brief       Trait class that try to obtains the character type of any kind of String.
 */
template<typename T>
using character_type_of_t = typename character_type_of<T>::type;

/**
 * @brief       Trait class that identifies whether T is a character type.
 */
template<typename T>
struct is_character
        : public detail::is_character_helper<
                typename std::remove_cv_t<T>
          >::type
{
};

/**
 * @brief       Trait class that identifies whether T is a character type.
 */
template<typename T>
inline constexpr bool is_character_v = is_character<T>::value;

/**
 * @brief       Trait class that identifies whether T is a character pointer type.
 */
template<typename T>
struct is_character_pointer
        : public detail::logical_and<
                std::is_pointer<T>,
                is_character<typename std::remove_pointer<T>::type>
          >::type
{
};

/**
 * @brief       Trait class that identifies whether T is a character pointer type.
 */
template<typename T>
inline constexpr bool is_character_pointer_v = is_character_pointer<T>::value;

/**
 * @brief       Trait class that identifies whether T is a character that can be used in standard io
 *              operations.
 */
template<typename T>
struct is_stdio_character
        : public detail::logical_or<
                std::is_same<std::remove_cv_t<T>, char>,
                std::is_same<std::remove_cv_t<T>, wchar_t>
          >::type
{
};

/**
 * @brief       Trait class that identifies whether T is a character that can be used in standard io
 *              operations.
 */
template<typename T>
inline constexpr bool is_stdio_character_v = is_stdio_character<T>::value;

/**
 * @brief       Trait class that try to obtains the string_view that will suit the given type.
 */
template<typename T>
struct string_view_of
{
    /** The string view type associated with `T`. */
    using type = std::basic_string_view<character_type_of_t<T>, character_traits_of_t<T>>;
};

/**
 * @brief       Trait class that try to obtains the string_view that will suit the given type.
 */
template<typename T>
using string_view_of_t = typename string_view_of<T>::type;

/**
 * @brief       Trait class that try to obtains the underlying type of enum type T.
 */
template<typename T>
struct underlying_type_of
{
    /** The underlying type associated with `T`. */
    using type = typename detail::underlying_type_of_helper<T, std::is_enum_v<T>>::type;
};

/**
 * @brief       Trait class that try to obtains the underlying type of enum type T.
 */
template<typename EnumT>
using underlying_type_of_t = typename underlying_type_of<EnumT>::type;

}

#endif

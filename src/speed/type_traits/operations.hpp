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
 * @file        operations.hpp
 * @brief       operations functions header.
 * @author      Killian Valverde
 * @date        2016/08/05
 */

#ifndef SPEED_TYPE_TRAITS_OPERATIONS_HPP
#define SPEED_TYPE_TRAITS_OPERATIONS_HPP

#include <type_traits>

namespace speed::type_traits {

/** @cond */
namespace detail {
template<typename...>
struct logical_or;

template<>
struct logical_or<> : public std::false_type {};

template<typename Tp>
struct logical_or<Tp> : public Tp {};

template<typename Tp1, typename Tp2>
struct logical_or<Tp1, Tp2> : public std::conditional<Tp1::value, Tp1, Tp2>::type {};

template<typename Tp1, typename Tp2, typename Tp3, typename... TpN>
struct logical_or<Tp1, Tp2, Tp3, TpN...>
        : public std::conditional<Tp1::value, Tp1, logical_or<Tp2, Tp3, TpN...>>::type {};

template<typename...>
struct logical_and;

template<>
struct logical_and<> : public std::true_type {};

template<typename Tp>
struct logical_and<Tp> : public Tp {};

template<typename Tp1, typename Tp2>
struct logical_and<Tp1, Tp2> : public std::conditional<Tp1::value, Tp2, Tp1>::type {};

template<typename Tp1, typename Tp2, typename Tp3, typename... TpN>
struct logical_and<Tp1, Tp2, Tp3, TpN...>
        : public std::conditional<Tp1::value, logical_and<Tp2, Tp3, TpN...>, Tp1>::type {};

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
struct try_underlying_type_helper
{
    using type = T;
};

template<typename T>
struct try_underlying_type_helper<T, true>
{
    using type = std::underlying_type_t<T>;
};
}
/** @endcond */

/**
 * @brief       Alias that simplifies to inherit from the base class in the context of a CRTP
 *              pattern.
 */
template<
        template<typename...> class TpBase,
        template<typename...> class TpDerived,
        typename TpActual,
        typename TpCondition,
        typename... Ts
>
using basic_crtp_base = typename std::conditional<
        std::is_same_v<TpActual, TpCondition>,
        TpBase<Ts..., TpDerived<Ts..., TpActual>>,
        TpBase<Ts..., TpActual>
>::type;

/**
 * @brief       Alias that simplifies getting access to the self type in a CRTP context.
 */
template<
        template<typename...> class TpSelf,
        typename TpActual,
        typename TpCondition,
        typename... Ts
>
using basic_crtp_self = typename std::conditional<
        std::is_same_v<TpActual, TpCondition>,
        TpSelf<Ts...>,
        TpActual
>::type;

/**
 * @brief       Trait class that identifies whether T is a character type.
 */
template<typename Tp>
struct is_character
        : public detail::is_character_helper<
                typename std::remove_cv_t<Tp>
          >::type
{
};

/**
 * @brief       Trait class that identifies whether T is a character type.
 */
template<typename Tp>
inline constexpr bool is_character_v = is_character<Tp>::value;

/**
 * @brief       Trait class that identifies whether T is a character pointer type.
 */
template<typename Tp>
struct is_character_pointer
        : public detail::logical_and<
                std::is_pointer<Tp>,
                is_character<typename std::remove_pointer<Tp>::type>
          >::type
{
};

/**
 * @brief       Trait class that identifies whether T is a character pointer type.
 */
template<typename Tp>
inline constexpr bool is_character_pointer_v = is_character_pointer<Tp>::value;

/**
 * @brief       Trait class that identifies whether T is a character that can be used in standard io
 *              operations.
 */
template<typename Tp>
struct is_stdio_character
        : public detail::logical_or<
                std::is_same<std::remove_cv_t<Tp>, char>,
                std::is_same<std::remove_cv_t<Tp>, wchar_t>
          >::type
{
};

/**
 * @brief       Trait class that identifies whether T is a character that can be used in standard io
 *              operations.
 */
template<typename Tp>
inline constexpr bool is_stdio_character_v = is_stdio_character<Tp>::value;

/**
 * @brief       Trait class that try to obtains the underlying type of enum type T.
 */
template<typename Tp>
struct try_underlying_type
{
    using type = typename detail::try_underlying_type_helper<Tp, std::is_enum_v<Tp>>::type;
};

/**
 * @brief       Trait class that try to obtains the underlying type of enum type T.
 */
template<typename TpEnum>
using try_underlying_type_t = typename try_underlying_type<TpEnum>::type;

}

#endif

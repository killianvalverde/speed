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
 * @file       is_flag_enum.hpp
 * @brief      is_flag_enum functions header.
 * @author     Killian Valverde
 * @date       2016/08/05
 */

#ifndef SPEED_SCALARS_IS_FLAG_ENUM_HPP
#define SPEED_SCALARS_IS_FLAG_ENUM_HPP

#include <iostream>

#include "../type_traits/type_traits.hpp"

namespace speed::scalars {

/** @cond */
template<typename T>
struct is_flag_enum : std::false_type {};

template<typename T>
inline constexpr bool is_flag_enum_v = is_flag_enum<T>::value;

template<typename LhsT, typename RhsT>
requires is_flag_enum_v<LhsT> || is_flag_enum_v<RhsT>
constexpr LhsT operator |(LhsT lhs, RhsT rhs) noexcept
{
    using lhs_underlying_type = type_traits::underlying_type_of_t<LhsT>;
    using rhs_underlying_type = type_traits::underlying_type_of_t<RhsT>;
    
    return static_cast<LhsT>(
            static_cast<lhs_underlying_type>(lhs) |
            static_cast<rhs_underlying_type>(rhs));
}

template<typename LhsT, typename RhsT>
requires is_flag_enum_v<LhsT> || is_flag_enum_v<RhsT>
constexpr LhsT operator &(LhsT lhs, RhsT rhs) noexcept
{
    using lhs_underlying_type = type_traits::underlying_type_of_t<LhsT>;
    using rhs_underlying_type = type_traits::underlying_type_of_t<RhsT>;
    
    return static_cast<LhsT>(
            static_cast<lhs_underlying_type>(lhs) &
            static_cast<rhs_underlying_type>(rhs));
}

template<typename LhsT, typename RhsT>
requires is_flag_enum_v<LhsT> || is_flag_enum_v<RhsT>
constexpr LhsT operator ^(LhsT lhs, RhsT rhs) noexcept
{
    using lhs_underlying_type = type_traits::underlying_type_of_t<LhsT>;
    using rhs_underlying_type = type_traits::underlying_type_of_t<RhsT>;
    
    return static_cast<LhsT>(
            static_cast<lhs_underlying_type>(lhs) ^
            static_cast<rhs_underlying_type>(rhs));
}

template<typename LhsT, typename RhsT>
requires is_flag_enum_v<LhsT> || is_flag_enum_v<RhsT>
constexpr LhsT operator <<(LhsT lhs, RhsT rhs) noexcept
{
    using lhs_underlying_type = type_traits::underlying_type_of_t<LhsT>;
    using rhs_underlying_type = type_traits::underlying_type_of_t<RhsT>;
    
    return static_cast<LhsT>(
            static_cast<lhs_underlying_type>(lhs) <<
            static_cast<rhs_underlying_type>(rhs));
}

template<typename LhsT, typename RhsT>
requires is_flag_enum_v<LhsT> || is_flag_enum_v<RhsT>
constexpr LhsT operator >>(LhsT lhs, RhsT rhs) noexcept
{
    using lhs_underlying_type = type_traits::underlying_type_of_t<LhsT>;
    using rhs_underlying_type = type_traits::underlying_type_of_t<RhsT>;
    
    return static_cast<LhsT>(
            static_cast<lhs_underlying_type>(lhs) >>
            static_cast<rhs_underlying_type>(rhs));
}

template<typename LhsT>
requires is_flag_enum_v<LhsT>
constexpr LhsT operator ~(LhsT lhs) noexcept
{
    using lhs_underlying_type = type_traits::underlying_type_of_t<LhsT>;
    return static_cast<LhsT>(~static_cast<lhs_underlying_type>(lhs));
}

template<typename LhsT, typename RhsT>
requires is_flag_enum_v<LhsT> || is_flag_enum_v<RhsT>
constexpr LhsT& operator |=(LhsT& lhs, RhsT rhs) noexcept
{
    using lhs_underlying_type = type_traits::underlying_type_of_t<LhsT>;
    using rhs_underlying_type = type_traits::underlying_type_of_t<RhsT>;
    
    return lhs = static_cast<LhsT>(
            static_cast<lhs_underlying_type>(lhs) |
            static_cast<rhs_underlying_type>(rhs));
}

template<typename LhsT, typename RhsT>
requires is_flag_enum_v<LhsT> || is_flag_enum_v<RhsT>
constexpr LhsT& operator &=(LhsT& lhs, RhsT rhs) noexcept
{
    using lhs_underlying_type = type_traits::underlying_type_of_t<LhsT>;
    using rhs_underlying_type = type_traits::underlying_type_of_t<RhsT>;
    
    return lhs = static_cast<LhsT>(
            static_cast<lhs_underlying_type>(lhs) &
            static_cast<rhs_underlying_type>(rhs));
}

template<typename LhsT, typename RhsT>
requires is_flag_enum_v<LhsT> || is_flag_enum_v<RhsT>
constexpr LhsT& operator ^=(LhsT& lhs, RhsT rhs) noexcept
{
    using lhs_underlying_type = type_traits::underlying_type_of_t<LhsT>;
    using rhs_underlying_type = type_traits::underlying_type_of_t<RhsT>;
    
    return lhs = static_cast<LhsT>(
            static_cast<lhs_underlying_type>(lhs) ^
            static_cast<rhs_underlying_type>(rhs));
}

template<typename LhsT, typename RhsT>
requires is_flag_enum_v<LhsT> || is_flag_enum_v<RhsT>
constexpr LhsT& operator <<=(LhsT& lhs, RhsT rhs) noexcept
{
    using lhs_underlying_type = type_traits::underlying_type_of_t<LhsT>;
    using rhs_underlying_type = type_traits::underlying_type_of_t<RhsT>;
    
    return lhs = static_cast<LhsT>(
            static_cast<lhs_underlying_type>(lhs) <<
            static_cast<rhs_underlying_type>(rhs));
}

template<typename LhsT, typename RhsT>
requires is_flag_enum_v<LhsT> || is_flag_enum_v<RhsT>
constexpr LhsT& operator >>=(LhsT& lhs, RhsT rhs) noexcept
{
    using lhs_underlying_type = type_traits::underlying_type_of_t<LhsT>;
    using rhs_underlying_type = type_traits::underlying_type_of_t<RhsT>;
    
    return lhs = static_cast<LhsT>(
            static_cast<lhs_underlying_type>(lhs) >>
            static_cast<rhs_underlying_type>(rhs));
}

// template<typename CharT, typename CharTraitsT, typename RhsT>
// requires is_flag_enum_v<RhsT>
// std::basic_ostream<CharT, CharTraitsT>& operator <<(
//         std::basic_ostream<CharT, CharTraitsT>& os,
//         const RhsT& rhs
// )
// {
//     using rhs_underlying_type = type_traits::underlying_type_of_t<RhsT>;
//     os << static_cast<rhs_underlying_type>(rhs);
//     return os;
// }
/** @endcond */

}

using speed::scalars::operator |;
using speed::scalars::operator &;
using speed::scalars::operator ^;
using speed::scalars::operator <<;
using speed::scalars::operator >>;
using speed::scalars::operator ~;
using speed::scalars::operator |=;
using speed::scalars::operator &=;
using speed::scalars::operator ^=;
using speed::scalars::operator <<=;
using speed::scalars::operator >>=;

#endif

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
 * @brief Contains generic mathematical operations on arithmetic types.
 * @author Killian Valverde
 * @date 2017-01-28
 */

#pragma once

#include <concepts>

namespace speed::math {

/**
 * @brief Computes the absolute value of an integer without overflow.
 *
 * Unlike `std::abs`, this function is well-defined for the minimum value of
 * a signed type (e.g. `INT_MIN`), because the result is returned as the
 * corresponding unsigned type, which can always represent it.
 *
 * @tparam IntegralT An integral type. Must not be `bool`, since
 *                   `std::make_unsigned_t<bool>` is ill-formed.
 *
 * @param val The value whose absolute value is computed.
 *
 * @return The absolute value of val as `std::make_unsigned_t<IntegralT>`.
 */
template<std::integral IntegralT>
[[nodiscard]] constexpr std::make_unsigned_t<IntegralT> absolute(IntegralT val) noexcept
{
    using unsigned_t = std::make_unsigned_t<IntegralT>;

    if constexpr (std::unsigned_integral<IntegralT>)
    {
        return val;
    }
    else
    {
        return val < 0 ? unsigned_t(-(val + 1)) + 1 : unsigned_t(val);
    }
}

}

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
 * @brief Internal helper operations for the numerics module.
 * @author Killian Valverde
 * @date 2026-06-17
 */

#pragma once

#include <concepts>
#include <limits>

/** @cond */
namespace speed::numerics::detail {

template<std::integral IntegralT>
[[nodiscard]] constexpr bool overflowing_add(
    IntegralT lhs,
    IntegralT rhs,
    IntegralT& result
) noexcept
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_add_overflow(lhs, rhs, &result);
#else
    using limits = std::numeric_limits<IntegralT>;

    if constexpr (std::unsigned_integral<IntegralT>)
    {
        result = lhs + rhs;
        return result < lhs;
    }
    else
    {
        if ((rhs > 0 && lhs > limits::max() - rhs) ||
            (rhs < 0 && lhs < limits::min() - rhs))
        {
            return true;
        }

        result = lhs + rhs;
        return false;
    }
#endif
}

}

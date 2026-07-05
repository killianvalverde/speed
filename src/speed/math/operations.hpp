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
 * @file        operations.hpp
 * @brief       math functions header.
 * @author      Killian Valverde
 * @date        2017/01/28
 */

#ifndef SPEED_MATH_OPERATIONS_HPP
#define SPEED_MATH_OPERATIONS_HPP

#include <concepts>

namespace speed::math {

template<std::integral IntegralT>
[[nodiscard]] constexpr std::make_unsigned_t<IntegralT> abs(IntegralT val) noexcept
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

#endif

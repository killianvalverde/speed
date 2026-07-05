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
 * @brief Numeric utility functions and overflow-aware arithmetic operations.
 * @author Killian Valverde
 * @date 2017-01-28
 */

#pragma once

#include <bit>
#include <concepts>
#include <cstdint>
#include <limits>
#include <stdexcept>

#include "../math/math.hpp"
#include "detail/operations.hpp"
#include "exception.hpp"

namespace speed::numerics {

/**
 * @brief Adds one or more values and throws if an overflow occurs.
 *
 * Computes the sum of target and all values in order. If any intermediate addition
 * cannot be represented by TargetT, an overflow_exception is thrown.
 *
 * @param target Initial value to which the additions are applied.
 * @param values Values to add to target.
 * @return The computed sum.
 * @throws overflow_exception If any addition overflows.
 */
template<std::integral TargetT, std::integral... ValueTs>
[[nodiscard]] constexpr TargetT checked_add(TargetT target, ValueTs... values)
{
    ([&]
    {
        TargetT result;

        if (detail::overflowing_add(target, static_cast<TargetT>(values), result))
        {
            throw overflow_exception();
        }

        target = result;
    }(), ...);

    return target;
}

/**
 * @brief Returns the number of decimal digits in an integer value.
 *
 * Computes the number of digits required to represent the absolute value of `val` in base 10.
 * The implementation uses a fast approximation based on the binary width of the value and a
 * lookup table of powers of ten, avoiding costly integer division operations.
 *
 * @tparam IntegralT An integral type.
 *
 * @param val The value whose decimal digit count is to be determined.
 * @return The number of decimal digits in the absolute value of `val`.
 */
template<std::integral IntegralT>
[[nodiscard]] constexpr std::size_t count_digits(IntegralT val) noexcept
{
    constexpr std::uint64_t powers10[] =
    {
        1ULL,
        10ULL,
        100ULL,
        1000ULL,
        10000ULL,
        100000ULL,
        1000000ULL,
        10000000ULL,
        100000000ULL,
        1000000000ULL,
        10000000000ULL,
        100000000000ULL,
        1000000000000ULL,
        10000000000000ULL,
        100000000000000ULL,
        1000000000000000ULL,
        10000000000000000ULL,
        100000000000000000ULL,
        1000000000000000000ULL,
        10000000000000000000ULL
    };

    const auto v = static_cast<std::uint64_t>(math::abs(val));
    if (v == 0)
    {
        return 1;
    }

    const std::size_t digits = (std::bit_width(v) * 1233) >> 12;
    return digits + (v >= powers10[digits]);
}

/**
 * @brief Returns the index of the least significant set bit.
 *
 * Returns the 1-based position of the least significant set bit in value.
 * If value is zero, the function returns 0.
 *
 * @param value Value to examine.
 * @return The 1-based index of the least significant set bit, or 0 if value is zero.
 */
template<std::unsigned_integral IntegralT>
[[nodiscard]] constexpr std::size_t one_hot_to_index(IntegralT value) noexcept
{
    std::size_t index = 1;

    while (value != 0)
    {
        if (value & 1)
        {
            return index;
        }

        value >>= 1;
        ++index;
    }

    return 0;
}

/**
 * @brief Adds one or more values using saturating arithmetic.
 *
 * Computes the sum of target and all values in order. If any intermediate addition
 * cannot be represented by TargetT, the result is clamped to the nearest
 * representable value and no further additions are performed.
 *
 * For unsigned types, the result is saturated to the maximum value of TargetT.
 * For signed types, the result is saturated to either the minimum or maximum value
 * of TargetT depending on the direction of the overflow.
 *
 * @param target Initial value to which the additions are applied.
 * @param values Values to add to target.
 * @return The computed sum, or a saturated value if an overflow occurs.
 */
template<std::integral TargetT, std::integral... ValueTs>
[[nodiscard]] constexpr TargetT saturating_add(TargetT target, ValueTs... values) noexcept
{
    (... && [&]
    {
        TargetT result;

        if (detail::overflowing_add(target, static_cast<TargetT>(values), result))
        {
            if constexpr (std::unsigned_integral<TargetT>)
            {
                target = std::numeric_limits<TargetT>::max();
            }
            else
            {
                target = values >= 0
                    ? std::numeric_limits<TargetT>::max()
                    : std::numeric_limits<TargetT>::min();
            }

            return false;
        }

        target = result;
        return true;
    }());

    return target;
}

/**
 * @brief Adds one or more values and reports whether an overflow occurred.
 *
 * Computes the sum of target and all values in order. If any intermediate addition
 * cannot be represented by TargetT, the function returns false.
 *
 * @param target Initial value to which the additions are applied. Receives the
 *               computed sum if no overflow occurs.
 * @param values Values to add to target.
 * @return True if all additions succeed; otherwise false.
 */
template<std::integral TargetT, std::integral... ValueTs>
[[nodiscard]] constexpr bool try_checked_add(TargetT& target, ValueTs... values) noexcept
{
    return (... && [&]
    {
        TargetT result;

        if (detail::overflowing_add(target, static_cast<TargetT>(values), result))
        {
            return false;
        }

        target = result;
        return true;
    }());
}

/**
 * @brief Adds one or more values using saturating arithmetic and reports whether
 *        saturation occurred.
 *
 * Computes the sum of target and all values in order. If any intermediate addition cannot be
 * represented by TargetT, the result is clamped to the nearest representable value, no further
 * additions are performed, and the function returns false.
 *
 * For unsigned types, the result is saturated to the maximum value of TargetT.
 * For signed types, the result is saturated to either the minimum or maximum value
 * of TargetT depending on the direction of the overflow.
 *
 * @param target Initial value to which the additions are applied. Receives the
 *               computed sum or the saturated value.
 * @param values Values to add to target.
 * @return True if all additions succeed without overflow; otherwise false.
 */
template<std::integral TargetT, std::integral... ValueTs>
[[nodiscard]] constexpr bool try_saturating_add(TargetT& target, ValueTs... values) noexcept
{
    return (... && [&]
    {
        TargetT result;

        if (detail::overflowing_add(target, static_cast<TargetT>(values), result))
        {
            if constexpr (std::unsigned_integral<TargetT>)
            {
                target = std::numeric_limits<TargetT>::max();
            }
            else
            {
                target = values >= 0
                    ? std::numeric_limits<TargetT>::max()
                    : std::numeric_limits<TargetT>::min();
            }

            return false;
        }

        target = result;
        return true;
    }());
}

}

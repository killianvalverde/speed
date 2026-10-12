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
 * @file operations_test.cpp
 * @brief Unit tests for numerics operations.
 * @author Killian Valverde
 * @date 2018-04-28
 */

#include <cstdint>
#include <limits>
#include <gtest/gtest.h>

#include "speed/numerics/numerics.hpp"

namespace num = speed::numerics;

TEST(numerics_checked_add, adds_values)
{
    EXPECT_EQ(num::checked_add(10, 20), 30);
    EXPECT_EQ(num::checked_add(10, 20, 30), 60);
    EXPECT_EQ(num::checked_add(-10, 20), 10);
    EXPECT_EQ(num::checked_add(10, -20), -10);
    EXPECT_EQ(num::checked_add(10), 10);
}

TEST(numerics_checked_add, throws_on_overflow)
{
    constexpr auto max = std::numeric_limits<int>::max();
    constexpr auto min = std::numeric_limits<int>::min();

    EXPECT_THROW((void)num::checked_add(max, 1), num::overflow_exception);
    EXPECT_THROW((void)num::checked_add(min, -1), num::overflow_exception);
    EXPECT_THROW((void)num::checked_add(max - 1, 1, 1), num::overflow_exception);
}

TEST(numerics_checked_add, throws_on_unsigned_overflow)
{
    constexpr auto max = std::numeric_limits<unsigned int>::max();

    EXPECT_THROW((void)num::checked_add(max, 1u), num::overflow_exception);
}

TEST(numerics_count_digits, counts_positive_digits)
{
    EXPECT_EQ(num::count_digits(0), 1);
    EXPECT_EQ(num::count_digits(1), 1);
    EXPECT_EQ(num::count_digits(9), 1);
    EXPECT_EQ(num::count_digits(10), 2);
    EXPECT_EQ(num::count_digits(99), 2);
    EXPECT_EQ(num::count_digits(100), 3);
    EXPECT_EQ(num::count_digits(999), 3);
    EXPECT_EQ(num::count_digits(1000), 4);
    EXPECT_EQ(num::count_digits(123456789), 9);
}

TEST(numerics_count_digits, counts_negative_digits)
{
    EXPECT_EQ(num::count_digits(-1), 1);
    EXPECT_EQ(num::count_digits(-9), 1);
    EXPECT_EQ(num::count_digits(-10), 2);
    EXPECT_EQ(num::count_digits(-999), 3);
    EXPECT_EQ(num::count_digits(-123456789), 9);
}

TEST(numerics_count_digits, handles_integer_limits)
{
    EXPECT_EQ(num::count_digits(std::numeric_limits<std::int32_t>::max()), 10);
    EXPECT_EQ(num::count_digits(std::numeric_limits<std::int32_t>::min()), 10);
    EXPECT_EQ(num::count_digits(std::numeric_limits<std::uint32_t>::max()), 10);
    EXPECT_EQ(num::count_digits(std::numeric_limits<std::uint64_t>::max()), 20);
    EXPECT_EQ(num::count_digits(std::numeric_limits<std::int64_t>::min()), 19);
}

TEST(numerics_one_hot_to_index, returns_position_of_set_bit)
{
    EXPECT_EQ(num::one_hot_to_index(0u), 0);
    EXPECT_EQ(num::one_hot_to_index(1u), 1);
    EXPECT_EQ(num::one_hot_to_index(2u), 2);
    EXPECT_EQ(num::one_hot_to_index(4u), 3);
    EXPECT_EQ(num::one_hot_to_index(8u), 4);
    EXPECT_EQ(num::one_hot_to_index(16u), 5);
    EXPECT_EQ(num::one_hot_to_index(128u), 8);
}

TEST(numerics_one_hot_to_index, returns_least_significant_set_bit)
{
    EXPECT_EQ(num::one_hot_to_index(3u), 1);
    EXPECT_EQ(num::one_hot_to_index(6u), 2);
    EXPECT_EQ(num::one_hot_to_index(12u), 3);
    EXPECT_EQ(num::one_hot_to_index(255u), 1);
    EXPECT_EQ(num::one_hot_to_index(0b101000u), 4);
}

TEST(numerics_saturating_add, adds_values)
{
    EXPECT_EQ(num::saturating_add(10, 20), 30);
    EXPECT_EQ(num::saturating_add(10, 20, 30), 60);
    EXPECT_EQ(num::saturating_add(-10, 20), 10);
    EXPECT_EQ(num::saturating_add(10, -20), -10);
    EXPECT_EQ(num::saturating_add(10), 10);
}

TEST(numerics_saturating_add, saturates_on_signed_overflow)
{
    constexpr auto max = std::numeric_limits<int>::max();
    constexpr auto min = std::numeric_limits<int>::min();

    EXPECT_EQ(num::saturating_add(max, 1), max);
    EXPECT_EQ(num::saturating_add(min, -1), min);
    EXPECT_EQ(num::saturating_add(max - 1, 10), max);
    EXPECT_EQ(num::saturating_add(min + 1, -10), min);
}

TEST(numerics_saturating_add, saturates_on_unsigned_overflow)
{
    constexpr auto max = std::numeric_limits<unsigned int>::max();

    EXPECT_EQ(num::saturating_add(max, 1u), max);
    EXPECT_EQ(num::saturating_add(max - 1, 10u), max);
}

TEST(numerics_saturating_add, stops_after_saturation)
{
    constexpr auto max = std::numeric_limits<int>::max();

    EXPECT_EQ(num::saturating_add(max, 1, -100), max);
}

TEST(numerics_try_checked_add, updates_target_on_success)
{
    int value = 10;

    const bool result = num::try_checked_add(value, 20, 30);

    EXPECT_TRUE(result);
    EXPECT_EQ(value, 60);
}

TEST(numerics_try_checked_add, preserves_target_on_overflow)
{
    int value = std::numeric_limits<int>::max();

    const bool result = num::try_checked_add(value, 1);

    EXPECT_FALSE(result);
    EXPECT_EQ(value, std::numeric_limits<int>::max());
}

TEST(numerics_try_checked_add, stops_at_first_overflow)
{
    int value = std::numeric_limits<int>::max() - 1;

    const bool result = num::try_checked_add(value, 1, 1, -100);

    EXPECT_FALSE(result);
    EXPECT_EQ(value, std::numeric_limits<int>::max());
}

TEST(numerics_try_saturating_add, updates_target_on_success)
{
    int value = 10;

    const bool result = num::try_saturating_add(value, 20, 30);

    EXPECT_TRUE(result);
    EXPECT_EQ(value, 60);
}

TEST(numerics_try_saturating_add, saturates_on_positive_overflow)
{
    int value = std::numeric_limits<int>::max();

    const bool result = num::try_saturating_add(value, 1);

    EXPECT_FALSE(result);
    EXPECT_EQ(value, std::numeric_limits<int>::max());
}

TEST(numerics_try_saturating_add, saturates_on_negative_overflow)
{
    int value = std::numeric_limits<int>::min();

    const bool result = num::try_saturating_add(value, -1);

    EXPECT_FALSE(result);
    EXPECT_EQ(value, std::numeric_limits<int>::min());
}

TEST(numerics_try_saturating_add, saturates_on_unsigned_overflow)
{
    unsigned int value = std::numeric_limits<unsigned int>::max();

    const bool result = num::try_saturating_add(value, 1u);

    EXPECT_FALSE(result);
    EXPECT_EQ(value, std::numeric_limits<unsigned int>::max());
}

TEST(numerics_try_saturating_add, stops_after_saturation)
{
    int value = std::numeric_limits<int>::max();

    const bool result = num::try_saturating_add(value, 1, -100);

    EXPECT_FALSE(result);
    EXPECT_EQ(value, std::numeric_limits<int>::max());
}

static_assert(num::checked_add(10, 20, 30) == 60);
static_assert(num::count_digits(12345) == 5);
static_assert(num::one_hot_to_index(0b1000u) == 4);
static_assert(num::saturating_add(10, 20, 30) == 60);
static_assert(num::saturating_add(std::numeric_limits<int>::max(), 1) ==
    std::numeric_limits<int>::max());

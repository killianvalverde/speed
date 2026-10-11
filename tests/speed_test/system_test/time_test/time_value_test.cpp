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
 * @file time_value_test.cpp
 * @brief Unit tests for the time_value class.
 * @author Killian Valverde
 * @date 2026-10-10
 */

#include <gtest/gtest.h>

#include "speed/system/time/time.hpp"

namespace tme = speed::system::time;

TEST(system_time_time_value, constructs_and_normalizes_correctly)
{
    const tme::time_value zero;
    EXPECT_TRUE(zero.is_zero());

    const tme::time_value value(2, 1'500'000'000);
    EXPECT_EQ(value.get_seconds(), 3u);
    EXPECT_EQ(value.get_nanoseconds(), 500'000'000u);
}

TEST(system_time_time_value, compares_values_correctly)
{
    const tme::time_value a(2, 500'000'000);
    const tme::time_value b(3, 500'000'000);

    EXPECT_EQ(a, tme::time_value(2, 500'000'000));
    EXPECT_NE(a, b);
    EXPECT_LT(a, b);
    EXPECT_LE(a, b);
    EXPECT_GT(b, a);
    EXPECT_GE(b, a);
}

TEST(system_time_time_value, performs_arithmetic_correctly)
{
    const tme::time_value a(3, 500'000'000);
    const tme::time_value b(1, 750'000'000);

    EXPECT_EQ(a + b, tme::time_value(5, 250'000'000));
    EXPECT_EQ(a - b, tme::time_value(1, 750'000'000));
    EXPECT_EQ(b - a, tme::time_value{});
}

TEST(system_time_time_value, performs_compound_arithmetic_correctly)
{
    tme::time_value value(3, 500'000'000);
    const tme::time_value other(1, 750'000'000);

    value += other;
    EXPECT_EQ(value, tme::time_value(5, 250'000'000));

    value -= other;
    EXPECT_EQ(value, tme::time_value(3, 500'000'000));

    value -= tme::time_value(10, 0);
    EXPECT_TRUE(value.is_zero());
}

TEST(system_time_time_value, handles_setters_and_reset_correctly)
{
    tme::time_value value;

    value.set_seconds(2);
    value.set_nanoseconds(1'500'000'000);

    EXPECT_EQ(value.get_seconds(), 3u);
    EXPECT_EQ(value.get_nanoseconds(), 500'000'000u);

    value.reset();
    EXPECT_TRUE(value.is_zero());
}

TEST(system_time_time_value, supports_boolean_and_constexpr_operations)
{
    EXPECT_FALSE(static_cast<bool>(tme::time_value{}));
    EXPECT_TRUE(static_cast<bool>(tme::time_value(1, 0)));

    static_assert(tme::time_value(1, 1'500'000'000) == tme::time_value(2, 500'000'000));
    static_assert(tme::time_value(3, 0) + tme::time_value(2, 0) == tme::time_value(5, 0));
}

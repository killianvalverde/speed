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
 * @file calendar_time_test.cpp
 * @brief Unit tests for the calendar_time class.
 * @author Killian Valverde
 * @date 2026-10-10
 */

#include <gtest/gtest.h>

#include "speed/system/time/time.hpp"

namespace tme = speed::system::time;

TEST(system_time_calendar_time, constructs_correctly)
{
    const tme::calendar_time default_value;
    EXPECT_EQ(default_value, tme::calendar_time(1970, 1, 1, 0, 0, 0, 0));

    const tme::calendar_time value(2026, 10, 10, 15, 30, 45, 500);
    EXPECT_EQ(value.get_year(), 2026);
    EXPECT_EQ(value.get_month(), 10);
    EXPECT_EQ(value.get_day(), 10);
    EXPECT_EQ(value.get_hour(), 15);
    EXPECT_EQ(value.get_minute(), 30);
    EXPECT_EQ(value.get_second(), 45);
    EXPECT_EQ(value.get_millisecond(), 500);
}

TEST(system_time_calendar_time, compares_values_correctly)
{
    const tme::calendar_time a(2025, 10, 10, 12, 0, 0, 0);
    const tme::calendar_time b(2026, 10, 10, 12, 0, 0, 0);

    EXPECT_EQ(a, tme::calendar_time(2025, 10, 10, 12, 0, 0, 0));
    EXPECT_NE(a, b);
    EXPECT_LT(a, b);
    EXPECT_LE(a, b);
    EXPECT_GT(b, a);
    EXPECT_GE(b, a);
}

TEST(system_time_calendar_time, validates_dates_correctly)
{
    EXPECT_TRUE(tme::calendar_time(2026, 10, 10, 23, 59, 59, 999).is_valid());
    EXPECT_TRUE(tme::calendar_time(2024, 2, 29, 0, 0, 0, 0).is_valid());

    EXPECT_FALSE(tme::calendar_time(2025, 2, 29, 0, 0, 0, 0).is_valid());
    EXPECT_FALSE(tme::calendar_time(2026, 4, 31, 0, 0, 0, 0).is_valid());
    EXPECT_FALSE(tme::calendar_time(2026, 13, 1, 0, 0, 0, 0).is_valid());
    EXPECT_FALSE(tme::calendar_time(2026, 1, 1, 24, 0, 0, 0).is_valid());
    EXPECT_FALSE(tme::calendar_time(2026, 1, 1, 0, 60, 0, 0).is_valid());
    EXPECT_FALSE(tme::calendar_time(2026, 1, 1, 0, 0, 60, 0).is_valid());
    EXPECT_FALSE(tme::calendar_time(2026, 1, 1, 0, 0, 0, 1000).is_valid());
}

TEST(system_time_calendar_time, handles_setters_correctly)
{
    tme::calendar_time value;

    value.set_year(2026);
    value.set_month(10);
    value.set_day(10);
    value.set_hour(15);
    value.set_minute(30);
    value.set_second(45);
    value.set_millisecond(500);

    EXPECT_EQ(value, tme::calendar_time(2026, 10, 10, 15, 30, 45, 500));
    EXPECT_TRUE(value.is_valid());
}

TEST(system_time_calendar_time, supports_constexpr_operations)
{
    static_assert(tme::calendar_time(2024, 2, 29, 0, 0, 0, 0).is_valid());
    static_assert(!tme::calendar_time(2025, 2, 29, 0, 0, 0, 0).is_valid());

    static_assert(
        tme::calendar_time(2025, 1, 1, 0, 0, 0, 0) <
        tme::calendar_time(2026, 1, 1, 0, 0, 0, 0));
}

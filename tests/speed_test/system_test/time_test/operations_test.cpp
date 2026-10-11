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
 * @brief Unit tests for system::time operations.
 * @author Killian Valverde
 * @date 2026-10-10
 */

#include <chrono>
#include <gtest/gtest.h>

#include "speed/system/time/time.hpp"

namespace tme = speed::system::time;

TEST(system_time_get_monotonic_time, retrieves_monotonic_time_correctly)
{
    tme::time_value first;
    tme::time_value second;
    std::error_code err_code;

    ASSERT_TRUE(speed::system::time::get_monotonic_time(first, &err_code));
    EXPECT_FALSE(err_code);

    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    ASSERT_TRUE(speed::system::time::get_monotonic_time(second, &err_code));
    EXPECT_FALSE(err_code);

    EXPECT_GE(second, first);
}

TEST(system_time_get_cpu_time, retrieves_cpu_time_correctly)
{
    tme::time_value first;
    tme::time_value second;
    std::error_code err_code;

    ASSERT_TRUE(speed::system::time::get_cpu_time(first, &err_code));
    EXPECT_FALSE(err_code);

    volatile std::uint64_t counter = 0;
    for (std::uint64_t i = 0; i < 1'000'000; ++i)
    {
        counter = counter + 1;
    }

    ASSERT_TRUE(speed::system::time::get_cpu_time(second, &err_code));
    EXPECT_FALSE(err_code);

    EXPECT_GE(second, first);
}

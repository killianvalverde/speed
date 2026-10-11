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
 * @file cpu_chrono_test.cpp
 * @brief Unit tests for cpu_chrono.
 * @author Killian Valverde
 * @date 2018-06-06
 */

#include <chrono>
#include <gtest/gtest.h>

#include "speed/time/time.hpp"

template <typename Duration>
void busy_wait_for(Duration duration)
{
    using clock = std::chrono::steady_clock;

    const auto start = clock::now();
    volatile std::uint64_t sink = 0;

    while (clock::now() - start < duration)
    {
        for (int i = 0; i < 1000; ++i)
        {
            sink += static_cast<std::uint64_t>(i) * i;
        }
    }
}

TEST(time_cpu_chrono, initial_sets_ready_and_elapsed_time_is_null)
{
    speed::time::cpu_chrono chrono;
    
    auto elapsed_time = chrono.get_elapsed_time();
    
    ASSERT_TRUE(chrono.is_ready());
    ASSERT_TRUE(elapsed_time.is_zero());
}

TEST(time_cpu_chrono, start_sets_running_and_elapsed_time_increases)
{
    speed::time::cpu_chrono chrono;

    chrono.start();

    ASSERT_TRUE(chrono.is_running());

    busy_wait_for(std::chrono::milliseconds(20));
    auto elapsed_time = chrono.get_elapsed_time();

    ASSERT_FALSE(elapsed_time.is_zero());
}

TEST(time_cpu_chrono, stop_sets_stopped_and_freezes_elapsed_time)
{
    speed::time::cpu_chrono chrono;

    chrono.start();
    busy_wait_for(std::chrono::milliseconds(20));

    auto time_before_stop = chrono.get_elapsed_time();
    ASSERT_FALSE(time_before_stop.is_zero());

    chrono.stop();
    ASSERT_TRUE(chrono.is_stopped());

    auto time_after_stop_1 = chrono.get_elapsed_time();
    busy_wait_for(std::chrono::milliseconds(20));
    auto time_after_stop_2 = chrono.get_elapsed_time();

    ASSERT_FALSE(time_after_stop_1.is_zero());
    ASSERT_EQ(time_after_stop_1, time_after_stop_2);

    chrono.stop();
    ASSERT_TRUE(chrono.is_stopped());
}

TEST(time_cpu_chrono, resume_sets_running_and_elapsed_time_continues)
{
    speed::time::cpu_chrono chrono;

    chrono.start();
    chrono.stop();
    ASSERT_TRUE(chrono.is_stopped());
    auto time_stopped = chrono.get_elapsed_time();

    chrono.resume();
    ASSERT_TRUE(chrono.is_running());

    busy_wait_for(std::chrono::milliseconds(20));
    auto time_resumed = chrono.get_elapsed_time();

    ASSERT_GT(time_resumed, time_stopped);
}

TEST(time_cpu_chrono, restard_sets_running_and_elapsed_time_resets)
{
    speed::time::cpu_chrono chrono;

    chrono.start();
    busy_wait_for(std::chrono::milliseconds(50));

    chrono.stop();
    ASSERT_TRUE(chrono.is_stopped());
    auto time_stopped = chrono.get_elapsed_time();

    chrono.restart();
    busy_wait_for(std::chrono::milliseconds(30));
    ASSERT_TRUE(chrono.is_running());

    auto time_after_restart = chrono.get_elapsed_time();
    ASSERT_FALSE(time_after_restart.is_zero());
    ASSERT_LT(time_after_restart, time_stopped);

    busy_wait_for(std::chrono::milliseconds(30));
    auto time_later = chrono.get_elapsed_time();
    ASSERT_GT(time_later, time_after_restart);
}

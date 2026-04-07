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
 * @file        monotonic_chrono_test.cpp
 * @brief       monotonic_chrono unit test.
 * @author      Killian Valverde
 * @date        2018/06/06
 */

#include <chrono>
#include <gtest/gtest.h>

#include "speed/time/time.hpp"

TEST(time_monotonic_chrono, initial_sets_ready_and_elapsed_time_is_null)
{
    speed::time::monotonic_chrono cpu_chron;
    
    auto tme = cpu_chron.get_elapsed_time();
    
    ASSERT_TRUE(cpu_chron.is_ready());
    ASSERT_TRUE(tme.is_zero());
}

TEST(time_monotonic_chrono, start_sets_running_and_elapsed_time_increases)
{
    speed::time::monotonic_chrono cpu_chrn;

    cpu_chrn.start();

    ASSERT_TRUE(cpu_chrn.is_running());

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    auto tme = cpu_chrn.get_elapsed_time();

    ASSERT_FALSE(tme.is_zero());
}

TEST(time_monotonic_chrono, stop_sets_stopped_and_freezes_elapsed_time)
{
    speed::time::monotonic_chrono cpu_chron;

    cpu_chron.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    auto time_before_stop = cpu_chron.get_elapsed_time();
    ASSERT_FALSE(time_before_stop.is_zero());

    cpu_chron.stop();
    ASSERT_TRUE(cpu_chron.is_stopped());

    auto time_after_stop_1 = cpu_chron.get_elapsed_time();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    auto time_after_stop_2 = cpu_chron.get_elapsed_time();

    ASSERT_FALSE(time_after_stop_1.is_zero());
    ASSERT_EQ(time_after_stop_1, time_after_stop_2);

    cpu_chron.stop();
    ASSERT_TRUE(cpu_chron.is_stopped());
}

TEST(time_monotonic_chrono, resume_sets_running_and_elapsed_time_continues)
{
    speed::time::monotonic_chrono cpu_chron;

    cpu_chron.start();
    cpu_chron.stop();
    ASSERT_TRUE(cpu_chron.is_stopped());
    auto time_stopped = cpu_chron.get_elapsed_time();

    cpu_chron.resume();
    ASSERT_TRUE(cpu_chron.is_running());

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    auto time_resumed = cpu_chron.get_elapsed_time();

    ASSERT_GT(time_resumed, time_stopped);
}

TEST(time_monotonic_chrono, restard_sets_running_and_elapsed_time_resets)
{
    speed::time::monotonic_chrono cpu_chron;

    cpu_chron.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    cpu_chron.stop();
    ASSERT_TRUE(cpu_chron.is_stopped());
    auto time_stopped = cpu_chron.get_elapsed_time();

    cpu_chron.restart();
    ASSERT_TRUE(cpu_chron.is_running());

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    auto time_after_restart = cpu_chron.get_elapsed_time();
    ASSERT_FALSE(time_after_restart.is_zero());
    ASSERT_LT(time_after_restart, time_stopped);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    auto time_later = cpu_chron.get_elapsed_time();
    ASSERT_GT(time_later, time_after_restart);
}

/* speed - Generic C++ library.
 * Copyright (C) 2015-2024 Killian Valverde.
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

#include <cmath>
#include <gtest/gtest.h>

#include "speed/time/time.hpp"

TEST(time_monotonic_chrono, default_constructor)
{
    speed::time::monotonic_chrono monotc_chrn;
    
    auto tme = monotc_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(monotc_chrn.is(speed::time::cs_t::READY));
    ASSERT_TRUE(tme.is_null());
}

TEST(time_monotonic_chrono, start)
{
    speed::time::monotonic_chrono monotc_chrn;
    
    ASSERT_TRUE(monotc_chrn.start());
    
    speed::system::process::nanosleep(0, 1000);
    auto tme = monotc_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(monotc_chrn.is(speed::time::cs_t::RUNNING));
    ASSERT_TRUE(!tme.is_null());
}

TEST(time_monotonic_chrono, stop)
{
    speed::time::monotonic_chrono monotc_chrn;
    
    ASSERT_TRUE(monotc_chrn.start());
    
    speed::system::process::nanosleep(0, 1000);
    
    ASSERT_TRUE(monotc_chrn.stop());
    ASSERT_TRUE(monotc_chrn.is(speed::time::cs_t::STOPED));
    
    auto tme_1 = monotc_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(!tme_1.is_null());
    
    speed::system::process::nanosleep(0, 1000);
    auto tme_2 = monotc_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(monotc_chrn.is(speed::time::cs_t::STOPED));
    ASSERT_TRUE(tme_1 == tme_2);
}

TEST(time_monotonic_chrono, resume)
{
    speed::time::monotonic_chrono monotc_chrn;
    
    ASSERT_TRUE(monotc_chrn.start());
    
    speed::system::process::nanosleep(0, 1000);
    
    ASSERT_TRUE(monotc_chrn.stop());
    ASSERT_TRUE(monotc_chrn.is(speed::time::cs_t::STOPED));
    
    auto tme_1 = monotc_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(!tme_1.is_null());
    
    speed::system::process::nanosleep(0, 1000);
    auto tme_2 = monotc_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(monotc_chrn.is(speed::time::cs_t::STOPED));
    ASSERT_TRUE(tme_1 == tme_2);
    
    ASSERT_TRUE(monotc_chrn.resume());
    
    speed::system::process::nanosleep(0, 1000);
    auto tme_3 = monotc_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(monotc_chrn.is(speed::time::cs_t::RUNNING));
    ASSERT_TRUE(tme_1 != tme_3);
}

TEST(time_monotonic_chrono, restart)
{
    speed::time::monotonic_chrono monotc_chrn;
    
    ASSERT_TRUE(monotc_chrn.start());
    
    speed::system::process::nanosleep(0, 1000);
    
    ASSERT_TRUE(monotc_chrn.stop());
    ASSERT_TRUE(monotc_chrn.is(speed::time::cs_t::STOPED));
    
    auto tme_1 = monotc_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(!tme_1.is_null());
    
    speed::system::process::nanosleep(0, 1000);
    auto tme_2 = monotc_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(monotc_chrn.is(speed::time::cs_t::STOPED));
    ASSERT_TRUE(tme_1 == tme_2);
    
    ASSERT_TRUE(monotc_chrn.restart());
    
    speed::system::process::nanosleep(0, 1000);
    auto tme_3 = monotc_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(monotc_chrn.is(speed::time::cs_t::RUNNING));
    ASSERT_TRUE(!tme_3.is_null());
}

TEST(time_monotonic_chrono, get_elapsed_time)
{
    speed::time::monotonic_chrono monotc_chrn;
    
    ASSERT_TRUE(monotc_chrn.start());
    
    auto tme_1 = monotc_chrn.get_elapsed_time();
    speed::system::process::nanosleep(0, 1000);
    auto tme_2 = monotc_chrn.get_elapsed_time();
    
    ASSERT_TRUE(tme_1 != tme_2);
}

TEST(time_monotonic_chrono, get_elapsed_raw_time)
{
    speed::time::monotonic_chrono monotc_chrn;
    
    ASSERT_TRUE(monotc_chrn.start());
    
    auto tme_1 = monotc_chrn.get_elapsed_raw_time();
    speed::system::process::nanosleep(0, 1000);
    auto tme_2 = monotc_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(tme_1 != tme_2);
}

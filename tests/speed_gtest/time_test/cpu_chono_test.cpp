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
 * @file        speed_gtest/system_test/cpu_chono_test.cpp
 * @brief       cpu_chono unit test.
 * @author      Killian Valverde
 * @date        2018/06/06
 */

#include <cmath>

#include <gtest/gtest.h>

#include "speed/time/time.hpp"


TEST(time_cpu_chono, default_constructor)
{
    speed::time::cpu_chrono cpu_chrn;
    
    auto tme = cpu_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::READY));
    ASSERT_TRUE(tme.is_null());
}


TEST(time_cpu_chono, start)
{
    speed::time::cpu_chrono cpu_chrn;
    std::size_t i;
    
    ASSERT_TRUE(cpu_chrn.start());
    
    for (i = static_cast<std::size_t>(pow(10, 8)); i > 0; --i)
    {
    }
    auto tme = cpu_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::RUNNING));
    ASSERT_TRUE(!tme.is_null());
    ASSERT_TRUE(i == 0);
}


TEST(time_cpu_chono, stop)
{
    speed::time::cpu_chrono cpu_chrn;
    std::size_t i;
    
    ASSERT_TRUE(cpu_chrn.start());
    
    for (i = static_cast<std::size_t>(pow(10, 8)); i > 0; --i)
    {
    }
    
    ASSERT_TRUE(cpu_chrn.stop());
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::STOPED));
    
    auto tme_1 = cpu_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(!tme_1.is_null());
    
    for (i = static_cast<std::size_t>(pow(10, 8)); i > 0; --i)
    {
    }
    auto tme_2 = cpu_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::STOPED));
    ASSERT_TRUE(tme_1 == tme_2);
    ASSERT_TRUE(i == 0);
}


TEST(time_cpu_chono, resume)
{
    speed::time::cpu_chrono cpu_chrn;
    std::size_t i;
    
    ASSERT_TRUE(cpu_chrn.start());
    
    for (i = static_cast<std::size_t>(pow(10, 8)); i > 0; --i)
    {
    }
    
    ASSERT_TRUE(cpu_chrn.stop());
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::STOPED));
    
    auto tme_1 = cpu_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(!tme_1.is_null());
    
    for (i = static_cast<std::size_t>(pow(10, 8)); i > 0; --i)
    {
    }
    auto tme_2 = cpu_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::STOPED));
    ASSERT_TRUE(tme_1 == tme_2);
    
    ASSERT_TRUE(cpu_chrn.resume());
    
    for (i = static_cast<std::size_t>(pow(10, 8)); i > 0; --i)
    {
    }
    auto tme_3 = cpu_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::RUNNING));
    ASSERT_TRUE(tme_1 != tme_3);
    ASSERT_TRUE(i == 0);
}


TEST(time_cpu_chono, restart)
{
    speed::time::cpu_chrono cpu_chrn;
    std::size_t i;
    
    ASSERT_TRUE(cpu_chrn.start());
    
    for (i = static_cast<std::size_t>(pow(10, 8)); i > 0; --i)
    {
    }
    
    ASSERT_TRUE(cpu_chrn.stop());
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::STOPED));
    
    auto tme_1 = cpu_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(!tme_1.is_null());
    
    for (i = static_cast<std::size_t>(pow(10, 8)); i > 0; --i)
    {
    }
    auto tme_2 = cpu_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::STOPED));
    ASSERT_TRUE(tme_1 == tme_2);
    
    ASSERT_TRUE(cpu_chrn.restart());
    
    for (i = static_cast<std::size_t>(pow(10, 8)); i > 0; --i)
    {
    }
    auto tme_3 = cpu_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::RUNNING));
    ASSERT_TRUE(!tme_3.is_null());
    ASSERT_TRUE(i == 0);
}


TEST(time_cpu_chono, get_elapsed_time)
{
    speed::time::cpu_chrono cpu_chrn;
    std::size_t i;
    
    ASSERT_TRUE(cpu_chrn.start());
    
    auto tme_1 = cpu_chrn.get_elapsed_time();
    for (i = static_cast<std::size_t>(pow(10, 8)); i > 0; --i)
    {
    }
    auto tme_2 = cpu_chrn.get_elapsed_time();
    
    ASSERT_TRUE(tme_1 != tme_2);
    ASSERT_TRUE(i == 0);
}


TEST(time_cpu_chono, get_elapsed_raw_time)
{
    speed::time::cpu_chrono cpu_chrn;
    std::size_t i;
    
    ASSERT_TRUE(cpu_chrn.start());
    
    auto tme_1 = cpu_chrn.get_elapsed_raw_time();
    for (i = static_cast<std::size_t>(pow(10, 8)); i > 0; --i)
    {
    }
    auto tme_2 = cpu_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(tme_1 != tme_2);
    ASSERT_TRUE(i == 0);
}

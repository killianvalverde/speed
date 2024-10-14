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
    speed::system::time::time_specification tme;
    
    ASSERT_TRUE(cpu_chrn.start());

    do
    {
        tme = cpu_chrn.get_elapsed_raw_time();
    } while(tme.is_null());
    
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::RUNNING));
    ASSERT_TRUE(!tme.is_null());
}


TEST(time_cpu_chono, stop)
{
    speed::time::cpu_chrono cpu_chrn;
    speed::system::time::time_specification tme_1;
    speed::system::time::time_specification tme_2;

    ASSERT_TRUE(cpu_chrn.start());

    do
    {
        tme_1 = cpu_chrn.get_elapsed_raw_time();
    } while(tme_1.is_null());
    
    ASSERT_TRUE(cpu_chrn.stop());
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::STOPED));
    ASSERT_TRUE(!tme_1.is_null());

    tme_1 = cpu_chrn.get_elapsed_raw_time();
    tme_2 = cpu_chrn.get_elapsed_raw_time();

    ASSERT_TRUE(!cpu_chrn.stop());
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::STOPED));
    ASSERT_TRUE(!tme_2.is_null());
    ASSERT_TRUE(tme_1 == tme_2);
}


TEST(time_cpu_chono, resume)
{
    speed::time::cpu_chrono cpu_chrn;
    speed::system::time::time_specification tme_1;
    speed::system::time::time_specification tme_2;
    speed::system::time::time_specification tme_3;

    ASSERT_TRUE(cpu_chrn.start());

    do
    {
        tme_1 = cpu_chrn.get_elapsed_raw_time();
    } while(tme_1.is_null());
    
    ASSERT_TRUE(cpu_chrn.stop());
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::STOPED));
    ASSERT_TRUE(!tme_1.is_null());

    tme_1 = cpu_chrn.get_elapsed_raw_time();
    tme_2 = cpu_chrn.get_elapsed_raw_time();

    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::STOPED));
    ASSERT_TRUE(tme_1 == tme_2);
    ASSERT_TRUE(cpu_chrn.resume());
    
    do
    {
        tme_3 = cpu_chrn.get_elapsed_raw_time();
    } while(tme_3 == tme_1);
    
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::RUNNING));
    ASSERT_TRUE(tme_1 != tme_3);
}


TEST(time_cpu_chono, restart)
{
    speed::time::cpu_chrono cpu_chrn;
    speed::system::time::time_specification tme_1;
    speed::system::time::time_specification tme_2;
    speed::system::time::time_specification tme_3;

    ASSERT_TRUE(cpu_chrn.start());

    do
    {
        tme_1 = cpu_chrn.get_elapsed_raw_time();
    } while(tme_1.is_null());
    
    ASSERT_TRUE(cpu_chrn.stop());
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::STOPED));
    ASSERT_TRUE(!tme_1.is_null());

    tme_1 = cpu_chrn.get_elapsed_raw_time();
    tme_2 = cpu_chrn.get_elapsed_raw_time();
    
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::STOPED));
    ASSERT_TRUE(tme_1 == tme_2);
    
    ASSERT_TRUE(cpu_chrn.restart());

    do
    {
        tme_3 = cpu_chrn.get_elapsed_raw_time();
    } while(tme_3.is_null());
    
    ASSERT_TRUE(cpu_chrn.is(speed::time::cs_t::RUNNING));
    ASSERT_TRUE(!tme_3.is_null());
}


TEST(time_cpu_chono, get_elapsed_time)
{
    speed::time::cpu_chrono cpu_chrn;
    long double tme_1;
    long double tme_2;

    ASSERT_TRUE(cpu_chrn.start());

    do
    {
        tme_1 = cpu_chrn.get_elapsed_time();
    } while(tme_1 == 0.0L);

    do
    {
        tme_2 = cpu_chrn.get_elapsed_time();
    } while(tme_2 == tme_1);
    
    ASSERT_TRUE(tme_1 != tme_2);
}


TEST(time_cpu_chono, get_elapsed_raw_time)
{
    speed::time::cpu_chrono cpu_chrn;
    speed::system::time::time_specification tme_1;
    speed::system::time::time_specification tme_2;
    
    ASSERT_TRUE(cpu_chrn.start());
    
    tme_1 = cpu_chrn.get_elapsed_raw_time();

    do
    {
        tme_2 = cpu_chrn.get_elapsed_raw_time();
    } while(tme_2 == tme_1);
    
    ASSERT_TRUE(tme_1 != tme_2);
}

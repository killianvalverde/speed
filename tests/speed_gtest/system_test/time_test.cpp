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
 * @file        time_test.cpp
 * @brief       time unit test.
 * @author      Killian Valverde
 * @date        2018/06/01
 */

#include <gtest/gtest.h>

#include "speed/system/system.hpp"

TEST(system_time, get_elapsed_time)
{
    speed::system::time::time_specification tme_spec_1(1, 0);
    speed::system::time::time_specification tme_spec_2(3, 0);
    speed::system::time::time_specification res = tme_spec_1.get_elapsed_time(tme_spec_2);
    
    ASSERT_TRUE(res.get_seconds() == 2);
    ASSERT_TRUE(res.get_nseconds() == 0);
}

TEST(system_time, get_monotonic_time)
{
    speed::system::time::time_specification tme_spec1;
    speed::system::time::time_specification tme_spec2;

    ASSERT_TRUE(speed::system::time::get_monotonic_time(&tme_spec1));

    do
    {
        speed::system::time::get_monotonic_time(&tme_spec2);
    } while (tme_spec1 == tme_spec2);

    ASSERT_TRUE(tme_spec1 != tme_spec2);
}

TEST(system_time, get_cpu_time)
{
    speed::system::time::time_specification tme_spec1;
    speed::system::time::time_specification tme_spec2;

    ASSERT_TRUE(speed::system::time::get_cpu_time(&tme_spec1));

    do
    {
        speed::system::time::get_cpu_time(&tme_spec2);
    } while (tme_spec1 == tme_spec2);

    ASSERT_TRUE(tme_spec1 != tme_spec2);
}

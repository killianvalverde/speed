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
 * @file        speed_gtest/system_test/process_test.cpp
 * @brief       process unit test.
 * @author      Killian Valverde
 * @date        2018/06/01
 */

#include <gtest/gtest.h>

#include "speed/system/system.hpp"


TEST(system_process, execute_command)
{
    // int ret_val;
    // ASSERT_TRUE(speed::system::process::execute_command("dir", &ret_val));
    // ASSERT_TRUE(ret_val == 0);
}


TEST(system_process, get_pid)
{
    ASSERT_TRUE(speed::system::process::get_pid() != -1);
}


TEST(system_process, get_ppid)
{
    ASSERT_TRUE(speed::system::process::get_ppid() != -1);
}


TEST(system_process, get_uid)
{
    ASSERT_TRUE(speed::system::process::get_uid() != -1);
}


TEST(system_process, get_gid)
{
    ASSERT_TRUE(speed::system::process::get_gid() != -1);
}


TEST(system_process, nanosleep_1)
{
    ASSERT_TRUE(speed::system::process::nanosleep(0, 1));
}


TEST(system_process, nanosleep_2)
{
    ASSERT_TRUE(speed::system::process::nanosleep(speed::system::time::time_specification(0, 1)));
}

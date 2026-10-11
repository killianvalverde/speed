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
 * @brief Unit tests for system::process operations.
 * @author Killian Valverde
 * @date 2026-10-11
 */

#include <gtest/gtest.h>

#include "speed/system/process/process.hpp"

namespace proc = speed::system::process;

TEST(system_process_execute, successful_command)
{
    proc::execution_result res;
    std::error_code err_code;

#ifdef SPEED_WINAPI
    const bool result = proc::execute("cmd /c exit 0", &res, &err_code);
#else
    const bool result = proc::execute("/bin/true", &res, &err_code);
#endif

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_EQ(res.exit_code, 0);
}

TEST(system_process_execute, unsuccessful_command)
{
    proc::execution_result res;
    std::error_code err_code;

#ifdef SPEED_WINAPI
    const bool result = proc::execute("cmd /c exit 1", &res, &err_code);
#else
    const bool result = proc::execute("/bin/false", &res, &err_code);
#endif

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_EQ(res.exit_code, 1);
}

TEST(system_process_execute, rejects_empty_command)
{
    std::error_code err_code;

    const bool result = proc::execute("", nullptr, &err_code);

    EXPECT_FALSE(result);
    EXPECT_TRUE(err_code);
}

TEST(system_process_get_pid, returns_valid_process_id)
{
    const auto pid = proc::get_pid();

    EXPECT_GT(pid, 0);
}

TEST(system_process_get_uid, returns_valid_user_id)
{
    const auto uid = proc::get_uid();

    EXPECT_FALSE(uid.is_empty());
    EXPECT_TRUE(static_cast<bool>(uid));
}

TEST(system_process_nanosleep, sleeps_for_requested_duration)
{
    std::error_code err_code;

    const auto start = std::chrono::steady_clock::now();

    const bool result = proc::nanosleep(0, 100'000'000, &err_code);

    const auto elapsed = std::chrono::steady_clock::now() - start;

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_GE(elapsed, std::chrono::milliseconds(100));
}

TEST(system_process_nanosleep, rejects_invalid_nanoseconds)
{
    std::error_code err_code;

    const bool result = proc::nanosleep(0, 1'000'000'000, &err_code);

    EXPECT_FALSE(result);
    EXPECT_TRUE(err_code);
}

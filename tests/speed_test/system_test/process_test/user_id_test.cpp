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
 * @brief Unit test for the user_id class.
 * @author Killian Valverde
 * @date 2026-10-11
 */

#include <gtest/gtest.h>

#include "speed/system/process/process.hpp"

namespace proc = speed::system::process;

TEST(system_process_user_id, default_constructor)
{
    proc::user_id uid;

    EXPECT_TRUE(uid.is_empty());
    EXPECT_FALSE(static_cast<bool>(uid));
}

TEST(system_process_user_id, native_constructor)
{
#ifdef SPEED_WINAPI
    const proc::user_id uid(L"S-1-5-18");
    EXPECT_EQ(uid.get_native(), L"S-1-5-18");
#else
    const proc::user_id uid(static_cast<proc::user_id::native_type>(1000));
    EXPECT_EQ(uid.get_native(), 1000);
#endif

    EXPECT_FALSE(uid.is_empty());
    EXPECT_TRUE(static_cast<bool>(uid));
}

TEST(system_process_user_id, assign_native_identifier)
{
    proc::user_id uid;

#ifdef SPEED_WINAPI
    const bool result = uid.assign(std::wstring(L"S-1-5-18"));
    EXPECT_EQ(uid.get_native(), L"S-1-5-18");
#else
    const bool result = uid.assign(static_cast<proc::user_id::native_type>(1000));
    EXPECT_EQ(uid.get_native(), 1000);
#endif

    EXPECT_TRUE(result);
    EXPECT_FALSE(uid.is_empty());
}

TEST(system_process_user_id, compare_identifiers)
{
#ifdef SPEED_WINAPI
    const proc::user_id uid1(L"S-1-5-18");
    const proc::user_id uid2(L"S-1-5-18");
    const proc::user_id uid3(L"S-1-5-19");
#else
    const proc::user_id uid1(static_cast<proc::user_id::native_type>(1000));
    const proc::user_id uid2(static_cast<proc::user_id::native_type>(1000));
    const proc::user_id uid3(static_cast<proc::user_id::native_type>(1001));
#endif

    EXPECT_EQ(uid1, uid2);
    EXPECT_NE(uid1, uid3);
    EXPECT_LT(uid1, uid3);
    EXPECT_GT(uid3, uid1);
}

TEST(system_process_user_id, convert_to_string)
{
#ifdef SPEED_WINAPI
    const proc::user_id uid(L"S-1-5-18");
    EXPECT_EQ(uid.to_string(), "S-1-5-18");
#else
    const proc::user_id uid(static_cast<proc::user_id::native_type>(1000));
    EXPECT_EQ(uid.to_string(), "1000");
#endif
}

TEST(system_process_user_id, assign_empty_identifier)
{
    proc::user_id uid;

#ifdef SPEED_WINAPI
    ASSERT_TRUE(uid.assign(L"S-1-5-18"));
    ASSERT_TRUE(uid.assign(std::wstring{}));
#else
    ASSERT_TRUE(uid.assign(static_cast<proc::user_id::native_type>(1000)));
    ASSERT_TRUE(uid.assign(std::numeric_limits<proc::user_id::native_type>::max()));
#endif

    EXPECT_TRUE(uid.is_empty());
    EXPECT_FALSE(static_cast<bool>(uid));
}

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
 * @brief Unit tests for system::errors operations.
 * @author Killian Valverde
 * @date 2026-10-11
 */

#include <gtest/gtest.h>

#include "speed/system/errors/errors.hpp"

namespace err = speed::system::errors;

TEST(system_errors_assign_system_error_code, assigns_system_category)
{
    std::error_code err_code;

    err::assign_system_error_code(5, &err_code);

    EXPECT_EQ(err_code.value(), 5);
    EXPECT_EQ(err_code.category(), std::system_category());
}

TEST(system_errors_assign_system_error_code, accepts_null_pointer)
{
    err::assign_system_error_code(5, nullptr);

    SUCCEED();
}

TEST(system_errors_assign_generic_error_code, assigns_generic_category)
{
    std::error_code err_code;

    err::assign_generic_error_code(22, &err_code);

    EXPECT_EQ(err_code.value(), 22);
    EXPECT_EQ(err_code.category(), std::generic_category());
}

TEST(system_errors_assign_generic_error_code, accepts_null_pointer)
{
    err::assign_generic_error_code(22, nullptr);

    SUCCEED();
}

TEST(system_errors_assign_errno_error_code, assigns_errno_value)
{
    std::error_code err_code;

    err::assign_errno_error_code(EINVAL, &err_code);

    EXPECT_EQ(err_code.value(), EINVAL);
    EXPECT_EQ(err_code.category(), std::generic_category());
}

TEST(system_errors_assign_errno_error_code, accepts_null_pointer)
{
    err::assign_errno_error_code(EINVAL, nullptr);

    SUCCEED();
}

TEST(system_errors_assign_errc, assigns_standard_error_condition)
{
    std::error_code err_code;

    err::assign_errc(std::errc::invalid_argument, &err_code);

    EXPECT_EQ(err_code, std::make_error_code(std::errc::invalid_argument));
    EXPECT_EQ(err_code.category(), std::generic_category());
}

TEST(system_errors_assign_errc, accepts_null_pointer)
{
    err::assign_errc(std::errc::invalid_argument, nullptr);

    SUCCEED();
}

TEST(system_errors_assign_error_code, copies_error_code)
{
    const std::error_code source(5, std::system_category());
    std::error_code destination;

    err::assign_error_code(source, &destination);

    EXPECT_EQ(destination, source);
    EXPECT_EQ(destination.category(), std::system_category());
}

TEST(system_errors_assign_error_code, accepts_null_pointer)
{
    const std::error_code source(5, std::system_category());

    err::assign_error_code(source, nullptr);

    SUCCEED();
}

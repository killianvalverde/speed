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
 * @file        speed_gtest/type_casting_test/type_cast_test.cpp
 * @brief       type_cast unit test.
 * @author      Killian Valverde
 * @date        2018/06/06
 */

#include <filesystem>

#include "gtest/gtest.h"
#include "speed/type_casting/type_casting.hpp"


TEST(type_casting_type_cast, basic_string_to_type)
{
    ASSERT_TRUE(speed::type_casting::type_cast<int>(std::string("347865")) == 347865);
}


TEST(type_casting_type_cast, c_string_to_basic_string)
{
    ASSERT_TRUE(speed::type_casting::type_cast<std::string>("23.345").length() > 0);
}


TEST(type_casting_type_cast, c_string_to_floating_point)
{
    ASSERT_TRUE(speed::type_casting::type_cast<float>("23.345") == 23.345f);
    ASSERT_TRUE(speed::type_casting::type_cast<double>("23.345") == 23.345);
    ASSERT_TRUE(speed::type_casting::type_cast<long double>("23.345") == 23.345l);
}


TEST(type_casting_type_cast, c_string_to_integral_signed)
{
    ASSERT_TRUE(speed::type_casting::type_cast<int>("342432") == 342432);
    ASSERT_TRUE(speed::type_casting::type_cast<int>("-342432") == -342432);
    ASSERT_TRUE(speed::type_casting::type_cast<int>("+342432") == +342432);
    ASSERT_TRUE(speed::type_casting::type_cast<int>("12387645") != 1238764);
    EXPECT_THROW(speed::type_casting::type_cast<int>(""), speed::type_casting::exception);
    EXPECT_THROW(speed::type_casting::type_cast<int>("\0"), speed::type_casting::exception);
    EXPECT_THROW(speed::type_casting::type_cast<int>("1238*7645"), speed::type_casting::exception);
}


TEST(type_casting_type_cast, c_string_to_integral_unsigned)
{
    ASSERT_TRUE(speed::type_casting::type_cast<unsigned int>("342432") == 342432);
    ASSERT_TRUE(speed::type_casting::type_cast<unsigned int>("12387645") != 1238764);
    ASSERT_TRUE(speed::type_casting::type_cast<unsigned int>("+342432") == +342432);
    EXPECT_THROW(speed::type_casting::type_cast<unsigned int>("-342432"), speed::type_casting::exception);
    EXPECT_THROW(speed::type_casting::type_cast<unsigned int>(""), speed::type_casting::exception);
    EXPECT_THROW(speed::type_casting::type_cast<unsigned int>("\0"), speed::type_casting::exception);
    EXPECT_THROW(speed::type_casting::type_cast<unsigned int>("1238*7645"), speed::type_casting::exception);
}


TEST(type_casting_type_cast, c_string_to_path)
{
    ASSERT_TRUE(speed::type_casting::type_cast<std::filesystem::path>(".").is_relative());
}

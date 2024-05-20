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
 * @file        speed_gtest/type_casting_test/try_type_cast_test.cpp
 * @brief       try_type_cast unit test.
 * @author      Killian Valverde
 * @date        2018/06/07
 */

#include <filesystem>

#include "gtest/gtest.h"
#include "speed/type_casting/type_casting.hpp"


TEST(type_casting_try_type_cast, basic_string_to_type)
{
    int res;
    ASSERT_TRUE(speed::type_casting::try_type_cast<int>(std::string("347865"), &res));
}


TEST(type_casting_try_type_cast, c_string_to_basic_string)
{
    std::string res;
    ASSERT_TRUE(speed::type_casting::try_type_cast<std::string>("23.345", &res));
}


TEST(type_casting_try_type_cast, c_string_to_floating_point)
{
    float res_f;
    double res_d;
    long double res_ld;
    
    ASSERT_TRUE(speed::type_casting::try_type_cast<float>("23.345", &res_f));
    ASSERT_TRUE(speed::type_casting::try_type_cast<double>("23.345", &res_d));
    ASSERT_TRUE(speed::type_casting::try_type_cast<long double>("23.345", &res_ld));
}


TEST(type_casting_try_type_cast, c_string_to_integral_signed)
{
    int res;
    
    ASSERT_TRUE(speed::type_casting::try_type_cast<int>("342432", &res));
    ASSERT_TRUE(speed::type_casting::try_type_cast<int>("-342432", &res));
    ASSERT_TRUE(speed::type_casting::try_type_cast<int>("+342432", &res));
    ASSERT_TRUE(speed::type_casting::try_type_cast<int>("12387645", &res));
    ASSERT_TRUE(!speed::type_casting::try_type_cast<int>("", &res));
    ASSERT_TRUE(!speed::type_casting::try_type_cast<int>("\0", &res));
    ASSERT_TRUE(!speed::type_casting::try_type_cast<int>("1238*7645", &res));
}


TEST(type_casting_try_type_cast, c_string_to_integral_unsigned)
{
    unsigned int res;
    
    ASSERT_TRUE(speed::type_casting::try_type_cast<unsigned int>("342432", &res));
    ASSERT_TRUE(speed::type_casting::try_type_cast<unsigned int>("12387645", &res));
    ASSERT_TRUE(speed::type_casting::try_type_cast<unsigned int>("+342432", &res));
    ASSERT_TRUE(!speed::type_casting::try_type_cast<unsigned int>("-342432", &res));
    ASSERT_TRUE(!speed::type_casting::try_type_cast<unsigned int>("", &res));
    ASSERT_TRUE(!speed::type_casting::try_type_cast<unsigned int>("\0", &res));
    ASSERT_TRUE(!speed::type_casting::try_type_cast<unsigned int>("1238*7645", &res));
}


TEST(type_casting_try_type_cast, c_string_to_path)
{
    std::filesystem::path pth;
    
    ASSERT_TRUE(speed::type_casting::try_type_cast<std::filesystem::path>(".", &pth));
}

/* Copyright (C) 2017 Killian Valverde.
   This file is part of speed.

   speed is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   speed is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with speed. If not, see <http://www.gnu.org/licenses/>. */

/**
 * @file        speed_test/stringutils_test/string_utils_test.cpp
 * @brief       string_utils unit test.
 * @author      Killian Valverde
 * @date        2018/06/07
 */

#include "gtest/gtest.h"
#include "speed/stringutils/stringutils.hpp"


TEST(stringutils_string_utils, get_string_length)
{
    const char* str1 = "12345";
    const wchar_t* str2 = L"12345";
    std::string str3 = "12345";
    std::wstring str4 = L"12345";
    
    EXPECT_TRUE(speed::stringutils::get_string_length(str1) == 5);
    EXPECT_TRUE(speed::stringutils::get_string_length(str2) == 5);
    EXPECT_TRUE(speed::stringutils::get_string_length(str3) == 5);
    EXPECT_TRUE(speed::stringutils::get_string_length(str4) == 5);
}

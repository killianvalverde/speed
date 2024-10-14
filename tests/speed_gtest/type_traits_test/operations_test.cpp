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
 * @file        speed_gtest/type_traits_test/operations_test.cpp
 * @brief       operations unit test.
 * @author      Killian Valverde
 * @date        2018/06/08
 */

#include <filesystem>

#include <gtest/gtest.h>

#include "speed/type_traits/type_traits.hpp"


TEST(type_traits_operations, is_char)
{
    EXPECT_TRUE(speed::type_traits::is_char<char>::value);
    EXPECT_TRUE(!speed::type_traits::is_char<wchar_t>::value);
}


TEST(type_traits_operations, is_wchar)
{
    EXPECT_TRUE(!speed::type_traits::is_wchar<char>::value);
    EXPECT_TRUE(speed::type_traits::is_wchar<wchar_t>::value);
}


TEST(type_traits_operations, is_character)
{
    EXPECT_TRUE(speed::type_traits::is_character<char>::value);
    EXPECT_TRUE(speed::type_traits::is_character<wchar_t>::value);
    EXPECT_TRUE(speed::type_traits::is_character<char16_t>::value);
    EXPECT_TRUE(speed::type_traits::is_character<char32_t>::value);
    EXPECT_TRUE(!speed::type_traits::is_character<int>::value);
}


TEST(type_traits_operations, is_character_pointer)
{
    EXPECT_TRUE(speed::type_traits::is_character_pointer<char*>::value);
    EXPECT_TRUE(speed::type_traits::is_character_pointer<const wchar_t*>::value);
    EXPECT_TRUE(speed::type_traits::is_character_pointer<volatile char16_t*>::value);
    EXPECT_TRUE(speed::type_traits::is_character_pointer<char32_t*>::value);
    EXPECT_TRUE(!speed::type_traits::is_character_pointer<char>::value);
    EXPECT_TRUE(!speed::type_traits::is_character_pointer<char []>::value);
}


TEST(type_traits_operations, is_stdio_character)
{
    EXPECT_TRUE(speed::type_traits::is_stdio_character<char>::value);
    EXPECT_TRUE(speed::type_traits::is_stdio_character<wchar_t>::value);
    EXPECT_TRUE(!speed::type_traits::is_stdio_character<char16_t>::value);
    EXPECT_TRUE(!speed::type_traits::is_stdio_character<char32_t>::value);
}


TEST(type_traits_operations, is_float)
{
    EXPECT_TRUE(speed::type_traits::is_float<float>::value);
    EXPECT_TRUE(!speed::type_traits::is_float<double>::value);
}


TEST(type_traits_operations, is_double)
{
    EXPECT_TRUE(!speed::type_traits::is_double<float>::value);
    EXPECT_TRUE(speed::type_traits::is_double<double>::value);
}


TEST(type_traits_operations, is_long_double)
{
    EXPECT_TRUE(!speed::type_traits::is_long_double<float>::value);
    EXPECT_TRUE(!speed::type_traits::is_long_double<double>::value);
    EXPECT_TRUE(speed::type_traits::is_long_double<long double>::value);
}


TEST(type_traits_operations, is_basic_string)
{
    EXPECT_TRUE(speed::type_traits::is_basic_string<std::string>::value);
    EXPECT_TRUE(speed::type_traits::is_basic_string<std::wstring>::value);
    EXPECT_TRUE(!speed::type_traits::is_basic_string<int>::value);
}


TEST(type_traits_operations, is_basic_string_vector)
{
    EXPECT_TRUE(speed::type_traits::is_basic_string_vector<std::vector<std::string>>::value);
    EXPECT_TRUE(speed::type_traits::is_basic_string_vector<std::vector<std::wstring>>::value);
    EXPECT_TRUE(!speed::type_traits::is_basic_string_vector<std::vector<int>>::value);
    EXPECT_TRUE(!speed::type_traits::is_basic_string_vector<int>::value);
}


TEST(type_traits_operations, is_basic_ostream)
{
    EXPECT_TRUE(speed::type_traits::is_basic_ostream<std::ostream>::value);
    EXPECT_TRUE(speed::type_traits::is_basic_ostream<std::wostream>::value);
    EXPECT_TRUE(!speed::type_traits::is_basic_ostream<int>::value);
}


TEST(type_traits_operations, is_path)
{
    EXPECT_TRUE(speed::type_traits::is_path<std::filesystem::path>::value);
    EXPECT_TRUE(!speed::type_traits::is_path<int>::value);
}


TEST(type_traits_operations, try_underlying_type)
{
    enum class item : char
    {
        POTION = 'P'
    };
    
    EXPECT_TRUE(speed::type_traits::is_char<
            speed::type_traits::try_underlying_type_t<item>>::value);
    EXPECT_TRUE(speed::type_traits::is_char<
            speed::type_traits::try_underlying_type_t<char>>::value);
    EXPECT_TRUE(!speed::type_traits::is_char<
            speed::type_traits::try_underlying_type_t<int>>::value);
}

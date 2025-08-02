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
 * @file        operations_test.cpp
 * @brief       operations unit test.
 * @author      Killian Valverde
 * @date        2018/06/08
 */

#include <filesystem>
#include <gtest/gtest.h>

#include "speed/type_traits/type_traits.hpp"

TEST(type_traits_operations, character_type_of)
{
    EXPECT_TRUE((std::is_same_v<speed::type_traits::character_type_of_t<
            std::string>, char>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::character_type_of_t<
            std::wstring>, wchar_t>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::character_type_of_t<
            std::string_view>, char>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::character_type_of_t<
            std::wstring_view>, wchar_t>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::character_type_of_t<
            const char*>, char>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::character_type_of_t<
            wchar_t*>, wchar_t>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::character_type_of_t<
            char[5]>, char>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::character_type_of_t
            <const wchar_t[42]>, wchar_t>));
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
    EXPECT_TRUE(!speed::type_traits::is_character_pointer<char*&>::value);
}

TEST(type_traits_operations, is_stdio_character)
{
    EXPECT_TRUE(speed::type_traits::is_stdio_character<char>::value);
    EXPECT_TRUE(speed::type_traits::is_stdio_character<wchar_t>::value);
    EXPECT_TRUE(!speed::type_traits::is_stdio_character<char16_t>::value);
    EXPECT_TRUE(!speed::type_traits::is_stdio_character<char32_t>::value);
}

TEST(type_traits_operations, string_view_of)
{
    EXPECT_TRUE((std::is_same_v<speed::type_traits::string_view_of_t<
            std::string>, std::string_view>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::string_view_of_t<
            std::wstring>, std::wstring_view>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::string_view_of_t<
            std::string&>, std::string_view>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::string_view_of_t<
            const std::wstring&>, std::wstring_view>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::string_view_of_t<
            std::string&&>, std::string_view>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::string_view_of_t<
            std::string_view>, std::string_view>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::string_view_of_t<
            std::wstring_view>, std::wstring_view>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::string_view_of_t<
            const char*>, std::string_view>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::string_view_of_t<
            wchar_t*>, std::wstring_view>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::string_view_of_t<
            char[5]>, std::string_view>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::string_view_of_t
            <const wchar_t[42]>, std::wstring_view>));
}

TEST(type_traits_operations, try_underlying_type)
{
    enum class item : char
    {
        POTION = 'P'
    };
    
    EXPECT_TRUE((std::is_same_v<speed::type_traits::underlying_type_of_t<item>, char>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::underlying_type_of_t<char>, char>));
    EXPECT_TRUE(!(std::is_same_v<speed::type_traits::underlying_type_of_t<int>, char>));
}

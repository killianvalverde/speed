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
 * @file        operations.cpp
 * @brief       stringutils operations unit test.
 * @author      Killian Valverde
 * @date        2017/12/24
 */

#include <gtest/gtest.h>

#include "speed/stringutils/stringutils.hpp"

TEST(stringutils_operations, cstr_copy)
{
    char dest[32] = "hello";
    const wchar_t* src = L"world";
    
    auto* res = speed::stringutils::cstr_copy(dest, src);
    
    EXPECT_STREQ(dest, "world");
    EXPECT_EQ(res, dest);
}

TEST(stringutils_operations, cstr_ncopy)
{
    char str1[32] = "hello";
    wchar_t str2[32] = L"world";
    std::string str_res = "wo";
    
    speed::stringutils::cstr_copy(str1, str2, 2);
    
    EXPECT_TRUE(str_res == str1);
}

TEST(stringutils_operations, cstr_concat)
{
    char str1[32] = "hello";
    wchar_t str2[32] = L", world";
    std::string str_res = "hello, world";
    
    speed::stringutils::cstr_concat(str1, str2);
    
    EXPECT_TRUE(str_res == str1);
}

TEST(stringutils_operations, cstr_nconcat)
{
    char str1[32] = "hello";
    wchar_t str2[32] = L", world";
    std::string str_res = "hello, ";
    
    speed::stringutils::cstr_concat(str1, str2, 2);
    
    EXPECT_TRUE(str_res == str1);
}

TEST(stringutils_operations, cstr_length)
{
    const char* str1 = "hello, world";
    const wchar_t* str2 = L"hello, world";
    char str3[] = "hello, world";
    wchar_t str4[] = L"hello, world";
    char* str5 = nullptr;
    
    EXPECT_TRUE(speed::stringutils::cstr_length(str1) == 12);
    EXPECT_TRUE(speed::stringutils::cstr_length(str2) == 12);
    EXPECT_TRUE(speed::stringutils::cstr_length(str3) == 12);
    EXPECT_TRUE(speed::stringutils::cstr_length(str4) == 12);
    EXPECT_TRUE(speed::stringutils::cstr_length(str5) == 0);
}

TEST(stringutils_operations, cstr_compare)
{
    char str1[32] = "kkk";
    wchar_t str2[32] = L"kkk";
    const char* str3 = "aaa";
    const char* str4 = "zzz";
    
    EXPECT_TRUE(speed::stringutils::cstr_compare(str1, str2) == 0);
    EXPECT_TRUE(speed::stringutils::cstr_compare(str1, str3) == 1);
    EXPECT_TRUE(speed::stringutils::cstr_compare(str1, str4) == -1);
}

TEST(stringutils_operations, cstr_ncompare)
{
    char str1[32] = "kkbb";
    wchar_t str2[32] = L"kkzz";
    const char* str3 = "aa";
    const char* str4 = "zz";
    
    EXPECT_TRUE(speed::stringutils::cstr_compare(str1, str2, 2) == 0);
    EXPECT_TRUE(speed::stringutils::cstr_compare(str1, str2, 4) == -1);
    EXPECT_TRUE(speed::stringutils::cstr_compare(str1, str3, 2) == 1);
    EXPECT_TRUE(speed::stringutils::cstr_compare(str1, str4, 2) == -1);
}

TEST(stringutils_operations, cstr_find_first)
{
    char str1[32] = "kkbb";
    wchar_t str2[32] = L"kkzz";
    
    EXPECT_TRUE(speed::stringutils::cstr_find_first_char(str1, 'b') == &str1[2]);
    EXPECT_TRUE(speed::stringutils::cstr_find_first_char(str2, 'z') == &str2[2]);
    EXPECT_TRUE(speed::stringutils::cstr_find_first_char(str1, 'l') == nullptr);
    EXPECT_TRUE(speed::stringutils::cstr_find_first_char(str2, 'l') == nullptr);
}

TEST(stringutils_operations, cstr_find_last)
{
    char str1[32] = "kkbb";
    wchar_t str2[32] = L"kkzz";
    
    EXPECT_TRUE(speed::stringutils::cstr_find_last_char(str1, 'b') == &str1[3]);
    EXPECT_TRUE(speed::stringutils::cstr_find_last_char(str2, 'z') == &str2[3]);
    EXPECT_TRUE(speed::stringutils::cstr_find_last_char(str1, 'l') == nullptr);
    EXPECT_TRUE(speed::stringutils::cstr_find_last_char(str2, 'l') == nullptr);
}

TEST(stringutils_operations, cstr_remove_if)
{
    char str1[32] = "123456";
    wchar_t str2[32] = L"123456";
    std::string str1_res = "456";
    std::wstring str2_res = L"56";
    
    EXPECT_TRUE(speed::stringutils::cstr_remove_if(str1, [](char ch) { return ch < '4'; }));
    EXPECT_TRUE(speed::stringutils::cstr_remove_if(str2, [](char ch) { return ch < '5'; }));
    EXPECT_TRUE(str1 == str1_res);
    EXPECT_TRUE(str2 == str2_res);
}

TEST(stringutils_operations, escape_regex)
{
    const char* str1 = "hello*bye";
    const char* str2 = "hello*$bye";
    const char* str3 = "hell*o*$bye";
    std::string strng1 = "hello*bye";
    std::string strng2 = "hello*$bye";
    std::string strng3 = "hell*o*$bye";
    
    EXPECT_TRUE(speed::stringutils::escape_regex(str1) == "hello\\*bye");
    EXPECT_TRUE(speed::stringutils::escape_regex(str2) == "hello\\*\\$bye");
    EXPECT_TRUE(speed::stringutils::escape_regex(str3) == "hell\\*o\\*\\$bye");
    EXPECT_TRUE(speed::stringutils::escape_regex(strng1) == "hello\\*bye");
    EXPECT_TRUE(speed::stringutils::escape_regex(strng2) == "hello\\*\\$bye");
    EXPECT_TRUE(speed::stringutils::escape_regex(strng3) == "hell\\*o\\*\\$bye");
}

TEST(stringutils_operations, get_length)
{
    const char* str1 = "hello, world";
    const wchar_t* str2 = L"hello, world";
    char str3[] = "hello, world";
    wchar_t str4[] = L"hello, world";
    char* str5 = nullptr;
    std::string str6 = "12345";
    std::wstring str7 = L"12345";
    
    EXPECT_TRUE(speed::stringutils::get_length(str1) == 12);
    EXPECT_TRUE(speed::stringutils::get_length(str2) == 12);
    EXPECT_TRUE(speed::stringutils::get_length(str3) == 12);
    EXPECT_TRUE(speed::stringutils::get_length(str4) == 12);
    EXPECT_TRUE(speed::stringutils::get_length(str5) == 0);
    EXPECT_TRUE(speed::stringutils::get_length(str6) == 5);
    EXPECT_TRUE(speed::stringutils::get_length(str7) == 5);
}

TEST(stringutils_operations, match_wildcard)
{
    const char* str = "hello, world";
    const char* wildcrd1 = "*rld";
    const char* wildcrd2 = "*zrld";
    
    EXPECT_TRUE(speed::stringutils::match_wildcard(str, wildcrd1));
    EXPECT_TRUE(!speed::stringutils::match_wildcard(str, wildcrd2));
}

TEST(stringutils_operations, split)
{
    char str1[32] = "1;2;3;4;5;6";
    std::wstring str2 = L"1;2;3;4;5;;6;;";
    std::size_t i;
    
    auto vec1 = speed::stringutils::split(str1, ";");
    
    i = '1';
    for (auto& x : vec1)
    {
        EXPECT_TRUE(x[0] == i++);
    }
    
    auto vec2 = speed::stringutils::split(str2, ";");
    
    i = '1';
    for (auto& x : vec2)
    {
        EXPECT_TRUE(x[0] == i++);
    }
}

TEST(stringutils_operations, to_lower)
{
    // EXPECT_TRUE(speed::stringutils::to_lower(L'Í') == L'í');
    // EXPECT_TRUE(speed::stringutils::to_lower(L"ÎÁÚÓ") == L"îáúó");
}

TEST(stringutils_operations, to_upper)
{
    // EXPECT_TRUE(speed::stringutils::to_upper(L'í') == L'Í');
    // EXPECT_TRUE(speed::stringutils::to_upper(L"îáúó") == L"ÎÁÚÓ");
}

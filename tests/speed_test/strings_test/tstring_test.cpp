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
 * @file tstring_test.cpp
 * @brief XXXXX.
 * @author Killian Valverde
 * @date 2026-10-11
 */

#include <string>
#include <type_traits>
#include <utility>
#include <gtest/gtest.h>

#include "speed/strings/strings.hpp"

namespace str = speed::strings;

TEST(strings_tchar_t, matches_platform_character_type)
{
#ifdef _WIN32
    EXPECT_TRUE((std::is_same_v<str::tchar_t, wchar_t>));
#else
    EXPECT_TRUE((std::is_same_v<str::tchar_t, char>));
#endif
}

TEST(strings_tstring, matches_platform_string_type)
{
#ifdef _WIN32
    EXPECT_TRUE((std::is_same_v<str::tstring, std::wstring>));
#else
    EXPECT_TRUE((std::is_same_v<str::tstring, std::string>));
#endif
}

TEST(strings_basic_tstring, stores_native_characters)
{
    str::basic_tstring<> text;

#ifdef _WIN32
    text = L"Hello World";
    EXPECT_EQ(text, L"Hello World");
#else
    text = "Hello World";
    EXPECT_EQ(text, "Hello World");
#endif

    EXPECT_EQ(text.size(), 11);
}

#ifdef SPEED_ENABLE_T

TEST(strings_tstring, native_string_literal)
{
    const str::tstring text = _T("Hello World");

    EXPECT_EQ(text.size(), 11);

#ifdef _WIN32
EXPECT_EQ(text, L"Hello World");
#else
EXPECT_EQ(text, "Hello World");
#endif
}

#endif

TEST(strings_select_string, selects_narrow_string)
{
    std::string narrow = "Hello";
    std::wstring wide = L"World";

    auto& result = str::select_string<char>(narrow, wide);

    EXPECT_EQ(result, "Hello");
    EXPECT_EQ(&result, &narrow);

    result = "Modified";

    EXPECT_EQ(narrow, "Modified");
}

TEST(strings_select_string, selects_wide_string)
{
    std::string narrow = "Hello";
    std::wstring wide = L"World";

    auto& result = str::select_string<wchar_t>(narrow, wide);

    EXPECT_EQ(result, L"World");
    EXPECT_EQ(&result, &wide);

    result = L"Modified";

    EXPECT_EQ(wide, L"Modified");
}

TEST(strings_select_string, selects_correct_type_regardless_of_argument_order)
{
    std::wstring wide = L"Wide";
    std::string narrow = "Narrow";

    auto& narrow_result = str::select_string<char>(wide, narrow);
    auto& wide_result = str::select_string<wchar_t>(wide, narrow);

    EXPECT_EQ(&narrow_result, &narrow);
    EXPECT_EQ(&wide_result, &wide);
}

#ifdef _WIN32
static_assert(std::is_same_v<str::tchar_t, wchar_t>);
static_assert(std::is_same_v<str::tstring, std::wstring>);
#else
static_assert(std::is_same_v<str::tchar_t, char>);
static_assert(std::is_same_v<str::tstring, std::string>);
#endif

static_assert(std::is_same_v<
    decltype(str::select_string<char>(
        std::declval<std::string&>(),
        std::declval<std::wstring&>())),
    std::string&>);

static_assert(std::is_same_v<
    decltype(str::select_string<wchar_t>(
        std::declval<std::string&>(),
        std::declval<std::wstring&>())),
    std::wstring&>);

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
 * @file type_cast_test.cpp
 * @brief Unit tests for type casting utilities.
 * @author Killian Valverde
 * @date 2018-06-07
 */

#include <filesystem>
#include <regex>
#include <gtest/gtest.h>

#include "speed/type_casting/type_casting.hpp"

TEST(type_casting_try_type_cast, parses_valid_inputs_to_uint32_and_rejects_invalid)
{
    std::uint32_t res;

    ASSERT_TRUE(speed::type_casting::try_type_cast("0", res) && res == 0);
    ASSERT_TRUE(speed::type_casting::try_type_cast("42", res) && res == 42);
    ASSERT_TRUE(speed::type_casting::try_type_cast("+77", res) && res == 77);
    ASSERT_TRUE(speed::type_casting::try_type_cast("0xFF", res) && res == 255);
    ASSERT_TRUE(speed::type_casting::try_type_cast("0X10", res) && res == 16);
    ASSERT_TRUE(speed::type_casting::try_type_cast("+0xFF", res) && res == 255);
    ASSERT_TRUE(speed::type_casting::try_type_cast("0b1010", res) && res == 10);
    ASSERT_TRUE(speed::type_casting::try_type_cast("0B11", res) && res == 3);
    ASSERT_TRUE(speed::type_casting::try_type_cast("0o77", res) && res == 63);
    ASSERT_TRUE(speed::type_casting::try_type_cast("0O10", res) && res == 8);
    ASSERT_TRUE(speed::type_casting::try_type_cast("4294967295", res) && res == 4294967295);

    ASSERT_FALSE(speed::type_casting::try_type_cast(nullptr, res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast(" ", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("+", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("abc", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("12abc", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("0x", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("-77", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("0b102", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("0o89", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast(" 42", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("42 ", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("4294967296", res));
}

TEST(type_casting_try_type_cast, parses_valid_inputs_to_int32_and_rejects_invalid)
{
    std::int32_t res;

    ASSERT_TRUE(speed::type_casting::try_type_cast("0", res) && res == 0);
    ASSERT_TRUE(speed::type_casting::try_type_cast("42", res) && res == 42);
    ASSERT_TRUE(speed::type_casting::try_type_cast("-42", res) && res == -42);
    ASSERT_TRUE(speed::type_casting::try_type_cast("+77", res) && res == 77);
    ASSERT_TRUE(speed::type_casting::try_type_cast("-0", res) && res == 0);
    ASSERT_TRUE(speed::type_casting::try_type_cast("0xFF", res) && res == 255);
    ASSERT_TRUE(speed::type_casting::try_type_cast("-0xFF", res) && res == -255);
    ASSERT_TRUE(speed::type_casting::try_type_cast("0X10", res) && res == 16);
    ASSERT_TRUE(speed::type_casting::try_type_cast("+0x10", res) && res == 16);
    ASSERT_TRUE(speed::type_casting::try_type_cast("0b1010", res) && res == 10);
    ASSERT_TRUE(speed::type_casting::try_type_cast("-0b1010", res) && res == -10);
    ASSERT_TRUE(speed::type_casting::try_type_cast("0B11", res) && res == 3);
    ASSERT_TRUE(speed::type_casting::try_type_cast("0o77", res) && res == 63);
    ASSERT_TRUE(speed::type_casting::try_type_cast("0O10", res) && res == 8);
    ASSERT_TRUE(speed::type_casting::try_type_cast("-0o10", res) && res == -8);
    ASSERT_TRUE(speed::type_casting::try_type_cast("2147483647", res) && res == 2147483647);
    ASSERT_TRUE(speed::type_casting::try_type_cast("-2147483648", res) && res == -2147483648);

    ASSERT_FALSE(speed::type_casting::try_type_cast(nullptr, res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast(" ", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("+", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("-", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("abc", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("12abc", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("0x", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("-0x", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("0b102", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("-0o89", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("4294967296", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast(" 42", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("-1 ", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("2147483648", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("-2147483649", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("4294967295", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("-4294967296", res));
}

TEST(type_casting_try_type_cast, parses_valid_inputs_to_double_and_rejects_invalid)
{
    double res;

    ASSERT_TRUE(speed::type_casting::try_type_cast("0", res) && res == 0.0);
    ASSERT_TRUE(speed::type_casting::try_type_cast("42", res) && res == 42.0);
    ASSERT_TRUE(speed::type_casting::try_type_cast("-42.5", res) && res == -42.5);
    ASSERT_TRUE(speed::type_casting::try_type_cast("+3.14", res) && res == 3.14);
    ASSERT_TRUE(speed::type_casting::try_type_cast("1e3", res) && res == 1000.0);
    ASSERT_TRUE(speed::type_casting::try_type_cast("-2.5e-2", res) && res == -0.025);
    ASSERT_TRUE(speed::type_casting::try_type_cast("0.0", res) && res == 0.0);
    ASSERT_TRUE(speed::type_casting::try_type_cast("-0.0", res) && res == 0.0);

    ASSERT_TRUE(speed::type_casting::try_type_cast("nan", res) && res != res);
    ASSERT_TRUE(speed::type_casting::try_type_cast("inf", res) && res ==
            std::numeric_limits<decltype(res)>::infinity());
    ASSERT_TRUE(speed::type_casting::try_type_cast("-inf", res) && res ==
            -std::numeric_limits<decltype(res)>::infinity());

    ASSERT_FALSE(speed::type_casting::try_type_cast("abc", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("1.2.3", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("+", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("-", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("1.23abc", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("3.14 ", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("1e10000", res));
    ASSERT_FALSE(speed::type_casting::try_type_cast("1e-10000", res));
}

TEST(type_casting_try_type_cast, parses_valid_inputs_to_duration_and_rejects_invalid)
{
    std::chrono::seconds sc;
    std::chrono::minutes min;

    ASSERT_TRUE(speed::type_casting::try_type_cast("42", sc) && sc == std::chrono::seconds(42));
    ASSERT_TRUE(speed::type_casting::try_type_cast(L"42", min) && min == std::chrono::minutes(42));
}

TEST(type_casting_try_type_cast, parses_wide_c_string_inputs_successfully)
{
    std::uint32_t res1;
    std::int32_t res2;
    double res3;

    ASSERT_TRUE(speed::type_casting::try_type_cast(L"42", res1) && res1 == 42);
    ASSERT_TRUE(speed::type_casting::try_type_cast(L"-42", res2) && res2 == -42);
    ASSERT_TRUE(speed::type_casting::try_type_cast(L"42.5", res3) && res3 == 42.5);
}

TEST(type_casting_try_type_cast, parses_c_string_inputs_to_string_successfully)
{
    std::string res;

    ASSERT_TRUE(speed::type_casting::try_type_cast("☆☆☆", res) && res == "☆☆☆");
}

TEST(type_casting_try_type_cast, parses_c_string_inputs_to_wstring_successfully)
{
    std::wstring res;

    ASSERT_TRUE(speed::type_casting::try_type_cast("☆☆☆", res) && res == L"☆☆☆");
}

TEST(type_casting_try_type_cast, parses_wide_c_string_inputs_to_string_successfully)
{
    std::string res;

    ASSERT_TRUE(speed::type_casting::try_type_cast(L"☆☆☆", res) && res == "☆☆☆");
}

TEST(type_casting_try_type_cast, parses_c_string_inputs_to_regex_successfully)
{
    std::regex rgx1;
    std::wregex rgx2;

    ASSERT_TRUE(speed::type_casting::try_type_cast("^.*$", rgx1));
    ASSERT_TRUE(std::regex_match("hello", rgx1));
    ASSERT_TRUE(speed::type_casting::try_type_cast(L"^.*$", rgx2));
    ASSERT_TRUE(std::regex_match(L"hello", rgx2));

    ASSERT_TRUE(speed::type_casting::try_type_cast<std::regex>(L"^☆☆☆$", rgx1));
    ASSERT_TRUE(std::regex_match("☆☆☆", rgx1));
    ASSERT_TRUE(speed::type_casting::try_type_cast<std::wregex>("^☆☆☆$", rgx2));
    ASSERT_TRUE(std::regex_match(L"☆☆☆", rgx2));
}

TEST(type_casting_try_type_cast, parses_c_string_inputs_to_path_successfully)
{
    std::filesystem::path pth;

    ASSERT_TRUE(speed::type_casting::try_type_cast(".", pth) && pth == ".");

    if constexpr (std::is_same_v<std::filesystem::path::value_type, char>)
    {
        ASSERT_TRUE(speed::type_casting::try_type_cast(L"☆☆☆", pth) && pth == "☆☆☆");
    }
    else
    {
        ASSERT_TRUE(speed::type_casting::try_type_cast("☆☆☆", pth) && pth == L"☆☆☆");
    }
}

TEST(type_casting_try_type_cast, parses_c_string_inputs_to_valid_path_successfully)
{
    speed::filesystem::r_regular_file_path reg_pth;
    speed::filesystem::r_directory_path dir_pth;

    ASSERT_TRUE(speed::type_casting::try_type_cast(".", dir_pth));
    ASSERT_TRUE(speed::type_casting::try_type_cast(L".", dir_pth));

    ASSERT_FALSE(speed::type_casting::try_type_cast(".", reg_pth));
}

TEST(type_casting_try_type_cast, parses_string_inputs_successfully)
{
    std::uint32_t res;

    ASSERT_TRUE(speed::type_casting::try_type_cast(std::string("0"), res) && res == 0);
    ASSERT_TRUE(speed::type_casting::try_type_cast(std::wstring(L"42"), res) && res == 42);
    ASSERT_TRUE(speed::type_casting::try_type_cast(std::string_view("0"), res) && res == 0);
    ASSERT_TRUE(speed::type_casting::try_type_cast(std::wstring_view(L"42"), res) && res == 42);
}

TEST(type_casting_try_type_cast, parses_path_inputs_successfully)
{
    std::uint32_t res;

    ASSERT_TRUE(speed::type_casting::try_type_cast(std::filesystem::path("42"), res) && res == 42);
}

TEST(type_casting_type_cast, parses_valid_inputs_and_throws_on_invalid)
{
    ASSERT_EQ(speed::type_casting::type_cast<int>("42"), 42);

    ASSERT_THROW(speed::type_casting::type_cast<int>("abc"),
            speed::type_casting::type_cast_exception);
}

TEST(type_casting_type_cast_or, parses_valid_inputs_and_executes_invocable_for_invalid)
{
    ASSERT_EQ(speed::type_casting::type_cast_or<int>("42", [] { return 0; }), 42);

    ASSERT_EQ(speed::type_casting::type_cast_or<int>("abc", [] { return 42; }), 42);
}

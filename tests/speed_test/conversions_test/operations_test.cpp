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
 * @brief Unit tests for conversions operations.
 * @author Killian Valverde
 * @date 2018-06-07
 */

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <regex>
#include <string>
#include <string_view>
#include <system_error>
#include <gtest/gtest.h>

#include "speed/conversions/conversions.hpp"

namespace conv = speed::conversions;

TEST(conversions_convert, converts_decimal_integers)
{
    EXPECT_EQ(conv::convert<int>("42"), 42);
    EXPECT_EQ(conv::convert<int>("-42"), -42);
    EXPECT_EQ(conv::convert<unsigned int>("42"), 42u);
    EXPECT_EQ(conv::convert<int>("+123"), 123);
    EXPECT_EQ(conv::convert<int>("0"), 0);
}

TEST(conversions_convert, converts_different_numeric_bases)
{
    EXPECT_EQ(conv::convert<int>("0xFF"), 255);
    EXPECT_EQ(conv::convert<int>("0b1010"), 10);
    EXPECT_EQ(conv::convert<int>("0o77"), 63);
    EXPECT_EQ(conv::convert<int>("-0x10"), -16);
    EXPECT_EQ(conv::convert<unsigned int>("0XFF"), 255u);
}

TEST(conversions_convert, converts_floating_point_values)
{
    EXPECT_DOUBLE_EQ(conv::convert<double>("3.14159"), 3.14159);
    EXPECT_FLOAT_EQ(conv::convert<float>("-2.5"), -2.5f);
    EXPECT_DOUBLE_EQ(conv::convert<double>("1.5e2"), 150.0);
    EXPECT_EQ(conv::convert<long double>("12.5"), 12.5L);
}

TEST(conversions_convert, converts_strings)
{
    EXPECT_EQ(conv::convert<std::string>("Hello World"), "Hello World");
    EXPECT_EQ(conv::convert<std::wstring>(L"Hello World"), L"Hello World");
    EXPECT_EQ(conv::convert<std::wstring>("Hello World"), L"Hello World");
    EXPECT_EQ(conv::convert<std::string>(L"Hello World"), "Hello World");
}

TEST(conversions_convert, converts_unicode_strings)
{
    EXPECT_EQ(conv::convert<std::wstring>("\xC3\xA9\xC3\xB1"), L"\u00E9\u00F1");
    EXPECT_EQ(conv::convert<std::string>(L"\u00E9\u00F1"), "\xC3\xA9\xC3\xB1");
}

TEST(conversions_convert, converts_string_views)
{
    EXPECT_EQ(conv::convert<int>(std::string_view("123")), 123);
    EXPECT_EQ(conv::convert<int>(std::string_view("12345").substr(0, 3)), 123);
    EXPECT_EQ(conv::convert<double>(std::string_view("2.5")), 2.5);
}

TEST(conversions_convert, converts_filesystem_paths)
{
    const auto path = conv::convert<std::filesystem::path>("folder/file.txt");

    EXPECT_EQ(path, std::filesystem::path("folder/file.txt"));

    const auto text = conv::convert<std::string>(path);

    EXPECT_EQ(text, "folder/file.txt");
}

TEST(conversions_convert, converts_chrono_durations)
{
    const auto seconds = conv::convert<std::chrono::seconds>("42");
    const auto milliseconds = conv::convert<std::chrono::milliseconds>("1500");

    EXPECT_EQ(seconds.count(), 42);
    EXPECT_EQ(milliseconds.count(), 1500);
}

TEST(conversions_convert, converts_regular_expressions)
{
    const auto regex = conv::convert<std::regex>("a[0-9]+");

    EXPECT_TRUE(std::regex_match("a123", regex));
    EXPECT_FALSE(std::regex_match("abc", regex));
}

TEST(conversions_convert, throws_on_invalid_input)
{
    EXPECT_THROW((void)conv::convert<int>("abc"), conv::exception);
    EXPECT_THROW((void)conv::convert<int>("12abc"), conv::exception);
    EXPECT_THROW((void)conv::convert<double>("invalid"), conv::exception);
    EXPECT_THROW((void)conv::convert<std::regex>("["), conv::exception);
}

TEST(conversions_convert, throws_on_out_of_range_values)
{
    EXPECT_THROW((void)conv::convert<std::uint8_t>("256"), conv::exception);
    EXPECT_THROW((void)conv::convert<std::int8_t>("128"), conv::exception);
    EXPECT_THROW((void)conv::convert<std::int8_t>("-129"), conv::exception);
    EXPECT_THROW((void)conv::convert<int>("999999999999999999999"), conv::exception);
}

TEST(conversions_convert_or, returns_converted_value_on_success)
{
    const int result = conv::convert_or<int>("42", []
    {
        return -1;
    });

    EXPECT_EQ(result, 42);
}

TEST(conversions_convert_or, returns_fallback_on_failure)
{
    const int result = conv::convert_or<int>("invalid", []
    {
        return 100;
    });

    EXPECT_EQ(result, 100);
}

TEST(conversions_convert_or, invokes_fallback_only_on_failure)
{
    bool fallback_called = false;

    const auto fallback = [&]
    {
        fallback_called = true;
        return -1;
    };

    EXPECT_EQ(conv::convert_or<int>("42", fallback), 42);
    EXPECT_FALSE(fallback_called);

    EXPECT_EQ(conv::convert_or<int>("invalid", fallback), -1);
    EXPECT_TRUE(fallback_called);
}

TEST(conversions_convert_or, supports_different_fallback_return_type)
{
    const auto result = conv::convert_or<double>("invalid", []
    {
        return 42;
    });

    EXPECT_DOUBLE_EQ(result, 42.0);
}

TEST(conversions_to_utf8, converts_narrow_string)
{
    EXPECT_EQ(conv::to_utf8("Hello World"), "Hello World");
    EXPECT_EQ(conv::to_utf8(""), "");
}

TEST(conversions_to_utf8, converts_wide_string)
{
    EXPECT_EQ(conv::to_utf8(L"Hello World"), "Hello World");
}

TEST(conversions_to_utf8, converts_unicode_string)
{
    EXPECT_EQ(conv::to_utf8(L"\u00E9\u00F1\u4E2D"), "\xC3\xA9\xC3\xB1\xE4\xB8\xAD");
}

TEST(conversions_to_utf8, converts_filesystem_path)
{
    const std::filesystem::path path("folder/file.txt");

    EXPECT_EQ(conv::to_utf8(path), "folder/file.txt");
}

TEST(conversions_try_convert, converts_valid_integer)
{
    int value = 0;
    std::error_code err_code;

    const bool result = conv::try_convert("123", value, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_EQ(value, 123);
}

TEST(conversions_try_convert, converts_valid_floating_point)
{
    double value = 0;
    std::error_code err_code;

    const bool result = conv::try_convert("3.14", value, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_DOUBLE_EQ(value, 3.14);
}

TEST(conversions_try_convert, converts_wide_character_input)
{
    int value = 0;
    std::error_code err_code;

    const bool result = conv::try_convert(L"123", value, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_EQ(value, 123);
}

TEST(conversions_try_convert, rejects_invalid_integer)
{
    int value = 42;
    std::error_code err_code;

    const bool result = conv::try_convert("invalid", value, &err_code);

    EXPECT_FALSE(result);
    EXPECT_EQ(err_code, std::make_error_code(std::errc::invalid_argument));
    EXPECT_EQ(value, 42);
}

TEST(conversions_try_convert, rejects_out_of_range_integer)
{
    std::uint8_t value = 42;
    std::error_code err_code;

    const bool result = conv::try_convert("256", value, &err_code);

    EXPECT_FALSE(result);
    EXPECT_EQ(err_code, std::make_error_code(std::errc::result_out_of_range));
    EXPECT_EQ(value, 42);
}

TEST(conversions_try_convert, rejects_null_pointer)
{
    const char* source = nullptr;
    int value = 42;
    std::error_code err_code;

    const bool result = conv::try_convert(source, value, &err_code);

    EXPECT_FALSE(result);
    EXPECT_EQ(err_code, std::make_error_code(std::errc::invalid_argument));
    EXPECT_EQ(value, 42);
}

TEST(conversions_try_convert, rejects_trailing_characters)
{
    int value = 42;
    std::error_code err_code;

    const bool result = conv::try_convert("123abc", value, &err_code);

    EXPECT_FALSE(result);
    EXPECT_EQ(err_code, std::make_error_code(std::errc::invalid_argument));
    EXPECT_EQ(value, 42);
}

TEST(conversions_try_convert, rejects_negative_unsigned_integer)
{
    unsigned int value = 42;
    std::error_code err_code;

    const bool result = conv::try_convert("-1", value, &err_code);

    EXPECT_FALSE(result);
    EXPECT_EQ(err_code, std::make_error_code(std::errc::invalid_argument));
    EXPECT_EQ(value, 42u);
}

TEST(conversions_try_convert, converts_string_view_without_null_terminator)
{
    const std::string source = "123abc";
    const std::string_view view(source.data(), 3);

    int value = 0;
    std::error_code err_code;

    const bool result = conv::try_convert(view, value, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_EQ(value, 123);
}

TEST(conversions_try_convert, converts_regular_expression)
{
    std::regex regex;
    std::error_code err_code;

    const bool result = conv::try_convert("a[0-9]+", regex, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_TRUE(std::regex_match("a123", regex));
}

TEST(conversions_try_convert, rejects_invalid_regular_expression)
{
    std::regex regex;
    std::error_code err_code;

    const bool result = conv::try_convert("[", regex, &err_code);

    EXPECT_FALSE(result);
    EXPECT_EQ(err_code, std::make_error_code(std::errc::invalid_argument));
}

TEST(conversions_try_convert, supports_null_error_code)
{
    int value = 0;

    const bool result = conv::try_convert("123", value, nullptr);

    EXPECT_TRUE(result);
    EXPECT_EQ(value, 123);
}

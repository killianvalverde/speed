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
 * @brief Unit tests for system::codecs operations.
 * @author Killian Valverde
 * @date 2026-10-11
 */

#include <gtest/gtest.h>

#include "speed/system/codecs/codecs.hpp"

namespace cod = speed::system::codecs;

TEST(system_codecs_convert_utf8_to_wstring, converts_ascii_string)
{
    std::wstring destination;
    std::error_code err_code;

    const bool result = cod::convert_utf8_to_wstring("Hello World", destination, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_EQ(destination, L"Hello World");
}

TEST(system_codecs_convert_utf8_to_wstring, converts_unicode_string)
{
    std::wstring destination;
    std::error_code err_code;

    const bool result = cod::convert_utf8_to_wstring(
        "\xC3\xA9\xC3\xB1\xE4\xB8\xAD", destination, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_EQ(destination, L"\u00E9\u00F1\u4E2D");
}

TEST(system_codecs_convert_utf8_to_wstring, converts_supplementary_unicode_character)
{
    std::wstring destination;
    std::error_code err_code;

    const bool result = cod::convert_utf8_to_wstring("\xF0\x9F\x98\x80", destination, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();

#ifdef SPEED_WINAPI
    EXPECT_EQ(destination, L"\xD83D\xDE00");
#else
    EXPECT_EQ(destination, L"\U0001F600");
#endif
}

TEST(system_codecs_convert_utf8_to_wstring, clears_destination_for_empty_source)
{
    std::wstring destination = L"Existing content";
    std::error_code err_code;

    const bool result = cod::convert_utf8_to_wstring("", destination, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_TRUE(destination.empty());
}

TEST(system_codecs_convert_utf8_to_wstring, rejects_null_source)
{
    std::wstring destination;
    std::error_code err_code;

    const bool result = cod::convert_utf8_to_wstring(nullptr, destination, &err_code);

    EXPECT_FALSE(result);
    EXPECT_EQ(err_code, std::make_error_code(std::errc::invalid_argument));
}

TEST(system_codecs_convert_utf8_to_wstring, rejects_invalid_utf8)
{
    std::wstring destination;
    std::error_code err_code;

    const bool result = cod::convert_utf8_to_wstring("\xC3\x28", destination, &err_code);

    EXPECT_FALSE(result);
    EXPECT_TRUE(err_code);
}

TEST(system_codecs_convert_wstring_to_utf8, converts_ascii_string)
{
    std::string destination;
    std::error_code err_code;

    const bool result = cod::convert_wstring_to_utf8(L"Hello World", destination, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_EQ(destination, "Hello World");
}

TEST(system_codecs_convert_wstring_to_utf8, converts_unicode_string)
{
    std::string destination;
    std::error_code err_code;

    const bool result = cod::convert_wstring_to_utf8(L"\u00E9\u00F1\u4E2D", destination, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_EQ(destination, "\xC3\xA9\xC3\xB1\xE4\xB8\xAD");
}

TEST(system_codecs_convert_wstring_to_utf8, converts_supplementary_unicode_character)
{
    std::string destination;
    std::error_code err_code;

    const bool result = cod::convert_wstring_to_utf8(L"\U0001F600", destination, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_EQ(destination, "\xF0\x9F\x98\x80");
}

TEST(system_codecs_convert_wstring_to_utf8, clears_destination_for_empty_source)
{
    std::string destination = "Existing content";
    std::error_code err_code;

    const bool result = cod::convert_wstring_to_utf8(L"", destination, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_TRUE(destination.empty());
}

TEST(system_codecs_convert_wstring_to_utf8, rejects_null_source)
{
    std::string destination;
    std::error_code err_code;

    const bool result = cod::convert_wstring_to_utf8(nullptr, destination, &err_code);

    EXPECT_FALSE(result);
    EXPECT_EQ(err_code, std::make_error_code(std::errc::invalid_argument));
}

TEST(system_codecs_conversion, preserves_unicode_round_trip)
{
    const std::string original = "Hello \xC3\xA9\xC3\xB1\xE4\xB8\xAD \xF0\x9F\x98\x80";

    std::wstring wide_string;
    std::string converted_string;
    std::error_code err_code;

    ASSERT_TRUE(cod::convert_utf8_to_wstring(original.c_str(), wide_string, &err_code))
        << err_code.message();

    ASSERT_FALSE(err_code) << err_code.message();

    ASSERT_TRUE(cod::convert_wstring_to_utf8(wide_string.c_str(), converted_string, &err_code))
        << err_code.message();

    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_EQ(converted_string, original);
}

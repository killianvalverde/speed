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
 * @brief Unit tests for strings operations.
 * @author Killian Valverde
 * @date 2017-12-24
 */

#include <gtest/gtest.h>

#include "speed/strings/strings.hpp"

namespace str = speed::strings;

TEST(strings_cstr_length, returns_correct_length)
{
    EXPECT_EQ(str::cstr_length("Hello"), 5);
    EXPECT_EQ(str::cstr_length(""), 0);
    EXPECT_EQ(str::cstr_length(L"Hello"), 5);

    const char* null_str = nullptr;
    EXPECT_EQ(str::cstr_length(null_str), 0);
}

TEST(strings_cstr_compare, compares_strings)
{
    EXPECT_EQ(str::cstr_compare("Hello", "Hello"), 0);
    EXPECT_LT(str::cstr_compare("Apple", "Banana"), 0);
    EXPECT_GT(str::cstr_compare("Banana", "Apple"), 0);
    EXPECT_LT(str::cstr_compare("Hello", "Hello World"), 0);
    EXPECT_GT(str::cstr_compare("Hello World", "Hello"), 0);
}

TEST(strings_cstr_compare, compares_limited_characters)
{
    EXPECT_EQ(str::cstr_compare("Hello", "Help", 3), 0);
    EXPECT_LT(str::cstr_compare("Hello", "Help", 4), 0);
    EXPECT_EQ(str::cstr_compare("Hello", "World", 0), 0);
    EXPECT_EQ(str::cstr_compare("Hello", "Hello", 5), 0);
}

TEST(strings_cstr_compare_icase, ignores_character_case)
{
    EXPECT_EQ(str::cstr_compare_icase("Hello", "HELLO"), 0);
    EXPECT_EQ(str::cstr_compare_icase("hello", "HeLlO"), 0);
    EXPECT_LT(str::cstr_compare_icase("Apple", "BANANA"), 0);
    EXPECT_GT(str::cstr_compare_icase("BANANA", "apple"), 0);
}

TEST(strings_cstr_compare_icase, compares_limited_characters)
{
    EXPECT_EQ(str::cstr_compare_icase("Hello", "HELp", 3), 0);
    EXPECT_LT(str::cstr_compare_icase("Hello", "HELP", 4), 0);
    EXPECT_EQ(str::cstr_compare_icase("Hello", "WORLD", 0), 0);
    EXPECT_EQ(str::cstr_compare_icase("Hello", "HELLO", 5), 0);
}

TEST(strings_cstr_find_substr, finds_existing_substring)
{
    const char* text = "Hello World";

    EXPECT_EQ(str::cstr_find_substr(text, "World"), text + 6);
    EXPECT_EQ(str::cstr_find_substr(text, "Hello"), text);
    EXPECT_EQ(str::cstr_find_substr(text, ""), text);
    EXPECT_EQ(str::cstr_find_substr(text, "Missing"), nullptr);
}

TEST(strings_cstr_find_substr_icase, finds_substring_ignoring_case)
{
    const char* text = "Hello World";

    EXPECT_EQ(str::cstr_find_substr_icase(text, "WORLD"), text + 6);
    EXPECT_EQ(str::cstr_find_substr_icase(text, "hello"), text);
    EXPECT_EQ(str::cstr_find_substr_icase(text, ""), text);
    EXPECT_EQ(str::cstr_find_substr_icase(text, "Missing"), nullptr);
}

TEST(strings_escape_regex, escapes_special_characters)
{
    EXPECT_EQ(str::escape_regex("Hello.World"), "Hello\\.World");
    EXPECT_EQ(str::escape_regex("a+b*c?"), "a\\+b\\*c\\?");
    EXPECT_EQ(str::escape_regex("[a-z]"), "\\[a-z\\]");
    EXPECT_EQ(str::escape_regex("Hello"), "Hello");
    EXPECT_EQ(str::escape_regex(""), "");
}

TEST(strings_escape_regex_inplace, escapes_special_characters)
{
    std::string text = "Hello.World[a-z]";

    const bool result = str::escape_regex_inplace(text);

    EXPECT_TRUE(result);
    EXPECT_EQ(text, "Hello\\.World\\[a-z\\]");
}

TEST(strings_escape_regex_inplace, preserves_string_without_special_characters)
{
    std::string text = "HelloWorld";

    const bool result = str::escape_regex_inplace(text);

    EXPECT_FALSE(result);
    EXPECT_EQ(text, "HelloWorld");
}

TEST(strings_get_length, returns_correct_length)
{
    EXPECT_EQ(str::get_length("Hello"), 5);
    EXPECT_EQ(str::get_length(std::string("Hello World")), 11);
    EXPECT_EQ(str::get_length(std::wstring(L"Hello")), 5);
    EXPECT_EQ(str::get_length(""), 0);
}

TEST(strings_is_empty, detects_empty_strings)
{
    EXPECT_TRUE(str::is_empty(""));
    EXPECT_TRUE(str::is_empty(std::string{}));
    EXPECT_FALSE(str::is_empty("Hello"));
    EXPECT_FALSE(str::is_empty(std::string("Hello")));
}

TEST(strings_is_null, detects_null_character_pointers)
{
    const char* null_str = nullptr;
    const char* valid_str = "Hello";

    EXPECT_TRUE(str::is_null(null_str));
    EXPECT_FALSE(str::is_null(valid_str));
    EXPECT_FALSE(str::is_null(std::string{}));
    EXPECT_FALSE(str::is_null(std::string("Hello")));
}

TEST(strings_match_wildcard, matches_wildcard_patterns)
{
    EXPECT_TRUE(str::match_wildcard("Hello World", "Hello*"));
    EXPECT_TRUE(str::match_wildcard("Hello World", "*World"));
    EXPECT_TRUE(str::match_wildcard("Hello World", "H?llo*"));
    EXPECT_TRUE(str::match_wildcard("Hello World", "*"));
    EXPECT_TRUE(str::match_wildcard("Hello", "Hello"));

    EXPECT_FALSE(str::match_wildcard("Hello", "World"));
    EXPECT_FALSE(str::match_wildcard("Hello", "H?llo?"));
    EXPECT_FALSE(str::match_wildcard("Hello", "hello"));
}

TEST(strings_match_wildcard_icase, matches_patterns_ignoring_case)
{
    EXPECT_TRUE(str::match_wildcard_icase("Hello World", "HELLO*"));
    EXPECT_TRUE(str::match_wildcard_icase("Hello World", "*WORLD"));
    EXPECT_TRUE(str::match_wildcard_icase("Hello World", "h?LLo*"));
    EXPECT_TRUE(str::match_wildcard_icase("Hello", "HELLO"));

    EXPECT_FALSE(str::match_wildcard_icase("Hello", "WORLD"));
    EXPECT_FALSE(str::match_wildcard_icase("Hello", "HELLO?"));
}

TEST(strings_split, splits_using_single_separator)
{
    const auto result = str::split("apple,banana,cherry", ",");

    EXPECT_EQ(
        result,
        (std::vector<std::string>{"apple", "banana", "cherry"})
    );
}

TEST(strings_split, splits_using_multiple_separators)
{
    const auto result = str::split("apple,banana;cherry", ",;");

    EXPECT_EQ(
        result,
        (std::vector<std::string>{"apple", "banana", "cherry"})
    );
}

TEST(strings_split, preserves_empty_substrings)
{
    const auto result = str::split(",apple,,banana,", ",");

    EXPECT_EQ(
        result,
        (std::vector<std::string>{"", "apple", "", "banana", ""})
    );
}

TEST(strings_split, returns_empty_vector_for_empty_input)
{
    EXPECT_TRUE(str::split("", ",").empty());
    EXPECT_TRUE(str::split("Hello", "").empty());
}

TEST(strings_to_lower, converts_character_to_lowercase)
{
    EXPECT_EQ(str::to_lower('A'), 'a');
    EXPECT_EQ(str::to_lower('Z'), 'z');
    EXPECT_EQ(str::to_lower('a'), 'a');
    EXPECT_EQ(str::to_lower('1'), '1');
}

TEST(strings_to_lower, converts_string_to_lowercase)
{
    EXPECT_EQ(str::to_lower("HELLO WORLD"), "hello world");
    EXPECT_EQ(str::to_lower("HeLLo123"), "hello123");
    EXPECT_EQ(str::to_lower(""), "");
}

TEST(strings_to_lower_inplace, converts_string_to_lowercase)
{
    std::string text = "HELLO World";

    const bool result = str::to_lower_inplace(text);

    EXPECT_TRUE(result);
    EXPECT_EQ(text, "hello world");
}

TEST(strings_to_lower_inplace, preserves_already_lowercase_string)
{
    std::string text = "hello world";

    const bool result = str::to_lower_inplace(text);

    EXPECT_FALSE(result);
    EXPECT_EQ(text, "hello world");
}

TEST(strings_to_upper, converts_character_to_uppercase)
{
    EXPECT_EQ(str::to_upper('a'), 'A');
    EXPECT_EQ(str::to_upper('z'), 'Z');
    EXPECT_EQ(str::to_upper('A'), 'A');
    EXPECT_EQ(str::to_upper('1'), '1');
}

TEST(strings_to_upper, converts_string_to_uppercase)
{
    EXPECT_EQ(str::to_upper("hello world"), "HELLO WORLD");
    EXPECT_EQ(str::to_upper("HeLLo123"), "HELLO123");
    EXPECT_EQ(str::to_upper(""), "");
}

TEST(strings_to_upper_inplace, converts_string_to_uppercase)
{
    std::string text = "hello World";

    const bool result = str::to_upper_inplace(text);

    EXPECT_TRUE(result);
    EXPECT_EQ(text, "HELLO WORLD");
}

TEST(strings_to_upper_inplace, preserves_already_uppercase_string)
{
    std::string text = "HELLO WORLD";

    const bool result = str::to_upper_inplace(text);

    EXPECT_FALSE(result);
    EXPECT_EQ(text, "HELLO WORLD");
}

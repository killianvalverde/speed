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
 * @brief Unit tests for type traits operations.
 * @author Killian Valverde
 * @date 2018-06-08
 */

#include <filesystem>
#include <gtest/gtest.h>

#include "speed/type_traits/type_traits.hpp"

template<typename...>
struct base {};

template<typename...>
struct derived {};

TEST(type_traits_allocator_of, resolves_allocator_type_correctly)
{
    EXPECT_TRUE((std::is_same_v<speed::type_traits::allocator_of_t<
            std::string>, std::string::allocator_type>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::allocator_of_t<
            char*>, std::allocator<char>>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::allocator_of_t<
            char[]>, std::allocator<char>>));
}

TEST(type_traits_basic_crtp_base, constructs_crtp_base_type_correctly)
{
    using result1 = speed::type_traits::basic_crtp_base_t<
        base,       // BaseT
        derived,    // DerivedT
        int,        // ActualT
        int,        // SentinelT
        float       // ParameterTs
    >;
    using result2 = speed::type_traits::basic_crtp_base_t<
        base,       // BaseT
        derived,    // DerivedT
        int,        // ActualT
        bool        // SentinelT
    >;

    using expected1 = base<float, derived<float, int>>;
    using expected2 = base<int>;

    EXPECT_TRUE((std::is_same_v<result1, expected1>));
    EXPECT_TRUE((std::is_same_v<result2, expected2>));
}

TEST(type_traits_basic_crtp_self, constructs_crtp_self_type_correctly)
{
    using result1 = speed::type_traits::basic_crtp_self_t<
        derived,    // SelfT
        int,        // ActualT
        int,        // SentinelT
        float       // ParameterTs
    >;
    using result2 = speed::type_traits::basic_crtp_self_t<
        derived,    // SelfT
        int,        // ActualT
        bool        // SentinelT
    >;

    using expected1 = derived<float>;
    using expected2 = int;

    EXPECT_TRUE((std::is_same_v<result1, expected1>));
    EXPECT_TRUE((std::is_same_v<result2, expected2>));
}

TEST(type_traits_character_traits_of, resolves_character_traits_type_correctly)
{
    EXPECT_TRUE((std::is_same_v<speed::type_traits::character_traits_of_t<
            std::string>, std::string::traits_type>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::character_traits_of_t<
            char*>, std::char_traits<char>>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::character_traits_of_t<
            char[]>, std::char_traits<char>>));
}

TEST(type_traits_character_type_of, extracts_character_type_correctly)
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
    EXPECT_TRUE((std::is_same_v<speed::type_traits::character_type_of_t<
            const wchar_t[42]>, wchar_t>));

    EXPECT_FALSE((std::is_same_v<speed::type_traits::character_type_of_t<
            std::string>, int>));
}

TEST(type_traits_is_character, identifies_character_types_correctly)
{
    EXPECT_TRUE(speed::type_traits::is_character_v<char>);
    EXPECT_TRUE(speed::type_traits::is_character_v<wchar_t>);
    EXPECT_TRUE(speed::type_traits::is_character_v<char16_t>);
    EXPECT_TRUE(speed::type_traits::is_character_v<char32_t>);

    EXPECT_FALSE(speed::type_traits::is_character_v<int>);
}

TEST(type_traits_is_character_pointer, identifies_character_pointers_correctly)
{
    EXPECT_TRUE(speed::type_traits::is_character_pointer_v<char*>);
    EXPECT_TRUE(speed::type_traits::is_character_pointer_v<const wchar_t*>);
    EXPECT_TRUE(speed::type_traits::is_character_pointer_v<volatile char16_t*>);
    EXPECT_TRUE(speed::type_traits::is_character_pointer_v<char32_t*>);

    EXPECT_FALSE(speed::type_traits::is_character_pointer_v<char>);
    EXPECT_FALSE(speed::type_traits::is_character_pointer_v<char[]>);
    EXPECT_FALSE(speed::type_traits::is_character_pointer_v<char*&>);
}

TEST(type_traits_string_view_of, transforms_to_string_view_correctly)
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

TEST(type_traits_underlying_type_of, resolves_underlying_type_when_available)
{
    enum class item : char
    {
        POTION = 'P'
    };

    EXPECT_TRUE((std::is_same_v<speed::type_traits::underlying_type_of_t<item>, char>));
    EXPECT_TRUE((std::is_same_v<speed::type_traits::underlying_type_of_t<char>, char>));

    EXPECT_FALSE((std::is_same_v<speed::type_traits::underlying_type_of_t<int>, char>));
}

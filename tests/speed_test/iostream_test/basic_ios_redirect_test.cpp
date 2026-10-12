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
 * @file basic_ios_redirect_test.cpp
 * @brief Unit tests for the basic_ios_redirect class.
 * @author Killian Valverde
 * @date 2026-10-12
 */

#include <ios>
#include <sstream>
#include <string>
#include <type_traits>
#include <gtest/gtest.h>

#include "speed/iostream/iostream.hpp"

namespace ios = speed::iostream;

TEST(iostream_basic_ios_redirect, starts_not_redirected)
{
    std::ostringstream stream;
    ios::ios_redirect redirect(stream);

    EXPECT_FALSE(redirect.is_redirected());
    EXPECT_TRUE(redirect.get_internal_string_view().empty());
}

TEST(iostream_basic_ios_redirect, redirects_to_external_buffer)
{
    std::ostringstream source;
    std::ostringstream destination;

    ios::ios_redirect redirect(source);

    redirect.redirect(destination.rdbuf());

    EXPECT_TRUE(redirect.is_redirected());
    EXPECT_EQ(static_cast<std::basic_ios<char>&>(source).rdbuf(), destination.rdbuf());

    source << "Hello World";

    EXPECT_EQ(destination.str(), "Hello World");
}

TEST(iostream_basic_ios_redirect, restores_original_buffer)
{
    std::ostringstream source;
    std::ostringstream destination;

    auto* original_buffer = source.rdbuf();

    ios::ios_redirect redirect(source);

    redirect.redirect(destination.rdbuf());
    redirect.unredirect();

    EXPECT_FALSE(redirect.is_redirected());
    EXPECT_EQ(source.rdbuf(), original_buffer);

    source << "Hello World";

    EXPECT_EQ(source.str(), "Hello World");
    EXPECT_TRUE(destination.str().empty());
}

TEST(iostream_basic_ios_redirect, preserves_original_buffer_after_multiple_redirections)
{
    std::ostringstream source;
    std::ostringstream first_destination;
    std::ostringstream second_destination;

    auto* original_buffer = source.rdbuf();

    ios::ios_redirect redirect(source);

    redirect.redirect(first_destination.rdbuf());
    source << "First";

    redirect.redirect(second_destination.rdbuf());
    source << "Second";

    EXPECT_EQ(first_destination.str(), "First");
    EXPECT_EQ(second_destination.str(), "Second");

    redirect.unredirect();

    EXPECT_FALSE(redirect.is_redirected());
    EXPECT_EQ(source.rdbuf(), original_buffer);
}

TEST(iostream_basic_ios_redirect, captures_internal_output)
{
    std::ostringstream stream;
    ios::ios_redirect redirect(stream);

    redirect.redirect_to_internal_stream();

    stream << "Hello World";

    EXPECT_TRUE(redirect.is_redirected());
    EXPECT_EQ(redirect.get_internal_string_view(), "Hello World");
}

TEST(iostream_basic_ios_redirect, captures_multiple_writes)
{
    std::ostringstream stream;
    ios::ios_redirect redirect(stream);

    redirect.redirect_to_internal_stream();

    stream << "Hello";
    stream << ' ';
    stream << "World";

    EXPECT_EQ(redirect.get_internal_string_view(), "Hello World");
}

TEST(iostream_basic_ios_redirect, clears_previous_capture_on_redirection)
{
    std::ostringstream stream;
    ios::ios_redirect redirect(stream);

    redirect.redirect_to_internal_stream();
    stream << "First";

    EXPECT_EQ(redirect.get_internal_string_view(), "First");

    redirect.redirect_to_internal_stream();

    EXPECT_TRUE(redirect.get_internal_string_view().empty());

    stream << "Second";

    EXPECT_EQ(redirect.get_internal_string_view(), "Second");
}

TEST(iostream_basic_ios_redirect, clears_internal_stream)
{
    std::ostringstream stream;
    ios::ios_redirect redirect(stream);

    redirect.redirect_to_internal_stream();

    stream << "Hello World";

    ASSERT_FALSE(redirect.get_internal_string_view().empty());

    redirect.clear_internal_stream();

    EXPECT_TRUE(redirect.get_internal_string_view().empty());

    stream << "New content";

    EXPECT_EQ(redirect.get_internal_string_view(), "New content");
}

TEST(iostream_basic_ios_redirect, clears_stream_error_state)
{
    std::ostringstream stream;
    ios::ios_redirect redirect(stream);

    redirect.redirect_to_internal_stream();

    stream.setstate(std::ios::failbit);

    ASSERT_TRUE(stream.fail());

    redirect.clear_internal_stream();

    EXPECT_TRUE(stream.good());

    stream << "Hello";

    EXPECT_EQ(redirect.get_internal_string_view(), "Hello");
}

TEST(iostream_basic_ios_redirect, handles_clear_without_internal_buffer)
{
    std::ostringstream stream;
    ios::ios_redirect redirect(stream);

    EXPECT_NO_THROW(redirect.clear_internal_stream());
    EXPECT_FALSE(redirect.is_redirected());
    EXPECT_TRUE(redirect.get_internal_string_view().empty());
}

TEST(iostream_basic_ios_redirect, restores_buffer_on_destruction)
{
    std::ostringstream source;
    std::ostringstream destination;

    auto& source_ios = static_cast<std::basic_ios<char>&>(source);
    auto* original_buffer = source_ios.rdbuf();

    {
        ios::ios_redirect redirect(source);

        redirect.redirect(destination.rdbuf());

        EXPECT_EQ(source_ios.rdbuf(), destination.rdbuf());

        source << "Redirected";
    }

    EXPECT_EQ(source_ios.rdbuf(), original_buffer);

    source << "Restored";

    EXPECT_EQ(destination.str(), "Redirected");
    EXPECT_EQ(source.str(), "Restored");
}

TEST(iostream_basic_ios_redirect, redirects_input_stream)
{
    std::istringstream source("Original");
    std::istringstream destination("Redirected");

    ios::ios_redirect redirect(source);

    redirect.redirect(destination.rdbuf());

    std::string result;
    source >> result;

    EXPECT_EQ(result, "Redirected");
    EXPECT_TRUE(redirect.is_redirected());
}

TEST(iostream_basic_ios_redirect, captures_and_reads_internal_stream)
{
    std::stringstream stream;
    ios::ios_redirect redirect(stream);

    redirect.redirect_to_internal_stream();

    stream << "Hello World";

    stream.seekg(0);

    std::string first;
    std::string second;

    stream >> first >> second;

    EXPECT_EQ(first, "Hello");
    EXPECT_EQ(second, "World");
}

TEST(iostream_basic_ios_redirect, captures_wide_character_output)
{
    std::wostringstream stream;
    ios::wios_redirect redirect(stream);

    redirect.redirect_to_internal_stream();

    stream << L"Hello World";

    EXPECT_TRUE(redirect.is_redirected());
    EXPECT_EQ(redirect.get_internal_string_view(), L"Hello World");
}

TEST(iostream_basic_ios_redirect, restores_wide_character_stream)
{
    std::wostringstream source;
    std::wostringstream destination;

    auto* original_buffer = source.rdbuf();

    ios::wios_redirect redirect(source);

    redirect.redirect(destination.rdbuf());

    source << L"Redirected";

    redirect.unredirect();

    source << L"Restored";

    EXPECT_EQ(source.rdbuf(), original_buffer);
    EXPECT_EQ(destination.str(), L"Redirected");
    EXPECT_EQ(source.str(), L"Restored");
}

static_assert(!std::is_copy_constructible_v<ios::ios_redirect>);
static_assert(!std::is_move_constructible_v<ios::ios_redirect>);
static_assert(!std::is_copy_assignable_v<ios::ios_redirect>);
static_assert(!std::is_move_assignable_v<ios::ios_redirect>);

static_assert(!std::is_copy_constructible_v<ios::wios_redirect>);
static_assert(!std::is_move_constructible_v<ios::wios_redirect>);
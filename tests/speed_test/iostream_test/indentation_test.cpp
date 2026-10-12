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
 * @file indentation_test.cpp
 * @brief Unit tests for the indentation class.
 * @author Killian Valverde
 * @date 2018-06-09
 */

#include <sstream>
#include <gtest/gtest.h>

#include "speed/iostream/iostream.hpp"

namespace ios = speed::iostream;

TEST(iostream_indentation, default_constructor)
{
    ios::indentation indent;
    std::ostringstream stream;

    stream << indent << "Hello";

    EXPECT_EQ(stream.str(), "Hello");
}

TEST(iostream_indentation, custom_initial_indentation)
{
    ios::indentation indent(4, 8);
    std::ostringstream stream;

    stream << indent << "Hello";

    EXPECT_EQ(stream.str(), "        Hello");
}

TEST(iostream_indentation, prefix_increment)
{
    ios::indentation indent;
    std::ostringstream stream;

    ++indent;
    stream << indent << "First\n";

    ++indent;
    stream << indent << "Second\n";

    EXPECT_EQ(stream.str(), "    First\n        Second\n");
}

TEST(iostream_indentation, postfix_increment)
{
    ios::indentation indent;
    std::ostringstream stream;

    const auto previous = indent++;

    stream << previous << "Before\n";
    stream << indent << "After\n";

    EXPECT_EQ(stream.str(), "Before\n    After\n");
}

TEST(iostream_indentation, prefix_decrement)
{
    ios::indentation indent(4, 8);
    std::ostringstream stream;

    --indent;
    stream << indent << "First\n";

    --indent;
    stream << indent << "Second\n";

    EXPECT_EQ(stream.str(), "    First\nSecond\n");
}

TEST(iostream_indentation, postfix_decrement)
{
    ios::indentation indent(4, 8);
    std::ostringstream stream;

    const auto previous = indent--;

    stream << previous << "Before\n";
    stream << indent << "After\n";

    EXPECT_EQ(stream.str(), "        Before\n    After\n");
}

TEST(iostream_indentation, decrement_clamps_to_zero)
{
    ios::indentation indent(4, 2);
    std::ostringstream stream;

    --indent;
    stream << indent << "First\n";

    --indent;
    stream << indent << "Second\n";

    EXPECT_EQ(stream.str(), "First\nSecond\n");
}

TEST(iostream_indentation, custom_indentation_step)
{
    ios::indentation indent(2);
    std::ostringstream stream;

    ++indent;
    stream << indent << "First\n";

    ++indent;
    stream << indent << "Second\n";

    --indent;
    stream << indent << "Third\n";

    EXPECT_EQ(stream.str(), "  First\n    Second\n  Third\n");
}

TEST(iostream_indentation, supports_wide_character_stream)
{
    ios::indentation indent(4, 4);
    std::wostringstream stream;

    stream << indent << L"Hello World";

    EXPECT_EQ(stream.str(), L"    Hello World");
}

TEST(iostream_indentation, formats_nested_output)
{
    ios::indentation indent(4);
    std::ostringstream stream;

    stream << indent << "root\n";

    ++indent;
    stream << indent << "child\n";

    ++indent;
    stream << indent << "grandchild\n";

    --indent;
    stream << indent << "child\n";

    --indent;
    stream << indent << "root\n";

    EXPECT_EQ(
        stream.str(),
        "root\n"
        "    child\n"
        "        grandchild\n"
        "    child\n"
        "root\n"
    );
}

static_assert([]()
{
    ios::indentation indent(4);

    ++indent;
    ++indent;
    --indent;

    return ((void)indent++ , true);
}());

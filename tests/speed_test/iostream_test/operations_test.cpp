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
 * @brief Unit tests for iostream operations.
 * @author Killian Valverde
 * @date 2026-10-12
 */

#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <gtest/gtest.h>

#include "speed/iostream/iostream.hpp"

namespace ios = speed::iostream;

TEST(iostream_get_cerr, returns_standard_error_stream)
{
    EXPECT_EQ(&ios::get_cerr<char>(), &std::cerr);
    EXPECT_EQ(&ios::get_cerr<wchar_t>(), &std::wcerr);
}

TEST(iostream_get_cin, returns_standard_input_stream)
{
    EXPECT_EQ(&ios::get_cin<char>(), &std::cin);
    EXPECT_EQ(&ios::get_cin<wchar_t>(), &std::wcin);
}

TEST(iostream_get_clog, returns_standard_logging_stream)
{
    EXPECT_EQ(&ios::get_clog<char>(), &std::clog);
    EXPECT_EQ(&ios::get_clog<wchar_t>(), &std::wclog);
}

TEST(iostream_get_cout, returns_standard_output_stream)
{
    EXPECT_EQ(&ios::get_cout<char>(), &std::cout);
    EXPECT_EQ(&ios::get_cout<wchar_t>(), &std::wcout);
}

TEST(iostream_newl, inserts_newline)
{
    std::ostringstream stream;

    stream << "Hello" << ios::newl << "World";

    EXPECT_EQ(stream.str(), "Hello\nWorld");
}

TEST(iostream_newl, supports_wide_character_stream)
{
    std::wostringstream stream;

    stream << L"Hello" << ios::newl << L"World";

    EXPECT_EQ(stream.str(), L"Hello\nWorld");
}

TEST(iostream_print_error_and_exit, terminates_with_specified_exit_code)
{
    EXPECT_EXIT(
        {
        ios::print_error_and_exit(std::cerr, std::string_view("ERROR"),
            std::string_view("Something went wrong"), 42);
        },
        ::testing::ExitedWithCode(42),
        "ERROR");
}

TEST(iostream_print_wrapped, preserves_text_within_line_limit)
{
    std::ostringstream stream;

    auto& result = ios::print_wrapped(stream, "Hello World", 20);

    EXPECT_EQ(&result, &stream);
    EXPECT_EQ(stream.str(), "Hello World");
}

TEST(iostream_print_wrapped, wraps_text_at_spaces)
{
    std::ostringstream stream;

    ios::print_wrapped(stream, "Hello world again", 11, 2);

    EXPECT_EQ(stream.str(), "Hello world\n  again");
}

TEST(iostream_print_wrapped, indents_explicit_newlines)
{
    std::ostringstream stream;

    ios::print_wrapped(stream, "First\nSecond", 80, 4);

    EXPECT_EQ(stream.str(), "First\n    Second");
}

TEST(iostream_print_wrapped, respects_current_line_length)
{
    std::ostringstream stream;

    stream << "Prefix";

    ios::print_wrapped(stream, " Hello", 10, 2, 6);

    EXPECT_EQ(stream.str(), "Prefix\n  Hello");
}

TEST(iostream_print_wrapped, supports_wide_character_stream)
{
    std::wostringstream stream;

    ios::print_wrapped(stream, L"Hello world again", 11, 2);

    EXPECT_EQ(stream.str(), L"Hello world\n  again");
}

TEST(iostream_set_colors, displays_all_combinations)
{
    // using color_manipulator = std::ostream& (*)(std::ostream&);
    //
    // struct color_entry
    // {
    //     const char* name;
    //     color_manipulator foreground;
    //     color_manipulator background;
    // };
    //
    // constexpr std::array colors = {
    //     color_entry{
    //         "DEFAULT", io::set_foreground_default<char, std::char_traits<char>>,
    //         io::set_background_default<char, std::char_traits<char>>
    //     },
    //     color_entry{
    //         "BLACK", io::set_foreground_black<char, std::char_traits<char>>,
    //         io::set_background_black<char, std::char_traits<char>>
    //     },
    //     color_entry{
    //         "RED", io::set_foreground_red<char, std::char_traits<char>>,
    //         io::set_background_red<char, std::char_traits<char>>
    //     },
    //     color_entry{
    //         "GREEN", io::set_foreground_green<char, std::char_traits<char>>,
    //         io::set_background_green<char, std::char_traits<char>>
    //     },
    //     color_entry{
    //         "YELLOW", io::set_foreground_yellow<char, std::char_traits<char>>,
    //         io::set_background_yellow<char, std::char_traits<char>>
    //     },
    //     color_entry{
    //         "BLUE", io::set_foreground_blue<char, std::char_traits<char>>,
    //         io::set_background_blue<char, std::char_traits<char>>
    //     },
    //     color_entry{
    //         "MAGENTA", io::set_foreground_magenta<char, std::char_traits<char>>,
    //         io::set_background_magenta<char, std::char_traits<char>>
    //     },
    //     color_entry{
    //         "CYAN", io::set_foreground_cyan<char, std::char_traits<char>>,
    //         io::set_background_cyan<char, std::char_traits<char>>
    //     },
    //     color_entry{
    //         "WHITE", io::set_foreground_white<char, std::char_traits<char>>,
    //         io::set_background_white<char, std::char_traits<char>>
    //     },
    //     color_entry{
    //         "BR_BLACK", io::set_foreground_bright_black<char, std::char_traits<char>>,
    //         io::set_background_bright_black<char, std::char_traits<char>>
    //     },
    //     color_entry{
    //         "BR_RED", io::set_foreground_bright_red<char, std::char_traits<char>>,
    //         io::set_background_bright_red<char, std::char_traits<char>>
    //     },
    //     color_entry{
    //         "BR_GREEN", io::set_foreground_bright_green<char, std::char_traits<char>>,
    //         io::set_background_bright_green<char, std::char_traits<char>>
    //     },
    //     color_entry{
    //         "BR_YELLOW", io::set_foreground_bright_yellow<char, std::char_traits<char>>,
    //         io::set_background_bright_yellow<char, std::char_traits<char>>
    //     },
    //     color_entry{
    //         "BR_BLUE", io::set_foreground_bright_blue<char, std::char_traits<char>>,
    //         io::set_background_bright_blue<char, std::char_traits<char>>
    //     },
    //     color_entry{
    //         "BR_MAGENTA", io::set_foreground_bright_magenta<char, std::char_traits<char>>,
    //         io::set_background_bright_magenta<char, std::char_traits<char>>
    //     },
    //     color_entry{
    //         "BR_CYAN", io::set_foreground_bright_cyan<char, std::char_traits<char>>,
    //         io::set_background_bright_cyan<char, std::char_traits<char>>
    //     },
    //     color_entry{
    //         "BR_WHITE", io::set_foreground_bright_white<char, std::char_traits<char>>,
    //         io::set_background_bright_white<char, std::char_traits<char>>
    //     }
    // };
    //
    // constexpr int label_width = 14;
    //
    // std::cout << "\nForeground / Background color combinations\n\n";
    //
    // std::cout << std::left << std::setw(label_width) << "BG / FG";
    //
    // for (std::size_t i = 0; i < colors.size(); ++i)
    // {
    //     if (i < 10)
    //     {
    //         std::cout << ' ' << i << "  ";
    //     }
    //     else
    //     {
    //         std::cout << ' ' << i << ' ';
    //     }
    // }
    //
    // std::cout << '\n';
    //
    // for (std::size_t bg = 0; bg < colors.size(); ++bg)
    // {
    //     std::cout << std::left << std::setw(label_width) << colors[bg].name;
    //
    //     for (std::size_t fg = 0; fg < colors.size(); ++fg)
    //     {
    //         std::cout << colors[bg].background
    //             << colors[fg].foreground
    //             << " XX "
    //             << io::set_foreground_default
    //             << io::set_background_default;
    //     }
    //
    //     std::cout << '\n';
    // }
    //
    // std::cout << '\n';
}

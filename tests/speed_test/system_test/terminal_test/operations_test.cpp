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
 * @brief Unit tests for system::terminal operations.
 * @author Killian Valverde
 * @date 2026-10-11
 */

#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>
#include <gtest/gtest.h>

#include "speed/system/terminal/terminal.hpp"

namespace term = speed::system::terminal;

TEST(system_terminal_flush_input_terminal, discards_pending_input)
{
    // std::error_code err_code;
    //
    // std::cout << "\nType some characters without pressing Enter, then wait 3 seconds...\n";
    // std::cout << "The input buffer will be flushed automatically.\n" << std::flush;
    //
    // std::this_thread::sleep_for(std::chrono::seconds(3));
    //
    // const bool result = term::flush_input_terminal(stdin, &err_code);
    // // const bool result = true;
    //
    // EXPECT_TRUE(result);
    // EXPECT_FALSE(err_code) << err_code.message();
    //
    // if (result)
    // {
    //     std::cout << "\nInput buffer flushed successfully.\n";
    //     std::cout << "Press a NEW key to continue..." << std::flush;
    //
    //     EXPECT_TRUE(term::wait_for_key(false, &err_code));
    //     EXPECT_FALSE(err_code) << err_code.message();
    //
    //     std::cout << '\n';
    // }
}

TEST(system_terminal_flush_output_terminal, flushes_buffered_output)
{
    // namespace term = speed::system::terminal;
    //
    // std::error_code err_code;
    //
    // std::cout << "Testing terminal output flush...";
    //
    // const bool result = term::flush_output_terminal(stdout, &err_code);
    //
    // term::wait_for_key(false, nullptr);
    // std::cout << '\n';
    //
    // EXPECT_TRUE(result);
    // EXPECT_FALSE(err_code) << err_code.message();
}

TEST(system_terminal_set_colors, display_all_combinations)
{
    // constexpr std::array colors = {
    //     term::color::DEFAULT,
    //     term::color::BLACK,
    //     term::color::RED,
    //     term::color::GREEN,
    //     term::color::YELLOW,
    //     term::color::BLUE,
    //     term::color::MAGENTA,
    //     term::color::CYAN,
    //     term::color::WHITE,
    //     term::color::BRIGHT_BLACK,
    //     term::color::BRIGHT_RED,
    //     term::color::BRIGHT_GREEN,
    //     term::color::BRIGHT_YELLOW,
    //     term::color::BRIGHT_BLUE,
    //     term::color::BRIGHT_MAGENTA,
    //     term::color::BRIGHT_CYAN,
    //     term::color::BRIGHT_WHITE
    // };
    //
    // constexpr std::array names = {
    //     "DEFAULT",
    //     "BLACK",
    //     "RED",
    //     "GREEN",
    //     "YELLOW",
    //     "BLUE",
    //     "MAGENTA",
    //     "CYAN",
    //     "WHITE",
    //     "BR_BLACK",
    //     "BR_RED",
    //     "BR_GREEN",
    //     "BR_YELLOW",
    //     "BR_BLUE",
    //     "BR_MAGENTA",
    //     "BR_CYAN",
    //     "BR_WHITE"
    // };
    //
    // constexpr int label_width = 14;
    //
    // std::cout << "\nForeground / Background color combinations:\n\n";
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
    //     std::cout << std::left << std::setw(label_width) << names[bg];
    //
    //     for (std::size_t fg = 0; fg < colors.size(); ++fg)
    //     {
    //         term::set_background_color(std::cout, colors[bg]);
    //         term::set_foreground_color(std::cout, colors[fg]);
    //
    //         std::cout << " XX ";
    //
    //         term::set_foreground_color(std::cout, term::color::DEFAULT);
    //         term::set_background_color(std::cout, term::color::DEFAULT);
    //     }
    //
    //     std::cout << '\n';
    // }
    //
    // std::cout << '\n';
}

TEST(system_terminal_wait_for_key, detects_key_press)
{
    // std::error_code err_code;
    //
    // std::cout << "\nPress any key to continue..." << std::flush;
    //
    // const bool result = term::wait_for_key(true, &err_code);
    //
    // std::cout << '\n';
    //
    // EXPECT_TRUE(result);
    // EXPECT_FALSE(err_code) << err_code.message();
}

/* speed - Generic C++ library.
 * Copyright (C) 2015-2024 Killian Valverde.
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
 * @file        speed_gtest/system_test/terminal_test.cpp
 * @brief       terminal unit test.
 * @author      Killian Valverde
 * @date        2018/06/01
 */

#include <iostream>

#include <gtest/gtest.h>

#include "speed/system/system.hpp"


TEST(system_terminal, flush_input_terminal)
{
    //ASSERT_TRUE(speed::system::flush_input_terminal(STDIN_FILENO));
}


TEST(system_terminal, flush_output_terminal)
{
    //ASSERT_TRUE(speed::system::flush_output_terminal(STDOUT_FILENO));
}


TEST(system_terminal, kbhit)
{
    //ASSERT_TRUE(speed::system::terminal::kbhit());
}


TEST(system_terminal, set_terminal_text_attribute_1)
{
    ASSERT_TRUE(speed::system::terminal::set_text_attribute(
            stdout, speed::system::terminal::ta_t::CYAN));
    ASSERT_TRUE(speed::system::terminal::set_text_attribute(
            stdout, speed::system::terminal::ta_t::DEFAULT));
}


TEST(system_terminal, set_terminal_text_attribute_2)
{
    ASSERT_TRUE(speed::system::terminal::set_text_attribute(
            std::cout, speed::system::terminal::ta_t::CYAN));
    ASSERT_TRUE(speed::system::terminal::set_text_attribute(
            stdout, speed::system::terminal::ta_t::DEFAULT));
}

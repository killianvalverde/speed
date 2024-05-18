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
 * @file        speed_test/containers_test/flags_test.cpp
 * @brief       flags unit test.
 * @author      Killian Valverde
 * @date        2018/06/10
 */

#include <iostream>

#include "gtest/gtest.h"
#include "speed/containers/containers.hpp"
#include "speed/iostream/iostream.hpp"


enum class colors : std::uint8_t
{
    NIL = 0x0,
    BLUE = 0x1,
    RED = 0x2,
    YELLOW = 0x4,
    GREEN = 0x8,
    ALL = 0xF,
};


TEST(containers_flags, begin)
{
    speed::containers::flags<colors> clrs;
    
    clrs.set(colors::BLUE);
    clrs.set(colors::RED);
    clrs.set(colors::GREEN);
    
    EXPECT_TRUE(*clrs.begin() == colors::BLUE);
}


TEST(containers_flags, cbegin)
{
    speed::containers::flags<colors> clrs;
    
    clrs.set(colors::BLUE);
    clrs.set(colors::RED);
    clrs.set(colors::GREEN);
    
    EXPECT_TRUE(*clrs.cbegin() == colors::BLUE);
}


TEST(containers_flags, end)
{
    speed::containers::flags<colors> clrs;
    speed::containers::flags<colors> clrs_cp;
    std::size_t i = 0;
    
    clrs.set(colors::BLUE);
    clrs.set(colors::RED);
    clrs.set(colors::GREEN);
    
    for (auto& x : clrs)
    {
        clrs_cp.set(x);
    }
    
    EXPECT_TRUE(clrs == clrs_cp);
}


TEST(containers_flags, cend)
{
    speed::containers::flags<colors> clrs;
    std::size_t i = 0;
    
    clrs.set(colors::BLUE);
    clrs.set(colors::RED);
    clrs.set(colors::GREEN);
    
    for (auto it = clrs.cbegin(); it != clrs.cend(); ++it)
    {
        ++i;
    }
    
    EXPECT_TRUE(i == 3);
}


TEST(containers_flags, get_value)
{
    speed::containers::flags<colors> clrs = colors::BLUE;
    
    EXPECT_TRUE(clrs.get_value() == colors::BLUE);
}


TEST(containers_flags, get_underlying_value)
{
    speed::containers::flags<colors> clrs = colors::BLUE;
    
    EXPECT_TRUE(clrs.get_underlying_value() == 1);
}


TEST(containers_flags, set)
{
    speed::containers::flags<colors> clrs;
    
    clrs.set(colors::BLUE);
    
    EXPECT_TRUE(clrs.get_value() == colors::BLUE);
}


TEST(containers_flags, set_by_index)
{
    speed::containers::flags<colors> clrs;
    
    clrs.set_by_index(0);
    
    EXPECT_TRUE(clrs.get_value() == colors::BLUE);
}


TEST(containers_flags, unset)
{
    speed::containers::flags<colors> clrs = colors::BLUE;
    
    clrs.unset(colors::BLUE);
    
    EXPECT_TRUE(clrs.get_value() == colors::NIL);
}


TEST(containers_flags, unset_by_index)
{
    speed::containers::flags<colors> clrs = colors::BLUE;
    
    clrs.unset_by_index(0);
    
    EXPECT_TRUE(clrs.get_value() == colors::NIL);
}


TEST(containers_flags, clear)
{
    speed::containers::flags<colors> clrs;
    
    clrs.set(colors::BLUE);
    clrs.set(colors::YELLOW);
    clrs.clear();
    
    EXPECT_TRUE(clrs.get_value() == colors::NIL);
}


TEST(containers_flags, is_set)
{
    speed::containers::flags<colors> clrs = colors::BLUE;
    
    EXPECT_TRUE(clrs.is_set(colors::BLUE));
}


TEST(containers_flags, is_set_by_index)
{
    speed::containers::flags<colors> clrs = colors::BLUE;
    
    EXPECT_TRUE(clrs.is_set_by_index(0));
}


TEST(containers_flags, is_empty)
{
    speed::containers::flags<colors> clrs;
    
    clrs.set(colors::BLUE);
    clrs.set(colors::YELLOW);
    clrs.clear();
    
    EXPECT_TRUE(clrs.is_empty());
}


TEST(containers_flags, print)
{
    speed::containers::flags<colors> clrs;
    speed::iostream::ios_redirect ios_redirect(std::cout);
    std::string expected_result("00000101\n");
    
    ios_redirect.redirect_to_embedded_stringstream();
    
    clrs.set(colors::BLUE);
    clrs.set(colors::YELLOW);
    clrs.print(std::cout);
    
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str() == expected_result);
}


TEST(containers_flags, operator_equal)
{
    speed::containers::flags<colors> clrs1 = colors::BLUE;
    speed::containers::flags<colors> clrs2 = colors::BLUE;
    
    EXPECT_TRUE(clrs1 == clrs2);
}


TEST(containers_flags, operator_inequal)
{
    speed::containers::flags<colors> clrs1 = colors::BLUE;
    speed::containers::flags<colors> clrs2 = colors::YELLOW;
    
    EXPECT_TRUE(clrs1 != clrs2);
}


TEST(containers_flags, operator_left_shift)
{
    speed::containers::flags<colors> clrs;
    speed::iostream::ios_redirect ios_redirect(std::cout);
    std::string expected_result("00000101\n");
    
    ios_redirect.redirect_to_embedded_stringstream();
    
    clrs.set(colors::BLUE);
    clrs.set(colors::YELLOW);
    std::cout << clrs << std::endl;
    
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str() == expected_result);
}

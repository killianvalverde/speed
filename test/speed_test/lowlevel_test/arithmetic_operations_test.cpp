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
 * @file        speed_test/lowlevel_test/arithmetic_operations_test.cpp
 * @brief       arithmetic_operations unit test.
 * @author      Killian Valverde
 * @date        2018/06/08
 */

#include "gtest/gtest.h"
#include "speed/lowlevel/lowlevel.hpp"


TEST(lowlevel_arithmetic_operations, add)
{
    std::uint32_t val1 = 4294967294;
    std::uint32_t val2 = 4294967295;
    
    EXPECT_TRUE(speed::lowlevel::add(val1, 1) == 4294967295);
    EXPECT_THROW(speed::lowlevel::add(val2, 1), speed::lowlevel::overflow_exception);
}


TEST(lowlevel_arithmetic_operations, addl)
{
    std::uint32_t val1 = 4294967293;
    std::uint32_t val2 = 4294967294;
    
    EXPECT_TRUE(speed::lowlevel::addl(val1, 1, 1) == 4294967295);
    EXPECT_THROW(speed::lowlevel::addl(val2, 1, 1), speed::lowlevel::overflow_exception);
}


TEST(lowlevel_arithmetic_operations, try_add)
{
    std::uint32_t val1 = 4294967294;
    std::uint32_t val2 = 4294967295;
    
    EXPECT_TRUE(speed::lowlevel::try_add(&val1, 1));
    EXPECT_TRUE(!speed::lowlevel::try_add(&val2, 1));
}


TEST(lowlevel_arithmetic_operations, try_addl)
{
    std::uint32_t val1 = 4294967293;
    std::uint32_t val2 = 4294967294;
    
    EXPECT_TRUE(speed::lowlevel::try_addl(&val1, 1, 1));
    EXPECT_TRUE(!speed::lowlevel::try_addl(&val2, 1, 1));
}


TEST(lowlevel_arithmetic_operations, addm)
{
    std::uint32_t val1 = 4294967294;
    std::uint32_t val2 = 4294967295;
    
    EXPECT_TRUE(speed::lowlevel::addm(val1, 1) == 4294967295);
    EXPECT_TRUE(speed::lowlevel::addm(val2, 1) == 4294967295);
}


TEST(lowlevel_arithmetic_operations, addml)
{
    std::uint32_t val1 = 4294967293;
    std::uint32_t val2 = 4294967294;
    
    EXPECT_TRUE(speed::lowlevel::addml(val1, 1, 1) == 4294967295);
    EXPECT_TRUE(speed::lowlevel::addml(val2, 1, 1) == 4294967295);
}


TEST(lowlevel_arithmetic_operations, try_addm)
{
    std::uint32_t val1 = 4294967294;
    std::uint32_t val2 = 4294967295;
    
    EXPECT_TRUE(speed::lowlevel::try_addm(&val1, 1) && val1 == 4294967295);
    EXPECT_TRUE(!speed::lowlevel::try_addm(&val2, 1) && val2 == 4294967295);
}


TEST(lowlevel_arithmetic_operations, try_addml)
{
    std::uint32_t val1 = 4294967293;
    std::uint32_t val2 = 4294967294;
    
    EXPECT_TRUE(speed::lowlevel::try_addml(&val1, 1, 1) && val1 == 4294967295);
    EXPECT_TRUE(!speed::lowlevel::try_addml(&val2, 1, 1) && val2 == 4294967295);
}

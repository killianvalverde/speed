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
 * @file        speed_gtest/scalars_test/scalars_test.cpp
 * @brief       scalars unit test.
 * @author      Killian Valverde
 * @date        2018/04/28
 */

#include <type_traits>

#include "gtest/gtest.h"
#include "speed/scalars/scalars.hpp"


TEST(scalars_scalars, get_n_digits)
{
    int i1 = 0;
    int i2 = 4;
    int i3 = 32;
    int i4 = 948;
    int i5 = 92384;
    
    EXPECT_TRUE(speed::scalars::get_n_digits(i1) == 1);
    EXPECT_TRUE(speed::scalars::get_n_digits(i2) == 1);
    EXPECT_TRUE(speed::scalars::get_n_digits(i3) == 2);
    EXPECT_TRUE(speed::scalars::get_n_digits(i4) == 3);
    EXPECT_TRUE(speed::scalars::get_n_digits(i5) == 5);
}

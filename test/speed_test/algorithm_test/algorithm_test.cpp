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
 * @file        speed_test/algorithm_test/algorithm_test.cpp
 * @brief       algorithm_test unit test.
 * @author      Killian Valverde
 * @date        2018/08/07
 */

#include <vector>

#include "gtest/gtest.h"
#include "speed/algorithm/algorithm.hpp"


TEST(algorithm_algorithm, quicksort)
{
    std::vector<std::size_t> vec = {12, 34553, 1, 90, 124124};
    
    speed::algorithm::quicksort(vec, vec.size());
    
    std::size_t min = 0;
    
    for (auto& x : vec)
    {
        EXPECT_TRUE(min < x);
        min = x;
    }
}

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
 * @file        speed_gtest/scalars_test/scalar_test.cpp
 * @brief       scalar unit test.
 * @author      Killian Valverde
 * @date        2018/01/10
 */

#include <type_traits>

#include "gtest/gtest.h"
#include "speed/scalars/scalars.hpp"


TEST(scalars_scalar, constructor_with_parameters)
{
    speed::scalars::scalar<int> nbr(8);
    
    EXPECT_TRUE(nbr == 8);
}


TEST(scalars_scalar, copy_constructor)
{
    speed::scalars::scalar<int> nbr = 8;
    speed::scalars::scalar<int> nbr_copy(nbr);
    
    EXPECT_TRUE(nbr_copy == 8);
}


TEST(scalars_scalar, move_constructor)
{
    speed::scalars::scalar<int> nbr_moved = 8;
    speed::scalars::scalar<int> nbr(std::move(nbr_moved));
    
    EXPECT_TRUE(nbr_moved == 0);
}


TEST(scalars_scalar, copy_assignment_operator)
{
    speed::scalars::scalar<int> nbr = 8;
    speed::scalars::scalar<int> nbr_copy;
    
    nbr_copy = nbr;
    
    EXPECT_TRUE(nbr_copy == 8);
}


TEST(scalars_scalar, move_assignment_operator)
{
    speed::scalars::scalar<int> nbr_moved = 8;
    speed::scalars::scalar<int> nbr = 0;
    
    nbr = std::move(nbr_moved);
    
    EXPECT_TRUE(nbr_moved == 0);
}


TEST(scalars_scalar, value)
{
    speed::scalars::scalar<int> nbr = 8;
    
    nbr.value() <<= 1;
    EXPECT_TRUE(nbr == 16);
}


TEST(scalars_scalar, TpScalar_address_of_operator)
{
    speed::scalars::scalar<int> nbr = 8;
    
    EXPECT_TRUE(nbr == 8);
    
    nbr = 16;
    EXPECT_TRUE(nbr == 16);
    
    nbr <<= 1;
    EXPECT_TRUE(nbr == 32);
    
    nbr >>= 2;
    EXPECT_TRUE(nbr == 8);
    
    nbr++;
    EXPECT_TRUE(nbr == 9);
    
    --nbr;
    EXPECT_TRUE(nbr == 8);
    
    EXPECT_TRUE(std::is_arithmetic<decltype((int)nbr)>::value);
    
    
    enum class color { BLUE };
    
    speed::scalars::scalar<color> colr = color::BLUE;
    
    switch (colr)
    {
        case color::BLUE:
            EXPECT_TRUE(true);
            break;
    
        default:
            EXPECT_TRUE(false);
    }
}


TEST(scalars_scalar, address_of_operator)
{
    speed::scalars::scalar<int> nbr = 8;
    int* pnbr = &nbr;
    
    *pnbr *= 2;
    EXPECT_TRUE(nbr == 16);
}


TEST(scalars_scalar, indirection_operator)
{
    speed::scalars::scalar<int> nbr = 8;
    speed::scalars::scalar<int*> pnbr = &nbr;
    
    *pnbr *= 2;
    EXPECT_TRUE(nbr == 16);
}


TEST(scalars_scalar, class_member_operator)
{
    struct item { int val; };
    
    item itm{};
    speed::scalars::scalar<item*> pitm = &itm;
    
    pitm->val = 8;
    EXPECT_TRUE(pitm->val == 8);
}

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
 * @brief Unit tests for math operations.
 * @author Killian Valverde
 * @date 2026-10-10
 */

#include <gtest/gtest.h>

#include "speed/math/math.hpp"

TEST(math_absolute, returns_magnitude_as_unsigned)
{
    EXPECT_EQ(speed::math::absolute(0), 0u);
    EXPECT_EQ(speed::math::absolute(42), 42u);
    EXPECT_EQ(speed::math::absolute(-42), 42u);
    EXPECT_EQ(speed::math::absolute(-1), 1u);

    EXPECT_EQ(speed::math::absolute(INT32_MAX), static_cast<uint32_t>(INT32_MAX));
    EXPECT_EQ(speed::math::absolute(INT32_MIN), uint32_t{INT32_MAX} + 1);
    EXPECT_EQ(speed::math::absolute(INT64_MIN), uint64_t{INT64_MAX} + 1);

    EXPECT_EQ(speed::math::absolute(42u), 42u);
    EXPECT_EQ(speed::math::absolute(UINT64_MAX), UINT64_MAX);
}

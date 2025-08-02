/* speed - Generic C++ library.
 * Copyright (C) 2015-2025 Killian Valverde.
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
 * @file        operations.hpp
 * @brief       scalars functions header.
 * @author      Killian Valverde
 * @date        2018/04/28
 */

#ifndef SPEED_SCALARS_OPERATIONS_HPP
#define SPEED_SCALARS_OPERATIONS_HPP

#include "../math/math.hpp"

namespace speed::scalars {

/**
 * @brief       Get the number of digits of the given scalar.
 * @param       val : The scalar to get the number of digits.
 * @return      The number of digits of the given scalar.
 */
template<typename IntegralT>
IntegralT count_digits(IntegralT val) noexcept
{
    IntegralT n_digits = 1;
    IntegralT abs_val = math::abs(val);
    
    while (abs_val > 9)
    {
        ++n_digits;
        abs_val /= 10;
    }
    
    return n_digits;
}

/**
 * @brief       Swaps two integral values using the XOR swap algorithm.
 * @param       lhs : Reference to the first variable to swap.
 * @param       rhs : Reference to the second variable to swap.
 */
template<typename IntegralT>
void xor_swap(IntegralT& lhs, IntegralT& rhs) noexcept
{
    if ((void*) &lhs == (void*) &rhs)
    {
        return;
    }
    
    lhs ^= rhs;
    rhs ^= lhs;
    lhs ^= rhs;
}

}

#endif

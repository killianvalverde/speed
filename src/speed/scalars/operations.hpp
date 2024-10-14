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
 * @file        speed/scalars/operations.hpp
 * @brief       scalars functions header.
 * @author      Killian Valverde
 * @date        2018/04/28
 */

#ifndef SPEED_SCALARS_OPERATIONS_HPP
#define SPEED_SCALARS_OPERATIONS_HPP

#include <cstdlib>


namespace speed::scalars {


/**
 * @brief       Get the number of digits of the given scalar.
 * @param       scalr : The scalar to get the number of digits.
 * @return      The number of digits of the given scalar.
 */
template<typename TpScalar>
TpScalar get_n_digits(TpScalar scalr) noexcept
{
    TpScalar cur_n_digits = 1;
    TpScalar abs_value = std::abs(scalr);
    
    while (abs_value > 9)
    {
        ++cur_n_digits;
        abs_value /= 10;
    }
    
    return cur_n_digits;
}


}


#endif

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
 * @file        simple_compare.hpp
 * @brief       simple_compare class header.
 * @author      Killian Valverde
 * @date        2018/08/07
 */

#ifndef SPEED_ALGORITHM_SIMPLE_COMPARE_HPP
#define SPEED_ALGORITHM_SIMPLE_COMPARE_HPP

#include <cstdlib>
#include <type_traits>

namespace speed::algorithm {

/**
 * @brief       Class used to compare easily two objects.
 */
template<typename TpComparable>
struct simple_compare
{
    /**
     * @brief       Compare two elements.
     * @param       rhs : First element to compare.
     * @param       lhs : Second element to compare
     * @return      True is returned whether the element passed as first argument is considered to
     *              go before the second.
     */
    [[nodiscard]] bool operator()(const TpComparable& rhs, const TpComparable& lhs) const
    {
        return rhs < lhs;
    }
};

}

#endif

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
 * @file        operations.hpp
 * @brief       algorithm operations header.
 * @author      Killian Valverde
 * @date        2018/08/07
 */

#ifndef SPEED_ALGORITHM_OPERATIONS_HPP
#define SPEED_ALGORITHM_OPERATIONS_HPP

#include <cstdlib>
#include <type_traits>

#include "simple_compare.hpp"

namespace speed::algorithm {

/** @cond */
namespace __private {

template<typename TpArray, typename TpCompare>
std::size_t __get_partition(TpArray& array, std::size_t lo, std::size_t hi, const TpCompare& comp)
{
    std::size_t partition_index = lo;
    std::size_t i;
    std::remove_reference_t<decltype(array[0])> pivot = array[hi];
    std::remove_reference_t<decltype(array[0])> aux;
    
    for (i = lo; i < hi; i++)
    {
        if (comp(array[i], pivot))
        {
            aux = array[partition_index];
            array[partition_index] = array[i];
            array[i] = aux;
            partition_index++;
        }
    }
    
    aux = array[hi];
    array[hi] = array[partition_index];
    array[partition_index] = aux;
    
    return partition_index;
}

template<typename TpArray, typename TpCompare>
void __quicksort(TpArray& array, std::size_t lo, std::size_t hi, const TpCompare& comp)
{
    std::size_t partition_index;
    
    if (lo < hi)
    {
        partition_index = __get_partition(array, lo, hi, comp);
        if (partition_index > 0)
        {
            __quicksort(array, lo, partition_index - 1, comp);
        }
        __quicksort(array, partition_index + 1, hi, comp);
    }
}

} /* __private */
/** @endcond */

/**
 * @brief       Quicksort the array elements.
 * @param       array : The array to sort.
 * @param       sz : The array size.
 * @param       comp : Binary function that accepts two elements in the range as arguments, and
 *              returns a value convertible to bool. The value returned indicates whether the
 *              element passed as first argument is considered to go before the second.
 */
template<typename TpArray, typename TpCompare>
void quicksort(TpArray& array, std::size_t sz, const TpCompare& comp)
{
    __private::__quicksort(array, 0, sz - 1, comp);
}

/**
 * @brief       Quicksort the array elements.
 * @param       array : The array to sort.
 * @param       sz : The array size.
 */
template<typename TpArray>
void quicksort(TpArray& array, std::size_t sz)
{
    simple_compare<std::remove_reference_t<decltype(array[0])>> comp;
    __private::__quicksort(array, 0, sz - 1, comp);
}

}

#endif

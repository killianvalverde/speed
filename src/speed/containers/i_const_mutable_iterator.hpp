/* speed - Generic C++ library.
 * Copyright (C) 2015-2023 Killian Valverde.
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
 * @file       speed/containers/i_const_mutable_iterator.hpp
 * @brief      i_const_mutable_iterator interface header.
 * @author     Killian
 * @date       2018/03/03 - 12:28
 */

#ifndef SPEED_CONTAINERS_I_CONST_MUTABLE_ITERATOR_HPP
#define SPEED_CONTAINERS_I_CONST_MUTABLE_ITERATOR_HPP

#include <utility>

#include "i_const_iterator.hpp"
#include "i_mutable_iterator.hpp"


namespace speed::containers {


/**
 * @brief       Class that represents the interface for a constant mutable iterator.
 */
template<typename TpValue, typename TpConstIterator, typename TpMutableIterator>
class i_const_mutable_iterator
        : public TpConstIterator
        , public i_mutable_iterator<TpValue, TpMutableIterator>
{
public:
    /** The value encapsulated by the iterator. */
    using value_type = TpValue;
    
    /** The const iterator derived type. */
    using const_iterator_type = TpConstIterator;
    
    /** The mutable iterator derived type. */
    using mutable_iterator_type = TpMutableIterator;
    
    /** The class itself. */
    using self_type = i_const_mutable_iterator<TpValue, TpConstIterator, TpMutableIterator>;
    
    /** The base class. */
    using base_type = i_mutable_iterator<value_type, TpMutableIterator>;
    
    /**
     * @brief       Perfect worwarding constructor.
     * @param       args : The variadic arguments to forward to the base class.
     */
    template<typename... TpArgs>
    i_const_mutable_iterator(TpArgs&&... args)
            : const_iterator_type(std::forward<TpArgs>(args)...)
    {
    }
    
    using base_type::operator ++;
    using base_type::operator --;
    using base_type::operator +;
    using base_type::operator -;
    using base_type::operator +=;
    using base_type::operator -=;
    using base_type::operator !=;
    using base_type::operator <;
    using base_type::operator >;
    using base_type::operator <=;
    using base_type::operator >=;
};


}


#endif

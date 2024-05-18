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
 * @file       speed/containers/const_iterator_base.hpp
 * @brief      const_iterator_base interface header.
 * @author     Killian Valverde
 * @date       2018/01/19
 */

#ifndef SPEED_CONTAINERS_CONST_ITERATOR_BASE_HPP
#define SPEED_CONTAINERS_CONST_ITERATOR_BASE_HPP

#include "iterator_base.hpp"


namespace speed::containers {


/**
 * @brief       Class that represents the interface for a constant iterator.
 */
template<typename TpValue, typename TpIterator>
class const_iterator_base : public virtual iterator_base<TpValue, TpIterator>
{
public:
    /** The value encapsulated by the iterator. */
    using value_type = TpValue;
    
    /** The iterator derived type. */
    using iterator_type = TpIterator;
    
    /** The class itself. */
    using self_type = const_iterator_base<value_type, iterator_type>;
    
    /** The base class. */
    using base_type = iterator_base<value_type, iterator_type>;
    
    /**
     * @brief       Get the reference of the current node value.
     * @return      The reference of the current node value.
     */
    virtual const value_type& operator *() const = 0;
    
    /**
     * @brief       Get the address of the current node value.
     * @return      The address of the current node value.
     */
    virtual const value_type* operator ->() const = 0;
    
    /**
     * @brief       Move to the forward node n times.
     * @param       n : The number of times to move.
     * @return      A reference to the resulting node.
     */
    virtual const value_type& operator [](std::size_t n) const
    {
        iterator_type old_it(dynamic_cast<const iterator_type&>(*this));
    
        old_it.self_type::operator +=(n);
    
        return *old_it;
    }
};


}


#endif

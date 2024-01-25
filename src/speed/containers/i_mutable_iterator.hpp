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
 * @file       speed/containers/i_mutable_iterator.hpp
 * @brief      i_mutable_iterator interface header.
 * @author     Killian
 * @date       2018/01/19 - 22:17
 */

#ifndef SPEED_CONTAINERS_I_MUTABLE_ITERATOR_HPP
#define SPEED_CONTAINERS_I_MUTABLE_ITERATOR_HPP

#include "i_iterator.hpp"


namespace speed::containers {


/**
 * @brief       Class that represents the interface for a mutable iterator.
 */
template<typename TpValue, typename TpIterator>
class i_mutable_iterator : public virtual i_iterator<TpValue, TpIterator>
{
public:
    /** The value encapsulated by the iterator. */
    using value_type = TpValue;
    
    /** The iterator derived type. */
    using iterator_type = TpIterator;
    
    /** The class itself. */
    using self_type = i_mutable_iterator<value_type, iterator_type>;
    
    /** The base class. */
    using base_type = i_iterator<value_type, iterator_type>;
    
    /**
     * @brief       Get the reference of the current node value.
     * @return      The reference of the current node value.
     */
    virtual value_type& operator *() = 0;
    
    /**
     * @brief       Get the address of the current node value.
     * @return      The address of the current node value.
     */
    virtual value_type* operator ->() = 0;
    
    /**
     * @brief       Move to the forward node n times.
     * @param       n : The number of times to move.
     * @return      A reference to the resulting node.
     */
    virtual value_type& operator [](std::size_t n)
    {
        iterator_type old_it(dynamic_cast<const iterator_type&>(*this));
        
        old_it.self_type::operator +=(n);
        
        return *old_it;
    }
};


}


#endif

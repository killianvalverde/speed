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
 * @file       speed/containers/doubly_linked_node.hpp
 * @brief      doubly_linked_node struct header.
 * @author     Killian Valverde
 * @date       2018/01/19
 */

#ifndef SPEED_CONTAINERS_DOUBLY_LINKED_NODE_HPP
#define SPEED_CONTAINERS_DOUBLY_LINKED_NODE_HPP

#include <utility>


namespace speed::containers {


// TODO: Replace this class by n_linked_node
/**
 * @brief        Class that represents a doubly linked node.
 */
template<typename TpValue>
struct doubly_linked_node
{
    /** The value encapsulated by the node. */
    using value_type = TpValue;
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       val : The value to use.
     */
    template<
            typename TpValue_,
            typename = std::enable_if_t<
                    !std::is_base_of<doubly_linked_node, std::decay_t<TpValue_>>::value
            >
    >
    doubly_linked_node(TpValue_&& val)
            : prev_(nullptr)
            , nxt_(nullptr)
            , val_(std::forward<TpValue_>(val))
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : The value to copy.
     */
    doubly_linked_node(const doubly_linked_node& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : The value to mode.
     */
    doubly_linked_node(doubly_linked_node&& rhs) noexcept
            : prev_(rhs.prev_)
            , nxt_(rhs.nxt_)
            , val_(std::move(rhs.val_))
    {
        rhs.prev_ = nullptr;
        rhs.nxt_ = nullptr;
    }
    
    /**
     * @brief       Destructor.
     */
    virtual ~doubly_linked_node() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : The value to copy.
     * @return      The object who call the method.
     */
    doubly_linked_node& operator =(const doubly_linked_node& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : The value to move.
     * @return      The object who call the method.
     */
    doubly_linked_node& operator =(doubly_linked_node&& rhs) noexcept
    {
        if (this != &rhs)
        {
            std::swap(prev_, rhs.prev_);
            std::swap(nxt_, rhs.nxt_);
            val_ = std::move(rhs.val_);
        }
        
        return *this;
    }
    
    /** The pointer to the previous node. */
    doubly_linked_node* prev_;
    
    /** The pointer to the next node. */
    doubly_linked_node* nxt_;
    
    /** The value encapsulated by the node. */
    value_type val_;
};


}


#endif

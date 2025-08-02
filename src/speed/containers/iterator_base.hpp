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
 * @file       iterator_base.hpp
 * @brief      iterator_base interface header.
 * @author     Killian Valverde
 * @date       2018/01/19
 */

#ifndef SPEED_CONTAINERS_ITERATOR_BASE_HPP
#define SPEED_CONTAINERS_ITERATOR_BASE_HPP

#include <cstddef>

namespace speed::containers {

/**
 * @brief       Class that represents the interface of the iterators base.
 */
template<typename ValueT, typename IteratorT>
class iterator_base
{
public:
    /** The value encapsulated by the iterator. */
    using value_type = ValueT;
    
    /** The iterator derived type. */
    using iterator_type = IteratorT;
    
    /** The class itself. */
    using self_type = iterator_base<value_type, iterator_type>;
    
    /**
     * @brief       Default destructor.
     */
    virtual ~iterator_base() = default;
    
    /**
     * @brief       Move to the forward node.
     * @return      A reference to an iterator pointing the current node.
     */
    virtual iterator_type& operator ++() = 0;
    
    /**
     * @brief       Move to the backward node.
     * @return      A reference to an iterator pointing the current node.
     */
    virtual iterator_type& operator --() = 0;
    
    /**
     * @brief       Allows knowing whether the two iterators are the same.
     * @param       rhs : The value to compare.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual bool operator ==(const iterator_type& rhs) const noexcept = 0;
    
    /**
     * @brief       Allows knowing whether the iterator is past-the-end or not.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] virtual bool end() const noexcept = 0;
    
    /**
     * @brief       Move to the forward node.
     * @return      A reference to the the iterator pointing the node before the operation.
     */
    virtual const iterator_type operator ++(int)
    {
        iterator_type old_it(dynamic_cast<iterator_type&>(*this));
    
        operator ++();
        
        return old_it;
    }
    
    /**
     * @brief       Move to the backward node.
     * @return      A reference to the iterator pointing the node before the operation.
     */
    virtual const iterator_type operator --(int)
    {
        iterator_type old_it(dynamic_cast<iterator_type&>(*this));
    
        operator --();
        
        return old_it;
    }
    
    /**
     * @brief       Move to the forward node n times.
     * @param       n : The number of times to move.
     * @return      A reference to a iterator pointing the resulting node.
     */
    virtual const iterator_type operator +(std::size_t n) const
    {
        iterator_type old_it(dynamic_cast<const iterator_type&>(*this));
        
        for (std::size_t i = 0; i < n; i++)
        {
            ++old_it;
        }
        
        return old_it;
    }
    
    /**
     * @brief       Move to the backward node n times.
     * @param       n : The number of times to move.
     * @return      A reference to a iterator pointing the resulting node.
     */
    virtual const iterator_type operator -(std::size_t n) const
    {
        iterator_type old_it(dynamic_cast<const iterator_type&>(*this));
        
        for (std::size_t i = 0; i < n; i++)
        {
            --old_it;
        }
        
        return old_it;
    }
    
    /**
     * @brief       Move to the forward node n times.
     * @param       n : The number of times to move.
     * @return      A reference to a iterator pointing the current node.
     */
    virtual iterator_type& operator +=(std::size_t n)
    {
        for (std::size_t i = 0; i < n; i++)
        {
            operator ++();
        }
    
        return dynamic_cast<iterator_type&>(*this);
    }
    
    /**
     * @brief       Move to the backward node n times.
     * @param       n : The number of times to move.
     * @return      A reference to a iterator pointing the current node.
     */
    virtual iterator_type& operator -=(std::size_t n)
    {
        for (std::size_t i = 0; i < n; i++)
        {
            operator --();
        }
    
        return dynamic_cast<iterator_type&>(*this);
    }
    
    /**
     * @brief       Allows knowing whether two iterators are inequal.
     * @param       rhs : The iterator to compare.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual bool operator !=(const iterator_type& rhs) const noexcept
    {
        return !(*this == rhs);
    }
    
    /**
     * @brief       Allows knowing whether a specified iterator is lower than the current.
     * @param       rhs : The iterator to compare.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual bool operator <(const iterator_type& rhs) const noexcept
    {
        iterator_type it(dynamic_cast<const iterator_type&>(*this));
        
        while (!it.end())
        {
            ++it;
        
            if (it == rhs)
            {
                return true;
            }
        }
    
        return false;
    }
    
    /**
     * @brief       Allows knowing whether a specified iterator is greater than the current.
     * @param       rhs : The iterator to compare.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual bool operator >(const iterator_type& rhs) const noexcept
    {
        iterator_type it(dynamic_cast<const iterator_type&>(*this));
    
        while (!it.end())
        {
            --it;
        
            if (it == rhs)
            {
                return true;
            }
        }
    
        return false;
    }
    
    /**
     * @brief       Allows knowing whether a specified iterator is lower or equal than the current.
     * @param       rhs : The iterator to compare.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual bool operator <=(const iterator_type& rhs) const noexcept
    {
        if (*this == rhs)
        {
            return true;
        }
    
        return *this < rhs;
    }
    
    /**
     * @brief       Allows knowing whether a specified iterator is greater or equal than the
     *              current.
     * @param       rhs : The iterator to compare.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual bool operator >=(const iterator_type& rhs) const noexcept
    {
        if (*this == rhs)
        {
            return true;
        }
    
        return *this > rhs;
    }
};

/**
 * @brief       Class that represents the interface for a constant iterator.
 */
template<typename ValueT, typename IteratorT>
class const_iterator_base : public virtual iterator_base<ValueT, IteratorT>
{
public:
    /** The value encapsulated by the iterator. */
    using value_type = ValueT;

    /** The iterator derived type. */
    using iterator_type = IteratorT;

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

/**
 * @brief       Class that represents the interface for a mutable iterator.
 */
template<typename ValueT, typename IteratorT>
class mutable_iterator_base : public virtual iterator_base<ValueT, IteratorT>
{
public:
    /** The value encapsulated by the iterator. */
    using value_type = ValueT;

    /** The iterator derived type. */
    using iterator_type = IteratorT;

    /** The class itself. */
    using self_type = mutable_iterator_base<value_type, iterator_type>;

    /** The base class. */
    using base_type = iterator_base<value_type, iterator_type>;

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

/**
 * @brief       Class that represents the interface for a constant mutable iterator.
 */
template<typename ValueT, typename ConstIteratorT, typename MutableIteratorT>
class const_mutable_iterator_base
        : public ConstIteratorT
        , public mutable_iterator_base<ValueT, MutableIteratorT>
{
public:
    /** The value encapsulated by the iterator. */
    using value_type = ValueT;

    /** The const iterator derived type. */
    using const_iterator_type = ConstIteratorT;

    /** The mutable iterator derived type. */
    using mutable_iterator_type = MutableIteratorT;

    /** The class itself. */
    using self_type = const_mutable_iterator_base<ValueT, ConstIteratorT, MutableIteratorT>;

    /** The base class. */
    using base_type = mutable_iterator_base<value_type, MutableIteratorT>;

    /**
     * @brief       Perfect worwarding constructor.
     * @param       args : The variadic arguments to forward to the base class.
     */
    template<typename... ArgTs_>
    const_mutable_iterator_base(ArgTs_&&... args)
            : const_iterator_type(std::forward<ArgTs_>(args)...)
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

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
 * @file       flags.hpp
 * @brief      flags class header.
 * @author     Killian Valverde
 * @date       2016/01/08
 */

#ifndef SPEED_CONTAINERS_FLAGS_HPP
#define SPEED_CONTAINERS_FLAGS_HPP

#include <iostream>

#include "../iostream/iostream.hpp"
#include "../type_traits/type_traits.hpp"
#include "exception.hpp"
#include "iterator_base.hpp"

namespace speed::containers {

/**
 * @brief       Class that represents flags conteiner.
 */
template<typename TpValue>
class flags
{
public:
    /** Value type of the flags conteiner. */
    using value_type = TpValue;
    
    /** Underlying value of the flags conteiner. */
    using underlying_type = std::conditional_t<
            std::is_enum<TpValue>::value,
            speed::type_traits::try_underlying_type_t<TpValue>,
            TpValue
    >;
    
    /**
     * @brief       Class that represents const iterators.
     */
    class const_iterator : public const_iterator_base<value_type, const_iterator>
    {
    public:
        /** The class itself. */
        using self_type = const_iterator;
    
        /** The base class. */
        using base_type = const_iterator_base<value_type, const_iterator>;
    
        /** The node iteration type. */
        using node_type = std::uint8_t;
        
        /**
         * @brief       Constructor with parameters.
         * @param       val : The value encapsulated by the iterator.
         */
        const_iterator(const value_type* val) noexcept
                : cur_(0)
                , las_(sizeof(underlying_type) * 8)
                , val_(val)
        {
            if (!end())
            {
                for (cur_ = 0; cur_ < las_; ++cur_)
                {
                    if (static_cast<underlying_type>(*val_) & (1u << cur_))
                    {
                        return;
                    }
                }
    
                val_ = nullptr;
            }
            else
            {
                make_end();
            }
        }
    
        /**
         * @brief       Move to the forward node.
         * @return      A reference to an iterator pointing the current node.
         */
        self_type& operator ++() override
        {
            do
            {
                if (cur_ >= las_)
                {
                    make_end();
                    break;
                }
                
                ++cur_;
                
            } while ((static_cast<underlying_type>(*val_) & (1u << cur_)) == 0);
            
            return *this;
        }
    
        /**
         * @brief       Move to the backward node.
         * @return      A reference to an iterator pointing the current node.
         */
        self_type& operator --() override
        {
            do
            {
                if (cur_ == 0)
                {
                    make_end();
                    break;
                }
                
                --cur_;
                
            } while ((static_cast<underlying_type>(*val_) & (1u << cur_)) == 0);
    
            return *this;
        }
    
        /**
         * @brief       Allows knowing whether the two iterators are the same.
         * @param       rhs : The value to compare.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        bool operator ==(const self_type& rhs) const noexcept override
        {
            return cur_ == rhs.cur_ && las_ == rhs.las_ && val_ == rhs.val_;
        }
    
        /**
         * @brief       Allows knowing whether the iterator is past-the-end or not.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        bool end() const noexcept override
        {
            return val_ == nullptr;
        }
    
        /**
         * @brief       Get the reference of the iterator current node.
         * @return      The reference of the iterator current node.
         */
        const value_type& operator *() const override
        {
            aux_val_ = static_cast<value_type>(static_cast<underlying_type>(*val_) & (1u << cur_));
            
            return aux_val_;
        }
    
        /**
         * @brief       Get the address of the iterator current node.
         * @return      The address of the iterator current node.
         */
        const value_type* operator ->() const override
        {
            throw bad_iteration_exception();
        }

    private:
        /**
         * @brief       Put the iterator in the past-the-end state.
         */
        void make_end()
        {
            cur_ = las_;
            val_ = nullptr;
        }
    
    protected:
        /** The current right shift to do for constant 1 to get the value of the current node. */
        node_type cur_;
        
        /** Past-the-end right shift value. */
        node_type las_;
    
        /** The value in which iterate. */
        const value_type* val_;
        
        /** Used for the production of the node value. */
        mutable value_type aux_val_;
    };
    
    /**
     * @brief       Class that represents iterators.
     */
    class iterator : public const_mutable_iterator_base<value_type, const_iterator, iterator>
    {
    public:
        /** The class itself. */
        using self_type = iterator;
    
        /** The const iterator class used as base. */
        using const_self_type = const_iterator;
    
        /** The base class. */
        using base_type = const_mutable_iterator_base<value_type, const_iterator, iterator>;
    
        /** The node iteration type. */
        using node_type = std::uint8_t;
    
        /**
         * @brief       Constructor with parameters.
         * @param       val : The value encapsulated by the iterator.
         */
        iterator(const value_type* val) noexcept
                : base_type(val)
        {
        }
    
        /**
         * @brief       Move to the forward node.
         * @return      A reference to an iterator pointing the current node.
         */
        self_type& operator ++() noexcept override
        {
            const_self_type::operator ++();
            
            return *this;
        }
    
        /**
         * @brief       Move to the backward node.
         * @return      A reference to an iterator pointing the current node.
         */
        self_type& operator --() noexcept override
        {
            const_self_type::operator --();
            
            return *this;
        }
    
        /**
         * @brief       Allows knowing whether the two iterators are the same.
         * @param       rhs : The value to compare.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        bool operator ==(const self_type& rhs) const noexcept override
        {
            return const_self_type::operator ==(rhs);
        }
    
        /**
         * @brief       Allows knowing whether the iterator is past-the-end or not.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        bool end() const noexcept override
        {
            return const_self_type::end();
        }
    
        /**
         * @brief       Get the reference of the iterator current node.
         * @return      The reference of the iterator current node.
         */
        value_type& operator *() noexcept override
        {
            return const_cast<value_type&>(const_self_type::operator*());
        }
    
        /**
         * @brief       Get the address of the iterator current node.
         * @return      The address of the iterator current node.
         */
        value_type* operator ->() noexcept override
        {
            return const_cast<value_type*>(const_self_type::operator->());
        }
    };
    
    // TODO: Implement the reverce iterators.
    
    /**
     * @brief       Default constructor.
     */
    inline flags() noexcept
            : val_(static_cast<value_type>(0))
    {
    }
    
    /**
     * @brief       Constructor with parameters.
     * @param       val : Default flags value.
     */
    inline flags(const value_type& val) noexcept
            : val_(val)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : The object to copy.
     */
    inline flags(const flags& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : The object to move.
     */
    inline flags(flags&& rhs) noexcept
            : val_(std::move(rhs.val_))
    {
        rhs.val_ = static_cast<value_type>(0);
    }
    
    /**
     * @brief       Destrunctor.
     */
    ~flags() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : The object to copy.
     * @return      The object who call the method.
     */
    flags& operator =(const flags& rhs) = default;
    
    /**
     * @brief       Move assignement operator.
     * @param       rhs : The object to move.
     * @return      The object who call the method.
     */
    flags& operator =(flags&& rhs) noexcept
    {
        if (this != &rhs)
        {
            if (std::is_scalar<value_type>::value)
            {
                std::swap(val_, rhs.val_);
            }
            else
            {
                val_ = std::move(rhs.val_);
            }
        }
        
        return *this;
    }

    /**
     * @brief       Copy assignment operator.
     * @param       flgs : The value to copy.
     * @return      The object who call the method.
     */
    flags& operator =(const value_type& flgs) noexcept
    {
        val_ = flgs;
        
        return *this;
    }
    
    /**
     * @brief       Get the first element iterator of the collection.
     * @return      The first element iterator of the collection.
     */
    inline iterator begin() noexcept
    {
        return iterator(&val_);
    }
    
    /**
     * @brief       Get the first element const iterator of the container.
     * @return      The first element const iterator of the container.
     */
    inline const_iterator begin() const noexcept
    {
        return const_iterator(&val_);
    }
    
    /**
     * @brief       Get the first element const iterator of the container.
     * @return      The first element const iterator of the container.
     */
    inline const_iterator cbegin() const noexcept
    {
        return const_iterator(&val_);
    }
    
    /**
     * @brief       Get an iterator to the past-the-end element in the container.
     * @return      An iterator to the past-the-end element in the container.
     */
    inline iterator end() noexcept
    {
        return iterator(nullptr);
    }
    
    /**
     * @brief       Get a const iterator to the past-the-end element in the container.
     * @return      A const iterator to the past-the-end element in the container
     */
    inline const_iterator end() const noexcept
    {
        return const_iterator(nullptr);
    }
    
    /**
     * @brief       Get a const iterator to the past-the-end element in the container.
     * @return      A const iterator to the past-the-end element in the container
     */
    inline const_iterator cend() const noexcept
    {
        return const_iterator(nullptr);
    }
    
    /**
     * @brief       Get the value that contains the flags conteiner.
     * @return      The value that constains the flags conteiner.
     */
    inline const value_type& get_value() const noexcept
    {
        return val_;
    }
    
    /**
     * @brief       Get the underlying value that contains the flags conteiner.
     * @return      The underlying value that constains the flags conteiner.
     */
    inline underlying_type get_underlying_value() const noexcept
    {
        return static_cast<underlying_type>(val_);
    }
    
    /**
     * @brief       Set a frag in the conteiner.
     * @param       flg : Flag to set in the conteiner.
     */
    inline flags& set(const value_type& flg) noexcept
    {
        val_ = static_cast<value_type>(static_cast<underlying_type>(val_) |
                                       static_cast<underlying_type>(flg));
        
        return *this;
    }
    
    /**
     * @brief       Set a frag in the conteiner by index.
     * @param       indx : Index in which set a flag in the conteiner.
     */
    inline flags& set_by_index(std::uint8_t indx) noexcept
    {
        val_ = static_cast<value_type>(static_cast<underlying_type>(val_) | (1u << indx));
    
        return *this;
    }
    
    /**
     * @brief       Unset a flag in the conteiner.
     * @param       flg : Flag to unset in the conteiner.
     */
    inline flags& unset(const value_type& flg) noexcept
    {
        val_ = static_cast<value_type>(static_cast<underlying_type>(val_) &
                                       ~static_cast<underlying_type>(flg));
    
        return *this;
    }
    
    /**
     * @brief       Unset a flag in the conteiner.
     * @param       indx : Index in which unset a flag in the conteiner.
     */
    inline flags& unset_by_index(std::uint8_t indx) noexcept
    {
        val_ = static_cast<value_type>(static_cast<underlying_type>(val_) & ~(1u << indx));
    
        return *this;
    }
    
    /**
     * @brief       Put all the flags to zero.
     */
    inline flags& clear() noexcept
    {
        val_ = static_cast<value_type>(0);
        
        return *this;
    }
    
    /**
     * @brief       Allows knowing whether a flag is set in the conteiner.
     * @param       flg : Flag to verify its presence in the conteiner.
     */
    inline bool is_set(const value_type& flg) const noexcept
    {
        return ((static_cast<underlying_type>(val_) & static_cast<underlying_type>(flg)) != 0);
    }
    
    /**
     * @brief       Allows knowing whether a flag is set in the conteiner.
     * @param       flg : Flag to verify its presence in the conteiner.
     */
    inline bool is_not_set(const value_type& flg) const noexcept
    {
        return ((static_cast<underlying_type>(val_) & static_cast<underlying_type>(flg)) == 0);
    }
    
    /**
     * @brief       Allows knowing whether a flag is set in the conteiner by index.
     * @param       indx : Index in which verify a flag presence in the conteiner.
     */
    inline bool is_set_by_index(std::uint8_t indx) const noexcept
    {
        return ((static_cast<underlying_type>(val_) & (1u << indx)) != 0);
    }
    
    /**
     * @brief       Allows knowing whether the container is empty.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    inline bool is_empty() const noexcept
    {
        return (static_cast<underlying_type>(val_) == 0);
    }
    
    /**
     * @brief       Allows knowing whether the container is not empty.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    inline bool is_not_empty() const noexcept
    {
        return (static_cast<underlying_type>(val_) != 0);
    }
     
    /**
     * @brief       Print the flag in the standard output with a byte separator.
     * @param       sequence_len : The number of flags to print until print a white space.
     */
    void print(std::uint8_t sequence_len = 8) const
    {
        constexpr std::uint8_t n_bits = sizeof(value_type) * 8;
        
        for (std::uint8_t i = 0; i < n_bits; i++)
        {
            std::cout << (static_cast<underlying_type>(val_) >> (n_bits - 1 - i) & 1);
    
            if (sequence_len != 0 && (i + 1) % sequence_len == 0 && i + 1 < n_bits)
            {
                std::cout << ' ';
            }
        }
        
        speed::iostream::newl(std::cout);
    }
    
    /**
     * @brief       Allows knowing whether two flags conteiner are equal.
     * @param       rhs : Object to compare.
     * @return      If the objets are equal true is returned, otherwise false is returned.
     */
    inline bool operator ==(const flags& rhs) const noexcept
    {
        return (static_cast<underlying_type>(val_) == static_cast<underlying_type>(rhs.val_));
    }
    
    /**
     * @brief       Allows knowing whether two flags conteiner are different.
     * @param       rhs : Object to compare.
     * @return      If the objets are different true is returned, otherwise false is returned.
     */
    inline bool operator !=(const flags& rhs) const noexcept
    {
        return (static_cast<underlying_type>(val_) != static_cast<underlying_type>(rhs.val_));
    }

private:
    /** The raw value hold by the object. */
    value_type val_;
};

/**
 * @brief       Allows to use an object to print in standard output the value that constains the
 *              flags conteiner.
 * @param       os : The object who call the function.
 * @param       flgs : The flags conteiner to print in standard output.
 * @return      The object who call the function.
 */
template<typename TpChar, typename TpCharTraits, typename TpValue>
std::basic_ostream<TpChar, TpCharTraits>& operator <<(
        std::basic_ostream<TpChar, TpCharTraits>& os,
        const flags<TpValue>& flgs
)
{
    constexpr std::uint8_t n_bits = sizeof(TpValue) * 8;
    
    for (std::uint8_t i = 0; i < n_bits; i++)
    {
        os << (flgs.get_underlying_value() >> (n_bits - 1 - i) & 1);
        
        if ((i + 1) % 8 == 0 && i + 1 < n_bits)
        {
            os << ' ';
        }
    }
    
    return os;
}

}

#endif

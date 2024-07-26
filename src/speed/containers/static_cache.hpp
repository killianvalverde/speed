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
 * @file       speed/containers/static_cache.hpp
 * @brief      static_cache class header.
 * @author     Killian Valverde
 * @date       2018/01/12
 */

#ifndef SPEED_CONTAINERS_STATIC_CACHE_HPP
#define SPEED_CONTAINERS_STATIC_CACHE_HPP

#include <cstdlib>
#include <functional>

#include "const_mutable_iterator_base.hpp"
#include "exception.hpp"
#include "flags.hpp"


namespace speed::containers {


/**
 * @brief       Contains all buffer cache buffers flags.
 */
enum class static_cache_buffer_flags : std::uint8_t
{
    /** Null flag. */
    NIL = 0x0,
    
    /** The buffer is inserted in the hash buffer. */
    INSERTED_IN_HASH_BUFFER = 0x1,
    
    /** The buffer is inserted in teh available list. */
    INSERTED_IN_AVAILABLE_LIST = 0x2,
    
    /** All flags. */
    ALL = 0x3
};


/** Contains all buffer cache buffers flags. */
using scbf_t = static_cache_buffer_flags;


/**
 * @brief       Class that represents a generic static cache.
 */
template<
        typename TpKey,
        typename TpValue,
        std::size_t SIZE,
        typename TpHash = std::hash<TpKey>,
        typename TpPred = std::equal_to<TpKey>
>
class static_cache
{
public:
    /** The key type. */
    using key_type = TpKey;
    
    /** The value type. */
    using value_type = TpValue;
    
    /** The hash type. */
    using hash_type = TpHash;
    
    /** The predicate type. */
    using pred_type = TpPred;
    
    /** Class that represents flags container */
    template<typename T>
    using flags_type = flags<T>;
    
    /**
     * @brief       Struct that represents the buffer that contains the data.
     */
    struct buffer
    {
        /** Pointer to the next element in the buffer. */
        buffer* b_nxt_;
        
        /** Pointer to the previous element in the buffer. */
        buffer* b_prev_;
        
        /** Pointer to the next element in the available list. */
        buffer* av_nxt_;
        
        /** Pointer to the previous element in the available list. */
        buffer* av_prev_;
        
        /** The buffer key. */
        key_type ky_;
        
        /** The buffer value. */
        value_type val_;
        
        /** The buffer flags. */
        flags_type<scbf_t> flgs_;
    };
    
    /**
     * @brief       Struct that represents the value stored in the hash table.
     */
    struct hash_buffer
    {
        /** The first element of the buffer list. */
        buffer* b_list_ = nullptr;
    };
    
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
        using node_type = buffer;
    
        /**
         * @brief       Default constructor.
         */
        const_iterator() noexcept
                : hb_(nullptr)
                , hb_size_(0)
                , current_hb_idx_(0)
                , current_hb_buf_(nullptr)
        {
        }
    
        /**
         * @brief       Constructor with parameters.
         * @param       hb : The hash buffer.
         * @param       hb_size : The hash buffer size.
         * @param       current_hb_idx : The current hash buffer.
         * @param       current_hb_buf : The current buffer in the hash buffer.
         */
        const_iterator(
                hash_buffer* hb,
                std::size_t hb_size,
                std::size_t current_hb_idx,
                buffer* current_hb_buf
        ) noexcept
                : hb_(hb)
                , hb_size_(hb_size)
                , current_hb_idx_(current_hb_idx)
                , current_hb_buf_(current_hb_buf)
        {
        }
    
        /**
         * @brief       Move to the forward node.
         * @return      A reference to an iterator pointing the current node.
         */
        self_type& operator ++() override
        {
            if (current_hb_buf_ != nullptr)
            {
                current_hb_buf_ = current_hb_buf_->b_nxt_;
    
                if (current_hb_buf_ == hb_[current_hb_idx_].b_list_)
                {
                    do
                    {
                        ++current_hb_idx_;
            
                        if (current_hb_idx_ < hb_size_)
                        {
                            current_hb_buf_ = hb_[current_hb_idx_].b_list_;
                        }
                        else
                        {
                            current_hb_buf_ = nullptr;
                        }
            
                    } while (current_hb_idx_ < hb_size_ && current_hb_buf_ == nullptr);
                }
            }
        
            return *this;
        }
    
        /**
         * @brief       Move to the backward node.
         * @return      A reference to an iterator pointing the current node.
         */
        self_type& operator --() override
        {
            if (current_hb_buf_ != nullptr)
            {
                if (current_hb_buf_ == hb_[current_hb_idx_].b_list_)
                {
                    do
                    {
                        if (current_hb_idx_ > 0)
                        {
                            --current_hb_idx_;
                            current_hb_buf_ = hb_[current_hb_idx_].b_list_;
                        }
                        else
                        {
                            current_hb_buf_ = nullptr;
                        }
            
                    } while (current_hb_idx_ > 0 && current_hb_buf_ == nullptr);
                }
    
                if (current_hb_buf_ != nullptr)
                {
                    current_hb_buf_ = current_hb_buf_->b_prev_;
                }
            }
    
            return *this;
        }
    
        /**
         * @brief       Allows knowing whether the two iterators are the same.
         * @param       rhs : The value to compare.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        bool operator ==(const self_type& rhs) const noexcept override
        {
            return current_hb_buf_ == rhs.current_hb_buf_;
        }
    
        /**
         * @brief       Allows knowing whether the iterator is past-the-end or not.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        [[nodiscard]] bool end() const noexcept override
        {
            return current_hb_buf_ == nullptr;
        }
    
        /**
         * @brief       Get the reference of the current node value.
         * @return      The reference of the current node value.
         */
        const value_type& operator *() const override
        {
            return current_hb_buf_->val_;
        }
    
        /**
         * @brief       Get the address of the current node value.
         * @return      The address of the current node value.
         */
        const value_type* operator ->() const override
        {
            return &current_hb_buf_->val_;
        }
    
        template<
                typename TpKey__,
                typename TpValue__,
                std::size_t SIZE__,
                typename TpHash__,
                typename TpPred__
        >
        friend class static_cache;

    protected:
        /** The hash buffer. */
        hash_buffer* hb_;
        
        /** The hash buffer size. */
        std::size_t hb_size_;
        
        /** The current hash buffer index. */
        std::size_t current_hb_idx_;
    
        /** The current hash buffer buffer. */
        buffer* current_hb_buf_;
    };
    
    /**
     * @brief       Class that represents iterators.
     */
    class iterator : public const_mutable_iterator_base<value_type, const_iterator, iterator>
    {
    public:
        /** The class itself. */
        using self_type = iterator;
    
        /** The const iterator base. */
        using const_self_type = const_iterator;
    
        /** The base class. */
        using base_type = const_mutable_iterator_base<value_type, const_iterator, iterator>;
    
        /** The node iteration type. */
        using node_type = buffer;
   
        /**
         * @brief       Default constructor.
         */
        iterator() = default;
    
        /**
         * @brief       Constructor with parameters.
         * @param       hb : The hash buffer.
         * @param       hb_size : The hash buffer size.
         * @param       current_hb_idx : The current hash buffer.
         * @param       current_hb_buf : The current buffer in the hash buffer.
         */
        iterator(
                hash_buffer* hb,
                std::size_t hb_size,
                std::size_t current_hb_idx,
                buffer* current_hb_buf
        ) noexcept
                : base_type(hb, hb_size, current_hb_idx, current_hb_buf)
        {
        }
    
        /**
         * @brief       Move to the forward node.
         * @return      A reference to an iterator pointing the current node.
         */
        self_type& operator ++() override
        {
            const_self_type::operator ++();
            
            return *this;
        }
    
        /**
         * @brief       Move to the backward node.
         * @return      A reference to an iterator pointing the current node.
         */
        self_type& operator --() override
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
        [[nodiscard]] bool end() const noexcept override
        {
            return const_self_type::end();
        }
    
        /**
         * @brief       Get the reference of the current node value.
         * @return      The reference of the current node value.
         */
        value_type& operator *() override
        {
            return const_self_type::current_hb_buf_->val_;
        }
    
        /**
         * @brief       Get the address of the current node value.
         * @return      The address of the current node value.
         */
        value_type* operator ->() override
        {
            return &const_self_type::current_hb_buf_->val_;
        }
    
        template<
                typename TpKey__,
                typename TpValue__,
                std::size_t SIZE__,
                typename TpHash__,
                typename TpPred__
        >
        friend class static_cache;
    };
    
    // TODO: Implement the reverce iterators.
    
    /**
     * @brief       Default constructor.
     */
    static_cache() noexcept
            : buffers_()
            , av_list_(&buffers_[0])
            , hbuf_()
    {
        std::size_t i;
        
        for (i = 0; i + 1 < SIZE; i++)
        {
            buffers_[i].av_nxt_ = &buffers_[i + 1];
            buffers_[i + 1].av_prev_ = &buffers_[i];
            buffers_[i].flgs_.set(scbf_t::INSERTED_IN_AVAILABLE_LIST);
        }
        
        buffers_[i].av_nxt_ = &buffers_[0];
        buffers_[0].av_prev_ = &buffers_[i];
        buffers_[i].flgs_.set(scbf_t::INSERTED_IN_AVAILABLE_LIST);
    }
    
    // TODO: Implement copy and move operators.

    /** @cond */
    static_cache(const static_cache& rhs) = delete;
    
    static_cache(static_cache&& rhs) = delete;
    
    static_cache& operator =(const static_cache& rhs) = delete;
    
    static_cache& operator =(static_cache&& rhs) = delete;
    /** @endcond */

    /**
     * @brief       Get the first element iterator of the container.
     * @return      The first element iterator of the container.
     */
    inline iterator begin() noexcept
    {
        constexpr std::size_t hb_size = get_hash_buffer_size();
        
        buffer* current_buf = hbuf_[0].b_list_;
        std::size_t current_hb_idx = 0;
        
        while (current_buf == nullptr && current_hb_idx < hb_size)
        {
            ++current_hb_idx;
            current_buf = hbuf_[current_hb_idx].b_list_;
        
        }
        
        return iterator(hbuf_, get_hash_buffer_size(), current_hb_idx, current_buf);
    }
    
    /**
     * @brief       Get the first element const iterator of the container.
     * @return      The first element const iterator of the container.
     */
    inline const_iterator cbegin() const noexcept
    {
        return const_cast<static_cache>(*this).begin();
    }
    
    /**
     * @brief       Get an iterator to the past-the-end element in the container.
     * @return      An iterator to the past-the-end element in the container.
     */
    inline iterator end() noexcept
    {
        return iterator(hbuf_, get_hash_buffer_size(), get_hash_buffer_size(), nullptr);
    }
    
    /**
     * @brief       Get a const iterator to the past-the-end element in the container.
     * @return      A const iterator to the past-the-end element in the container
     */
    inline const_iterator cend() const noexcept
    {
        return const_iterator(hbuf_, get_hash_buffer_size(), get_hash_buffer_size(), nullptr);
    }
    
    /**
     * @brief       Erase from the available list the element.
     * @param       it : The element to erase from the available list.
     */
    void lock(const_iterator& it) noexcept
    {
        if (it.current_hb_buf_->flgs_.is_set(scbf_t::INSERTED_IN_AVAILABLE_LIST))
        {
            erase_from_available_list(it.current_hb_buf_);
        }
    }
    
    /**
     * @brief       Erase from the available list the element with the specified key.
     * @param       ky : The element key.
     */
    void lock(const key_type& ky) noexcept
    {
        iterator it = find(ky);
        lock(it);
    }
    
    /**
     * @brief       Insert in the available list the specified element.
     * @param       it : The element to insert in the available list.
     */
    void unlock(const_iterator& it) noexcept
    {
        if (!it.current_hb_buf_->flgs_.is_set(scbf_t::INSERTED_IN_AVAILABLE_LIST))
        {
            insert_in_available_list(it.current_hb_buf_);
        }
    }
    
    /**
     * @brief       Insert in the available list the element with the specified key.
     * @param       ky : The element key.
     */
    void unlock(const key_type& ky) noexcept
    {
        iterator it = find(ky);
        unlock(it);
    }
    
    /**
     * @brief       Find the key associated value.
     * @param       ky : The key.
     * @return      If function was successful an iterator to the key associated value is returned,
     *              an end iterator is returned.
     */
    iterator find(const key_type& ky) noexcept
    {
        const std::size_t hash_ky = get_hash_key(ky);
        buffer* const first = hbuf_[hash_ky].b_list_;
        buffer* cur = first;
        pred_type equal_to;
        
        if (first != nullptr)
        {
            do
            {
                if (equal_to(cur->ky_, ky))
                {
                    if (cur->flgs_.is_set(scbf_t::INSERTED_IN_AVAILABLE_LIST))
                    {
                        set_most_recently_used_buffer(cur);
                    }
                    
                    return iterator(hbuf_, get_hash_buffer_size(), hash_ky, cur);
                }
                
                cur = cur->b_nxt_;
                
            } while (cur != first);
        }
        
        return end();
    }
    
    /**
     * @brief       Find the key associated value and remove it from the available list.
     * @param       ky : The key.
     * @return      If function was successful an iterator to the key associated value is returned,
     *              an end iterator is returned.
     */
    iterator find_and_lock(const key_type& ky) noexcept
    {
        iterator it = find(ky);
        lock(it);
    
        return it;
    }
    
    /**
     * @brief       Insert a key value pair in the container.
     * @param       ky : The key.
     * @param       val : The value.
     * @return      An iterator to the inserted element.
     */
    template<typename TpKey_, typename TpValue_>
    iterator insert(TpKey_&& ky, TpValue_&& val)
    {
        if (!find(ky).end())
        {
            throw insertion_exception();
        }
        
        buffer* buf = get_least_recently_used_buffer();
        
        if (buf->flgs_.is_set(scbf_t::INSERTED_IN_HASH_BUFFER))
        {
            erase_from_hash_buffer(buf);
        }
        
        buf->ky_ = std::forward<TpKey_>(ky);
        buf->val_ = std::forward<TpValue_>(val);
        
        insert_in_hash_buffer_list(buf);
        set_most_recently_used_buffer(buf);
    
        return iterator(hbuf_, get_hash_buffer_size(), get_hash_key(buf->ky_), buf);
    }
    
    /**
     * @brief       Insert a key value pair in the container and erase it from the available list.
     * @param       ky : The key.
     * @param       val : The value.
     * @return      An iterator to the inserted element.
     */
    template<typename TpKey_, typename TpValue_>
    iterator insert_and_lock(TpKey_&& ky, TpValue_&& val)
    {
        iterator it = insert(std::forward<TpKey_>(ky), std::forward<TpValue_>(val));
        lock(it);
        
        return it;
    }
    
    /**
     * @brief       Check whether the least recently used element is free (never used).
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool is_least_recently_used_free() const
    {
        return !get_least_recently_used_buffer()->flgs_.is_set(scbf_t::INSERTED_IN_HASH_BUFFER);
    }
    
    /**
     * @brief       Get the least recently used element.
     * @return      The least recently used element.
     */
    value_type& get_least_recently_used()
    {
        return get_least_recently_used_buffer()->val_;
    }

protected:
    /**
     * @brief       Get the hash key associated with the key.
     * @param       ky : The key.
     * @return      The hash key associated with the key.
     */
    std::size_t get_hash_key(const key_type& ky) const noexcept
    {
        return static_cast<std::size_t>((hash_type()(ky)) % (SIZE * 2));
    }
    
    /**
     * @brief       Insert a buffer in the end of a hash buffer.
     * @param       buf : The buffer to insert.
     */
    void insert_in_hash_buffer_list(buffer* buf) noexcept
    {
        buffer** const pb_list = &hbuf_[get_hash_key(buf->ky_)].b_list_;
        
        if (*pb_list == nullptr)
        {
            *pb_list = buf;
            buf->b_nxt_ = buf;
            buf->b_prev_ = buf;
        }
        else
        {
            buf->b_prev_ = (*pb_list)->b_prev_;
            buf->b_nxt_ = *pb_list;
            (*pb_list)->b_prev_->b_nxt_ = buf;
            (*pb_list)->b_prev_ = buf;
        }
    
        buf->flgs_.set(scbf_t::INSERTED_IN_HASH_BUFFER);
    }
    
    /**
     * @brief       Insert a buffer in the end of the available list.
     * @param       buf : The buffer to insert.
     */
    void insert_in_available_list(buffer* buf) noexcept
    {
        if (av_list_ == nullptr)
        {
            av_list_ = buf;
            buf->av_nxt_ = buf;
            buf->av_prev_ = buf;
        }
        else
        {
            buf->av_prev_ = av_list_->av_prev_;
            buf->av_nxt_ = av_list_;
            av_list_->av_prev_->av_nxt_ = buf;
            av_list_->av_prev_ = buf;
        }
    
        buf->flgs_.set(scbf_t::INSERTED_IN_AVAILABLE_LIST);
    }
    
    /**
     * @brief       Erase the specified buffer from the hash buffer.
     * @param       buf : The buffer to erase.
     */
    void erase_from_hash_buffer(buffer* buf) noexcept
    {
        buffer** const pb_list = &hbuf_[get_hash_key(buf->ky_)].b_list_;
        
        if (buf->b_nxt_ == buf)
        {
            *pb_list = nullptr;
        }
        else
        {
            if (buf == *pb_list)
            {
                *pb_list = (*pb_list)->av_nxt_;
            }
            
            buf->b_prev_->b_nxt_ = buf->b_nxt_;
            buf->b_nxt_->b_prev_ = buf->b_prev_;
        }
    
        buf->flgs_.unset(scbf_t::INSERTED_IN_HASH_BUFFER);
    }
    
    /**
     * @brief       Erase the specified buffer from the available list.
     * @param       buf : The buffer to erase.
     */
    void erase_from_available_list(buffer* buf) noexcept
    {
        if (buf->av_nxt_ == buf)
        {
            av_list_ = nullptr;
        }
        else
        {
            if (buf == av_list_)
            {
                av_list_ = av_list_->av_nxt_;
            }
            
            buf->av_prev_->av_nxt_ = buf->av_nxt_;
            buf->av_nxt_->av_prev_ = buf->av_prev_;
        }
    
        buf->flgs_.unset(scbf_t::INSERTED_IN_AVAILABLE_LIST);
    }
    
    /**
     * @brief       Get the least recently used buffer in the available list.
     * @return      The least recently used buffer in the available list.
     */
    buffer* get_least_recently_used_buffer() const
    {
        if (av_list_ == nullptr)
        {
            throw exhausted_resources_exception();
        }
        
        return av_list_;
    }
    
    /**
     * @brief       Set the specified buffer as the most recetly used.
     * @param       buf : The buffer to set as the most recetly used.
     */
    void set_most_recently_used_buffer(buffer* buf) noexcept
    {
        if (buf == av_list_)
        {
            av_list_ = av_list_->av_nxt_;
        }
        else
        {
            erase_from_available_list(buf);
            insert_in_available_list(buf);
        }
    }
    
    /**
     * @brief       Get the hash buffer size.
     * @return      The hash buffer size.
     */
    [[nodiscard]] constexpr std::size_t get_hash_buffer_size() const noexcept
    {
        return sizeof(hbuf_) / sizeof(hash_buffer);
    }
    
private:
    /** All the buffers. */
    buffer buffers_[SIZE];
    
    /** The available list. */
    buffer* av_list_;
    
    /** The hash buffer. */
    hash_buffer hbuf_[SIZE * 2];
};


}


#endif

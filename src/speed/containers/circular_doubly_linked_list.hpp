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
 * @file       speed/containers/circular_doubly_linked_list.hpp
 * @brief      circular_doubly_linked_list class header.
 * @author     Killian Valverde
 * @date       2018/01/19
 */

#ifndef SPEED_CONTAINERS_CIRCULAR_DOUBLY_LINKED_LIST_HPP
#define SPEED_CONTAINERS_CIRCULAR_DOUBLY_LINKED_LIST_HPP

#include "doubly_linked_node.hpp"
#include "exception.hpp"
#include "iterator_base.hpp"


namespace speed::containers {


/**
 * @brief       Class that represents a doubly linked list.
 */
template<typename TpValue, typename TpAllocator = std::allocator<int>>
class circular_doubly_linked_list
{
public:
    /** The value type. */
    using value_type = TpValue;
    
    /** The allocator type. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;
    
    /** The node type. */
    using node_type = doubly_linked_node<value_type>;
    
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
        using node_type = doubly_linked_node<value_type>;
        
        /**
         * @brief       Constructor with parameters.
         * @param       fir : The first list node.
         * @param       cur : The current list node.
         */
        const_iterator(node_type* fir, node_type* cur) noexcept
                : fir_(fir)
                , cur_(cur)
        {
        }
    
        /**
         * @brief       Move to the forward node.
         * @return      A reference to an iterator pointing the current node.
         */
        self_type& operator ++() override
        {
            cur_ = cur_->nxt_;
            
            if (cur_ == fir_)
            {
                cur_ = nullptr;
            }
            
            return *this;
        }
    
        /**
         * @brief       Move to the backward node.
         * @return      A reference to an iterator pointing the current node.
         */
        self_type& operator --() override
        {
            cur_ = cur_->prev_;

            if (cur_ == fir_->prev_)
            {
                cur_ = nullptr;
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
            return cur_ == rhs.cur_;
        }
    
        /**
         * @brief       Allows knowing whether the iterator is past-the-end or not.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        [[nodiscard]] bool end() const noexcept override
        {
            return cur_ == nullptr;
        }
    
        /**
         * @brief       Get the reference of the current node value.
         * @return      The reference of the current node value.
         */
        const value_type& operator *() const override
        {
            return cur_->val_;
        }
    
        /**
         * @brief       Get the address of the current node value.
         * @return      The address of the current node value.
         */
        const value_type* operator ->() const override
        {
            return &cur_->val_;
        }
        
        template<typename TpValue__, typename TpAllocator__>
        friend class circular_doubly_linked_list;
    
    protected:
        /** The first list node. */
        node_type* fir_;
        
        /** The current list node. */
        node_type* cur_;
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
        using node_type = doubly_linked_node<value_type>;
    
        /**
         * @brief       Constructor with parameters.
         * @param       fir : The first list node.
         * @param       cur : The current list node.
         */
        iterator(node_type* fir, node_type* cur) noexcept
                : base_type(fir, cur)
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
        value_type& operator *() noexcept override
        {
            return const_self_type::cur_->val_;
        }
    
        /**
         * @brief       Get the address of the current node value.
         * @return      The address of the current node value.
         */
        value_type* operator ->() noexcept override
        {
            return &const_self_type::cur_->val_;
        }
        
        template<typename TpValue_, typename TpAllocator_>
        friend class circular_doubly_linked_list;
    };
    
    // TODO: Implement reverce iterators.
    
    /**
     * @brief       Default constructor.
     */
    circular_doubly_linked_list()
            : fir_(nullptr)
            , sz_(0)
            , alloctr_()
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : The object to copy.
     */
    circular_doubly_linked_list(const circular_doubly_linked_list& rhs)
            : circular_doubly_linked_list()
    {
        for (auto& x : rhs)
        {
            push_back(x);
        }
    }
    
    /**
     * @brief       Move constrcutor.
     * @param       rhs : The object to move.
     */
    circular_doubly_linked_list(circular_doubly_linked_list&& rhs) noexcept
            : fir_(rhs.fir_)
            , sz_(rhs.sz_)
            , alloctr_(std::move(rhs.alloctr_))
    {
        rhs.fir_ = nullptr;
        rhs.sz_ = 0;
    }
    
    /**
     * @brief       Destructor.
     */
    ~circular_doubly_linked_list()
    {
        clear();
    }
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : The object to copy.
     * @return      The object who call the method.
     */
    circular_doubly_linked_list& operator =(const circular_doubly_linked_list& rhs)
    {
        if (this != &rhs)
        {
            clear();
    
            for (auto& x : rhs)
            {
                push_back(x);
            }
        }
    }
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : The object to move.
     * @return      The object who call the method.
     */
    circular_doubly_linked_list& operator =(circular_doubly_linked_list&& rhs) noexcept
    {
        if (this != &rhs)
        {
            std::swap(fir_, rhs.fir_);
            std::swap(sz_, rhs.sz_);
            alloctr_ = std::move(rhs.alloctr_);
        }
    }
    
    /**
     * @brief       Get an iterator for the first element in the container.
     * @return      An iterator for the first element in the container.
     */
    inline iterator begin() noexcept
    {
        return iterator(fir_, fir_);
    }
    
    /**
     * @brief       Get a const iterator for the first element in the container.
     * @return      A const iterator for the first element in the container.
     */
    inline const_iterator cbegin() const noexcept
    {
        return const_iterator(fir_, fir_);
    }
    
    /**
     * @brief       Get an iterator to the past-the-end element in the container.
     * @return      An iterator to the past-the-end element in the container
     */
    inline iterator end() noexcept
    {
        return iterator(nullptr, nullptr);
    }
    
    /**
     * @brief       Get a const iterator to the past-the-end element in the container.
     * @return      A const iterator to the past-the-end element in the container
     */
    inline const_iterator cend() const noexcept
    {
        return const_iterator(nullptr, nullptr);
    }
    
    /**
     * @brief       Check whether the container is empty.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool empty() const noexcept
    {
        return fir_ == nullptr;
    }
    
    /**
     * @brief       Get the container size.
     * @return      The container size.
     */
    [[nodiscard]] inline std::size_t size() const noexcept
    {
        return sz_;
    }
    
    /**
     * @brief       Get the first element.
     * @return      The first element.
     */
    value_type& front()
    {
        if (fir_ == nullptr)
        {
            throw empty_container_exception();
        }
    
        return fir_->val_;
    }
    
    /**
     * @brief       Get the first element.
     * @return      The first element.
     */
    const value_type& front() const
    {
        if (fir_ == nullptr)
        {
            throw empty_container_exception();
        }
    
        return fir_->val_;
    }
    
    /**
     * @brief       The the last element.
     * @return      The last element.
     */
    value_type& back()
    {
        if (fir_ == nullptr)
        {
            throw empty_container_exception();
        }
    
        return fir_->prev_->val_;
    }
    
    /**
     * @brief       The the last element.
     * @return      The last element.
     */
    const value_type& back() const
    {
        if (fir_ == nullptr)
        {
            throw empty_container_exception();
        }
    
        return fir_->prev_->val_;
    }
    
    /**
     * @brief       Add a element in the front of the container.
     * @param       val : The element to add in the container.
     */
    template<typename TpValue_>
    void push_front(TpValue_&& val)
    {
        node_type* trg = alloctr_.allocate(1);
        alloctr_.construct(trg, std::forward<TpValue_>(val));
        
        insert_before_node(trg, fir_);
        fir_ = fir_->prev_;
    }
    
    /**
     * @brief       Erase the front element in the container.
     */
    void pop_front()
    {
        if (fir_ == nullptr)
        {
            throw empty_container_exception();
        }
        
        node_type* trg = fir_;

        erase_node(trg);

        alloctr_.destroy(trg);
        alloctr_.deallocate(trg, 1);
    }
    
    /**
     * @brief       Add a element in the back of the container.
     * @param       val : The element to add in the container.
     */
    template<typename TpValue_>
    void push_back(TpValue_&& val)
    {
        node_type* trg = alloctr_.allocate(1);
        alloctr_.construct(trg, std::forward<TpValue_>(val));
    
        insert_before_node(trg, fir_);
    }
    
    /**
     * @brief       Erase the back element in the container.
     */
    void pop_back()
    {
        if (fir_ == nullptr)
        {
            throw empty_container_exception();
        }
        
        node_type* trg = fir_->prev_;

        erase_node(trg);

        alloctr_.destroy(trg);
        alloctr_.deallocate(trg, 1);
    }
    
    /**
     * @brief       Add an element before the specified position.
     * @param       pos : The specified position.
     * @param       val : The value to add.
     */
    template<typename TpValue_>
    void insert_before(const_iterator& pos, TpValue_&& val)
    {
        node_type* trg = alloctr_.allocate(1);
        alloctr_.construct(trg, std::forward<TpValue_>(val));
    
        insert_before_iterator(trg, pos);
        
        if (pos.cur_ == fir_)
        {
            fir_ = fir_->prev_;
            pos.fir_ = fir_;
        }
    }
    
    /**
     * @brief       Add an element after the specified position.
     * @param       pos : The specified position.
     * @param       val : The value to add.
     */
    template<typename TpValue_>
    void insert_after(const_iterator& pos, TpValue_&& val)
    {
        node_type* trg = alloctr_.allocate(1);
        alloctr_.construct(trg, std::forward<TpValue_>(val));
        
        insert_after_iterator(trg, pos);
    }
    
    /**
     * @brief       Erase the element before the specified position.
     * @param       pos : The specified position.
     */
    void erase_before(const_iterator& pos)
    {
        if (fir_ == nullptr)
        {
            throw empty_container_exception();
        }
        
        node_type* trg = pos.cur_->prev_;

        erase_before_iterator(pos);

        alloctr_.destroy(trg);
        alloctr_.deallocate(trg, 1);
    }
    
    /**
     * @brief       Erase the element after the specified position.
     * @param       pos : The specified position.
     */
    void erase_after(const_iterator& pos)
    {
        if (fir_ == nullptr)
        {
            throw empty_container_exception();
        }
        
        node_type* trg = pos.cur_->nxt_;
    
        erase_after_iterator(pos);

        alloctr_.destroy(trg);
        alloctr_.deallocate(trg, 1);
    }
    
    /**
     * @brief       Erase the element in the specified position and move the iterator backward.
     * @param       pos : The specified position.
     */
    void erase_and_move_backward(const_iterator& pos)
    {
        if (fir_ == nullptr)
        {
            throw empty_container_exception();
        }
        
        node_type* trg = pos.cur_;

        erase_iterator_and_move_backward(pos);

        alloctr_.destroy(trg);
        alloctr_.deallocate(trg, 1);
    }
    
    /**
     * @brief       Erase the element in the specified position and move the iterator forward.
     * @param       pos : The specified position.
     */
    void erase_and_move_forward(const_iterator& pos)
    {
        if (fir_ == nullptr)
        {
            throw empty_container_exception();
        }
        
        node_type* trg = pos.cur_;
    
        erase_iterator_and_move_forward(pos);

        alloctr_.destroy(trg);
        alloctr_.deallocate(trg, 1);
    }
    
    /**
     * @brief       Erase all elements in the container.
     */
    void clear() noexcept
    {
        if (fir_ != nullptr)
        {
            node_type* end = fir_;
            node_type* cur;
        
            do
            {
                cur = fir_;
                fir_ = fir_->nxt_;
            
                alloctr_.destroy(cur);
                alloctr_.deallocate(cur, 1);
            
            } while (fir_ != end);
            
            fir_ = nullptr;
        }
    }

private:
    /**
     * @brief       Insert the node before the iterator.
     * @param       trg : The node to insert.
     * @param       it : The iterator.
     */
    void insert_before_iterator(node_type* trg, const_iterator& it) noexcept
    {
        insert_before_node(trg, it.cur_);
        
        if (it.fir_ == nullptr)
        {
            it.fir_ = fir_;
            it.cur_ = fir_;
        }
    }
    
    /**
     * @brief       Insert the node after the iterator.
     * @param       trg : The node to insert.
     * @param       it : The iterator.
     */
    void insert_after_iterator(node_type* trg, const_iterator& it) noexcept
    {
        insert_after_node(trg, it.cur_);
        
        if (it.fir_ == nullptr)
        {
            it.fir_ = fir_;
            it.cur_ = fir_;
        }
    }
    
    /**
     * @brief       Insert node before node.
     * @param       trg : The node to insert.
     * @param       nod : The node used to specify the position.
     */
    void insert_before_node(node_type* trg, node_type* nod) noexcept
    {
        if (fir_ == nullptr)
        {
            trg->nxt_ = trg;
            trg->prev_ = trg;
            fir_ = trg;
        }
        else
        {
            trg->prev_ = nod->prev_;
            trg->nxt_ = nod;
            nod->prev_->nxt_ = trg;
            nod->prev_ = trg;
        }
    
        sz_++;
    }
    
    /**
     * @brief       Insert node after node.
     * @param       trg : The node to insert.
     * @param       nod : The node used to specify the position.
     */
    void insert_after_node(node_type* trg, node_type* nod) noexcept
    {
        if (fir_ == nullptr)
        {
            trg->nxt_ = trg;
            trg->prev_ = trg;
            fir_ = trg;
        }
        else
        {
            trg->nxt_ = nod->nxt_;
            trg->prev_ = nod;
            nod->nxt_->prev_ = trg;
            nod->nxt_ = trg;
        }
        
        sz_++;
    }
    
    /**
     * @brief       Erase node before iterator.
     * @param       it : The iterator.
     */
    void erase_before_iterator(const_iterator& it) noexcept
    {
        erase_node(it.cur_->prev_);
        it.fir_ = fir_;
        
        if (fir_ == nullptr)
        {
            it.cur_ = nullptr;
        }
    }
    
    /**
     * @brief       Erase node after iterator.
     * @param       it : The iterator.
     */
    void erase_after_iterator(const_iterator& it) noexcept
    {
        erase_node(it.cur_->nxt_);
        it.fir_ = fir_;
        
        if (fir_ == nullptr)
        {
            it.cur_ = nullptr;
        }
    }
    
    /**
     * @brief       Erase the node pointed by the iterator and move the iterator backward.
     * @param       it : The iterator.
     */
    void erase_iterator_and_move_backward(const_iterator& it) noexcept
    {
        node_type* prev = it.cur_->prev_;
        
        erase_node(it.cur_);
        it.fir_ = fir_;
        
        if (fir_ == nullptr)
        {
            it.cur_ = nullptr;
        }
        else
        {
            it.cur_ = prev;
        }
    }
    
    /**
     * @brief       Erase the node pointed by the iterator and move the iterator forward.
     * @param       it : The iterator.
     */
    void erase_iterator_and_move_forward(const_iterator& it) noexcept
    {
        node_type* nxt = it.cur_->nxt_;
        
        erase_node(it.cur_);
        it.fir_ = fir_;
    
        if (fir_ == nullptr)
        {
            it.cur_ = nullptr;
        }
        else
        {
            it.cur_ = nxt;
        }
    }
    
    /**
     * @brief       Erase the specified node.
     * @param       trg : The node to erase.
     */
    void erase_node(node_type* trg) noexcept
    {
        if (fir_->nxt_ == fir_)
        {
            fir_ = nullptr;
        }
        else
        {
            if (fir_ == trg)
            {
                fir_ = fir_->nxt_;
            }
    
            trg->prev_->nxt_ = trg->nxt_;
            trg->nxt_->prev_ = trg->prev_;
        }
        
        sz_--;
    }
    
private:
    /** The first node in the list. */
    node_type* fir_;
    
    /** The list size. */
    std::size_t sz_;
    
    /** The allocator for the nodes. */
    allocator_type<node_type> alloctr_;
};


}


#endif

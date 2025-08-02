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
 * @file        basic_type_caster.hpp
 * @brief       basic_type_caster main header.
 * @author      Killian Valverde
 * @date        2024/05/15
 */

#ifndef SPEED_ARGPARSE_DETAIL_BASIC_TYPE_CASTER_HPP
#define SPEED_ARGPARSE_DETAIL_BASIC_TYPE_CASTER_HPP

#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../../type_casting/type_casting.hpp"
#include "type_traits.hpp"

namespace speed::argparse::detail {

/**
 * @brief       Class used as the base for holding a type-casting strategy.
 */
template<typename SourceT>
class type_caster_base
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;

     /** Default constructor. */
     type_caster_base() = default;

     /** Default destructor. */
    virtual ~type_caster_base() = default;
    
    /**
     * @brief       Notifies the observer of an event. This method is intended to be overridden in
     *              derived classes to define custom notification behavior. It is called when an
     *              event occurs that the observer should react to.
     */
    virtual void request_addition() noexcept
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] virtual bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept
    {
        return false;
    }
};

/**
 * @brief       Class used as the base for holding a type-casting strategy for nested structures.
 */
template<typename SourceT>
class type_caster_nested_base : public type_caster_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;

    /**
     * @brief       Default constructor.
     */
    explicit type_caster_nested_base()
            : needs_additn(true)
    {
    }
    
    /**
     * @brief       Checks whether the element is marked as needing an addition.
     * @return      true if an element addition is required; false otherwise.
     */
    [[nodiscard]] bool needs_addition() const noexcept
    {
        return needs_additn;
    }
    
    /**
     * @brief       Notifies the type caster that a new element needs to be added.
     */
    void request_addition() noexcept override
    {
        needs_additn = true;
    }
    
    /**
     * @brief       Marks the element as no longer needing an addition.
     */
    void set_addition_done() noexcept
    {
        needs_additn = false;
    }
    
private:
    /** If set to true, a new element will be added to the collection. */
    bool needs_additn;
};

/**
 * @brief       Class used for holding a type-casting strategy.
 */
template<typename SourceT, typename TargetT>
class basic_type_caster : public type_caster_base<SourceT>
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<
            typename SourceT::allocator_type>::template rebind_alloc<T>;
    
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(target_type* holdr)
            : holdr_(holdr)
    {
    }
    
    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        return type_casting::try_type_cast(arg, *holdr_, err_code);
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    target_type* holdr_;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in an array.
 */
template<typename SourceT, typename TargetT, std::size_t SIZE>
class basic_type_caster<SourceT, std::array<TargetT, SIZE>>
        : public type_caster_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;
    
    /** Array type used in the class. */
    using array_type = std::array<target_type, SIZE>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(array_type* holdr)
            : holdr_(holdr)
    {
    }
    
    /**
     * @brief       Attempts to perform a type cast from a source value to a stored value.
     * @param       arg : The source value to be type-cast.
     * @param       err_code : Optional pointer to a `std::error_code` to receive error details
     *              if the cast fails. Can be `nullptr` if not needed.
     * @return      true If the type cast succeeded, false If the type cast failed.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        auto& val = holdr_->at(idx_);
        if (!type_casting::try_type_cast(arg, val, err_code))
        {
            return false;
        }
        
        if (idx_ + 1 < SIZE)
        {
            ++idx_;
        }
        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    array_type* holdr_;

    /** Index of the next element in which set the casting result. */
    std::size_t idx_ = 0;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in an array.
 */
template<typename SourceT, typename TargetT, std::size_t SIZE>
/** @cond */requires (is_supported_container_v<TargetT>)/** @endcond */
class basic_type_caster<SourceT, std::array<TargetT, SIZE>>
        : public type_caster_nested_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;
    
    /** Type that represents the base class. */
    using base_class_type = type_caster_nested_base<SourceT>;
    
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<
            typename SourceT::allocator_type>::template rebind_alloc<T>;
    
    /** Unique pointer type used in the class. */
    template<typename T>
    using unique_ptr_type = std::unique_ptr<T>;
    
    /** Array type used in the class. */
    using array_type = std::array<target_type, SIZE>;
    
    /** The base of the type caster. */
    using type_caster_base_type = type_caster_base<source_type>;
    
    /** The type caster. */
    template<typename T>
    using type_caster_type = basic_type_caster<source_type, T>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(array_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        if (base_class_type::needs_addition())
        {
            castr_ = memory::allocate_unique<type_caster_type<target_type>>(
                    allocator_type<type_caster_type<target_type>>(), &holdr_->at(idx_));
        
            if (idx_ + 1 < SIZE)
            {
                ++idx_;
            }
            base_class_type::set_addition_done();
        }
        
        return castr_->try_type_cast(arg, err_code);
    }

private:
    /** Type casters used to cast the values. */
    unique_ptr_type<type_caster_base_type> castr_;
    
    /** Holds a reference towards the object that will contain the result of the casting. */
    array_type* holdr_;

    /** Index of the next element in which set the casting result. */
    std::size_t idx_ = 0;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in a vector.
 */
template<typename SourceT, typename TargetT, typename AllocatorT>
class basic_type_caster<SourceT, std::vector<TargetT, AllocatorT>>
        : public type_caster_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;
    
    /** Allocator type used in the class. */
    using allocator_type = AllocatorT;
    
    /** Vector type used in the class. */
    using vector_type = std::vector<target_type, allocator_type>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The vector that will be holding the casted values.
     */
    explicit basic_type_caster(vector_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type val;
        if (!type_casting::try_type_cast(arg, val, err_code))
        {
            return false;
        }

        holdr_->emplace_back(std::move(val));
        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    vector_type* holdr_;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in a vector.
 */
template<typename SourceT, typename TargetT, typename AllocatorT>
/** @cond */requires (is_supported_container_v<TargetT>)/** @endcond */
class basic_type_caster<SourceT, std::vector<TargetT, AllocatorT>>
        : public type_caster_nested_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;
    
    /** Type that represents the base class. */
    using base_class_type = type_caster_nested_base<SourceT>;
    
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<T>;
    
    /** Unique pointer type used in the class. */
    template<typename T>
    using unique_ptr_type = std::unique_ptr<T>;
    
    /** Vector type used in the class. */
    using vector_type = std::vector<target_type, allocator_type<target_type>>;
    
    /** The base of the type caster. */
    using type_caster_base_type = type_caster_base<source_type>;
    
    /** The type caster. */
    template<typename T>
    using type_caster_type = basic_type_caster<source_type, T>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(vector_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        if (base_class_type::needs_addition())
        {
            castr_ = memory::allocate_unique<type_caster_type<target_type>>(
                    allocator_type<type_caster_type<target_type>>(), &holdr_->emplace_back());
            base_class_type::set_addition_done();
        }
        
        return castr_->try_type_cast(arg, err_code);
    }

private:
    /** Type casters used to cast the values. */
    unique_ptr_type<type_caster_base_type> castr_;
    
    /** Holds a reference towards the object that will contain the result of the casting. */
    vector_type* holdr_;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in a deque.
 */
template<typename SourceT, typename TargetT, typename AllocatorT>
class basic_type_caster<SourceT, std::deque<TargetT, AllocatorT>>
        : public type_caster_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;

    /** Allocator type used in the class. */
    using allocator_type = AllocatorT;

    /** Deque type used in the class. */
    using deque_type = std::deque<target_type, allocator_type>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(deque_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type val;
        if (!type_casting::try_type_cast(arg, val, err_code))
        {
            return false;
        }

        holdr_->emplace_back(std::move(val));
        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    deque_type* holdr_;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in a deque.
 */
template<typename SourceT, typename TargetT, typename AllocatorT>
/** @cond */requires (is_supported_container_v<TargetT>)/** @endcond */
class basic_type_caster<SourceT, std::deque<TargetT, AllocatorT>>
        : public type_caster_nested_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;
    
    /** Type that represents the base class. */
    using base_class_type = type_caster_nested_base<SourceT>;
    
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<T>;
    
    /** Unique pointer type used in the class. */
    template<typename T>
    using unique_ptr_type = std::unique_ptr<T>;
    
    /** Deque type used in the class. */
    using deque_type = std::deque<target_type, AllocatorT>;
    
    /** The base of the type caster. */
    using type_caster_base_type = type_caster_base<source_type>;
    
    /** The type caster. */
    template<typename T>
    using type_caster_type = basic_type_caster<source_type, T>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(deque_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        if (base_class_type::needs_addition())
        {
            castr_ = memory::allocate_unique<type_caster_type<target_type>>(
                    allocator_type<type_caster_type<target_type>>(), &holdr_->emplace_back());
            base_class_type::set_addition_done();
        }
        
        return castr_->try_type_cast(arg, err_code);
    }

private:
    /** Type casters used to cast the values. */
    unique_ptr_type<type_caster_base_type> castr_;
    
    /** Holds a reference towards the object that will contain the result of the casting. */
    deque_type* holdr_;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in a queue.
 */
template<typename SourceT, typename TargetT, typename ContainerT>
class basic_type_caster<SourceT, std::queue<TargetT, ContainerT>>
        : public type_caster_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;

    /** Vector type used in the class. */
    using container_type = ContainerT;

    /** Queue type used in the class. */
    using queue_type = std::queue<target_type, container_type>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(queue_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type val;
        if (!type_casting::try_type_cast(arg, val, err_code))
        {
            return false;
        }

        holdr_->emplace(std::move(val));
        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    queue_type* holdr_;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in a queue.
 */
template<typename SourceT, typename TargetT, typename ContainerT>
/** @cond */requires (is_supported_container_v<TargetT>)/** @endcond */
class basic_type_caster<SourceT, std::queue<TargetT, ContainerT>>
        : public type_caster_nested_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;

    /** Vector type used in the class. */
    using container_type = ContainerT;
    
    /** Type that represents the base class. */
    using base_class_type = type_caster_nested_base<SourceT>;
    
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<
            typename SourceT::allocator_type>::template rebind_alloc<T>;
    
    /** Unique pointer type used in the class. */
    template<typename T>
    using unique_ptr_type = std::unique_ptr<T>;
    
    /** Deque type used in the class. */
    using queue_type = std::queue<target_type, container_type>;
    
    /** The base of the type caster. */
    using type_caster_base_type = type_caster_base<source_type>;
    
    /** The type caster. */
    template<typename T>
    using type_caster_type = basic_type_caster<source_type, T>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(queue_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        if (base_class_type::needs_addition())
        {
            castr_ = memory::allocate_unique<type_caster_type<target_type>>(
                    allocator_type<type_caster_type<target_type>>(), &holdr_->emplace());
            base_class_type::set_addition_done();
        }
        
        return castr_->try_type_cast(arg, err_code);
    }

private:
    /** Type casters used to cast the values. */
    unique_ptr_type<type_caster_base_type> castr_;
    
    /** Holds a reference towards the object that will contain the result of the casting. */
    queue_type* holdr_;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in a priority
 *              queue.
 */
template<typename SourceT, typename TargetT, typename ContainerT, typename CompareT>
class basic_type_caster<SourceT, std::priority_queue<TargetT, ContainerT, CompareT>>
        : public type_caster_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;

    /** Container type used in the priority queue. */
    using container_type = ContainerT;

    /** Type that represents the functor used to compare. */
    using compare_type = CompareT;
    
    /** Priority queue type used in the class. */
    using priority_queue_type = std::priority_queue<target_type, container_type, compare_type>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(priority_queue_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type val;
        if (!type_casting::try_type_cast(arg, val, err_code))
        {
            return false;
        }

        holdr_->emplace(std::move(val));
        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    priority_queue_type* holdr_;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in a stack.
 */
template<typename SourceT, typename TargetT, typename ContainerT>
class basic_type_caster<SourceT, std::stack<TargetT, ContainerT>>
        : public type_caster_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;

    /** Double ended queue type used in the class. */
    using container_type = ContainerT;

    /** Stack type used in the class. */
    using stack_type = std::stack<target_type, container_type>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(stack_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type val;
        if (!type_casting::try_type_cast(arg, val, err_code))
        {
            return false;
        }

        holdr_->emplace(std::move(val));
        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    stack_type* holdr_;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in a stack.
 */
template<typename SourceT, typename TargetT, typename ContainerT>
/** @cond */requires (is_supported_container_v<TargetT>)/** @endcond */
class basic_type_caster<SourceT, std::stack<TargetT, ContainerT>>
        : public type_caster_nested_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;

    /** Vector type used in the class. */
    using container_type = ContainerT;
    
    /** Type that represents the base class. */
    using base_class_type = type_caster_nested_base<SourceT>;
    
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<
            typename SourceT::allocator_type>::template rebind_alloc<T>;
    
    /** Unique pointer type used in the class. */
    template<typename T>
    using unique_ptr_type = std::unique_ptr<T>;

    /** Stack type used in the class. */
    using stack_type = std::stack<target_type, container_type>;
    
    /** The base of the type caster. */
    using type_caster_base_type = type_caster_base<source_type>;
    
    /** The type caster. */
    template<typename T>
    using type_caster_type = basic_type_caster<source_type, T>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(stack_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        if (base_class_type::needs_addition())
        {
            castr_ = memory::allocate_unique<type_caster_type<target_type>>(
                    allocator_type<type_caster_type<target_type>>(), &holdr_->emplace());
            base_class_type::set_addition_done();
        }
        
        return castr_->try_type_cast(arg, err_code);
    }

private:
    /** Type casters used to cast the values. */
    unique_ptr_type<type_caster_base_type> castr_;
    
    /** Holds a reference towards the object that will contain the result of the casting. */
    stack_type* holdr_;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in a forward
 *              list.
 */
template<typename SourceT, typename TargetT, typename AllocatorT>
class basic_type_caster<SourceT, std::forward_list<TargetT, AllocatorT>>
        : public type_caster_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;
    
    /** Allocator type used in the class. */
    using allocator_type = AllocatorT;

    /** Forward list type used in the class. */
    using forward_list_type = std::forward_list<target_type, allocator_type>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(forward_list_type* holdr)
            : holdr_(holdr)
            , it_(holdr_->before_begin())
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type val;
        if (!type_casting::try_type_cast(arg, val, err_code))
        {
            return false;
        }

        it_ = holdr_->emplace_after(it_, std::move(val));
        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    forward_list_type* holdr_;

    /** Iterator that holds the next position in which insert the element. */
    typename forward_list_type::iterator it_;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in a forward
 *              list.
 */
template<typename SourceT, typename TargetT, typename AllocatorT>
/** @cond */requires (is_supported_container_v<TargetT>)/** @endcond */
class basic_type_caster<SourceT, std::forward_list<TargetT, AllocatorT>>
        : public type_caster_nested_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;
    
    /** Type that represents the base class. */
    using base_class_type = type_caster_nested_base<SourceT>;
    
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<
            typename SourceT::allocator_type>::template rebind_alloc<T>;
    
    /** Unique pointer type used in the class. */
    template<typename T>
    using unique_ptr_type = std::unique_ptr<T>;

    /** Forward list type used in the class. */
    using forward_list_type = std::forward_list<target_type, AllocatorT>;
    
    /** The base of the type caster. */
    using type_caster_base_type = type_caster_base<source_type>;
    
    /** The type caster. */
    template<typename T>
    using type_caster_type = basic_type_caster<source_type, T>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(forward_list_type* holdr)
            : holdr_(holdr)
            , it_(holdr_->before_begin())
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        if (base_class_type::needs_addition())
        {
            it_ = holdr_->emplace_after(it_);
            castr_ = memory::allocate_unique<type_caster_type<target_type>>(
                    allocator_type<type_caster_type<target_type>>(), &*it_);
            base_class_type::set_addition_done();
        }
        
        return castr_->try_type_cast(arg, err_code);
    }

private:
    /** Type casters used to cast the values. */
    unique_ptr_type<type_caster_base_type> castr_;
    
    /** Holds a reference towards the object that will contain the result of the casting. */
    forward_list_type* holdr_;

    /** Iterator that holds the next position in which insert the element. */
    typename forward_list_type::iterator it_;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in a list.
 */
template<typename SourceT, typename TargetT, typename AllocatorT>
class basic_type_caster<SourceT, std::list<TargetT, AllocatorT>>
        : public type_caster_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;

    /** Allocator type used in the class. */
    using allocator_type = AllocatorT;

    /** Forward list type used in the class. */
    using list_type = std::list<target_type, allocator_type>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(list_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type val;
        if (!type_casting::try_type_cast(arg, val, err_code))
        {
            return false;
        }

        holdr_->emplace_back(std::move(val));
        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    list_type* holdr_;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in a list.
 */
template<typename SourceT, typename TargetT, typename AllocatorT>
/** @cond */requires (is_supported_container_v<TargetT>)/** @endcond */
class basic_type_caster<SourceT, std::list<TargetT, AllocatorT>>
        : public type_caster_nested_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;
    
    /** Type that represents the base class. */
    using base_class_type = type_caster_nested_base<SourceT>;
    
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<T>;
    
    /** Unique pointer type used in the class. */
    template<typename T>
    using unique_ptr_type = std::unique_ptr<T>;

    /** Forward list type used in the class. */
    using list_type = std::list<target_type, AllocatorT>;
    
    /** The base of the type caster. */
    using type_caster_base_type = type_caster_base<source_type>;
    
    /** The type caster. */
    template<typename T>
    using type_caster_type = basic_type_caster<source_type, T>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(list_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        if (base_class_type::needs_addition())
        {
            castr_ = memory::allocate_unique<type_caster_type<target_type>>(
                    allocator_type<type_caster_type<target_type>>(), &holdr_->emplace_back());
            base_class_type::set_addition_done();
        }
        
        return castr_->try_type_cast(arg, err_code);
    }

private:
    /** Type casters used to cast the values. */
    unique_ptr_type<type_caster_base_type> castr_;
    
    /** Holds a reference towards the object that will contain the result of the casting. */
    list_type* holdr_;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in a set.
 */
template<typename SourceT, typename TargetT, typename CompareT, typename AllocatorT>
class basic_type_caster<SourceT, std::set<TargetT, CompareT, AllocatorT>>
        : public type_caster_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;

    /** Type that represents the functor used to compare. */
    using compare_type = CompareT;

    /** Allocator type used in the class. */
    using allocator_type = AllocatorT;

    /** Set type used in the class. */
    using set_type = std::set<target_type, compare_type, allocator_type>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(set_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type val;
        if (!type_casting::try_type_cast(arg, val, err_code))
        {
            return false;
        }

        holdr_->emplace(std::move(val));
        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    set_type* holdr_;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in an unordered
 *              set.
 */
template<
        typename SourceT,
        typename TargetT,
        typename HashT,
        typename KeyEqualT,
        typename AllocatorT
>
class basic_type_caster<SourceT, std::unordered_set<TargetT, HashT, KeyEqualT, AllocatorT>>
        : public type_caster_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;
    
    /** Type used to hash the target. */
    using hash_type = HashT;

    /** Type used to compare the targets. */
    using key_equal_type = KeyEqualT;

    /** Allocator type used in the class. */
    using allocator_type = AllocatorT;

    /** Unordered set type used in the class. */
    using unordered_set_type = std::unordered_set<
            target_type, hash_type, key_equal_type, allocator_type>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(unordered_set_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type val;
        if (!type_casting::try_type_cast(arg, val, err_code))
        {
            return false;
        }

        holdr_->emplace(std::move(val));
        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    unordered_set_type* holdr_;
};

/**
 * @brief       Class used to hold a type-casting strategy that stores the results in an unordered
 *              multiset.
 */
template<
        typename SourceT,
        typename TargetT,
        typename HashT,
        typename KeyEqualT,
        typename AllocatorT
>
class basic_type_caster<SourceT, std::unordered_multiset<
        TargetT,
        HashT,
        KeyEqualT,
        AllocatorT>
>
        : public type_caster_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;
    
    /** Type that represents the target. */
    using target_type = TargetT;
    
    /** Type used to hash the target. */
    using hash_type = HashT;

    /** Type used to compare the targets. */
    using key_equal_type = KeyEqualT;

    /** Allocator type used in the class. */
    using allocator_type = AllocatorT;

    /** Unordered multiset type used in the class. */
    using unordered_multiset_type = std::unordered_multiset<
            target_type, hash_type, key_equal_type, allocator_type>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(unordered_multiset_type* holdr)
            : holdr_(holdr)
    {
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        target_type val;
        if (!type_casting::try_type_cast(arg, val, err_code))
        {
            return false;
        }

        holdr_->emplace(std::move(val));
        return true;
    }

private:
    /** Holds a reference towards the object that will contain the result of the casting. */
    unordered_multiset_type* holdr_;
};

/**
 * @brief       Classs used as a type casting strategy holder.
 */
template<typename SourceT, typename TargetT1, typename TargetT2>
class basic_type_caster<SourceT, std::pair<TargetT1, TargetT2>>
        : public type_caster_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;

    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<
            typename SourceT::allocator_type>::template rebind_alloc<T>;
    
    /** Unique pointer type used in the class. */
    template<typename T>
    using unique_ptr_type = std::unique_ptr<T>;
    
    /** Tuple type used in the class. */
    using pair_type = std::tuple<TargetT1, TargetT2>;
    
    /** Array type used in the class. */
    template<typename T>
    using array_type = std::array<T, 2>;
    
    /** The base of the type caster. */
    using type_caster_base_type = type_caster_base<source_type>;
    
    /** The type caster. */
    template<typename T>
    using type_caster_type = basic_type_caster<source_type, T>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(pair_type* holdr)
            : holdr_(holdr)
            , idx_(0)
    {
        castrs_.at(0) = memory::allocate_unique<type_caster_type<TargetT1>>(
                allocator_type<type_caster_type<TargetT1>>(), &holdr_->first);
        castrs_.at(1) = memory::allocate_unique<type_caster_type<TargetT2>>(
                allocator_type<type_caster_type<TargetT2>>(), &holdr_->second);
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        if (!castrs_.at(idx_)->try_type_cast(arg, err_code))
        {
            return false;
        }

        if (idx_ == 0)
        {
            ++idx_;
        }
        return true;
    }

private:
    /** Type casters used to validate the values syntax. */
    array_type<unique_ptr_type<type_caster_base_type>> castrs_;
    
    /** Holds a reference towards the object that will contain the result of the casting. */
    pair_type* holdr_;
    
    /** Index of the next caster to use. */
    std::size_t idx_;
};

/**
 * @brief       Classs used as a type casting strategy holder.
 */
template<typename SourceT, typename... TargetTs>
class basic_type_caster<SourceT, std::tuple<TargetTs...>>
        : public type_caster_base<SourceT>
{
public:
    /** Type that represents the source. */
    using source_type = SourceT;

    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<
            typename SourceT::allocator_type>::template rebind_alloc<T>;
    
    /** Unique pointer type used in the class. */
    template<typename T>
    using unique_ptr_type = std::unique_ptr<T>;
    
    /** Tuple type used in the class. */
    using tuple_type = std::tuple<TargetTs...>;
    
    /** Number of elements in the tuple. */
    static constexpr std::size_t SIZE = std::tuple_size<tuple_type>::value;
    
    /** Array type used in the class. */
    template<typename T>
    using array_type = std::array<T, SIZE>;
    
    /** The base of the type caster. */
    using type_caster_base_type = type_caster_base<source_type>;
    
    /** The type caster. */
    template<typename T>
    using type_caster_type = basic_type_caster<source_type, T>;

    /**
     * @brief       Constructor with parameters.
     * @param       holdr : The collection that will be holding the casted values.
     */
    explicit basic_type_caster(tuple_type* holdr)
            : holdr_(holdr)
    {
        initialize_helper(std::index_sequence_for<TargetTs...>{});
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept override
    {
        if (!castrs_.at(idx_)->try_type_cast(arg, err_code))
        {
            return false;
        }

        if (idx_ + 1 < SIZE)
        {
            ++idx_;
        }
        return true;
    }
    
private:
    /**
     * @brief       Initializes each element in the `castrs_` container using the `holdr_` values.
     * @param       sequenc : An index sequence used for template expansion
     *              (e.g., std::index_sequence<0, 1, 2, ...>).
     */
    template<std::size_t... Is>
    void initialize_helper(std::index_sequence<Is...> sequenc)
    {
        ((castrs_.at(Is) = memory::allocate_unique<type_caster_type<TargetTs>>(
                allocator_type<type_caster_type<TargetTs>>(), &std::get<Is>(*holdr_))), ...);
    }

private:
    /** Type casters used to validate the values syntax. */
    array_type<unique_ptr_type<type_caster_base_type>> castrs_;
    
    /** Holds a reference towards the object that will contain the result of the casting. */
    tuple_type* holdr_;
    
    /** Index of the next caster to use. */
    std::size_t idx_ = 0;
};

}

#endif

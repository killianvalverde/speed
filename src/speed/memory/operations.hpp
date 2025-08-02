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
 * @file        operations.hpp
 * @brief       memory functions header.
 * @author      Killian Valverde
 * @date        2024/10/10
 */

#ifndef SPEED_MEMORY_OPERATIONS_HPP
#define SPEED_MEMORY_OPERATIONS_HPP

#include <memory>
#include <utility>

namespace speed::memory {

/**
 * @brief       Allocate using an allocator and construct the object.
 * @param       alloc : Allocator used to allocate the memory.
 * @param       ptr : Pointer to object to allocate and construct.
 * @param       args : Arguments to forward to the constructor.
 * @return      The address of the allocated memory.
 */
template<typename ValueT, typename AllocatorT, typename... Ts>
ValueT* allocate_and_construct(const AllocatorT& alloc, ValueT*& ptr, Ts&&... args)
{
    using allocator_traits_type = std::allocator_traits<AllocatorT>;
    using value_allocator_type = typename allocator_traits_type::template rebind_alloc<ValueT>;
    
    value_allocator_type value_alloc(alloc);
    ptr = allocator_traits_type::allocate(value_alloc, 1);
    
    try
    {
        allocator_traits_type::construct(value_alloc, ptr, std::forward<Ts>(args)...);
    }
    catch (...)
    {
        allocator_traits_type::deallocate(value_alloc, ptr, 1);
        throw;
    }

    return ptr;
}

/**
 * @brief       Allocates and constructs an object using a custom allocator.
 * @param       alloc : The allocator instance to use for allocating and constructing the object.
 * @param       args : The arguments to forward to the constructor of `T`.
 * @return      A `std::unique_ptr<T, std::function<void(T*)>>` that owns the allocated and
 *              constructed object,
 */
template <typename ValueT, typename AllocatorT, typename... Ts>
std::unique_ptr<ValueT> allocate_unique(
        const AllocatorT& alloc,
        Ts&&... args
)
{
    using allocator_traits_type = std::allocator_traits<AllocatorT>;
    using value_allocator_type = typename allocator_traits_type::template rebind_alloc<ValueT>;
    
    ValueT* ptr;
    value_allocator_type value_alloc(alloc);
    
    allocate_and_construct(alloc, ptr, std::forward<Ts>(args)...);

    // auto deleter = [value_alloc](ValueT* p) mutable {
    //     std::allocator_traits<value_allocator_type>::destroy(value_alloc, p);
    //     std::allocator_traits<value_allocator_type>::deallocate(value_alloc, p, 1);
    // };

    return std::unique_ptr<ValueT>(ptr);
}

/**
 * @brief       Construct the given object pointer.
 * @param       ptr : The pointer to the object to construct.
 * @param       args : Arguments to forward to the object constructor.
 * @return      Same pointer as the passed argument, but points to the newly constructed object.
 */
template<typename ValueT, typename... Ts>
ValueT* construct_at(ValueT* ptr, Ts&&... args)
{
    return ::new (static_cast<void*>(ptr)) ValueT(std::forward<Ts>(args)...);
}

/**
 * @brief       Destroy the object pointer by the pointer.
 * @param       ptr : Pointer to the object to destroy.
 */
template<typename ValueT>
void destroy_at(ValueT* ptr)
{
    if (ptr)
    {
        ptr->~ValueT();
    }
}

/**
 * @brief       Destroy and deallocate using an allocator.
 * @param       alloc : Allocator used to deallocate.
 * @param       ptr : Pointer to object to destruct and deallocate.
 */
template<typename ValueT, typename AllocatorT>
void destroy_and_deallocate(const AllocatorT& alloc, ValueT* ptr)
{
    using allocator_traits_type = std::allocator_traits<AllocatorT>;
    using value_allocator_type = typename allocator_traits_type::template rebind_alloc<ValueT>;
    
    value_allocator_type value_alloc(alloc);
    
    allocator_traits_type::destroy(value_alloc, ptr);
    allocator_traits_type::deallocate(value_alloc, ptr, 1);
}

}

#endif

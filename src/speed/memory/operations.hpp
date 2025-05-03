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
template<typename TpValue, typename TpAllocator, typename... Ts>
TpValue* allocate_and_construct(const TpAllocator& alloc, TpValue*& ptr, Ts&&... args)
{
    using allocator_traits_type = std::allocator_traits<TpAllocator>;
    using value_allocator_type = typename allocator_traits_type::template rebind_alloc<TpValue>;
    
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
template <typename TpValue, typename TpAllocator, typename... Ts>
std::unique_ptr<TpValue> allocate_unique(
        const TpAllocator& alloc,
        Ts&&... args
)
{
    using allocator_traits_type = std::allocator_traits<TpAllocator>;
    using value_allocator_type = typename allocator_traits_type::template rebind_alloc<TpValue>;
    
    TpValue* ptr;
    value_allocator_type value_alloc(alloc);
    
    allocate_and_construct(alloc, ptr, std::forward<Ts>(args)...);

    // auto deleter = [value_alloc](TpValue* p) mutable {
    //     std::allocator_traits<value_allocator_type>::destroy(value_alloc, p);
    //     std::allocator_traits<value_allocator_type>::deallocate(value_alloc, p, 1);
    // };

    return std::unique_ptr<TpValue>(ptr);
}

/**
 * @brief       Construct the given object pointer.
 * @param       ptr : The pointer to the object to construct.
 * @param       args : Arguments to forward to the object constructor.
 * @return      Same pointer as the passed argument, but points to the newly constructed object.
 */
template<typename Tp, typename... Ts>
Tp* construct_at(Tp* ptr, Ts&&... args)
{
    return ::new (static_cast<void*>(ptr)) Tp(std::forward<Ts>(args)...);
}

/**
 * @brief       Destroy the object pointer by the pointer.
 * @param       ptr : Pointer to the object to destroy.
 */
template<typename Tp>
void destroy_at(Tp* ptr)
{
    if (ptr)
    {
        ptr->~Tp();
    }
}

/**
 * @brief       Destroy and deallocate using an allocator.
 * @param       alloc : Allocator used to deallocate.
 * @param       ptr : Pointer to object to destruct and deallocate.
 */
template<typename TpValue, typename TpAllocator>
void destroy_and_deallocate(const TpAllocator& alloc, TpValue* ptr)
{
    using allocator_traits_type = std::allocator_traits<TpAllocator>;
    using value_allocator_type = typename allocator_traits_type::template rebind_alloc<TpValue>;
    
    value_allocator_type value_alloc(alloc);
    
    allocator_traits_type::destroy(value_alloc, ptr);
    allocator_traits_type::deallocate(value_alloc, ptr, 1);
}

}

#endif

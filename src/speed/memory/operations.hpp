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

namespace speed::memory {

/**
 * @brief       Allocate using an allocator and construct the object.
 * @param       alloc : Allocator used to allocate the memory.
 * @param       ptr : Pointer to object to allocate and construct.
 * @param       args : Arguments to forward to the constructor.
 * @return      The address of the allocated memory.
 */
template<typename TpAllocator, typename... Ts>
typename std::allocator_traits<TpAllocator>::value_type*
allocate_and_construct(
        TpAllocator& alloc,
        typename std::allocator_traits<TpAllocator>::value_type*& ptr,
        Ts&&... args
)
{
    ptr = std::allocator_traits<TpAllocator>::allocate(alloc, 1);
    std::allocator_traits<TpAllocator>::construct(alloc, ptr, std::forward<Ts>(args)...);

    return ptr;
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
template<typename TpAllocator>
void destruct_and_deallocate(
        TpAllocator& alloc,
        typename std::allocator_traits<TpAllocator>::value_type* ptr
)
{
    std::allocator_traits<TpAllocator>::destroy(alloc, ptr);
    std::allocator_traits<TpAllocator>::deallocate(alloc, ptr, 1);
}

}

#endif

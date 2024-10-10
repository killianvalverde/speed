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
 * @file        speed/memory/operations.hpp
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
 * @param       args : Arguments to forward to the constructor.
 * @return      The address of the allocated memory.
 */
template<typename TpAllocator, typename... Ts>
typename std::allocator_traits<TpAllocator>::value_type*
allocate_and_construct(TpAllocator& alloc, Ts&&... args)
{
    using pointer_type = typename std::allocator_traits<TpAllocator>::value_type*;

    pointer_type ptr = std::allocator_traits<TpAllocator>::allocate(alloc, 1);
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


}


#endif

/* speed - Generic C++ library.
 * Copyright (C) 2015-2026 Killian Valverde.
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
 * @file operations.hpp
 * @brief Allocator-aware memory management operations.
 * @author Killian Valverde
 * @date 2024-10-10
 */

#pragma once

#include <memory>
#include <utility>

#include "allocator_deleter.hpp"
#include "types.hpp"

namespace speed::memory {

/**
 * @brief Allocates storage for an object and constructs it.
 *
 * Allocates memory for a single object of type ValueT using alloc and constructs the object
 * with the provided arguments.
 *
 * If construction throws an exception, the allocated storage is released before the exception
 * is propagated.
 *
 * @tparam ValueT Type of the object to allocate and construct.
 * @tparam AllocatorT Allocator type used to allocate storage.
 * @tparam ArgsT Types of the construction arguments.
 *
 * @param alloc Allocator used to allocate storage.
 * @param args Arguments forwarded to the constructor of ValueT.
 * @return Pointer to the constructed object.
 */
template<typename ValueT, typename AllocatorT, typename... ArgsT>
[[nodiscard]] ValueT* allocate_and_construct(const AllocatorT& alloc, ArgsT&&... args)
{
    using value_allocator_type = std::allocator_traits<AllocatorT>::template rebind_alloc<ValueT>;
    using allocator_traits_type = std::allocator_traits<value_allocator_type>;

    value_allocator_type value_alloc(alloc);
    ValueT* ptr = allocator_traits_type::allocate(value_alloc, 1);

    try
    {
        allocator_traits_type::construct(value_alloc, ptr, std::forward<ArgsT>(args)...);
    }
    catch (...)
    {
        allocator_traits_type::deallocate(value_alloc, ptr, 1);
        throw;
    }

    return ptr;
}

/**
 * @brief Destroys an object and releases its storage.
 *
 * If ptr is not null, the object is destroyed and its storage is deallocated using alloc.
 *
 * @tparam ValueT Type of the object to destroy.
 * @tparam AllocatorT Allocator type used to deallocate storage.
 *
 * @param alloc Allocator used to destroy and deallocate the object.
 * @param ptr Pointer to the object to destroy.
 */
template<typename ValueT, typename AllocatorT>
void destroy_and_deallocate(const AllocatorT& alloc, ValueT* ptr) noexcept
{
    if (ptr == nullptr)
    {
        return;
    }

    using value_allocator_type = std::allocator_traits<AllocatorT>::template rebind_alloc<ValueT>;
    using allocator_traits_type = std::allocator_traits<value_allocator_type>;

    value_allocator_type value_alloc(alloc);

    allocator_traits_type::destroy(value_alloc, ptr);
    allocator_traits_type::deallocate(value_alloc, ptr, 1);
}

/**
 * @brief Allocates, constructs, and returns an allocator-aware unique pointer.
 *
 * Allocates memory for a single object of type ValueT using alloc, constructs the object with
 * the provided arguments, and returns an allocator_unique_ptr that automatically destroys and
 * deallocates the object using the same allocator.
 *
 * @tparam ValueT Type of the object to allocate and construct.
 * @tparam AllocatorT Allocator type used to allocate storage.
 * @tparam ArgsT Types of the construction arguments.
 *
 * @param alloc Allocator used to allocate storage.
 * @param args Arguments forwarded to the constructor of ValueT.
 * @return An allocator-aware unique pointer managing the constructed object.
 */
template<typename ValueT, typename AllocatorT, typename... ArgsT>
[[nodiscard]] allocator_unique_ptr<ValueT, AllocatorT> allocate_unique(
    const AllocatorT& alloc,
    ArgsT&&... args
)
{
    return {
        allocate_and_construct<ValueT>(alloc, std::forward<ArgsT>(args)...),
        allocator_deleter<ValueT, AllocatorT>(alloc)
    };
}

}

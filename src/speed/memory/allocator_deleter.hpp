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
 * @file allocator_deleter.hpp
 * @brief Allocator-aware deleter for use with std::unique_ptr.
 * @author Killian Valverde
 * @date 2026-06-22
 */

#pragma once

#include <memory>

namespace speed::memory {

/**
 * @brief Deleter that destroys and deallocates an object using a specified allocator.
 *
 * This deleter stores a rebound allocator and can be used with std::unique_ptr
 * to provide allocator-aware object destruction and deallocation.
 *
 * @tparam ValueT Type of the managed object.
 * @tparam AllocatorT Allocator type used to deallocate the object.
 */
template<typename ValueT, typename AllocatorT>
class allocator_deleter
{
public:
    /** Managed object type. */
    using value_type = ValueT;

    /** Allocator rebound to value_type. */
    using allocator_type = typename std::allocator_traits<
        AllocatorT>::template rebind_alloc<ValueT>;

    /**
     * @brief Constructs an allocator-aware deleter.
     *
     * @param alloc Allocator used to destroy and deallocate managed objects.
     */
    explicit allocator_deleter(const AllocatorT& alloc)
        : alloc_(alloc)
    {
    }

    /**
     * @brief Destroys and deallocates the managed object.
     *
     * If ptr is not null, the object is destroyed and its storage is released
     * using the stored allocator.
     *
     * @param ptr Pointer to the object to destroy and deallocate.
     */
    void operator()(ValueT* ptr) noexcept
    {
        if (ptr == nullptr)
        {
            return;
        }

        using allocator_traits_type = std::allocator_traits<allocator_type>;

        allocator_traits_type::destroy(alloc_, ptr);
        allocator_traits_type::deallocate(alloc_, ptr, 1);
    }

private:
    /** Allocator used to destroy and deallocate managed objects. */
    allocator_type alloc_;
};

}

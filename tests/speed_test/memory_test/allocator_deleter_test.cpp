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
 * @file allocator_deleter_test.cpp
 * @brief Unit tests for the allocator_deleter class.
 * @author Killian Valverde
 * @date 2026-10-12
 */

#include <memory>
#include <gtest/gtest.h>

#include "speed/memory/memory.hpp"

namespace mem = speed::memory;

TEST(memory_allocator_deleter, destroys_object)
{
    struct tracked_object
    {
        bool* destroyed;

        ~tracked_object()
        {
            *destroyed = true;
        }
    };

    bool destroyed = false;

    using allocator_type = std::allocator<tracked_object>;
    allocator_type alloc;

    auto* ptr = std::allocator_traits<allocator_type>::allocate(alloc, 1);
    std::allocator_traits<allocator_type>::construct(alloc, ptr, &destroyed);

    mem::allocator_deleter<tracked_object, allocator_type> deleter(alloc);
    deleter(ptr);

    EXPECT_TRUE(destroyed);
}

TEST(memory_allocator_deleter, handles_null_pointer)
{
    std::allocator<int> alloc;

    mem::allocator_deleter<int, std::allocator<int>> deleter(alloc);

    EXPECT_NO_THROW(deleter(nullptr));
}

TEST(memory_allocator_deleter, works_with_unique_ptr)
{
    struct tracked_object
    {
        int value;
        bool* destroyed;

        ~tracked_object()
        {
            *destroyed = true;
        }
    };

    bool destroyed = false;

    using allocator_type = std::allocator<tracked_object>;
    using deleter_type = mem::allocator_deleter<tracked_object, allocator_type>;

    allocator_type alloc;

    auto* ptr = std::allocator_traits<allocator_type>::allocate(alloc, 1);
    std::allocator_traits<allocator_type>::construct(alloc, ptr, 42, &destroyed);

    {
        std::unique_ptr<tracked_object, deleter_type> object(ptr, deleter_type(alloc));

        EXPECT_EQ(object->value, 42);
        EXPECT_FALSE(destroyed);
    }

    EXPECT_TRUE(destroyed);
}

TEST(memory_allocator_deleter, supports_allocator_rebinding)
{
    using source_allocator_type = std::allocator<char>;
    using allocator_type = std::allocator<int>;
    using deleter_type = mem::allocator_deleter<int, source_allocator_type>;

    source_allocator_type source_alloc;
    allocator_type alloc(source_alloc);

    int* ptr = std::allocator_traits<allocator_type>::allocate(alloc, 1);
    std::allocator_traits<allocator_type>::construct(alloc, ptr, 42);

    std::unique_ptr<int, deleter_type> object(ptr, deleter_type(source_alloc));

    EXPECT_EQ(*object, 42);
}

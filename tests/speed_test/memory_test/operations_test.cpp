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
 * @file operations_test.cpp
 * @brief Unit tests for memory operations.
 * @author Killian Valverde
 * @date 2026-10-12
 */

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <gtest/gtest.h>

#include "speed/memory/memory.hpp"

namespace mem = speed::memory;

TEST(memory_allocate_and_construct, constructs_object)
{
    std::allocator<int> alloc;

    int* ptr = mem::allocate_and_construct<int>(alloc, 42);

    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(*ptr, 42);

    mem::destroy_and_deallocate(alloc, ptr);
}

TEST(memory_allocate_and_construct, forwards_constructor_arguments)
{
    struct test_object
    {
        int value;
        std::string text;

        test_object(int val, std::string str)
            : value(val)
            , text(std::move(str))
        {
        }
    };

    std::allocator<test_object> alloc;

    auto* ptr = mem::allocate_and_construct<test_object>(alloc, 42, "Hello");

    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(ptr->value, 42);
    EXPECT_EQ(ptr->text, "Hello");

    mem::destroy_and_deallocate(alloc, ptr);
}

TEST(memory_allocate_and_construct, propagates_constructor_exception)
{
    struct throwing_object
    {
        throwing_object()
        {
            throw std::runtime_error("Construction failed");
        }
    };

    std::allocator<throwing_object> alloc;

    EXPECT_THROW((void)mem::allocate_and_construct<throwing_object>(alloc), std::runtime_error);
}

TEST(memory_destroy_and_deallocate, destroys_object)
{
    struct tracked_object
    {
        bool* destroyed;

        explicit tracked_object(bool* flag)
            : destroyed(flag)
        {
        }

        ~tracked_object()
        {
            *destroyed = true;
        }
    };

    bool destroyed = false;
    std::allocator<tracked_object> alloc;

    auto* ptr = mem::allocate_and_construct<tracked_object>(alloc, &destroyed);

    ASSERT_NE(ptr, nullptr);
    EXPECT_FALSE(destroyed);

    mem::destroy_and_deallocate(alloc, ptr);

    EXPECT_TRUE(destroyed);
}

TEST(memory_destroy_and_deallocate, handles_null_pointer)
{
    std::allocator<int> alloc;

    EXPECT_NO_THROW(mem::destroy_and_deallocate(alloc, static_cast<int*>(nullptr)));
}

TEST(memory_allocate_unique, constructs_managed_object)
{
    std::allocator<int> alloc;

    auto ptr = mem::allocate_unique<int>(alloc, 42);

    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(*ptr, 42);
}

TEST(memory_allocate_unique, destroys_object_automatically)
{
    struct tracked_object
    {
        bool* destroyed;

        explicit tracked_object(bool* flag)
            : destroyed(flag)
        {
        }

        ~tracked_object()
        {
            *destroyed = true;
        }
    };

    bool destroyed = false;
    std::allocator<tracked_object> alloc;

    {
        auto ptr = mem::allocate_unique<tracked_object>(alloc, &destroyed);

        ASSERT_NE(ptr, nullptr);
        EXPECT_FALSE(destroyed);
    }

    EXPECT_TRUE(destroyed);
}

TEST(memory_allocate_unique, supports_allocator_rebinding)
{
    std::allocator<char> alloc;

    auto ptr = mem::allocate_unique<int>(alloc, 42);

    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(*ptr, 42);
}

TEST(memory_allocate_unique, propagates_constructor_exception)
{
    struct throwing_object
    {
        throwing_object()
        {
            throw std::runtime_error("Construction failed");
        }
    };

    std::allocator<throwing_object> alloc;

    EXPECT_THROW((void)mem::allocate_unique<throwing_object>(alloc), std::runtime_error);
}

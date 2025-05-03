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
 * @file        linked_lists_test.cpp
 * @brief       linked_lists_test unit test.
 * @author      Killian Valverde
 * @date        2018/01/19
 */

#include <vector>
#include <gtest/gtest.h>

#include "speed/containers/containers.hpp"

TEST(cotainers_circular_doubly_linked_lists, default_constructor)
{
    speed::containers::linked_list<int> lst;
}

TEST(cotainers_circular_doubly_linked_lists, empty)
{
    speed::containers::linked_list<int> lst;
    
    ASSERT_TRUE(lst.empty());
    
    lst.push_back(32);
    
    ASSERT_TRUE(!lst.empty());
}

TEST(cotainers_circular_doubly_linked_lists, size)
{
    speed::containers::linked_list<int> lst;
    
    ASSERT_TRUE(lst.size() == 0);
    
    lst.push_back(32);
    
    ASSERT_TRUE(lst.size() == 1);
    
    lst.pop_back();
    
    ASSERT_TRUE(lst.size() == 0);
}

TEST(cotainers_circular_doubly_linked_lists, front)
{
    speed::containers::linked_list<int> lst;
    
    lst.push_back(32);
    
    ASSERT_TRUE(lst.front() == 32);
}

TEST(cotainers_circular_doubly_linked_lists, back)
{
    speed::containers::linked_list<int> lst;
    
    lst.push_back(32);
    lst.push_back(64);
    
    ASSERT_TRUE(lst.front() == 32);
    ASSERT_TRUE(lst.back() == 64);
}

TEST(cotainers_circular_doubly_linked_lists, push_front)
{
    speed::containers::linked_list<int> lst;
    
    lst.push_front(32);
    lst.push_front(64);
    
    ASSERT_TRUE(lst.front() == 64);
    ASSERT_TRUE(lst.back() == 32);
}

TEST(cotainers_circular_doubly_linked_lists, pop_front)
{
    speed::containers::linked_list<int> lst;
    
    lst.push_front(32);
    lst.push_front(64);
    lst.pop_front();
    
    ASSERT_TRUE(lst.front() == 32);
}

TEST(cotainers_circular_doubly_linked_lists, push_back)
{
    speed::containers::linked_list<int> lst;
    
    lst.push_back(32);
    lst.push_back(64);
    
    ASSERT_TRUE(lst.front() == 32);
    ASSERT_TRUE(lst.back() == 64);
}

TEST(cotainers_circular_doubly_linked_lists, pop_back)
{
    speed::containers::linked_list<int> lst;
    
    lst.push_front(32);
    lst.push_front(64);
    lst.pop_back();
    
    ASSERT_TRUE(lst.front() == 64);
}

TEST(cotainers_circular_doubly_linked_lists, insert_before)
{
    speed::containers::linked_list<int> lst;
    
    auto it = lst.begin();
    
    lst.insert_before(it, 8);
    lst.insert_before(it, 16);
    
    ASSERT_TRUE(lst.front() == 16);
    ASSERT_TRUE(lst.back() == 8);
}

TEST(cotainers_circular_doubly_linked_lists, insert_after)
{
    speed::containers::linked_list<int> lst;
    
    auto it = lst.begin();
    
    lst.insert_after(it, 8);
    lst.insert_after(it, 16);
    
    ASSERT_TRUE(lst.front() == 8);
    ASSERT_TRUE(lst.back() == 16);
}

TEST(cotainers_circular_doubly_linked_lists, erase_before)
{
    speed::containers::linked_list<int> lst;
    
    lst.push_back(8);
    lst.push_back(16);
    lst.push_back(32);
    
    auto it = lst.begin();
    
    lst.erase_before(it);
    
    ASSERT_TRUE(lst.front() == 8);
    ASSERT_TRUE(lst.back() == 16);
}

TEST(cotainers_circular_doubly_linked_lists, erase_after)
{
    speed::containers::linked_list<int> lst;
    
    lst.push_back(8);
    lst.push_back(16);
    lst.push_back(32);
    
    auto it = lst.begin();
    
    lst.erase_after(it);
    
    ASSERT_TRUE(lst.front() == 8);
    ASSERT_TRUE(lst.back() == 32);
}

TEST(cotainers_circular_doubly_linked_lists, erase_and_move_backward)
{
    speed::containers::linked_list<int> lst;
    
    lst.push_back(8);
    lst.push_back(16);
    lst.push_back(32);
    
    auto it = lst.begin();
    
    lst.erase_and_move_backward(it);
    
    ASSERT_TRUE(lst.front() == 16);
    ASSERT_TRUE(lst.back() == 32);
    ASSERT_TRUE(*it == 32);
}

TEST(cotainers_circular_doubly_linked_lists, erase_and_move_forward)
{
    speed::containers::linked_list<int> lst;
    
    lst.push_back(8);
    lst.push_back(16);
    lst.push_back(32);
    
    auto it = lst.begin();
    
    lst.erase_and_move_forward(it);
    
    ASSERT_TRUE(lst.front() == 16);
    ASSERT_TRUE(lst.back() == 32);
    ASSERT_TRUE(*it == 16);
}

TEST(cotainers_circular_doubly_linked_lists, clear)
{
    speed::containers::linked_list<int> lst;
    
    lst.push_back(8);
    lst.push_back(16);
    lst.push_back(32);
    
    lst.clear();
    
    ASSERT_THROW(lst.front(), speed::containers::empty_container_exception);
    ASSERT_THROW(lst.back(), speed::containers::empty_container_exception);
}

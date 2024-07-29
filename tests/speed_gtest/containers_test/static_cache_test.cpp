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
 * @file        speed_gtest/containers_test/static_cache_test.cpp
 * @brief       static_cache unit test.
 * @author      Killian Valverde
 * @date        2018/01/12
 */

#include <gtest/gtest.h>

#include "speed/containers/containers.hpp"


TEST(cotainers_static_cache, default_constructor)
{
    speed::containers::static_cache<std::uint32_t, std::string, 16> buf_cache;
}


TEST(cotainers_static_cache, insert)
{
    speed::containers::static_cache<std::uint32_t, std::string, 4> buf_cache;
    
    buf_cache.insert(1, "good");
    buf_cache.insert(2, "bye");
    buf_cache.insert(3, "sad");
    buf_cache.insert(4, "world");
    
    EXPECT_THROW(buf_cache.insert(1, "..."), speed::containers::insertion_exception);
    EXPECT_TRUE(*buf_cache.find(1) == "good");
    EXPECT_TRUE(*buf_cache.find(2) == "bye");
    EXPECT_TRUE(*buf_cache.find(3) == "sad");
    EXPECT_TRUE(*buf_cache.find(4) == "world");
    
    buf_cache.insert(32874, "next");
    
    EXPECT_TRUE(buf_cache.find(1) == buf_cache.end());
}


TEST(cotainers_static_cache, find)
{
    speed::containers::static_cache<std::uint32_t, std::string, 16> buf_cache;
    
    EXPECT_TRUE(buf_cache.find(1) == buf_cache.end());
}


TEST(cotainers_static_cache, find_and_lock)
{
    speed::containers::static_cache<std::uint32_t, std::string, 4> buf_cache;
    
    buf_cache.insert(1, "good");
    buf_cache.insert(2, "bye");
    buf_cache.insert(3, "sad");
    buf_cache.insert(4, "world");
    
    EXPECT_THROW(buf_cache.insert(1, "..."), speed::containers::insertion_exception);
    EXPECT_TRUE(*buf_cache.find_and_lock(1) == "good");
    EXPECT_TRUE(*buf_cache.find(2) == "bye");
    EXPECT_TRUE(*buf_cache.find(3) == "sad");
    EXPECT_TRUE(*buf_cache.find(4) == "world");
    
    buf_cache.insert(32874, "next");
    
    EXPECT_TRUE(buf_cache.find(1) != buf_cache.end());
    EXPECT_TRUE(buf_cache.find(2) == buf_cache.end());
}


TEST(cotainers_static_cache, unlock)
{
    speed::containers::static_cache<std::uint32_t, std::string, 4> buf_cache;
    
    buf_cache.insert(1, "good");
    buf_cache.insert(2, "bye");
    buf_cache.insert(3, "sad");
    buf_cache.insert(4, "world");
    
    EXPECT_THROW(buf_cache.insert(1, "..."), speed::containers::insertion_exception);
    EXPECT_TRUE(*buf_cache.find_and_lock(1) == "good");
    EXPECT_TRUE(*buf_cache.find(2) == "bye");
    EXPECT_TRUE(*buf_cache.find(3) == "sad");
    EXPECT_TRUE(*buf_cache.find(4) == "world");
    
    buf_cache.insert(32874, "next");
    
    EXPECT_TRUE(buf_cache.find(1) != buf_cache.end());
    EXPECT_TRUE(buf_cache.find(2) == buf_cache.end());
    
    buf_cache.unlock(1);
    buf_cache.insert(5, "bye");
    buf_cache.insert(6, "sad");
    buf_cache.insert(7, "world");
    buf_cache.insert(8, "next");
    
    EXPECT_TRUE(buf_cache.find(1) == buf_cache.end());
}

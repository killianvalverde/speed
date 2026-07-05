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
 * @file types.hpp
 * @brief Memory-related type aliases.
 * @author Killian Valverde
 * @date 2026-06-22
 */

#pragma once

#include <memory>

#include "allocator_deleter.hpp"

namespace speed::memory {

/**
 * @brief Allocator-aware unique pointer.
 *
 * Combines std::unique_ptr with allocator_deleter to provide automatic
 * destruction and deallocation of objects using a specified allocator.
 *
 * @tparam ValueT Type of the managed object.
 * @tparam AllocatorT Allocator type used to deallocate the object.
 */
template<typename ValueT, typename AllocatorT>
using allocator_unique_ptr = std::unique_ptr<ValueT, allocator_deleter<ValueT, AllocatorT>>;

}

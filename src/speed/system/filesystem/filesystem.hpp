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
 * @file filesystem.hpp
 * @brief Main header for system::filesystem submodule.
 * @author Killian Valverde
 * @date 2023-10-10
 */

#pragma once

#include "access_modes.hpp"
#include "directory_stream.hpp"
#include "file_types.hpp"
#include "operations.hpp"
#include "symlink_mode.hpp"
#include "symlink_target_type.hpp"
#include "types.hpp"

namespace speed::system {

/**
 * @brief Utilities related to filesystem operations.
 */
namespace filesystem {}

#ifndef SPEED_DISABLE_ALIAS
/**
 * @brief Alias for speed::system::filesystem namespace.
 *
 * Define SPEED_DISABLE_ALIAS to disable this alias.
 */
namespace fs = filesystem;
#endif

}

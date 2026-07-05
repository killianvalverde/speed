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
 * @file symlink_mode.hpp
 * @brief Defines symbolic link resolution modes for filesystem operations.
 * @author Killian Valverde
 * @date 2026-06-08
 */

#pragma once

#include <cstdint>

namespace speed::system::filesystem {

/**
 * @brief Specifies how symbolic links are handled by filesystem operations.
 *
 * This enumeration controls whether a filesystem operation is performed on a symbolic link
 * itself or on the filesystem object referenced by the symbolic link.
 *
 * This distinction is useful for operations such as access checks, metadata retrieval, and file
 * type queries where the caller may need to choose between inspecting a symbolic link or
 * its target.
 */
enum class symlink_mode : std::uint8_t
{
    /** Resolve symbolic links and operate on their targets. */
    RESOLVE,

    /** Do not resolve symbolic links and operate on the symbolic links themselves. */
    NO_RESOLVE
};

/** Alias for symlink_mode. */
using sm_t = symlink_mode;

}

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
 * @file symlink_target_type.hpp
 * @brief Defines the symlink_target_type enumeration.
 * @author Killian Valverde
 * @date 2026-06-10
 */

#pragma once

#include <cstdint>

namespace speed::system::filesystem {

/**
 * @brief Specifies the type of target referenced by a symbolic link.
 *
 * This enumeration is primarily used when creating symbolic links.
 * Some operating systems, such as Windows, require the target type to be specified when the
 * symbolic link is created, even if the target does not yet exist.
 */
enum class symlink_target_type : std::uint8_t
{
    /** Automatically determine the target type. */
    AUTO,

    /** Indicates that the symbolic link targets a file. */
    FILE,

    /** Indicates that the symbolic link targets a directory. */
    DIRECTORY
};

/** Alias for symlink_target_type. */
using st_t = symlink_target_type;

}

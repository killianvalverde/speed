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
 * @file file_types.hpp
 * @brief File type flags.
 * @author Killian Valverde
 * @date 2019-03-22
 */

#pragma once

#include <cstdint>

#include "../../enums/enums.hpp"

namespace speed::system::filesystem {

/**
 * @brief File type flags.
 *
 * Defines the types of filesystem objects.
 * Multiple values may be combined using bitwise operators.
 */
enum class file_types : std::uint8_t
{
    /** No file type. */
    NIL = 0x0,

    /** Block device. */
    BLOCK_DEVICE = 0x1,

    /** Character device. */
    CHARACTER_DEVICE = 0x2,

    /** Directory. */
    DIRECTORY = 0x4,

    /** Regular file. */
    REGULAR_FILE = 0x8,

    /** Named pipe (FIFO). */
    PIPE = 0x10,

    /** Socket. */
    SOCKET = 0x20,

    /** Symbolic link. */
    SYMLINK = 0x40,

    /** Combination of all file type flags. */
    ALL = 0x7F
};

/**
 * @brief Alias for file_types.
 */
using ft_t = file_types;

}

/** @cond */
template<>
struct speed::scalars::is_flag_enum<speed::system::filesystem::file_types>
        : std::true_type {};
/** @endcond */

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
 * @file directory_stream.hpp
 * @brief Defines the directory_stream structure.
 * @author Killian Valverde
 * @date 2019-04-06
 */

#pragma once

#include "../platform/platform.hpp"

#include <limits>

#ifndef SPEED_WINAPI
#include <dirent.h>
#endif

#include "file_types.hpp"
#include "types.hpp"

namespace speed::system::filesystem {

/**
 * @brief Represents an open directory stream.
 *
 * This structure stores the state required to enumerate the contents of a directory using
 * open_directory(), read_directory(), and close_directory().
 *
 * After a successful call to read_directory(), the members name, inode, and type describe
 * the current directory entry.
 *
 * Instances of this structure are not intended to be manipulated directly except through the
 * filesystem directory enumeration functions.
 */
struct directory_stream
{
    /** Character type used for directory entry names. */
    using char_type = path_char_t;

    /**
     * @brief Name of the current directory entry.
     *
     * This pointer is updated by read_directory() and remains valid until the next successful
     * call to read_directory() or until the directory stream is closed.
     *
     * The value is null when no directory entry is currently available.
     */
    const char_type* name = nullptr;

    /**
     * @brief Inode identifier of the current directory entry.
     *
     * This value is updated by read_directory().
     *
     * The value is initialized to an invalid inode identifier.
     */
    inode_t inode = std::numeric_limits<inode_t>::max();

    /**
     * @brief Type of the current directory entry.
     *
     * This value is updated by read_directory().
     *
     * The value is file_types::NIL when no directory entry is currently available.
     */
    file_types type = file_types::NIL;

    /** @cond */
    struct directory_stream_native
    {
#ifdef SPEED_WINAPI
        HANDLE dir_handle = nullptr;

        std::uint8_t buffer[64 * 1024]{};

        std::size_t buffer_offset = 0;

        std::size_t valid_bytes = 0;

        bool restart_scan = true;

        std::wstring name_storage;
#else
        DIR* dir = nullptr;

        int dir_fd = -1;

        dirent* entry = nullptr;
#endif
    } native{};
    /** @endcond */
};

}

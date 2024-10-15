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
 * @file        speed/system/filesystem/file_types.hpp
 * @brief       file_types header.
 * @author      Killian Valverde
 * @date        2019/03/22
 */

#ifndef SPEED_SYSTEM_FILESYSTEM_FILE_TYPES_HPP
#define SPEED_SYSTEM_FILESYSTEM_FILE_TYPES_HPP

#include <cstdint>


namespace speed::system::filesystem {


/**
 * @brief       Represents a file type.
 */
enum class file_types : std::uint8_t
{
    /** Any file type. */
    NIL = 0x0,
    
    /** Block device. */
    BLOCK_DEVICE = 0x1,
    
    /** Character device. */
    CHARACTER_DEVICE = 0x2,
    
    /** Directory. */
    DIRECTORY = 0x4,
    
    /** Named pipe */
    FIFO = 0x8,
    
    /** Regular file. */
    REGULAR_FILE = 0x10,
    
    /** Named IPC socket. */
    SOCKET = 0x20,
    
    /** Symbolic link. */
    SYMLINK = 0x40,

    /** All the flags. */
    ALL = 0x7F
};


/** Represents the file types. */
using ft_t = file_types;


}


/** @cond */
namespace speed::lowlevel {
template<>
struct enable_bitwise_operators<speed::system::filesystem::file_types>
{
    static constexpr bool enable_with_same_type = true;
};
}
/** @endcond */


#endif

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
 * @file        speed/system/api/glibc/filesystem/directory_entity_extension.hpp
 * @brief       directory_entity_extension struct header.
 * @author      Killian Valverde
 * @date        2019/04/10
 */

#ifndef SPEED_SYSTEM_API_GLIBC_FILESYSTEM_DIRECTORY_ENTITY_EXTENSION_HPP
#define SPEED_SYSTEM_API_GLIBC_FILESYSTEM_DIRECTORY_ENTITY_EXTENSION_HPP

#include "../../../type_traits/type_traits.hpp"
#ifdef SPEED_GLIBC

#include <dirent.h>


namespace speed::system::api::glibc::filesystem {


/**
 * @brief       Struct that represents a directory entity extension.
 */
struct directory_entity_extension
{
    /** Directory iterator. */
    DIR* dir;
    
    /** Direcotry entry. */
    struct dirent* entry;
};


}


#endif
#endif

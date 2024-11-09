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
 * @file        speed/system/filesystem/basic_directory_entity.hpp
 * @brief       basic_directory_entity header.
 * @author      Killian Valverde
 * @date        2019/04/06
 */

#ifndef SPEED_SYSTEM_FILESYSTEM_BASIC_DIRECTORY_ENTITY_HPP
#define SPEED_SYSTEM_FILESYSTEM_BASIC_DIRECTORY_ENTITY_HPP

#include "types.hpp"


namespace speed::system::filesystem {


/**
 * @brief       Struct that represents a directory entity.
 */
template<typename TpChar>
struct basic_directory_entity
{
    /** The character type. */
    using char_type = TpChar;
    
    /** Name of the current entity. */
    const char_type* nme;
    
    /** Additional api dependent data. */
    void* ext;
};


/** Struct that represents a directory entity. */
using directory_entity = basic_directory_entity<char>;

/** Struct that represents a directory entity. */
using wdirectory_entity = basic_directory_entity<wchar_t>;


}


#endif

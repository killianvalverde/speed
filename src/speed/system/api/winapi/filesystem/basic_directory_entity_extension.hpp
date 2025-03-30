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
 * @file        speed/system/api/winapi/filesystem/basic_directory_entity_extension.hpp
 * @brief       basic_directory_entity_extension struct header.
 * @author      Killian Valverde
 * @date        2019/04/10
 */

#ifndef SPEED_SYSTEM_API_WINAPI_FILESYSTEM_BASIC_DIRECTORY_ENTITY_EXTENSION_HPP
#define SPEED_SYSTEM_API_WINAPI_FILESYSTEM_BASIC_DIRECTORY_ENTITY_EXTENSION_HPP

#include "../../../compatibility/compatibility.hpp"
#ifdef SPEED_WINAPI

#include "../../../filesystem/types.hpp"


namespace speed::system::api::winapi::filesystem {


/** Struct that represents a directory entity extension. */
template<typename TpChar>
struct basic_directory_entity_extension
{
};


/**
 * @brief       Struct that represents a directory entity extension.
 */
template<>
struct basic_directory_entity_extension<char>
{
    /** Directory data. */
    WIN32_FIND_DATAA find_dat;

    /** Directory handle. */
    HANDLE dir_handl;
    
    /** Path of the opened directory. */
    char pth[MAX_PATH];

    /** Indicates if a read has already been done. */
    bool read_dne;
};


/**
 * @brief       Struct that represents a directory entity extension.
 */
template<>
struct basic_directory_entity_extension<wchar_t>
{
    /** Directory data. */
    WIN32_FIND_DATAW find_dat;

    /** Directory handle. */
    HANDLE dir_handl;
    
    /** Path of the opened directory. */
    wchar_t pth[MAX_PATH];

    /** Indicates if a read has already been done. */
    bool read_dne;
};


}


namespace speed::system::filesystem {


using speed::system::api::winapi::filesystem::basic_directory_entity_extension;


}


#endif
#endif

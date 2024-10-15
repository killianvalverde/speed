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
 * @file        speed/system/filesystem/filesystem.hpp
 * @brief       system filesystem main header.
 * @author      Killian Valverde
 * @date        2023/10/10
 */

#ifndef SPEED_SYSTEM_FILESYSTEM_FILESYSTEM_HPP
#define SPEED_SYSTEM_FILESYSTEM_FILESYSTEM_HPP

#include "access_modes.hpp"
#include "basic_directory_entity.hpp"
#include "file_types.hpp"
#include "operations.hpp"


namespace speed::system {


/**
 * @brief       Contains filesystem interaction resources.
 */
namespace filesystem {}


#ifndef SPEED_DISABLE_ALIAS

/**
 * @brief       Contains filesystem interaction resources.
 */
namespace fsys = filesystem;

#endif


}


#endif

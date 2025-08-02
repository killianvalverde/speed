/* speed - Generic C++ library.
 * Copyright (C) 2015-2025 Killian Valverde.
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
 * @file        types.hpp
 * @brief       types header.
 * @author      Killian Valverde
 * @date        2025/07/07
 */

#ifndef SPEED_SYSTEM_DETAIL_WINAPI_FILESYSTEM_TYPES_HPP
#define SPEED_SYSTEM_DETAIL_WINAPI_FILESYSTEM_TYPES_HPP

#include "../../../platform/platform.hpp"
#ifdef SPEED_WINAPI

#include "../../../filesystem/access_modes.hpp"
#include "../../../filesystem/directory_entity.hpp"
#include "../../../filesystem/file_types.hpp"
#include "../../../filesystem/types.hpp"

namespace speed::system::detail::winapi::filesystem {

using system::filesystem::access_modes;

using system::filesystem::directory_entity;

using system::filesystem::file_types;

using system::filesystem::inode_t;

using system::filesystem::path_char_t;

}

#endif
#endif

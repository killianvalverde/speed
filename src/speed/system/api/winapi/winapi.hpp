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
 * @file        winapi.hpp
 * @brief       winapi main header.
 * @author      Killian Valverde
 * @date        2024/10/11
 */

#ifndef SPEED_SYSTEM_API_WINAPI_WINAPI_HPP
#define SPEED_SYSTEM_API_WINAPI_WINAPI_HPP

#include "../../compatibility/compatibility.hpp"
#ifdef SPEED_WINAPI

#include "codecs/codecs.hpp"
#include "filesystem/filesystem.hpp"
#include "process/process.hpp"
#include "terminal/terminal.hpp"
#include "time/time.hpp"

namespace speed::system::api {

/**
 * @brief       Contains all the implementation of the system functions using the WinAPI.
 */
namespace winapi {}

}

#endif
#endif

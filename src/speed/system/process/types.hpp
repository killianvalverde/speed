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
 * @file        types.hpp
 * @brief       types header.
 * @author      Killian Valverde
 * @date        2024/11/01
 */

#ifndef SPEED_SYSTEM_PROCESS_TYPES_HPP
#define SPEED_SYSTEM_PROCESS_TYPES_HPP

#include <cstdint>

namespace speed::system::process {

/** PID type. */
using pid_t = std::int64_t;

/** PPID type. */
using ppid_t = std::int64_t;

/** UID type. */
using uid_t = std::uint64_t;

/** GID type. */
using gid_t = std::uint64_t;

}

#endif

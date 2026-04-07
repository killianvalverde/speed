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
 * @file system.hpp
 * @brief Main header for system module.
 * @author Killian Valverde
 * @date 2017-01-08
 */

#ifndef SPEED_SYSTEM_SYSTEM_HPP
#define SPEED_SYSTEM_SYSTEM_HPP

#include "detail/detail.hpp"
#include "codecs/codecs.hpp"
#include "errors/errors.hpp"
#include "exception/exceptions.hpp"
#include "filesystem/filesystem.hpp"
#include "platform/platform.hpp"
#include "process/process.hpp"
#include "terminal/terminal.hpp"
#include "time/time.hpp"

namespace speed {

/**
 * @brief Contains system-level utilities and abstractions.
 *
 * This namespace is intended to group functionality related to operating system interactions.
 */
namespace system {}

#ifndef SPEED_DISABLE_ALIAS
/**
 * @brief Alias for the speed::system namespace.
 *
 * Define SPEED_DISABLE_ALIAS to disable this alias.
 */
namespace sys = system;
#endif

}

#endif

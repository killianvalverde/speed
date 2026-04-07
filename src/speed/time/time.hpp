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
 * @file time.hpp
 * @brief Main header for time module.
 * @author Killian Valverde
 * @date 2017-10-29
 */

#pragma once

#include "basic_chrono.hpp"
#include "clock_base.hpp"
#include "cpu_clock.hpp"
#include "monotonic_clock.hpp"

namespace speed {

/**
 * @brief Utilities related to time handling and measurement.
 */
namespace time {}

#ifndef SPEED_DISABLE_ALIAS
/**
 * @brief Alias for speed::time namespace.
 *
 * Define SPEED_DISABLE_ALIAS to disable this alias.
 */
namespace tm = time;
#endif

}

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
 * @file enums.hpp
 * @brief Main header for enums module.
 * @author Killian Valverde
 * @date 2018-04-28
 */

#pragma once

#include "is_flag_enum.hpp"
#include "operations.hpp"

namespace speed {

/**
 * @brief Enums utilities and operations.
 */
namespace enums {}

#ifndef SPEED_DISABLE_ALIAS
/**
 * @brief Alias for speed::enums namespace.
 *
 * Define SPEED_DISABLE_ALIAS to disable this alias.
 */
namespace enm = enums;
#endif

}

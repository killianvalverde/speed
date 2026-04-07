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
 * @file type_traits.hpp
 * @brief Main header for type_traits module.
 * @author Killian Valverde
 * @date 2016-08-05
 */

#pragma once

#include "operations.hpp"

namespace speed {

/**
 * @brief Namespace containing type trait utilities.
 */
namespace type_traits {}

#ifndef SPEED_DISABLE_ALIAS
/**
 * @brief Alias for speed::type_traits.
 *
 * Define SPEED_DISABLE_ALIAS to disable this alias.
 */
namespace traits = type_traits;
#endif

}

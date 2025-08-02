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
 * @file        speed.hpp
 * @brief       File that include all speed librarie headers.
 * @author      Killian Valverde
 * @date        2016/01/08
 */

#ifndef SPEED_SPEED_HPP
#define SPEED_SPEED_HPP

#include "algorithm/algorithm.hpp"
#include "argparse/argparse.hpp"
#include "containers/containers.hpp"
#include "errors/errors.hpp"
#include "exceptions/exceptions.hpp"
#include "filesystem/filesystem.hpp"
#include "iostream/iostream.hpp"
#include "lowlevel/lowlevel.hpp"
#include "math/math.hpp"
#include "memory/memory.hpp"
#include "safety/safety.hpp"
#include "scalars/scalars.hpp"
#include "stringutils/stringutils.hpp"
#include "system/system.hpp"
#include "time/time.hpp"
#include "type_casting/type_casting.hpp"
#include "type_traits/type_traits.hpp"
#include "version.hpp"

/**
 * @brief       Contians all speed librarie.
 */
namespace speed {}

#ifndef SPEED_DISABLE_ALIAS
/**
 * @brief       Contians all speed librarie.
 */
namespace spd = speed;
#endif

#endif

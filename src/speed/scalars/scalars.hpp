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
 * @file        speed/scalars/scalars.hpp
 * @brief       scalars main header.
 * @author      Killian Valverde
 * @date        2018/04/28
 */

#ifndef SPEED_SCALARS_SCALARS_HPP
#define SPEED_SCALARS_SCALARS_HPP

#include "scalar.hpp"
#include "operations.hpp"


namespace speed {


/**
 * @brief       Contains a set of resources to compute common scalars operations.
 */
namespace scalars {}


#ifndef SPEED_DISABLE_ALIAS

/**
 * @brief       Contains a set of resources to compute common scalars operations.
 */
namespace scals = scalars;

#endif


}


#endif

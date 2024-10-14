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
 * @file        speed/containers/containers.hpp
 * @brief       containers main header
 * @author      Killian Valverde
 * @date        2018/01/13
 */

#ifndef SPEED_CONTAINERS_CONTAINERS_HPP
#define SPEED_CONTAINERS_CONTAINERS_HPP

#include "circular_doubly_linked_list.hpp"
#include "doubly_linked_node.hpp"
#include "exception.hpp"
#include "flags.hpp"
#include "iterator_base.hpp"
#include "static_cache.hpp"


namespace speed {


/**
 * @brief       Contians definitions of containers.
 */
namespace containers {}


#ifndef SPEED_DISABLE_ALIAS

/**
 * @brief       Contians definitions of containers.
 */
namespace contain = containers;

#endif


}


#endif

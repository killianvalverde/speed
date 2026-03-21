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
 * @file        operations.hpp
 * @brief       operations main header.
 * @author      Killian Valverde
 * @date        2026/03/21
 */

#ifndef SPEED_TYPE_CASTING_OPERATIONS_HPP
#define SPEED_TYPE_CASTING_OPERATIONS_HPP

#include <string>

#include "type_cast.hpp"

namespace speed::type_casting {

/**
 * @brief       Converts a value to a UTF-8 encoded string.
 * @param       arg : The input value to be converted to UTF-8.
 * @return      A std::string containing the UTF-8 encoded representation of the input.
 */
template<typename SourceT>
std::string to_utf8(const SourceT& arg)
{
    return type_cast<std::string>(arg);
}

}

#endif

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
 * @file        speed/system/compatibility/types.hpp
 * @brief       system compatibility types main header.
 * @author      Killian Valverde
 * @date        2024/10/10
 */

#ifndef SPEED_SYSTEM_COMPATIBILITY_TYPES_HPP
#define SPEED_SYSTEM_COMPATIBILITY_TYPES_HPP

#include "select_api.hpp"


namespace speed::system::compatibility {


#ifdef SPEED_GLIBC

using path_char = char;

#elif defined(SPEED_WINAPI)

#ifdef UNICODE
using path_char = wchar_t;
#else
using path_char = char;

#endif
#else
#endif


}


#endif

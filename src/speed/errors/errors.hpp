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
 * @file        errors.hpp
 * @brief       error main header.
 * @author      Killian Valverde
 * @date        2024/04/16
 */

#ifndef SPEED_ERRORS_ERRORS_HPP
#define SPEED_ERRORS_ERRORS_HPP

namespace speed {

/**
 * @brief       Defines a series of standardized elements to report errors originating from the 
 *              speed library.
 */
namespace errors {}

#ifndef SPEED_DISABLE_ALIAS
/**
 * @brief       Defines a series of standardized elements to report errors originating from the
 *              speed library.
 */
namespace err = errors;
#endif

}

#endif

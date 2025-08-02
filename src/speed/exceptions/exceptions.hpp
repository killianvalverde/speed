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
 * @file        exceptions.hpp
 * @brief       exceptions main header.
 * @author      Killian Valverde
 * @date        2016/08/19
 */

#ifndef SPEED_EXCEPTIONS_EXCEPTIONS_HPP
#define SPEED_EXCEPTIONS_EXCEPTIONS_HPP

#include "exception_base.hpp"
#include "basic_message_exception.hpp"

namespace speed {

/**
 * @brief       Contains resources for all exceptions thrown by the elements of speed along with
 *              several types and utilities to assist handling exceptions.
 */
namespace exceptions {}

#ifndef SPEED_DISABLE_ALIAS
/**
 * @brief       Contains resources for all exceptions thrown by the elements of speed along with
 *              several types and utilities to assist handling exceptions.
 */
namespace except = exceptions;
#endif

}

#endif

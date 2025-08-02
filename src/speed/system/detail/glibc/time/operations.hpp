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
 * @brief       time operations header.
 * @author      Killian Valverde
 * @date        2017/10/18
 */

#ifndef SPEED_SYSTEM_DETAIL_GLIBC_TIME_OPERATIONS_HPP
#define SPEED_SYSTEM_DETAIL_GLIBC_TIME_OPERATIONS_HPP

#include "../../../platform/platform.hpp"
#ifdef SPEED_GLIBC

#include "../../../errors/errors.hpp"
#include "../../../time/time_specification.hpp"

namespace speed::system::detail::glibc::time {

/**
 * @brief       Get a monotonic time since some unspecified starting point.
 * @param       time_spec : The value in which store the result.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool get_monotonic_time(
        system::time::time_specification& time_spec,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Get a cpu time since some unspecified starting point.
 * @param       time_spec : The value in which store the result.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool get_cpu_time(
        system::time::time_specification& time_spec,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Get a child cpu time since some unspecified starting point.
 * @param       time_spec : The value in which store the result.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool get_child_cpu_time(
        system::time::time_specification& time_spec,
        std::error_code* err_code = nullptr
) noexcept;

}

#endif
#endif

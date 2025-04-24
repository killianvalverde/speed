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
 * @file        operations.hpp
 * @brief       operations functions header.
 * @author      Killian Valverde
 * @date        2017/10/18
 */

#ifndef SPEED_SYSTEM_TIME_OPERATIONS_HPP
#define SPEED_SYSTEM_TIME_OPERATIONS_HPP

#include <cstdint>
#include <system_error>

#include "../api/api.hpp"
#include "../compatibility/compatibility.hpp"
#include "time_specification.hpp"

namespace speed::system::time {

/**
 * @brief       Get a monotonic time since some unspecified starting point.
 * @param       time_spec : The value in which store the result.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool get_monotonic_time(
        time_specification* time_spec,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(time::get_monotonic_time, false, time_spec, err_code);
}

/**
 * @brief       Get a cpu time since some unspecified starting point.
 * @param       time_spec : The value in which store the result.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool get_cpu_time(
        time_specification* time_spec,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(time::get_cpu_time, false, time_spec, err_code);
}

}

#endif

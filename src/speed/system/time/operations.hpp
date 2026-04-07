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
 * @file operations.hpp
 * @brief Core operations for the system::time submodule.
 * @author Killian Valverde
 * @date 2017/10/18
 */

#pragma once

#include <system_error>

#include "time_value.hpp"

namespace speed::system::time {

/**
 * @brief Retrieves the current monotonic time using a high-resolution performance counter.
 *
 * @param[out] time_val Reference to a time_value object that will be populated
 *                      with the current monotonic time (seconds and nanoseconds).
 * @param[out] err_code Optional pointer to a std::error_code that will be set
 *                      in case of failure. If nullptr, errors are ignored.
 *
 * @return true if the time was successfully retrieved, false otherwise.
 */
bool get_monotonic_time(time_value& time_val, std::error_code* err_code = nullptr) noexcept;

/**
 * @brief Retrieves the total CPU time consumed by the current process.
 *
 * @param[out] time_val Reference to a time_value object that will be populated
 *                      with the total CPU time (user + kernel) consumed by the process.
 * @param[out] err_code Optional pointer to a std::error_code that will be set
 *                      in case of failure. If nullptr, errors are ignored.
 *
 * @return true if the CPU time was successfully retrieved, false otherwise.
 */
bool get_cpu_time(time_value& time_val, std::error_code* err_code = nullptr) noexcept;

}

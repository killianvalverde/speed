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
 * @file operations.cpp
 * @brief Core operations implementation for the system::time submodule.
 * @author Killian Valverde
 * @date 2017/10/18
 */

#include "../../operations.hpp"
#include "../../../errors/errors.hpp"
#include "../../../platform/platform.hpp"

#include <ctime>

namespace speed::system::time {

bool get_monotonic_time(time_value& time_val, std::error_code* err_code) noexcept
{
    timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1)
    {
        errors::assign_system_error_code(errno, err_code);
        return false;
    }

    time_val.set_seconds(ts.tv_sec).set_nanoseconds(ts.tv_nsec);

    return true;
}

bool get_cpu_time(time_value& time_val, std::error_code* err_code) noexcept
{
    timespec ts;

    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts) == -1)
    {
        errors::assign_system_error_code(errno, err_code);
        return false;
    }

    time_val.set_seconds(ts.tv_sec).set_nanoseconds(ts.tv_nsec);

    return true;
}

}

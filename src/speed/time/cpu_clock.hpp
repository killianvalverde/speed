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
 * @file cpu_clock.hpp
 * @brief Defines the cpu_clock class.
 * @author Killian Valverde
 * @date 2018-06-06
 */

#pragma once

#include "../system/system.hpp"
#include "clock_base.hpp"

namespace speed::time {

/**
 * @brief CPU time clock implementation.
 *
 * This clock provides the amount of CPU time consumed by the current process.
 * It is typically used for performance analysis and profiling.
 */
class cpu_clock : public clock_base
{
public:
    /**
     * @brief Retrieves the current CPU time.
     *
     * @return A speed::system::time::time_specification representing the CPU time consumed.
     */
    [[nodiscard]] system::time::time_value get_time() const noexcept override
    {
        system::time::time_value time_spec;
        system::time::get_cpu_time(time_spec);
        return time_spec;
    }
};

}

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
 * @file monotonic_clock.hpp
 * @brief Defines the monotonic_clock class.
 * @author Killian Valverde
 * @date 2017-10-28
 */

#pragma once

#include "../system/system.hpp"
#include "clock_base.hpp"

namespace speed::time {

/**
 * @brief Monotonic clock implementation.
 *
 * This clock provides a monotonically increasing time value that is not
 * affected by system clock adjustments (e.g., manual changes or NTP updates).
 *
 * It is typically used for measuring time intervals and durations.
 */
class monotonic_clock : public clock_base
{
public:
    /**
     * @brief Retrieves the current monotonic time.
     *
     * @return A @ref system::time::time_specification representing
     *         the current monotonic time.
     */
    [[nodiscard]] system::time::time_value get_time() const noexcept override
    {
        system::time::time_value time_spec;
        system::time::get_monotonic_time(time_spec);
        return time_spec;
    }
};

}

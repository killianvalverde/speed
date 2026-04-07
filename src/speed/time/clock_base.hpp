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
 * @file clock_base.hpp
 * @brief Defines the clock_base interface.
 * @author Killian Valverde
 * @date 2026-04-04
 */

#pragma once

#include "../system/system.hpp"

namespace speed::time {

/**
 * @brief Base interface for clock implementations.
 *
 * This class defines a common interface for retrieving the current time.
 * Derived classes must implement the get_time function to provide a time value according to
 * their specific clock source.
 */
class clock_base
{
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~clock_base() = default;

    /**
     * @brief Retrieves the current time.
     *
     * @return A speed::system::time::time_specification representing the current time of the clock.
     */
    [[nodiscard]] virtual system::time::time_value get_time() const noexcept = 0;
};

}

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
 * @file       speed/time/monotonic_chrono.hpp
 * @brief      monotonic_chrono class header.
 * @author     Killian Valverde
 * @date       2017/10/28
 */

#ifndef SPEED_TIME_MONOTONIC_CHRONO_HPP
#define SPEED_TIME_MONOTONIC_CHRONO_HPP

#include "chrono_base.hpp"


namespace speed::time {


/**
 * @brief       Class that represents a monotonic chrono.
 */
class monotonic_chrono : public chrono_base
{
public:
    /**
     * @brief       Get the specific implementation time since some unspecified starting point.
     * @return      The specific implementation time since some unspecified starting point.
     */
    [[nodiscard]] speed::system::time::time_specification get_time() const noexcept override
    {
        speed::system::time::time_specification time_spec;
        speed::system::time::get_monotonic_time(&time_spec);
        return time_spec;
    }
};


}


#endif

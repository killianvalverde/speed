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
     * @brief       Default constructor.
     */
    monotonic_chrono() noexcept;
    
    /**
     * @brief       Start the process.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool start() noexcept override;
    
    /**
     * @brief       Stop the process.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool stop() noexcept override;
    
    /**
     * @brief       Resume the process.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool resume() noexcept override;
    
    /**
     * @brief       Restart the process.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool restart() noexcept override;
    
    /**
     * @brief       Get the elapsed time since the process has begun.
     * @return      The elapsed time converted to long double.
     */
    long double get_elapsed_time() const noexcept override;
    
    /**
     * @brief       Get the elapsed time since the process has begun.
     * @return      The elapsed time in a time_specification type.
     */
    speed::system::time::time_specification get_elapsed_raw_time() const noexcept override;
    
    /**
     * @brief       Allows knowing whether the chrono is in a specified state.
     * @param       chrn_state : The state to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool is(chrono_states chrn_state) const noexcept override
    {
        return chrn_state_ == chrn_state;
    }

private:
    /** The process start time. */
    speed::system::time::time_specification start_time_spec_;
    
    /** The process elapsed time. */
    speed::system::time::time_specification elapsed_time_spec_;
    
    /** The state of the chrono. */
    chrono_states chrn_state_;
};


}


#endif

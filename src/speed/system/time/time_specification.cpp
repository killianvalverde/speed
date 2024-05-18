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
 * @file        speed/system/time/time_specification.cpp
 * @brief       time_specification class methods definition.
 * @author      Killian Valverde
 * @date        2019/04/05
 */
 
#include "time_specification.hpp"


namespace speed::system::time {


time_specification time_specification::get_elapsed_time(
        const time_specification& rhs
) const noexcept
{
    time_specification elapsed_tm;
    decltype(sec_) elapsed_sec;
    decltype(nsec_) elapsed_nsec;
    
    if (rhs.sec_ < sec_ || (rhs.sec_ == sec_ && rhs.nsec_ < nsec_))
    {
        return elapsed_tm;
    }
    
    elapsed_sec = rhs.sec_ - sec_;
    elapsed_nsec = rhs.nsec_ - nsec_;
    
    if (rhs.nsec_ < nsec_)
    {
        --elapsed_sec;
        elapsed_nsec += 1000000000L;
    }
    
    elapsed_tm.set_time_specfication(elapsed_sec, elapsed_nsec);
    
    return elapsed_tm;
}


void time_specification::balance_nseconds() noexcept
{
    while (nsec_ >= 1000000000L)
    {
        nsec_ -= 1000000000L;
        ++sec_;
    }
}


}

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
 * @file       speed/time/cpu_chrono.cpp
 * @brief      cpu_chrono class methods definition.
 * @author     Killian Valverde
 * @date       2017/10/28
 */

#include "cpu_chrono.hpp"


namespace speed::time {


cpu_chrono::cpu_chrono() noexcept
        : start_time_spec_()
        , elapsed_time_spec_()
        , chrn_state_(chrono_states::READY)
{
}


bool cpu_chrono::start() noexcept
{
    if (chrn_state_ == chrono_states::READY)
    {
        chrn_state_ = chrono_states::RUNNING;
        speed::system::time::get_cpu_time(&start_time_spec_);
        return true;
    }
    
    return false;
}


bool cpu_chrono::stop() noexcept
{
    if (chrn_state_ == chrono_states::RUNNING)
    {
        chrn_state_ = chrono_states::STOPED;
        speed::system::time::get_cpu_time(&elapsed_time_spec_);
        elapsed_time_spec_ = start_time_spec_.get_elapsed_time(elapsed_time_spec_);
        return true;
    }
    
    return false;
}


bool cpu_chrono::resume() noexcept
{
    if (chrn_state_ == chrono_states::STOPED)
    {
        chrn_state_ = chrono_states::RUNNING;
        speed::system::time::get_cpu_time(&start_time_spec_);
        start_time_spec_ = elapsed_time_spec_.get_elapsed_time(start_time_spec_);
        return true;
    }
    
    return false;
}


bool cpu_chrono::restart() noexcept
{
    chrn_state_ = chrono_states::RUNNING;
    speed::system::time::get_cpu_time(&start_time_spec_);
    return true;
}


long double cpu_chrono::get_elapsed_time() const noexcept
{
    speed::system::time::time_specification cur_time_spec;

    if (chrn_state_ == chrono_states::READY)
    {
        return 0.0L;
    }
    else if (chrn_state_ == chrono_states::RUNNING)
    {
        speed::system::time::get_cpu_time(&cur_time_spec);
        cur_time_spec = start_time_spec_.get_elapsed_time(cur_time_spec);
    }
    else
    {
        cur_time_spec = elapsed_time_spec_;
    }

    return cur_time_spec.get_time(); 
}


speed::system::time::time_specification cpu_chrono::get_elapsed_raw_time() const noexcept
{
    speed::system::time::time_specification elapsed_tme;

    if (chrn_state_ == chrono_states::RUNNING)
    {
        speed::system::time::get_cpu_time(&elapsed_tme);
        return start_time_spec_.get_elapsed_time(elapsed_tme);
    }
    else if (chrn_state_ == chrono_states::STOPED)
    {
        return elapsed_time_spec_;
    }
    
    return elapsed_tme;
}


}

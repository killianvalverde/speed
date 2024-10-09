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
 * @file       speed/time/chrono_base.cpp
 * @brief      chrono_base class methods definition.
 * @author     Killian Valverde
 * @date       2017/10/28
 */

#include "chrono_base.hpp"


namespace speed::time {


chrono_base::chrono_base() noexcept
        : start_time_spec_()
        , elapsed_time_spec_()
        , chrn_state_(chrono_states::READY)
{
}


bool chrono_base::start() noexcept
{
    if (chrn_state_ == chrono_states::READY)
    {
        chrn_state_ = chrono_states::RUNNING;
        start_time_spec_ = get_time();
        return true;
    }
    
    return false;
}


bool chrono_base::stop() noexcept
{
    if (chrn_state_ == chrono_states::RUNNING)
    {
        chrn_state_ = chrono_states::STOPED;
        elapsed_time_spec_ = start_time_spec_.get_elapsed_time(get_time());
        return true;
    }
    
    return false;
}


bool chrono_base::resume() noexcept
{
    if (chrn_state_ == chrono_states::STOPED)
    {
        chrn_state_ = chrono_states::RUNNING;
        start_time_spec_ = elapsed_time_spec_.get_elapsed_time(get_time());
        return true;
    }
    
    return false;
}


bool chrono_base::restart() noexcept
{
    if (chrn_state_ != chrono_states::READY)
    {
        chrn_state_ = chrono_states::RUNNING;
        start_time_spec_ = get_time();
        return true;
    }

    return false;
}


long double chrono_base::get_elapsed_time() const noexcept
{
    return get_elapsed_raw_time().get_time();
}


speed::system::time::time_specification chrono_base::get_elapsed_raw_time() const noexcept
{
    speed::system::time::time_specification elapsed_tme;

    if (chrn_state_ == chrono_states::RUNNING)
    {
        return start_time_spec_.get_elapsed_time(get_time());
    }
    else if (chrn_state_ == chrono_states::STOPED)
    {
        return elapsed_time_spec_;
    }
    
    return elapsed_tme;
}


template<typename TpChar, typename TpCharTraits>
std::basic_ostream<TpChar, TpCharTraits>& chrono_base::print(
        std::basic_ostream<TpChar, TpCharTraits>& os,
        std::uint8_t fixd_precision
) const
{
    char decimls[10] = {0};
    std::uint32_t cur_divider = 100'000'000;
    std::uint8_t i;
    speed::system::time::time_specification elapsd_time = get_elapsed_raw_time();
    auto nsec_bldr = elapsd_time.get_nseconds();

    if (fixd_precision > 9)
    {
        fixd_precision = 9;
    }

    for (i = 0; i < fixd_precision; i++)
    {
        decimls[i] = (char)(nsec_bldr / cur_divider) + '0';
        nsec_bldr %= cur_divider;
        cur_divider /= 10;
    }

    os << elapsd_time.get_seconds();
    if (decimls[0] != '\0')
    {
        os << "." << decimls;
    }

    return os;
}


}

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
 * @file       speed/time/chrono_base.hpp
 * @brief      chrono_base class header.
 * @author     Killian Valverde
 * @date       2017/10/28
 */

#ifndef SPEED_TIME_I_CHRONO_HPP
#define SPEED_TIME_I_CHRONO_HPP

#include <ctime>
#include <ostream>

#include "../system/system.hpp"
#include "chrono_states.hpp"


namespace speed::time {


// TODO: Implement some of the functions in the base class.
/**
 * @brief       Class that represents the base of all chronos.
 */
class chrono_base
{
public:
    /**
     * @brief       Start the process.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual bool start() noexcept = 0;
    
    /**
     * @brief       Stop the process.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual bool stop() noexcept = 0;
    
    /**
     * @brief       Resume the process.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual bool resume() noexcept = 0;
    
    /**
     * @brief       Restart the process.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual bool restart() noexcept = 0;
    
    /**
     * @brief       Get the elapsed time since the process has begun.
     * @return      The elapsed time converted to long double.
     */
    virtual long double get_elapsed_time() const noexcept = 0;
    
    /**
     * @brief       Get the elapsed time since the process has begun.
     * @return      The elapsed time in a time_specification type.
     */
    virtual speed::system::time::time_specification get_elapsed_raw_time() const noexcept = 0;
    
    /**
     * @brief       Allows knowing whether the chrono is in a specified state.
     * @param       chrn_state : The state to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual inline bool is(chrono_states chrn_state) const noexcept = 0;
    
    /**
     * @brief       Print the elapsed time with the object specified.
     * @param       os : The object used to print the elapsed time.
     */
    template<typename TpChar, typename TpCharTraits>
    std::basic_ostream<TpChar, TpCharTraits>& print(
            std::basic_ostream<TpChar, TpCharTraits>& os
    ) const
    {
        os << get_elapsed_time();
        return os;
    }
    
    /**
     * @brief       Print the elapsed time with the object specified using a scale.
     * @param       os : The object used to print the elapsed time.
     * @param       fixd_precision : The number of digits of the number decimal part.
     */
    template<typename TpChar, typename TpCharTraits>
    std::basic_ostream<TpChar, TpCharTraits>& print(
            std::basic_ostream<TpChar, TpCharTraits>& os,
            std::uint8_t fixd_precision
    ) const
    {
        char decimls[10] = {0};
        std::uint32_t cur_divider = 100'000'000;
        std::uint8_t i = 0;
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
    
    /**
     * @brief       Print the elapsed time and a new line with the object specified.
     * @param       os : The object used to print the elapsed time.
     */
    template<typename TpChar, typename TpCharTraits>
    void println(std::basic_ostream<TpChar, TpCharTraits>& os) const
    {
        print(os);
        os << '\n';
    }
    
    /**
     * @brief       Print the elapsed time and a new line with the object specified using a scale.
     * @param       os : The object used to print the elapsed time.
     * @param       fixd_precesion : The number of digits of the number decimal part.
     */
    template<typename TpChar, typename TpCharTraits>
    void println(
            std::basic_ostream<TpChar, TpCharTraits>& os,
            std::uint8_t fixd_precesion
    ) const
    {
        print(os, fixd_precesion);
        os << '\n';
    };
};


/**
 * @brief       Print the elapsed time with the object specified.
 * @param       os : The object used to print the elapsed time.
 * @param       rhs : The ichrono object to print.
 * @return      os paramether is always returned.
 */
template<typename TpChar, typename TpCharTraits>
std::basic_ostream<TpChar, TpCharTraits>& operator <<(
        std::basic_ostream<TpChar, TpCharTraits>& os,
        const chrono_base& rhs
)
{
    rhs.print(os);
    return os;
}


}


#endif

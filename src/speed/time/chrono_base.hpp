/* speed - Generic C++ library.
 * Copyright (C) 2015-2025 Killian Valverde.
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
 * @file       chrono_base.hpp
 * @brief      chrono_base class header.
 * @author     Killian Valverde
 * @date       2017/10/28
 */

#ifndef SPEED_TIME_CHRONO_BASE_HPP
#define SPEED_TIME_CHRONO_BASE_HPP

#include <ctime>
#include <ostream>

#include "../system/system.hpp"
#include "chrono_states.hpp"

namespace speed::time {

/**
 * @brief       Class that represents the base of all chronos.
 */
class chrono_base
{
public:
    /**
     * @brief       Default constructor.
     */
    chrono_base() noexcept;

    /**
     * @brief       Start the process.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual bool start() noexcept;
    
    /**
     * @brief       Stop the process.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual bool stop() noexcept;
    
    /**
     * @brief       Resume the process.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual bool resume() noexcept;
    
    /**
     * @brief       Restart the process.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual bool restart() noexcept;
    
    /**
     * @brief       Get the elapsed time since the process has begun.
     * @return      The elapsed time converted to long double.
     */
    [[nodiscard]] virtual long double get_elapsed_time() const noexcept;
    
    /**
     * @brief       Get the elapsed time since the process has begun.
     * @return      The elapsed time in a time_specification type.
     */
    [[nodiscard]] virtual system::time::time_specification
    get_elapsed_raw_time() const noexcept;
    
    /**
     * @brief       Allows knowing whether the chrono is in a specified state.
     * @param       chrn_state : The state to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] virtual bool is(chrono_states chrn_state) const noexcept
    {
        return chrn_state_ == chrn_state;
    }
    
    /**
     * @brief       Print the elapsed time with the object specified.
     * @param       os : The object used to print the elapsed time.
     */
    template<typename CharT, typename CharTraitsT>
    std::basic_ostream<CharT, CharTraitsT>& print(
            std::basic_ostream<CharT, CharTraitsT>& os
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
    template<typename CharT, typename CharTraitsT>
    std::basic_ostream<CharT, CharTraitsT>& print(
            std::basic_ostream<CharT, CharTraitsT>& os,
            std::uint8_t fixd_precision
    ) const;
    
    /**
     * @brief       Print the elapsed time and a new line with the object specified.
     * @param       os : The object used to print the elapsed time.
     */
    template<typename CharT, typename CharTraitsT>
    void println(std::basic_ostream<CharT, CharTraitsT>& os) const
    {
        print(os);
        os << '\n';
    }
    
    /**
     * @brief       Print the elapsed time and a new line with the object specified using a scale.
     * @param       os : The object used to print the elapsed time.
     * @param       fixd_precesion : The number of digits of the number decimal part.
     */
    template<typename CharT, typename CharTraitsT>
    void println(
            std::basic_ostream<CharT, CharTraitsT>& os,
            std::uint8_t fixd_precesion
    ) const
    {
        print(os, fixd_precesion);
        os << '\n';
    };

protected:
    /**
     * @brief       Get the specific implementation time since some unspecified starting point.
     * @return      The specific implementation time since some unspecified starting point.
     */
    [[nodiscard]] virtual system::time::time_specification get_time() const noexcept = 0;

private:
    /** The process start time. */
    system::time::time_specification start_time_spec_;

    /** The process elapsed time. */
    system::time::time_specification elapsed_time_spec_;

    /** The state of the chrono. */
    chrono_states chrn_state_;
};

/**
 * @brief       Print the elapsed time with the object specified.
 * @param       os : The object used to print the elapsed time.
 * @param       rhs : The ichrono object to print.
 * @return      os paramether is always returned.
 */
template<typename CharT, typename CharTraitsT>
std::basic_ostream<CharT, CharTraitsT>& operator <<(
        std::basic_ostream<CharT, CharTraitsT>& os,
        const chrono_base& rhs
)
{
    rhs.print(os);
    return os;
}

}

#endif

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
 * @file        system_time.hpp
 * @brief       system_time class header.
 * @author      Killian Valverde
 * @date        2024/10/21
 */

#ifndef SPEED_SYSTEM_TIME_SYSTEM_TIME_HPP
#define SPEED_SYSTEM_TIME_SYSTEM_TIME_HPP

#include <cstdint>

namespace speed::system::time {

/**
 * @brief       Represents a system time.
 */
class system_time
{
public:
    /** @brief Type alias for a unsigned integer to represent time. */
    using time_type = std::uint16_t;
    
    /**
     * @brief       Default constructor.
     */
    inline system_time() noexcept
            : yer_(0)
            , monh_(0)
            , dy_(0)
            , hor_(0)
            , min_(0)
            , sec_(0)
    {
    }
    
    /**
     * @brief       Constructor with parameters.
     */
    inline system_time(
            time_type yer,
            time_type monh,
            time_type dy,
            time_type hor,
            time_type min,
            time_type sec
    ) noexcept
            : yer_(yer)
            , monh_(monh)
            , dy_(dy)
            , hor_(hor)
            , min_(min)
            , sec_(sec)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : The object to copy.
     */
    inline system_time(const system_time& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    inline system_time(system_time&& rhs) noexcept
            : yer_(rhs.yer_)
            , monh_(rhs.monh_)
            , dy_(rhs.dy_)
            , hor_(rhs.hor_)
            , min_(rhs.min_)
            , sec_(rhs.sec_)
    {
        rhs.yer_ = 0;
        rhs.monh_ = 0;
        rhs.dy_ = 0;
        rhs.hor_ = 0;
        rhs.min_ = 0;
        rhs.sec_ = 0;
    }
    
    /**
     * @brief       Destructor.
     */
    virtual ~system_time() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    inline system_time& operator =(const system_time& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    inline system_time& operator =(system_time&& rhs) noexcept
    {
        if (this != &rhs)
        {
            std::swap(yer_, rhs.yer_);
            std::swap(monh_, rhs.monh_);
            std::swap(dy_, rhs.dy_);
            std::swap(hor_, rhs.hor_);
            std::swap(min_, rhs.min_);
            std::swap(sec_, rhs.sec_);
        }
        
        return *this;
    }

    /**
     * @brief       Allows knowing whether the two system times are the same.
     * @param       rhs : The object to compare.
     * @return      If the objects are the same true is returned, otherwise false is returned.
     */
    inline bool operator ==(const system_time& rhs) const noexcept
    {
        return yer_ == rhs.yer_ &&
               monh_ == rhs.monh_ &&
               dy_ == rhs.dy_ &&
               hor_ == rhs.hor_ &&
               min_ == rhs.min_ &&
               sec_ == rhs.sec_;
    }

    /**
     * @brief       Allows knowing whether two time specifications are different.
     * @param       rhs : The object to compare.
     * @return      If the objects are different true is returned, otherwise false is returned.
     */
    inline bool operator !=(const system_time& rhs) const noexcept
    {
        return !(rhs == *this);
    }
    
    /**
     * @brief       Less-than operator.
     * @param       rhs : The object to compare.
     * @return      True if this object is less than rhs, otherwise false.
     */
    inline bool operator <(const system_time& rhs) const noexcept
    {
        if (yer_ != rhs.yer_)
        {
            return yer_ < rhs.yer_;
        }
        if (monh_ != rhs.monh_)
        {
            return monh_ < rhs.monh_;
        }
        if (dy_ != rhs.dy_)
        {
            return dy_ < rhs.dy_;
        }
        if (hor_ != rhs.hor_)
        {
            return hor_ < rhs.hor_;
        }
        if (min_ != rhs.min_)
        {
            return min_ < rhs.min_;
        }
        
        return sec_ < rhs.sec_;
    }

    /**
     * @brief       Less-than or equal-to operator.
     * @param       rhs : The object to compare.
     * @return      True if this object is less than or equal to rhs, otherwise false.
     */
    inline bool operator <=(const system_time& rhs) const noexcept
    {
        return *this < rhs || *this == rhs;
    }
    
    /**
     * @brief       Greater-than operator.
     * @param       rhs : The object to compare.
     * @return      True if this object is greater than rhs, otherwise false.
     */
    inline bool operator >(const system_time& rhs) const noexcept
    {
        return !(*this <= rhs);
    }
    
    /**
     * @brief       Greater-than or equal-to operator.
     * @param       rhs : The object to compare.
     * @return      True if this object is greater than or equal to rhs, otherwise false.
     */
    inline bool operator >=(const system_time& rhs) const noexcept
    {
        return !(*this < rhs);
    }
    
    /**
     * @brief       Get the number of years.
     * @return      The number of years.
     */
    [[nodiscard]] inline time_type get_years() const noexcept
    {
        return yer_;
    }
    
    /**
     * @brief       Get the number of months.
     * @return      The number of months.
     */
    [[nodiscard]] inline time_type get_months() const noexcept
    {
        return monh_;
    }
    
    /**
     * @brief       Get the number of days.
     * @return      The number of days.
     */
    [[nodiscard]] inline time_type get_days() const noexcept
    {
        return dy_;
    }
    
    /**
     * @brief       Get the number of hours.
     * @return      The number of hours.
     */
    [[nodiscard]] inline time_type get_hours() const noexcept
    {
        return hor_;
    }
    
    /**
     * @brief       Get the number of minutes.
     * @return      The number of minutes.
     */
    [[nodiscard]] inline time_type get_minutes() const noexcept
    {
        return min_;
    }
    
    /**
     * @brief       Get the number of seconds.
     * @return      The number of seconds.
     */
    [[nodiscard]] inline time_type get_seconds() const noexcept
    {
        return sec_;
    }
    
    /**
     * @brief       Set the number of years.
     * @return      The object who called the method.
     */
    inline system_time& set_years(time_type yer) noexcept
    {
        yer_ = yer;
        return *this;
    }
    
    /**
     * @brief       Set the number of months.
     * @return      The object who called the method.
     */
    inline system_time& set_months(time_type monh) noexcept
    {
        monh_ = monh;
        return *this;
    }
    
    /**
     * @brief       Set the number of days.
     * @return      The object who called the method.
     */
    inline system_time& set_days(time_type dy) noexcept
    {
        dy_ = dy;
        return *this;
    }
    
    /**
     * @brief       Set the number of hours.
     * @return      The object who called the method.
     */
    inline system_time& set_hours(time_type hor) noexcept
    {
        hor_ = hor;
        return *this;
    }
    
    /**
     * @brief       Set the number of minutes.
     * @return      The object who called the method.
     */
    inline system_time& set_minutes(time_type min) noexcept
    {
        min_ = min;
        return *this;
    }
    
    /**
     * @brief       Set the number of seconds.
     * @return      The object who called the method.
     */
    inline system_time& set_seconds(time_type sec) noexcept
    {
        sec_ = sec;
        return *this;
    }
    
private:
    /** Years. */
    time_type yer_;
    
    /** Months. */
    time_type monh_;
    
    /** Days. */
    time_type dy_;
    
    /** Hours. */
    time_type hor_;
    
    /** Minutes. */
    time_type min_;
    
    /** Seconds. */
    time_type sec_;
};

}

#endif

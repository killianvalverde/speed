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
 * @file        speed/system/time/time_specification.hpp
 * @brief       time_specification functions header.
 * @author      Killian Valverde
 * @date        2019/04/04
 */

#ifndef SPEED_SYSTEM_TIME_TIME_SPECIFICATION_HPP
#define SPEED_SYSTEM_TIME_TIME_SPECIFICATION_HPP

#include <cstdint>
#include <memory>


namespace speed::system::time {


/**
 * @brief       Represents a time specification.
 */
class time_specification
{
public:
    /**
     * @brief       Default constructor.
     */
    inline time_specification() noexcept
            : sec_(0)
            , nsec_(0)
    {
    }
    
    /**
     * @brief       Constructor with parameters.
     * @param       sec : The number of seconds.
     * @param       nsec : The number of nanoseconds.
     */
    inline time_specification(std::uint64_t sec, std::uint64_t nsec) noexcept
            : sec_(sec)
            , nsec_(nsec)
    {
        balance_nseconds();
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : The object to copy.
     */
    inline time_specification(const time_specification& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    inline time_specification(time_specification&& rhs) noexcept
            : sec_(rhs.sec_)
            , nsec_(rhs.nsec_)
    {
        rhs.sec_ = 0;
        rhs.nsec_ = 0;
    }
    
    /**
     * @brief       Destructor.
     */
    virtual ~time_specification() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    inline time_specification& operator =(const time_specification& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    inline time_specification& operator =(time_specification&& rhs) noexcept
    {
        if (this != &rhs)
        {
            std::swap(sec_, rhs.sec_);
            std::swap(nsec_, rhs.nsec_);
        }
        
        return *this;
    }
    
    /**
     * @brief       Set the number of seconds and nano seconds.
     * @param       sec : The new number of seconds.
     * @param       nsec : The new number of nano seconds.
     */
    inline void set_time_specfication(std::uint64_t sec, std::uint64_t nsec) noexcept
    {
        sec_ = sec;
        nsec_ = nsec;
        balance_nseconds();
    }
    
    /**
     * @brief       Check whether the time is null.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool is_null() const noexcept
    {
        return sec_ == 0 && nsec_ == 0;
    }
    
    /**
     * @brief       Get the number of nano seconds.
     * @return      The number of nano seconds.
     */
    inline std::uint64_t get_nseconds() const noexcept
    {
        return nsec_;
    }
    
    /**
     * @brief       Get the number of seconds.
     * @return      The number of seconds.
     */
    inline std::uint64_t get_seconds() const noexcept
    {
        return sec_;
    }
    
    /**
     * @brief       Get the relative number of nano seconds.
     * @return      The relative number of nano seconds.
     */
    inline std::uint64_t get_relative_nseconds() const noexcept
    {
        return nsec_;
    }
    
    /**
     * @brief       Get the relative number of seconds.
     * @return      The relative number of seconds.
     */
    inline std::uint64_t get_relative_seconds() const noexcept
    {
        return sec_ % 60;
    }
    
    /**
     * @brief       Get the relative number of minutes.
     * @return      The relative number of minutes.
     */
    inline std::uint64_t get_relative_minutes() const noexcept
    {
        return (sec_ / 60) % 60;
    }
    
    /**
     * @brief       Get the relative number of hours.
     * @return      The relative number of hours.
     */
    inline std::uint64_t get_relative_hours() const noexcept
    {
        return sec_ / 3600;
    }
    
    /**
     * @brief       Get the current time under a floating point data type.
     * @return      The time under a floating point data type.
     */
    inline long double get_time() const noexcept
    {
        return ((long double)sec_ + ((long double)nsec_ / 10e8l));
    }
    
    /**
     * @brief       Get the elapsed time between the curremt pbject and another one.
     * @param       rhs : The second time specfication to get the elapsed time.
     * @return      The elapsed time between the current object and the other one.
     */
    time_specification get_elapsed_time(const time_specification& rhs) const noexcept;
    
    /**
     * @brief       Reset the current time to zero.
     */
    inline void reset() noexcept
    {
        sec_ = 0;
        nsec_ = 0;
    }
    
    /**
     * @brief       Allows knowing whether the two time specifications are the same.
     * @param       rhs : The object to compare.
     * @return      If the objects are the same true is returned, otherwise false is returned.
     */
    inline bool operator ==(const time_specification& rhs) const noexcept
    {
        return sec_ == rhs.sec_ &&
               nsec_ == rhs.nsec_;
    }
    
    /**
     * @brief       Allows knowing whether two time specifications are different.
     * @param       rhs : The object to compare.
     * @return      If the objects are different true is returned, otherwise false is returned.
     */
    inline bool operator !=(const time_specification& rhs) const noexcept
    {
        return !(rhs == *this);
    }
    
private:
    /**
     * @brief       If the nanoseconds are bigger or equal one second it will increment the seconds
     *              and decrease the nanoseconds until the nanoseconds will be lower than one
     *              second.
     */
    void balance_nseconds() noexcept;
    
private:
    /** Seconds. */
    std::uint64_t sec_;
    
    /** Nanoseconds. */
    std::uint64_t nsec_;
};


}


#endif

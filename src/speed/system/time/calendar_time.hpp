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
 * @file calendar_time.hpp
 * @brief Defines the calendar_time class.
 * @author Killian Valverde
 * @date 2024-10-21
 */

#pragma once

#include <compare>
#include <cstdint>

namespace speed::system::time {

/**
 * @brief Represents a calendar date and time with millisecond precision.
 *
 * Stores a broken-down (civil) time composed of:
 * - year
 * - month
 * - day
 * - hour
 * - minute
 * - second
 * - millisecond
 *
 * The class provides:
 * - Field accessors and modifiers
 * - Lexicographical comparison (chronological order)
 * - Optional validation via is_valid()
 *
 * No automatic validation is performed when setting values. It is the caller's
 * responsibility to ensure that the stored values represent a valid date/time.
 *
 * Default value corresponds to:
 * 1970-01-01 00:00:00.000
 */
class calendar_time
{
public:
    /** Type used to represent the year. */
    using year_type = std::uint16_t;

    /** Type used to represent the month. */
    using month_type = std::uint8_t;

    /** Type used to represent the day. */
    using day_type = std::uint8_t;

    /** Type used to represent the hour. */
    using hour_type = std::uint8_t;

    /** Type used to represent the minute. */
    using minute_type = std::uint8_t;

    /** Type used to represent the second. */
    using second_type = std::uint8_t;

    /** Type used to represent the millisecond. */
    using millisecond_type = std::uint16_t;

    /**
     * @brief Constructs a default calendar time (1970-01-01 00:00:00.000).
     */
    constexpr calendar_time() noexcept = default;

    /**
     * @brief Constructs a calendar time from individual components.
     *
     * @param year Year value.
     * @param month Month value.
     * @param day Day value.
     * @param hour Hour value.
     * @param minute Minute value.
     * @param second Second value.
     * @param millisecond Millisecond value.
     *
     * No validation is performed on the provided values.
     */
    constexpr calendar_time(
        year_type year,
        month_type month,
        day_type day,
        hour_type hour,
        minute_type minute,
        second_type second,
        millisecond_type millisecond
    ) noexcept
        : year_(year)
        , month_(month)
        , day_(day)
        , hour_(hour)
        , minute_(minute)
        , second_(second)
        , millisecond_(millisecond)
    {
    }

    /**
     * @brief Checks whether the stored date and time is valid.
     *
     * Validates:
     * - month in [1, 12]
     * - day according to month and leap year rules
     * - hour in [0, 23]
     * - minute in [0, 59]
     * - second in [0, 59]
     * - millisecond in [0, 999]
     *
     * @return true if the date/time is valid, false otherwise.
     */
    [[nodiscard]] constexpr bool is_valid() const noexcept
    {
        constexpr std::uint8_t days_in_month[] = {
            31, 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31
        };

        if (month_ < 1 || month_ > 12 || hour_ > 23 ||
            minute_ > 59 || second_ > 59 || millisecond_ > 999)
        {
            return false;
        }

        std::uint8_t dim = days_in_month[month_ - 1];

        if (month_ == 2)
        {
            if ((year_ % 4 == 0 && year_ % 100 != 0) || year_ % 400 == 0)
            {
                dim = 29;
            }
        }

        return day_ >= 1 && day_ <= dim;
    }

    /**
     * @brief Three-way comparison operator.
     *
     * Provides total ordering based on chronological order.
     */
    [[nodiscard]] constexpr auto operator<=>(const calendar_time&) const noexcept = default;

    /**
     * @brief Gets the year.
     *
     * @return The stored year value.
     */
    [[nodiscard]] constexpr year_type get_year() const noexcept
    {
        return year_;
    }

    /**
     * @brief Gets the month.
     *
     * @return The stored month value.
     */
    [[nodiscard]] constexpr month_type get_month() const noexcept
    {
        return month_;
    }

    /**
     * @brief Gets the day.
     *
     * @return The stored day value.
     */
    [[nodiscard]] constexpr day_type get_day() const noexcept
    {
        return day_;
    }

    /**
     * @brief Gets the hour.
     *
     * @return The stored hour value.
     */
    [[nodiscard]] constexpr hour_type get_hour() const noexcept
    {
        return hour_;
    }

    /**
     * @brief Gets the minute.
     *
     * @return The stored minute value.
     */
    [[nodiscard]] constexpr minute_type get_minute() const noexcept
    {
        return minute_;
    }

    /**
     * @brief Gets the second.
     *
     * @return The stored second value.
     */
    [[nodiscard]] constexpr second_type get_second() const noexcept
    {
        return second_;
    }

    /**
     * @brief Gets the millisecond.
     *
     * @return The stored millisecond value.
     */
    [[nodiscard]] constexpr millisecond_type get_millisecond() const noexcept
    {
        return millisecond_;
    }

    /**
     * @brief Sets the year.
     *
     * @param year New year value.
     * @return Reference to this object.
     */
    constexpr calendar_time& set_year(year_type year) noexcept
    {
        year_ = year;
        return *this;
    }

    /**
     * @brief Sets the month.
     *
     * @param month New month value.
     * @return Reference to this object.
     */
    constexpr calendar_time& set_month(month_type month) noexcept
    {
        month_ = month;
        return *this;
    }

    /**
     * @brief Sets the day.
     *
     * @param day New day value.
     * @return Reference to this object.
     */
    constexpr calendar_time& set_day(day_type day) noexcept
    {
        day_ = day;
        return *this;
    }

    /**
     * @brief Sets the hour.
     *
     * @param hour New hour value.
     * @return Reference to this object.
     */
    constexpr calendar_time& set_hour(hour_type hour) noexcept
    {
        hour_ = hour;
        return *this;
    }

    /**
     * @brief Sets the minute.
     *
     * @param minute New minute value.
     * @return Reference to this object.
     */
    constexpr calendar_time& set_minute(minute_type minute) noexcept
    {
        minute_ = minute;
        return *this;
    }

    /**
     * @brief Sets the second.
     *
     * @param second New second value.
     * @return Reference to this object.
     */
    constexpr calendar_time& set_second(second_type second) noexcept
    {
        second_ = second;
        return *this;
    }

    /**
     * @brief Sets the millisecond.
     *
     * @param millisecond New millisecond value.
     * @return Reference to this object.
     */
    constexpr calendar_time& set_millisecond(millisecond_type millisecond) noexcept
    {
        millisecond_ = millisecond;
        return *this;
    }
    
private:
    /** Year component. */
    year_type year_ = 1970;

    /** Month component. */
    month_type month_ = 1;

    /** Day component. */
    day_type day_ = 1;

    /** Hour component. */
    hour_type hour_ = 0;

    /** Minute component. */
    minute_type minute_ = 0;

    /** Second component. */
    second_type second_ = 0;

    /** Millisecond component. */
    millisecond_type millisecond_ = 0;
};

}

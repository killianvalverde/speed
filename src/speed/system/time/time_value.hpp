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
 * @file time_value.hpp
 * @brief Defines the time_value class.
 * @author Killian Valverde
 * @date 2019-04-04
 */

#pragma once

#include <compare>
#include <cstdint>

namespace speed::system::time {

/**
 * @brief Represents a normalized time value with nanosecond precision.
 *
 * Stores time as a pair of seconds and nanoseconds. The nanoseconds field is always
 * normalized such that:
 *
 * @code
 * 0 <= nanoseconds < 1'000'000'000
 * @endcode
 *
 * Features:
 * - Arithmetic operations (+, -, +=, -=)
 * - Total ordering via three-way comparison
 * - Compile-time support (constexpr)
 *
 * Subtraction never produces negative values; results are clamped to zero.
 */
class time_value
{
public:
    /** Type used to represent nanoseconds. */
    using nanoseconds_type = std::uint64_t;

    /** Type used to represent seconds. */
    using seconds_type = std::uint64_t;

private:
    /** Number of nanoseconds in one second. */
    static constexpr std::uint64_t NSEC_PER_SEC = 1'000'000'000ULL;

public:
    /**
     * @brief Constructs a zero time value.
     */
    constexpr time_value() noexcept = default;

    /**
     * @brief Constructs a time value from seconds and nanoseconds.
     *
     * @param sec Number of seconds.
     * @param nsec Number of nanoseconds.
     *
     * @note The resulting value is normalized so that nanoseconds is less than one second.
     */
    constexpr time_value(seconds_type sec, nanoseconds_type nsec) noexcept
        : sec_(sec)
        , nsec_(nsec)
    {
        balance_nseconds();
    }

    /**
     * @brief Three-way comparison operator.
     *
     * Provides full comparison support (==, !=, <, <=, >, >=).
     */
    [[nodiscard]] constexpr auto operator<=>(const time_value&) const noexcept = default;

    /**
     * @brief Adds two time values.
     *
     * @param rhs The value to add.
     * @return The sum of the two time values.
     */
    [[nodiscard]] constexpr time_value operator+(const time_value& rhs) const noexcept
    {
        time_value res;

        res.sec_ = sec_ + rhs.sec_;
        res.nsec_ = nsec_ + rhs.nsec_;
        res.balance_nseconds();

        return res;
    }

    /**
     * @brief Subtracts a time value from another.
     *
     * @param rhs The value to subtract.
     * @return The result of the subtraction, or zero if rhs is greater.
     */
    [[nodiscard]] constexpr time_value operator-(const time_value& rhs) const noexcept
    {
        if (*this < rhs)
        {
            return time_value{};
        }

        time_value res;

        res.sec_ = sec_ - rhs.sec_;
        res.nsec_ = nsec_ - rhs.nsec_;

        if (nsec_ < rhs.nsec_)
        {
            --res.sec_;
            res.nsec_ += NSEC_PER_SEC;
        }

        return res;
    }

    /**
     * @brief Adds a time value to this object.
     *
     * @param rhs The value to add.
     * @return Reference to this object.
     */
    constexpr time_value& operator+=(const time_value& rhs) noexcept
    {
        sec_ += rhs.sec_;
        nsec_ += rhs.nsec_;
        balance_nseconds();

        return *this;
    }

    /**
     * @brief Subtracts a time value from this object.
     *
     * @param rhs The value to subtract.
     * @return Reference to this object.
     *
     * @note If rhs is greater than the current value, the result is reset to zero.
     */
    constexpr time_value& operator-=(const time_value& rhs) noexcept
    {
        if (*this < rhs)
        {
            reset();
            return *this;
        }

        sec_  -= rhs.sec_;

        if (nsec_ < rhs.nsec_)
        {
            --sec_;
            nsec_ += NSEC_PER_SEC;
        }

        nsec_ -= rhs.nsec_;

        return *this;
    }

    /**
     * @brief Checks whether the time value is non-zero.
     *
     * @return true if the value is not zero, false otherwise.
     */
    [[nodiscard]] constexpr explicit operator bool() const noexcept
    {
        return !is_zero();
    }

    /**
     * @brief Checks whether the time value is zero.
     *
     * @return true if both seconds and nanoseconds are zero.
     */
    [[nodiscard]] constexpr bool is_zero() const noexcept
    {
        return sec_ == 0 && nsec_ == 0;
    }

    /**
     * @brief Resets the time value to zero.
     */
    constexpr void reset() noexcept
    {
        sec_ = 0;
        nsec_ = 0;
    }

    /**
     * @brief Gets the nanoseconds component.
     *
     * @return The number of nanoseconds.
     */
    [[nodiscard]] constexpr nanoseconds_type get_nanoseconds() const noexcept
    {
        return nsec_;
    }

    /**
     * @brief Gets the seconds component.
     *
     * @return The number of seconds.
     */
    [[nodiscard]] constexpr seconds_type get_seconds() const noexcept
    {
        return sec_;
    }

    /**
     * @brief Sets the nanoseconds.
     *
     * @param nsec The number of nanoseconds to set.
     * @return Reference to this time_value instance.
     *
     * @note The resulting value is normalized so that nanoseconds is less than one second.
     */
    constexpr time_value& set_nanoseconds(nanoseconds_type nsec) noexcept
    {
        nsec_ = nsec;
        balance_nseconds();

        return *this;
    }

    /**
     * @brief Sets the seconds.
     *
     * @param sec The number of seconds to set.
     * @return Reference to this time_value instance.
     */
    constexpr time_value& set_seconds(seconds_type sec) noexcept
    {
        sec_ = sec;
        return *this;
    }
    
private:
    /**
     * @brief Normalizes the nanoseconds component.
     *
     * Ensures that nanoseconds is strictly less than one second by
     * carrying excess nanoseconds into the seconds component.
     */
    constexpr void balance_nseconds() noexcept
    {
        sec_ += nsec_ / NSEC_PER_SEC;
        nsec_ %= NSEC_PER_SEC;
    }
    
private:
    /** Number of seconds. */
    seconds_type sec_ = 0;

    /** Number of nanoseconds. */
    nanoseconds_type nsec_ = 0;
};

}

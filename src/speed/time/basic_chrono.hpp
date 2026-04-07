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
 * @file basic_chrono.hpp
 * @brief Defines the basic_chrono class.
 * @author Killian Valverde
 * @date 2017-10-28
 */

#pragma once

#include <iomanip>
#include <type_traits>

#include "cpu_clock.hpp"
#include "monotonic_clock.hpp"

namespace speed::time {

/**
 * @brief Utility for measuring elapsed time.
 *
 * The clock type must inherit from speed::time::clock_base.
 * The chrono operates with three states: READY, RUNNING, and STOPPED.
 *
 * @tparam ClockT Clock type used to retrieve time values.
 */
template<typename ClockT>
requires std::is_base_of_v<clock_base, ClockT>
class basic_chrono
{
public:
    /** Type of the underlying clock. */
    using clock_type = ClockT;

private:
    /**
    * @brief Enumeration representing the state of a chrono.
    */
    enum class chrono_states : std::uint8_t
    {
        /** Chrono is initialized and ready to start. */
        READY,

        /** Chrono is currently running. */
        RUNNING,

        /** Chrono has been stopped. */
        STOPPED
    };

public:
    /**
     * @brief Starts the chrono.
     *
     * Transitions the state from READY to RUNNING.
     * Has no effect if the chrono is not in READY state.
     */
    void start() noexcept
    {
        if (chrono_stt_ != chrono_states::READY)
        {
            return;
        }

        start_tm_ = clck_.get_time();
        chrono_stt_ = chrono_states::RUNNING;
    }

    /**
     * @brief Stops the chrono.
     *
     * Transitions the state from RUNNING to STOPPED and stores the elapsed time.
     * Has no effect if the chrono is not in RUNNING state.
     */
    void stop() noexcept
    {
        if (chrono_stt_ != chrono_states::RUNNING)
        {
            return;
        }

        elapsed_tm_ = clck_.get_time() - start_tm_;
        chrono_stt_ = chrono_states::STOPPED;
    }

    /**
     * @brief Resumes the chrono.
     *
     * Continues time measurement from the STOPPED state.
     * Has no effect if the chrono is not in STOPPED state.
     */
    void resume() noexcept
    {
        if (chrono_stt_ != chrono_states::STOPPED)
        {
            return;
        }

        start_tm_ = clck_.get_time() - elapsed_tm_;
        chrono_stt_ = chrono_states::RUNNING;
    }

    /**
     * @brief Resets the chrono.
     *
     * Sets the state to READY and clears all stored time values.
     */
    void reset() noexcept
    {
        chrono_stt_ = chrono_states::READY;
        start_tm_.reset();
        elapsed_tm_.reset();
    }

    /**
     * @brief Restarts the chrono.
     *
     * Equivalent to calling reset followed by start.
     */
    void restart() noexcept
    {
        reset();
        start();
    }

    /**
     * @brief Retrieves the elapsed time.
     *
     * @return A speed::system::time::time_specification representing the elapsed time.
     *
     * @note The returned value depends on the current state:
     * - RUNNING: current elapsed time
     * - STOPPED: stored elapsed time
     * - READY:   zero time
     */
    [[nodiscard]] system::time::time_value get_elapsed_time() const noexcept
    {
        if (chrono_stt_ == chrono_states::RUNNING)
        {
            return clck_.get_time() - start_tm_;
        }
        if (chrono_stt_ == chrono_states::STOPPED)
        {
            return elapsed_tm_;
        }

        return {};
    }

    /**
     * @brief Checks if the chrono is in READY state.
     */
    [[nodiscard]] bool is_ready() const noexcept
    {
        return chrono_stt_ == chrono_states::READY;
    }

    /**
     * @brief Checks if the chrono is in RUNNING state.
     */
    [[nodiscard]] bool is_running() const noexcept
    {
        return chrono_stt_ == chrono_states::RUNNING;
    }

    /**
     * @brief Checks if the chrono is in STOPPED state.
     */
    [[nodiscard]] bool is_stopped() const noexcept
    {
        return chrono_stt_ == chrono_states::STOPPED;
    }

private:
    /** Start time of the current measurement. */
    system::time::time_value start_tm_;

    /** Stored elapsed time when stopped. */
    system::time::time_value elapsed_tm_;

    /** Current state of the chrono. */
    chrono_states chrono_stt_ = chrono_states::READY;

    /** Clock instance used for time retrieval. */
    clock_type clck_;
};

/**
 * @brief Stream output operator for basic_chrono.
 *
 * @tparam CharT Character type.
 * @tparam CharTraitsT Character traits type.
 * @tparam ClockT Clock type.
 *
 * @param os Output stream.
 * @param rhs Chrono instance.
 * @return Reference to the output stream.
 *
 * @details The elapsed time is formatted as: seconds.nanoseconds (9 digits, zero-padded).
 */
template<typename CharT, typename CharTraitsT, typename ClockT>
std::basic_ostream<CharT, CharTraitsT>& operator <<(
    std::basic_ostream<CharT, CharTraitsT>& os,
    const basic_chrono<ClockT>& rhs
)
{
    // TODO: Implement RAII for ostream state.
    const system::time::time_value elapsed_tm = rhs.get_elapsed_time();
    auto old_fill = os.fill();

    os << elapsed_tm.get_seconds()
            << '.'
            << std::setw(9)
            << std::setfill('0')
            << elapsed_tm.get_nanoseconds();

    os.fill(old_fill);
    return os;
}

/**
 * @brief Chrono type using a CPU clock.
 *
 * Measures elapsed CPU time using speed::time::cpu_clock.
 */
using cpu_chrono = basic_chrono<cpu_clock>;

/**
 * @brief Chrono type using a monotonic clock.
 *
 * Measures elapsed time using speed::time::monotonic_clock.
 */
using monotonic_chrono = basic_chrono<monotonic_clock>;

}

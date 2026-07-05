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
 * @file operations.hpp
 * @brief Core operations for the system::process submodule.
 * @author Killian Valverde
 * @date 2017-01-08
 */

#pragma once

#include <system_error>

#include "execution_result.hpp"
#include "types.hpp"
#include "user_id.hpp"

namespace speed::system::process {

/**
 * @brief Execute a command in a child process and optionally collect its results.
 *
 * Spawns a child process to run the given command string, waits for it to complete,
 * and optionally fills an execution_result with the exit code and timing information.
 * Behaves similarly to the standard system() function but provides finer-grained control
 * and result reporting. On POSIX systems, if the child process is terminated by a signal,
 * the exit code is set to 128 + signal_number, following shell conventions. On both
 * platforms, exit code -1 indicates an abnormal termination. Note that a `true` return
 * does not imply the command succeeded — only that it was executed. Check res->exit_code
 * for the command's own result.
 *
 * @param cmd The command string to execute. Leading whitespace is ignored. Must not be
 *            null or empty.
 * @param res Optional pointer to an execution_result that will be populated with the exit
 *            code, user CPU time, kernel CPU time, and elapsed wall time. Pass nullptr if
 *            not needed.
 * @param err_code Optional pointer to a std::error_code that will be set on failure. Pass
 *                 nullptr to ignore errors.
 * @return `true` if the child process was successfully spawned and waited for, `false` otherwise.
 */
bool execute(
    const char* cmd,
    execution_result* res = nullptr,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Get the process ID of the calling process.
 *
 * @return The process ID of the calling process.
 */
pid_t get_pid() noexcept;

/**
 * @brief Get the user ID of the calling process.
 *
 * On POSIX systems, returns the real user ID. On Windows, returns a 64-bit hash derived
 * from the process token's SID, since Windows has no direct equivalent to a POSIX UID.
 *
 * @return The user ID of the calling process.
 */
user_id get_uid() noexcept;

/**
 * @brief Suspend the calling thread for the specified duration.
 *
 * Suspends execution for at least the given number of seconds and nanoseconds. The actual
 * sleep duration may be longer due to system scheduling. On POSIX systems the sleep may be
 * interrupted by a signal. On Windows the sleep is not interruptible.
 *
 * @param sec Number of seconds to sleep. Must not exceed the platform maximum.
 * @param nsec Additional nanoseconds to sleep. Must be less than 1,000,000,000.
 * @param err_code Optional pointer to a std::error_code that will be set on failure. Pass
 *                 nullptr to ignore errors.
 * @return `true` on success, `false` if the arguments are invalid or the sleep was interrupted.
 */
bool nanosleep(std::uint64_t sec, std::uint64_t nsec, std::error_code* err_code = nullptr) noexcept;

}

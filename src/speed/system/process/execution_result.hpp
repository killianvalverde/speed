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
 * @file execution_result.hpp
 * @brief Defines the execution_result structure.
 * @author Killian Valverde
 * @date 2026-05-29
 */

#pragma once

#include "../time/time_value.hpp"

namespace speed::system::process {

/**
 * @brief Holds the result of a process execution.
 *
 * Populated by execute() upon successful completion of a child process.
 */
struct execution_result
{
    /** Wall clock time elapsed between process creation and exit. */
    time::time_value elapsed_time;

    /** CPU time spent in kernel mode by the process. */
    time::time_value kernel_cpu_time;

    /** CPU time spent in user mode by the process. */
    time::time_value user_cpu_time;

    /** Exit code of the child process. -1 if the process was terminated abnormally,
     * or 128 + signal_number on POSIX if killed by a signal. */
    int exit_code = 0;
};

}

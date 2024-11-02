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
 * @file        speed/system/process/operations.hpp
 * @brief       process operations header.
 * @author      Killian Valverde
 * @date        2017/01/08
 */

#ifndef SPEED_SYSTEM_PROCESS_OPERATIONS_HPP
#define SPEED_SYSTEM_PROCESS_OPERATIONS_HPP

#include "../api/api.hpp"
#include "../compatibility/compatibility.hpp"
#include "../time/time.hpp"
#include "types.hpp"


namespace speed::system::process {


/**
 * @brief       Execute the specified command.
 * @param       cmd : The command to execute.
 * @param       return_val : The return value of the command.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool execute_command(
        const char* cmd,
        int* return_val = nullptr,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(process::execute_command, false, cmd, return_val, err_code);
}


/**
 * @brief       Get the PID of the current process.
 * @return      The PID of the current process.
 */
inline pid_t get_pid() noexcept
{
    return SPEED_SELECT_API(process::get_pid, -1);
}


/**
 * @brief       Get the PPID of the current process.
 * @return      The PPID of the current process.
 */
inline ppid_t get_ppid() noexcept
{
    return SPEED_SELECT_API(process::get_ppid, -1);
}


/**
 * @brief       Get the UID of the current process.
 * @return      The UID of the current process.
 */
inline uid_t get_uid() noexcept
{
    return SPEED_SELECT_API(process::get_uid, -1);
}


/**
 * @brief       Get the GID of the current process.
 * @return      The GID of the current process.
 */
inline gid_t get_gid() noexcept
{
    return SPEED_SELECT_API(process::get_gid, -1);
}


/**
 * @brief       Suspends  the  execution  of the calling thread until either at least the time
 *              specified has elapsed, or the delivery of a signal that triggers the invocation of a
 *              handler in the calling thread or that terminates the process.
 * @param       sec : The number of seconds.
 * @param       nsec : The number of nano seconds.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool nanosleep(
        std::uint64_t sec,
        std::uint64_t nsec,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(process::nanosleep, false, sec, nsec, err_code);
}


/**
 * @brief       Suspends  the  execution  of the calling thread until either at least the time
 *              specified has elapsed, or the delivery of a signal that triggers the invocation of a
 *              handler in the calling thread or that terminates the process.
 * @param       time_spec : The time to suspend the execution.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool nanosleep(
        const speed::system::time::time_specification& time_spec,
        std::error_code* err_code = nullptr
) noexcept
{
    return nanosleep(time_spec.get_seconds(), time_spec.get_nseconds(), err_code);
}


}


#endif

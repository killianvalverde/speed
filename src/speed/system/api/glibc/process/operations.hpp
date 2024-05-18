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
 * @file        speed/system/api/glibc/process/operations.hpp
 * @brief       process operations header.
 * @author      Killian Valverde
 * @date        2017/01/08
 */

#ifndef SPEED_SYSTEM_API_GLIBC_PROCESS_OPERATIONS_HPP
#define SPEED_SYSTEM_API_GLIBC_PROCESS_OPERATIONS_HPP

#include "../../../type_traits/type_traits.hpp"
#ifdef SPEED_GLIBC

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../../../errors/errors.hpp"
#include "../../../time/time_specification.hpp"


namespace speed::system::api::glibc::process {


using namespace speed::system::errors;
using speed::system::time::time_specification;


/**
 * @brief       Execute the specified command.
 * @param       cmd : The command to execute.
 * @param       ret_val : The return value of the command.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool execute_command(
        const char* cmd,
        int* ret_val = nullptr,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Get the PID of the current process.
 * @return      The PID of the current process.
 */
int get_pid() noexcept;


/**
 * @brief       Get the PPID of the current process.
 * @return      The PPID of the current process.
 */
int get_ppid() noexcept;


/**
 * @brief       Get the UID of the current process.
 * @return      The UID of the current process.
 */
unsigned int get_uid() noexcept;


/**
 * @brief       Get the GID of the current process.
 * @return      The GID of the current process.
 */
unsigned int get_gid() noexcept;


/**
 * @brief       Suspends  the  execution  of the calling thread until either at least the time
 *              specified has elapsed, or the delivery of a signal that triggers the invocation of a
 *              handler in the calling thread or that terminates the process.
 * @param       sec : The number of seconds.
 * @param       nsec : The number of nano seconds.
 * @param       rem_time : The remaining time if the function was interrupted.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool nanosleep(
        std::uint64_t sec,
        std::uint64_t nsec,
        time_specification* rem_time = nullptr,
        std::error_code* err_code = nullptr
) noexcept;


}


#endif
#endif

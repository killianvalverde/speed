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
 * @file        operations.hpp
 * @brief       time operations definitions.
 * @author      Killian Valverde
 * @date        2017/10/18
 */

#include "../../../compatibility/compatibility.hpp"
#ifdef SPEED_GLIBC

#include "operations.hpp"

#include <ctime>
#include <sys/times.h>

namespace speed::system::api::glibc::time {

bool get_monotonic_time(
        system::time::time_specification* time_spec,
        std::error_code* err_code
) noexcept
{
    ::timespec tp;
    int clk;

#if defined(__linux__)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39)
    clk = CLOCK_BOOTTIME;
#else
    clk = CLOCK_MONOTONIC;
#endif
#else
    clk = CLOCK_MONOTONIC;
#endif
    
    if (::clock_gettime(clk, &tp) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }

    time_spec->set_time((std::uint64_t)tp.tv_sec, (std::uint64_t)tp.tv_nsec);
    
    return true;
}

bool get_cpu_time(
        system::time::time_specification* time_spec,
        std::error_code* err_code
) noexcept
{
    ::clock_t cpu_tme;
    ::tms buf;
    const long cps = ::sysconf(_SC_CLK_TCK);
    
    if (cps == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    if (::times(&buf) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }

    cpu_tme = buf.tms_utime + buf.tms_stime;

    time_spec->set_time((std::uint64_t)cpu_tme / cps,
                        (1'000'000'000 / cps) * ((std::uint64_t)cpu_tme % cps));
    
    return true;
}

bool get_child_cpu_time(
        system::time::time_specification* time_spec,
        std::error_code* err_code
) noexcept
{
    ::clock_t cpu_tme;
    ::tms buf;
    const long cps = ::sysconf(_SC_CLK_TCK);
    
    if (cps == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    if (::times(&buf) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }

    cpu_tme = buf.tms_cutime + buf.tms_cstime;

    time_spec->set_time((std::uint64_t)cpu_tme / cps,
                        (1'000'000'000 / cps) * ((std::uint64_t)cpu_tme % cps));
    
    return true;
}

}

#endif

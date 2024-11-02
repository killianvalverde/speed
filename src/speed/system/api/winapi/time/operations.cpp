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
 * @file        speed/system/api/winapi/time/operations.hpp
 * @brief       time operations definitions.
 * @author      Killian Valverde
 * @date        2017/10/18
 */

#include "../../../compatibility/compatibility.hpp"
#ifdef SPEED_WINAPI

#include "operations.hpp"


namespace speed::system::api::winapi::time {


bool get_monotonic_time(
        system::time::time_specification* time_spec,
        std::error_code* err_code
) noexcept
{
    LARGE_INTEGER freq;
    LARGE_INTEGER count;

    if (!::QueryPerformanceFrequency(&freq) ||
        !::QueryPerformanceCounter(&count))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    time_spec->set_time(
            static_cast<std::uint64_t>(count.QuadPart / freq.QuadPart),
            static_cast<std::uint64_t>(
                    ((count.QuadPart % freq.QuadPart) * 1000000000ull) / freq.QuadPart));

    return true;
}


bool get_cpu_time(
        system::time::time_specification* time_spec,
        std::error_code* err_code
) noexcept
{
    FILETIME creation_tme;
    FILETIME exit_tme;
    FILETIME kernel_tme;
    FILETIME user_tme;
    std::uint64_t total_ns;
    HANDLE process_handl = ::GetCurrentProcess();

    if (!::GetProcessTimes(process_handl, &creation_tme, &exit_tme, &kernel_tme, &user_tme))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    auto filetime_to_uint64 = [](const FILETIME& ft) -> std::uint64_t
    {
        return (static_cast<std::uint64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
    };

    total_ns = filetime_to_uint64(user_tme) * 100 + filetime_to_uint64(kernel_tme) * 100;
    time_spec->set_time(total_ns / 1'000'000'000ull, total_ns % 1'000'000'000ull);

    return true;
}


}


#endif

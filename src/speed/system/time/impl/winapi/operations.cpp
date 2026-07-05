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
 * @file operations.cpp
 * @brief Core operations implementation for the system::time submodule.
 * @author Killian Valverde
 * @date 2017-10-18
 */

#include "../../../platform/platform.hpp"
#include "../../operations.hpp"

#include "../../../errors/errors.hpp"

namespace speed::system::time {

bool get_monotonic_time(time_value& time_val, std::error_code* err_code) noexcept
{
    static LARGE_INTEGER freq = []
    {
        LARGE_INTEGER f;
        QueryPerformanceFrequency(&f);
        return f;
    }();

    LARGE_INTEGER count;

    if (!QueryPerformanceCounter(&count))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        return false;
    }

    time_val.set_seconds(static_cast<std::uint64_t>(count.QuadPart / freq.QuadPart))
        .set_nanoseconds(count.QuadPart % freq.QuadPart * 1'000'000'000ULL / freq.QuadPart);

    return true;
}

bool get_cpu_time(time_value& time_val, std::error_code* err_code) noexcept
{
    FILETIME creation_tm;
    FILETIME exit_tm;
    FILETIME kernel_tm;
    FILETIME user_tm;

    if (!GetProcessTimes(GetCurrentProcess(), &creation_tm, &exit_tm, &kernel_tm, &user_tm))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        return false;
    }

    constexpr auto filetime_to_uint64 = [](const FILETIME& ft) noexcept -> std::uint64_t
    {
        return static_cast<std::uint64_t>(ft.dwHighDateTime) << 32 | ft.dwLowDateTime;
    };

    const std::uint64_t total_100ns = filetime_to_uint64(user_tm) + filetime_to_uint64(kernel_tm);

    time_val.set_seconds(total_100ns / 10'000'000ULL)
        .set_nanoseconds(total_100ns % 10'000'000ULL * 100);

    return true;
}

}

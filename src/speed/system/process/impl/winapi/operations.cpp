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
 * @brief Core operations implementation for the system::process submodule.
 * @author Killian Valverde
 * @date 2017-01-08
*/

#include "../../../platform/platform.hpp"
#include "../../operations.hpp"

#include <cstdlib>

#include <sddl.h>

#include "../../../errors/errors.hpp"

namespace speed::system::process {

bool execute(const char* cmd, execution_result* res, std::error_code* err_code) noexcept
{
    constexpr std::size_t stack_cmd_size = 256;

    STARTUPINFOA startup_info{};
    PROCESS_INFORMATION process_info{};
    char stack_cmd[stack_cmd_size];
    char* cmd_line = stack_cmd;
    bool success = true;

    if (cmd == nullptr || *cmd == '\0')
    {
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }
    while (std::isspace(static_cast<unsigned char>(*cmd)))
    {
        ++cmd;
    }
    if (*cmd == '\0')
    {
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    const std::size_t cmd_len = std::strlen(cmd);
    if (cmd_len >= stack_cmd_size)
    {
        cmd_line = static_cast<char*>(std::malloc(cmd_len + 1));
        if (cmd_line == nullptr)
        {
            errors::assign_errc(std::errc::not_enough_memory, err_code);
            return false;
        }
    }

    std::memcpy(cmd_line, cmd, cmd_len + 1);
    startup_info.cb = sizeof(startup_info);

    if (!CreateProcessA(
        nullptr, // Use command line as application name
        cmd_line, // Command line
        nullptr, // Process handle not inheritable
        nullptr, // Thread handle not inheritable
        FALSE, // Set handle inheritance to FALSE
        0, // No creation flags
        nullptr, // Use parent's environment block
        nullptr, // Use parent's starting directory
        &startup_info, // Pointer to STARTUPINFO structure
        &process_info)) // Pointer to PROCESS_INFORMATION structure
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        success = false;
        goto cleanup;
    }

    if (WaitForSingleObject(process_info.hProcess, INFINITE) == WAIT_FAILED)
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        success = false;
        goto cleanup;
    }

    if (res != nullptr)
    {
        constexpr std::uint64_t ticks_per_sec = 10'000'000ULL;
        constexpr std::uint64_t nsec_per_tick = 100ULL;

        DWORD exit_code;
        FILETIME creation_time;
        FILETIME exit_time;
        FILETIME kernel_time;
        FILETIME user_time;

        auto filetime_to_uint64 = [](const FILETIME& ft) noexcept -> std::uint64_t
        {
            return static_cast<std::uint64_t>(ft.dwHighDateTime) << 32 | ft.dwLowDateTime;
        };

        if (!GetExitCodeProcess(process_info.hProcess, &exit_code) ||
            !GetProcessTimes(process_info.hProcess, &creation_time, &exit_time, &kernel_time,
                &user_time))
        {
            errors::assign_system_error_code(GetLastError(), err_code);
            success = false;
            goto cleanup;
        }

        res->exit_code = exit_code >= 0xC0000000U
            ? -1
            : static_cast<decltype(res->exit_code)>(exit_code);

        const std::uint64_t kernel_100ns = filetime_to_uint64(kernel_time);
        const std::uint64_t user_100ns = filetime_to_uint64(user_time);
        const std::uint64_t elapsed_100ns = filetime_to_uint64(exit_time) -
            filetime_to_uint64(creation_time);

        res->kernel_cpu_time.set_seconds(kernel_100ns / ticks_per_sec);
        res->kernel_cpu_time.set_nanoseconds(kernel_100ns % ticks_per_sec * nsec_per_tick);
        res->user_cpu_time.set_seconds(user_100ns / ticks_per_sec);
        res->user_cpu_time.set_nanoseconds(user_100ns % ticks_per_sec * nsec_per_tick);
        res->elapsed_time.set_seconds(elapsed_100ns / ticks_per_sec);
        res->elapsed_time.set_nanoseconds(elapsed_100ns % ticks_per_sec * nsec_per_tick);
    }

cleanup:
    if (cmd_line != stack_cmd)
    {
        std::free(cmd_line);
    }
    if (process_info.hProcess != nullptr)
    {
        CloseHandle(process_info.hProcess);
    }
    if (process_info.hThread != nullptr)
    {
        CloseHandle(process_info.hThread);
    }

    return success;
}

pid_t get_pid() noexcept
{
    return static_cast<pid_t>(GetCurrentProcessId());
}

user_id get_uid() noexcept
{
    HANDLE token_process = nullptr;
    DWORD token_size = 0;
    PTOKEN_USER token_user = nullptr;
    LPWSTR sid_string = nullptr;
    user_id result_uid;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token_process))
    {
        return {};
    }

    if (GetTokenInformation(token_process, TokenUser, nullptr, 0, &token_size) ||
        GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        CloseHandle(token_process);
        return {};
    }

    if (token_user = static_cast<PTOKEN_USER>(std::malloc(token_size)); token_user == nullptr)
    {
        CloseHandle(token_process);
        return {};
    }

    if (GetTokenInformation(token_process, TokenUser, token_user, token_size, &token_size) &&
        ConvertSidToStringSidW(token_user->User.Sid, &sid_string))
    {
        result_uid.assign(sid_string);
        LocalFree(sid_string);
    }

    std::free(token_user);
    CloseHandle(token_process);

    return result_uid;
}

bool nanosleep(std::uint64_t sec, std::uint64_t nsec, std::error_code* err_code) noexcept
{
    constexpr std::uint64_t nsec_per_sec = 1'000'000'000ULL;
    constexpr std::uint64_t msec_per_sec = 1'000ULL;
    constexpr std::uint64_t nsec_per_msec = 1'000'000ULL;
    constexpr std::uint64_t max_milliseconds = std::numeric_limits<DWORD>::max();

    if (nsec >= nsec_per_sec)
    {
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    if (sec > max_milliseconds / msec_per_sec)
    {
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    const std::uint64_t milliseconds = sec * msec_per_sec + nsec / nsec_per_msec;

    if (milliseconds > max_milliseconds)
    {
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    SleepEx(static_cast<DWORD>(milliseconds), FALSE);
    return true;
}

}

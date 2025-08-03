/* speed - Generic C++ library.
 * Copyright (C) 2015-2025 Killian Valverde.
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
 * @file        operations.cpp
 * @brief       process operations source.
 * @author      Killian Valverde
 * @date        2017/01/08
 */

#include "../../../platform/platform.hpp"
#ifdef SPEED_WINAPI

#include "operations.hpp"

#include <sddl.h>
#include <tlhelp32.h>

#include "../../../../cryptography/cryptography.hpp"

namespace speed::system::detail::winapi::process {

bool execute(
        const char* cmd,
        int* ret_val,
        system::time::time_specification* cpu_time_spec,
        system::time::time_specification* elapsed_time_spec,
        std::error_code* err_code
) noexcept
{
    STARTUPINFOA startup_info;
    PROCESS_INFORMATION process_info;
    bool succss = true;

    ZeroMemory(&startup_info, sizeof(startup_info));
    ZeroMemory(&process_info, sizeof(process_info));
    startup_info.cb = sizeof(startup_info);

    if (!::CreateProcessA(
            nullptr,                   // Use command line as application name
            (char*)cmd,                // Command line
            nullptr,                   // Process handle not inheritable
            nullptr,                   // Thread handle not inheritable
            FALSE,                     // Set handle inheritance to FALSE
            0,                         // No creation flags
            nullptr,                   // Use parent's environment block
            nullptr,                   // Use parent's starting directory
            &startup_info,             // Pointer to STARTUPINFO structure
            &process_info              // Pointer to PROCESS_INFORMATION structure
    ))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        succss = false;
        goto cleanup;
    }

    ::WaitForSingleObject(process_info.hProcess, INFINITE);

    if (ret_val != nullptr)
    {
        DWORD exit_cod;
        if (!::GetExitCodeProcess(process_info.hProcess, &exit_cod))
        {
            system::errors::assign_system_error_code((int) GetLastError(), err_code);
            succss = false;
            goto cleanup;
        }

        *ret_val = (int) exit_cod;
    }
    
    if (cpu_time_spec || elapsed_time_spec)
    {
        FILETIME create_tm, exit_tm, kernel_tm, user_tm;
        if (!::GetProcessTimes(process_info.hProcess, &create_tm, &exit_tm, &kernel_tm, &user_tm))
        {
            system::errors::assign_system_error_code((int)GetLastError(), err_code);
            succss = false;
            goto cleanup;
        }

        if (cpu_time_spec)
        {
            std::uint64_t kt = ((std::uint64_t)kernel_tm.dwHighDateTime << 32) |
                    kernel_tm.dwLowDateTime;
            std::uint64_t ut = ((std::uint64_t)user_tm.dwHighDateTime << 32) |
                    user_tm.dwLowDateTime;
            std::uint64_t total_time_100ns = kt + ut;

            std::uint64_t secs = total_time_100ns / 10'000'000ULL;
            std::uint64_t nsecs = (total_time_100ns % 10'000'000ULL) * 100ULL;

            cpu_time_spec->set_time(secs, nsecs);
        }

        if (elapsed_time_spec)
        {
            std::uint64_t ct = ((std::uint64_t)create_tm.dwHighDateTime << 32) |
                    create_tm.dwLowDateTime;
            std::uint64_t et   = ((std::uint64_t)exit_tm.dwHighDateTime << 32) |
                    exit_tm.dwLowDateTime;

            std::uint64_t elapsed_100ns = et - ct;
            std::uint64_t secs = elapsed_100ns / 10'000'000ULL;
            std::uint64_t nsecs = (elapsed_100ns % 10'000'000ULL) * 100ULL;

            elapsed_time_spec->set_time(secs, nsecs);
        }
    }

cleanup:
    ::CloseHandle(process_info.hProcess);
    ::CloseHandle(process_info.hThread);
    
    return succss;
}

system::process::pid_t get_pid() noexcept
{
    return ::GetCurrentProcessId();
}

system::process::ppid_t get_ppid() noexcept
{
    PROCESSENTRY32 entr;
    DWORD pid = ::GetCurrentProcessId();
    HANDLE snapsht = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapsht == INVALID_HANDLE_VALUE)
    {
        return -1;
    }

    entr.dwSize = sizeof(PROCESSENTRY32);

    if (::Process32First(snapsht, &entr))
    {
        do
        {
            if (entr.th32ProcessID == pid)
            {
                return entr.th32ParentProcessID;
            }
        } while (::Process32Next(snapsht, &entr));
    }

    ::CloseHandle(snapsht);

    return -1;
}

system::process::uid_t get_uid() noexcept
{
    HANDLE tokn;
    PTOKEN_USER token_usr;
    DWORD token_sz;
    LPSTR sid_cstr;
    system::process::uid_t uid = -1;

    if (::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY, &tokn))
    {
        ::GetTokenInformation(tokn, TokenUser, nullptr, 0, &token_sz);
        if (token_sz)
        {
            token_usr = (PTOKEN_USER)malloc(token_sz);
            if (::GetTokenInformation(tokn, TokenUser, token_usr, token_sz, &token_sz))
            {
                if (::ConvertSidToStringSidA(token_usr->User.Sid, &sid_cstr))
                {
                    uid = speed::cryptography::city_hash_64((const char*)sid_cstr);
                    ::LocalFree(sid_cstr);
                }
            }

            ::free(token_usr);
        }

        ::CloseHandle(tokn);
    }

    return uid;
}

system::process::gid_t get_gid() noexcept
{
    HANDLE tokn;
    PTOKEN_PRIMARY_GROUP token_grp;
    DWORD token_sz;
    LPSTR group_cstr;
    system::process::gid_t gid = -1;

    if (::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY, &tokn))
    {
        ::GetTokenInformation(tokn, TokenPrimaryGroup, nullptr, 0, &token_sz);
        if (token_sz)
        {
            token_grp = (PTOKEN_PRIMARY_GROUP)malloc(token_sz);
            if (::GetTokenInformation(tokn, TokenPrimaryGroup, token_grp, token_sz, &token_sz))
            {
                if (::ConvertSidToStringSidA(token_grp->PrimaryGroup, &group_cstr))
                {
                    gid = speed::cryptography::city_hash_64((const char*)group_cstr);
                    ::LocalFree(group_cstr);
                }
            }

            free(token_grp);
        }

        ::CloseHandle(tokn);
    }

    return gid;
}

bool nanosleep(
        std::uint64_t sec,
        std::uint64_t nsec,
        std::error_code* err_code
) noexcept
{
    DWORD result = ::SleepEx((sec * 1000 + nsec / 1000000), false);
    
    if (result != 0)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}

}

#endif

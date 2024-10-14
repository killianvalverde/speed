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
 * @file        speed/system/api/winapi/process/operations.cpp
 * @brief       process operations source.
 * @author      Killian Valverde
 * @date        2017/01/08
 */

#include "../../../compatibility/compatibility.hpp"
#ifdef SPEED_WINAPI

#include <sddl.h>
#include <tlhelp32.h>

#include "operations.hpp"


namespace speed::system::api::winapi::process {


bool execute_command(
        const char* cmd,
        int* ret_val,
        std::error_code* err_code
) noexcept
{
    STARTUPINFOA startup_info;
    PROCESS_INFORMATION process_info;
    DWORD ret_val_buildr;

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
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    ::WaitForSingleObject(process_info.hProcess, INFINITE);

    if (ret_val != nullptr)
    {
        if (!::GetExitCodeProcess(process_info.hProcess, &ret_val_buildr))
        {
            assign_system_error_code((int)GetLastError(), err_code);
            return false;
        }

        *ret_val = (int)ret_val_buildr;
    }

    ::CloseHandle(process_info.hProcess);
    ::CloseHandle(process_info.hThread);

    return true;
}


int get_pid() noexcept
{
    return (int)::GetCurrentProcessId();
}


int get_ppid() noexcept
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
                return (int)entr.th32ParentProcessID;
            }
        } while (::Process32Next(snapsht, &entr));
    }

    ::CloseHandle(snapsht);

    return -1;
}


unsigned int get_uid() noexcept
{
    // TODO: Compute a 32 bits hash using the windows api.
    HANDLE tokn = nullptr;
    DWORD token_sz = 0;
    unsigned int uid = -1;

    if (::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY, &tokn))
    {
        ::GetTokenInformation(tokn, TokenUser, nullptr, 0, &token_sz);
        if (token_sz)
        {
            auto* usr = (TOKEN_USER*)malloc(token_sz);
            if (::GetTokenInformation(tokn, TokenUser, usr, token_sz, &token_sz))
            {
                LPSTR sid_string = nullptr;
                if (::ConvertSidToStringSidA(usr->User.Sid, &sid_string))
                {
                    uid = std::hash<std::string>{}(sid_string);
                    ::LocalFree(sid_string);
                }
            }

            ::free(usr);
        }

        ::CloseHandle(tokn);
    }

    return uid;
}


unsigned int get_gid() noexcept
{
    // TODO: Compute a 32 bits hash using the windows api.
    HANDLE tokn = nullptr;
    DWORD sz = 0;
    unsigned int gid = -1;

    if (::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY, &tokn))
    {
        ::GetTokenInformation(tokn, TokenPrimaryGroup, nullptr, 0, &sz);
        if (sz)
        {
            auto* grop = (TOKEN_PRIMARY_GROUP*)malloc(sz);
            if (::GetTokenInformation(tokn, TokenPrimaryGroup, grop, sz, &sz))
            {
                LPSTR sid_string = nullptr;
                if (ConvertSidToStringSidA(grop->PrimaryGroup, &sid_string))
                {
                    gid = std::hash<std::string>{}(sid_string);
                    LocalFree(sid_string);
                }
            }

            free(grop);
        }

        CloseHandle(tokn);
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
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}


}


#endif

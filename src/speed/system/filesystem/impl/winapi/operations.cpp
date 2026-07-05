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
 * @brief Core operations implementation for the system::filesystem submodule.
 * @author Killian Valverde
 * @date 2017-05-26
 */

#include "../../../platform/platform.hpp"
#include "../../operations.hpp"

#include <cstdlib>
#include <memory>

#include <accctrl.h>
#include <aclapi.h>
#include <io.h>
#include <sddl.h>
#include <shlguid.h>
#include <shobjidl_core.h>
#include <winioctl.h>

#include "../../../errors/errors.hpp"

namespace speed::system::filesystem {

static bool get_file_handle(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    DWORD desired_access,
    HANDLE& file_handle,
    std::error_code* err_code
) noexcept
{
    DWORD flgs = FILE_FLAG_BACKUP_SEMANTICS;

    if (symlink_md == symlink_mode::NO_RESOLVE)
    {
        flgs |= FILE_FLAG_OPEN_REPARSE_POINT;
    }

    file_handle = CreateFileW(
        file_path,
        desired_access,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        nullptr,
        OPEN_EXISTING,
        flgs,
        nullptr);

    if (file_handle == INVALID_HANDLE_VALUE)
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        return false;
    }

    return true;
}

template<typename TraitsT, typename AllocatorT>
static bool make_nt_path(
    const wchar_t* source,
    std::basic_string<wchar_t, TraitsT, AllocatorT>& destination,
    std::error_code* err_code = nullptr
) noexcept
{
    try
    {
        if (source == nullptr || source[0] == L'\0')
        {
            errors::assign_errc(std::errc::invalid_argument, err_code);
            return false;
        }

        DWORD full_path_len = GetFullPathNameW(source, 0, nullptr, nullptr);
        if (full_path_len == 0)
        {
            errors::assign_system_error_code(GetLastError(), err_code);
            return false;
        }

        std::basic_string<wchar_t, TraitsT, AllocatorT> full_path;
        full_path.resize(full_path_len - 1);

        if (GetFullPathNameW(source, full_path_len, full_path.data(), nullptr) == 0)
        {
            errors::assign_system_error_code(GetLastError(), err_code);
            return false;
        }

        destination.assign(LR"(\??\)");
        destination.append(full_path);

        return true;
    }
    catch (const std::bad_alloc&)
    {
        errors::assign_system_error_code(ERROR_NOT_ENOUGH_MEMORY, err_code);
    }
    catch (...)
    {
        errors::assign_system_error_code(ERROR_BAD_ARGUMENTS, err_code);
    }

    return false;
}

bool close_directory(directory_stream& directory_strm, std::error_code* err_code) noexcept
{
    auto& native = directory_strm.native;
    if (native.dir_handle == nullptr)
    {
        return true;
    }

    NTSTATUS status = NtClose(native.dir_handle);
    if (!NT_SUCCESS(status))
    {
        errors::assign_system_error_code(RtlNtStatusToDosError(status), err_code);
        return false;
    }

    native.dir_handle = nullptr;
    native.buffer_offset = 0;
    native.valid_bytes = 0;
    native.restart_scan = true;
    native.name_storage.clear();
    directory_strm.name = nullptr;
    directory_strm.inode = 0;
    directory_strm.type = file_types::NIL;

    return true;
}

bool create_directory(const path_char_t* directory_path, std::error_code* err_code) noexcept
{
    if (!CreateDirectoryW(directory_path, nullptr))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        return false;
    }

    return true;
}

bool create_directories(const path_char_t* directory_path, std::error_code* err_code) noexcept
{
    if (directory_path == nullptr || directory_path[0] == L'\0')
    {
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    try
    {
        std::wstring path(directory_path);
        for (auto& ch : path)
        {
            if (ch == L'/')
            {
                ch = L'\\';
            }
        }

        // ------------------------------------------------------------------ //
        //  Determine where the fixed prefix ends so we never try to create    //
        //  it.  The prefix is the part of the path that always exists (or     //
        //  that CreateDirectoryW would refuse to touch):                      //
        //    \\?\UNC\server\share   -> prefix_len = position after "share\"   //
        //    \\?\C:\                -> prefix_len = 7  ("\\?\C:\")            //
        //    \\server\share         -> prefix_len = position after "share\"   //
        //    C:\                    -> prefix_len = 3  ("C:\")                //
        //    relative               -> prefix_len = 0                         //
        // ------------------------------------------------------------------ //
        std::size_t prefix_len = 0;

        if (path.size() >= 4 &&
            path[0] == L'\\' &&
            path[1] == L'\\' &&
            (path[2] == L'?' || path[2] == L'.') &&
            path[3] == L'\\')
        {
            // Extended-length path: \\?\ or \\.\ prefix.
            if (path.size() >= 8
                && (path[4] == L'U' || path[4] == L'u')
                && (path[5] == L'N' || path[5] == L'n')
                && (path[6] == L'C' || path[6] == L'c')
                && path[7] == L'\\')
            {
                // \\?\UNC\server\share  — skip past the share name.
                std::size_t pos = path.find(L'\\', 8);
                if (pos != std::wstring::npos)
                {
                    pos = path.find(L'\\', pos + 1);
                }
                prefix_len = pos != std::wstring::npos ? pos + 1 : path.size();
            }
            else if (path.size() >= 7 && path[5] == L':' && path[6] == L'\\')
            {
                // \\?\C:\
                prefix_len = 7;
            }
            else
            {
                errors::assign_errc(std::errc::invalid_argument, err_code);
                return false;
            }
        }
        else if (path.size() >= 2 && path[0] == L'\\' && path[1] == L'\\')
        {
            // UNC path: \\server\share
            std::size_t pos = path.find(L'\\', 2);
            if (pos != std::wstring::npos)
            {
                pos = path.find(L'\\', pos + 1);
            }
            prefix_len = pos != std::wstring::npos ? pos + 1 : path.size();
        }
        else if (path.size() >= 3 && path[1] == L':' && path[2] == L'\\')
        {
            // Absolute path with drive letter: C:\
            prefix_len = 3;
        }

        while (path.size() > prefix_len && path.back() == L'\\')
        {
            path.pop_back();
        }
        if (path.empty())
        {
            errors::assign_errc(std::errc::invalid_argument, err_code);
            return false;
        }
        if (path.size() == prefix_len)
        {
            return is_directory(path.c_str(), symlink_mode::RESOLVE, err_code);
        }

        auto create_directory_impl = [&](const path_char_t* path) noexcept
        {
            if (CreateDirectoryW(path, nullptr))
            {
                return true;
            }

            DWORD last_error = GetLastError();
            if (last_error == ERROR_ALREADY_EXISTS)
            {
                return is_directory(path, symlink_mode::RESOLVE, nullptr);
            }

            errors::assign_system_error_code(last_error, err_code);
            return false;
        };

        for (std::size_t i = prefix_len; i < path.size(); ++i)
        {
            if (path[i] == L'\\')
            {
                path_char_t saved = path[i];
                path[i] = L'\0';

                if (path[0] != L'\0' && !create_directory_impl(path.c_str()))
                {
                    return false;
                }

                path[i] = saved;
            }
        }

        return create_directory_impl(path.c_str());
    }
    catch (const std::bad_alloc&)
    {
        errors::assign_errc(std::errc::not_enough_memory, err_code);
        return false;
    }
}

bool create_regular_file(const path_char_t* regular_file_path, std::error_code* err_code) noexcept
{
    HANDLE file_handle = CreateFileW(regular_file_path, GENERIC_WRITE, 0, nullptr, CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handle == INVALID_HANDLE_VALUE)
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        return false;
    }

    CloseHandle(file_handle);
    return true;
}

bool create_shortcut(
    const path_char_t* target_path,
    const path_char_t* shortcut_path,
    std::error_code* err_code
) noexcept
{
    HRESULT result = E_FAIL;
    IShellLinkW* shell_link = nullptr;
    IPersistFile* persist_file = nullptr;

    try
    {
        std::basic_string absolute_target_path(MAX_PATH, path_char_t{});
        DWORD path_length = GetFullPathNameW(target_path,
            static_cast<DWORD>(absolute_target_path.size()), absolute_target_path.data(), nullptr);

        if (path_length == 0)
        {
            errors::assign_system_error_code(GetLastError(), err_code);
            return false;
        }
        if (path_length >= absolute_target_path.size())
        {
            absolute_target_path.resize(path_length + 1);

            path_length = GetFullPathNameW(target_path,
                static_cast<DWORD>(absolute_target_path.size()), absolute_target_path.data(),
                nullptr);

            if (path_length == 0)
            {
                errors::assign_system_error_code(GetLastError(), err_code);
                return false;
            }
            if (path_length >= absolute_target_path.size())
            {
                errors::assign_system_error_code(ERROR_INSUFFICIENT_BUFFER, err_code);
                return false;
            }
        }
        absolute_target_path.resize(path_length);

        std::basic_string shortcut_file(shortcut_path);
        shortcut_file += L".lnk";

        result = CoInitialize(nullptr);
        if (FAILED(result) && result != RPC_E_CHANGED_MODE)
        {
            errors::assign_system_error_code(static_cast<int>(result), err_code);
            return false;
        }
        bool must_uninitialize_com = result != RPC_E_CHANGED_MODE;

        auto cleanup = [&]() noexcept
        {
            if (persist_file != nullptr)
            {
                persist_file->Release();
            }
            if (shell_link != nullptr)
            {
                shell_link->Release();
            }
            if (must_uninitialize_com)
            {
                CoUninitialize();
            }
        };

        result = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLinkW,
            reinterpret_cast<void**>(&shell_link));
        if (FAILED(result))
        {
            cleanup();
            errors::assign_system_error_code(result, err_code);
            return false;
        }

        result = shell_link->SetPath(absolute_target_path.c_str());
        if (FAILED(result))
        {
            cleanup();
            errors::assign_system_error_code(static_cast<int>(result), err_code);
            return false;
        }

        result = shell_link->QueryInterface(IID_IPersistFile,
            reinterpret_cast<void**>(&persist_file));
        if (FAILED(result))
        {
            cleanup();
            errors::assign_system_error_code(static_cast<int>(result), err_code);
            return false;
        }

        result = persist_file->Save(shortcut_file.c_str(), TRUE);
        cleanup();
        if (FAILED(result))
        {
            errors::assign_system_error_code(result, err_code);
            return false;
        }

        return true;
    }
    catch (const std::bad_alloc&)
    {
        errors::assign_errc(std::errc::not_enough_memory, err_code);
        return false;
    }
}

bool create_symlink(
    const path_char_t* target_path,
    const path_char_t* link_path,
    symlink_target_type target_type,
    std::error_code* err_code
) noexcept
{
    DWORD flags = SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE;

    if (target_type == symlink_target_type::AUTO)
    {
        std::error_code local_error;

        if (is_directory(target_path, symlink_mode::RESOLVE, &local_error))
        {
            flags |= SYMBOLIC_LINK_FLAG_DIRECTORY;
        }
        else if (local_error)
        {
            if (err_code != nullptr)
            {
                *err_code = local_error;
            }

            return false;
        }
    }
    else if (target_type == symlink_target_type::DIRECTORY)
    {
        flags |= SYMBOLIC_LINK_FLAG_DIRECTORY;
    }

    if (!CreateSymbolicLinkW(link_path, target_path, flags))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        return false;
    }

    return true;
}

bool file_exists(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    if (symlink_md == symlink_mode::RESOLVE)
    {
        DWORD attrs = GetFileAttributesW(file_path);
        if (attrs == INVALID_FILE_ATTRIBUTES)
        {
            errors::assign_system_error_code(GetLastError(), err_code);
            return false;
        }

        return true;
    }

    HANDLE file_handle = nullptr;

    bool result = get_file_handle(file_path, symlink_mode::NO_RESOLVE, 0, file_handle, err_code);
    if (result)
    {
        CloseHandle(file_handle);
    }

    return result;
}

bool file_has_access(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    access_modes access_mds,
    std::error_code* err_code
) noexcept
{
    HANDLE file_handle = nullptr;
    HANDLE token = nullptr;
    HANDLE impersonation_token = nullptr;
    PACL dacl = nullptr;
    PSECURITY_DESCRIPTOR security_descriptor = nullptr;
    DWORD desired_access = 0;
    DWORD granted_access = 0;
    BOOL access_status = FALSE;
    PRIVILEGE_SET privilege_set{};
    DWORD privilege_set_size = sizeof(PRIVILEGE_SET);
    GENERIC_MAPPING generic_mapping = {
        FILE_GENERIC_READ, FILE_GENERIC_WRITE,
        FILE_GENERIC_EXECUTE, FILE_ALL_ACCESS
    };

    auto cleanup = [&]() noexcept
    {
        if (security_descriptor != nullptr)
        {
            LocalFree(security_descriptor);
        }

        if (token != nullptr)
        {
            CloseHandle(token);
        }

        if (impersonation_token != nullptr)
        {
            CloseHandle(impersonation_token);
        }

        if (file_handle != nullptr)
        {
            CloseHandle(file_handle);
        }
    };

    if ((access_mds & access_modes::READ) != access_modes::NIL)
    {
        desired_access |= GENERIC_READ;
    }
    if ((access_mds & access_modes::WRITE) != access_modes::NIL)
    {
        desired_access |= GENERIC_WRITE;
    }
    if ((access_mds & access_modes::EXECUTE) != access_modes::NIL)
    {
        desired_access |= GENERIC_EXECUTE;
    }

    if (!get_file_handle(file_path, symlink_md, READ_CONTROL, file_handle, err_code))
    {
        return false;
    }

    DWORD result = GetSecurityInfo(file_handle, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION |
        GROUP_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION, nullptr, nullptr, &dacl,
        nullptr, &security_descriptor);

    if (result != ERROR_SUCCESS)
    {
        errors::assign_system_error_code(result, err_code);
        cleanup();
        return false;
    }
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_IMPERSONATE,
        &token))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        cleanup();
        return false;
    }
    if (!DuplicateToken(token, SecurityImpersonation, &impersonation_token))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        cleanup();
        return false;
    }

    MapGenericMask(&desired_access, &generic_mapping);

    if (!AccessCheck(security_descriptor, impersonation_token, desired_access, &generic_mapping,
        &privilege_set, &privilege_set_size, &granted_access, &access_status))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        cleanup();
        return false;
    }

    cleanup();
    return access_status == TRUE;
}

bool file_matches(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    access_modes access_mds,
    file_types file_ts,
    std::error_code* err_code
) noexcept
{
    return is_file_type(file_path, symlink_md, file_ts, err_code) &&
        file_has_access(file_path, symlink_md, access_mds, err_code);
}

inode_t get_file_inode(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    HANDLE file_handle = nullptr;
    BY_HANDLE_FILE_INFORMATION file_info;

    if (!get_file_handle(file_path, symlink_md, 0, file_handle, err_code))
    {
        return std::numeric_limits<inode_t>::max();
    }

    if (!GetFileInformationByHandle(file_handle, &file_info))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        CloseHandle(file_handle);
        return std::numeric_limits<inode_t>::max();
    }

    CloseHandle(file_handle);

    return static_cast<inode_t>(file_info.nFileIndexHigh) << 32 |
        static_cast<inode_t>(file_info.nFileIndexLow);
}

process::user_id get_file_user_id(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    HANDLE file_handle = nullptr;
    PSID owner_sid = nullptr;
    PSECURITY_DESCRIPTOR security_descriptor = nullptr;
    LPWSTR string_sid = nullptr;
    process::user_id uid;

    auto cleanup = [&]() noexcept
    {
        if (string_sid != nullptr)
        {
            LocalFree(string_sid);
        }

        if (security_descriptor != nullptr)
        {
            LocalFree(security_descriptor);
        }

        if (file_handle != nullptr)
        {
            CloseHandle(file_handle);
        }
    };

    if (!get_file_handle(file_path, symlink_md, READ_CONTROL, file_handle, err_code))
    {
        return {};
    }

    DWORD result = GetSecurityInfo(file_handle, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION,
        &owner_sid, nullptr, nullptr, nullptr, &security_descriptor);

    if (result != ERROR_SUCCESS)
    {
        errors::assign_system_error_code(result, err_code);
        cleanup();
        return uid;
    }

    if (!ConvertSidToStringSidW(owner_sid, &string_sid))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        cleanup();
        return uid;
    }

    uid.assign(string_sid);
    cleanup();
    return uid;
}

file_size_t get_file_size(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    HANDLE file_handle = nullptr;
    LARGE_INTEGER file_size;

    if (!get_file_handle(file_path, symlink_md, 0, file_handle, err_code))
    {
        return std::numeric_limits<file_size_t>::max();
    }

    if (!GetFileSizeEx(file_handle, &file_size))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        CloseHandle(file_handle);
        return std::numeric_limits<file_size_t>::max();
    }

    CloseHandle(file_handle);
    return static_cast<file_size_t>(file_size.QuadPart);
}

bool get_file_times(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    time::calendar_time* creation_time,
    time::calendar_time* last_access_time,
    time::calendar_time* modification_time,
    std::error_code* err_code
) noexcept
{
    HANDLE file_handle = nullptr;
    FILETIME native_creation_time{};
    FILETIME native_last_access_time{};
    FILETIME native_last_write_time{};

    auto assign_file_time = [&](
        const FILETIME& file_time,
        time::calendar_time* calendar_time
    ) noexcept
    {
        if (calendar_time == nullptr)
        {
            return true;
        }

        SYSTEMTIME utc_system_time;

        if (!FileTimeToSystemTime(&file_time, &utc_system_time))
        {
            errors::assign_system_error_code(GetLastError(), err_code);
            return false;
        }

        calendar_time
            ->set_year(utc_system_time.wYear)
            .set_month(utc_system_time.wMonth)
            .set_day(utc_system_time.wDay)
            .set_hour(utc_system_time.wHour)
            .set_minute(utc_system_time.wMinute)
            .set_second(utc_system_time.wSecond)
            .set_millisecond(utc_system_time.wMilliseconds);

        return true;
    };

    if (!get_file_handle(file_path, symlink_md, 0, file_handle, err_code))
    {
        return false;
    }
    if (!GetFileTime(file_handle, &native_creation_time, &native_last_access_time,
        &native_last_write_time))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        CloseHandle(file_handle);
        return false;
    }

    CloseHandle(file_handle);

    if (!assign_file_time(native_creation_time, creation_time))
    {
        return false;
    }

    if (!assign_file_time(native_last_access_time, last_access_time))
    {
        return false;
    }

    if (!assign_file_time(native_last_write_time, modification_time))
    {
        return false;
    }

    return true;
}

std::basic_string<path_char_t> get_temporary_path(std::error_code* err_code) noexcept
{
    try
    {
        std::basic_string temporary_path(MAX_PATH, path_char_t{});

        DWORD path_length = GetTempPathW(static_cast<DWORD>(temporary_path.size()),
            temporary_path.data());

        if (path_length == 0)
        {
            errors::assign_system_error_code(GetLastError(), err_code);
            return {};
        }

        if (path_length >= temporary_path.size())
        {
            temporary_path.resize(path_length + 1);

            path_length = GetTempPathW(static_cast<DWORD>(temporary_path.size()),
                temporary_path.data());

            if (path_length == 0)
            {
                errors::assign_system_error_code(GetLastError(), err_code);
                return {};
            }

            if (path_length >= temporary_path.size())
            {
                errors::assign_system_error_code(ERROR_INSUFFICIENT_BUFFER, err_code);
                return {};
            }
        }

        temporary_path.resize(path_length);
        return temporary_path;
    }
    catch (const std::bad_alloc&)
    {
        errors::assign_errc(std::errc::not_enough_memory, err_code);
        return {};
    }
}

bool is_block_device(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    return is_file_type(file_path, symlink_md, file_types::BLOCK_DEVICE, err_code);
}

bool is_character_device(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    return is_file_type(file_path, symlink_md, file_types::CHARACTER_DEVICE, err_code);
}

bool is_directory(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    return is_file_type(file_path, symlink_md, file_types::DIRECTORY, err_code);
}

bool is_file_type(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    file_types file_ts,
    std::error_code* err_code
) noexcept
{
    if (file_ts == file_types::NIL)
    {
        return true;
    }

    // Character device
    if ((file_ts & file_types::CHARACTER_DEVICE) != file_types::NIL)
    {
        HANDLE char_handle = CreateFileW(file_path, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
            OPEN_EXISTING, 0, nullptr);

        if (char_handle != INVALID_HANDLE_VALUE)
        {
            bool is_char_device = GetFileType(char_handle) == FILE_TYPE_CHAR;
            CloseHandle(char_handle);
            if (is_char_device)
            {
                return true;
            }
        }
    }

    HANDLE file_handle = nullptr;
    if (!get_file_handle(file_path, symlink_md, 0, file_handle, err_code))
    {
        return false;
    }

    // Block device
    if ((file_ts & file_types::BLOCK_DEVICE) != file_types::NIL)
    {
        DISK_GEOMETRY disk_geometry;
        DWORD bytes_returned;

        if (DeviceIoControl(file_handle, IOCTL_DISK_GET_DRIVE_GEOMETRY, nullptr, 0, &disk_geometry,
            sizeof(disk_geometry), &bytes_returned, nullptr) != FALSE)
        {
            CloseHandle(file_handle);
            return true;
        }
    }

    // Pipe
    if ((file_ts & file_types::PIPE) != file_types::NIL)
    {
        DWORD file_type = GetFileType(file_handle);

        if (file_type == FILE_TYPE_UNKNOWN)
        {
            DWORD last_error = GetLastError();

            if (last_error != NO_ERROR)
            {
                errors::assign_system_error_code(last_error, err_code);
                CloseHandle(file_handle);
                return false;
            }
        }
        else if (file_type == FILE_TYPE_PIPE)
        {
            CloseHandle(file_handle);
            return true;
        }
    }

    // Symlink
    if ((file_ts & file_types::SYMLINK) != file_types::NIL)
    {
        BYTE buffer[MAXIMUM_REPARSE_DATA_BUFFER_SIZE];
        DWORD bytes_returned;

        BOOL result = DeviceIoControl(file_handle, FSCTL_GET_REPARSE_POINT, nullptr, 0, buffer,
            sizeof(buffer), &bytes_returned, nullptr);

        if (!result)
        {
            DWORD last_error = GetLastError();
            if (last_error != ERROR_NOT_A_REPARSE_POINT)
            {
                errors::assign_system_error_code(last_error, err_code);
                CloseHandle(file_handle);
                return false;
            }
        }
        else
        {
            auto* reparse_data = reinterpret_cast<detail::reparse_data_buffer*>(buffer);
            if (reparse_data->ReparseTag == IO_REPARSE_TAG_SYMLINK)
            {
                CloseHandle(file_handle);
                return true;
            }
        }
    }

    BY_HANDLE_FILE_INFORMATION info;
    if (!GetFileInformationByHandle(file_handle, &info))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        CloseHandle(file_handle);
        return false;
    }

    CloseHandle(file_handle);

    // Directory
    if ((file_ts & file_types::DIRECTORY) != file_types::NIL &&
        (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
    {
        return true;
    }

    // Regular file
    if ((file_ts & file_types::REGULAR_FILE) != file_types::NIL &&
        (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
    {
        return true;
    }

    return false;
}

bool is_pipe(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    return is_file_type(file_path, symlink_md, file_types::PIPE, err_code);
}

bool is_regular_file(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    return is_file_type(file_path, symlink_md, file_types::REGULAR_FILE, err_code);
}

bool is_socket(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    return is_file_type(file_path, symlink_md, file_types::SOCKET, err_code);
}

bool is_symlink(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    return is_file_type(file_path, symlink_md, file_types::SYMLINK, err_code);
}

bool open_directory(
    directory_stream& directory_strm,
    const path_char_t* directory_path,
    std::error_code* err_code
) noexcept
{
    std::wstring nt_path;
    UNICODE_STRING path;
    OBJECT_ATTRIBUTES obj_attr;
    IO_STATUS_BLOCK io_status;

    auto& native = directory_strm.native;

    if (!make_nt_path(directory_path, nt_path, err_code))
    {
        return false;
    }

    path.Buffer = nt_path.data();
    path.Length = static_cast<USHORT>(nt_path.length() * sizeof(WCHAR));
    path.MaximumLength = path.Length + sizeof(WCHAR);

    InitializeObjectAttributes(
        &obj_attr,
        &path,
        OBJ_CASE_INSENSITIVE,
        nullptr,
        nullptr);

    NTSTATUS status = NtOpenFile(
        &native.dir_handle,
        FILE_LIST_DIRECTORY | SYNCHRONIZE,
        &obj_attr,
        &io_status,
        FILE_SHARE_READ | FILE_SHARE_WRITE |
        FILE_SHARE_DELETE,
        FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);

    if (!NT_SUCCESS(status))
    {
        errors::assign_system_error_code(RtlNtStatusToDosError(status), err_code);
        return false;
    }

    native.buffer_offset = 0;
    native.valid_bytes = 0;
    native.restart_scan = true;
    directory_strm.name = nullptr;
    directory_strm.inode = 0;
    directory_strm.type = file_types::NIL;

    return true;
}

bool open_directory(
    directory_stream& child,
    const directory_stream& parent,
    std::error_code* err_code
) noexcept
{
    UNICODE_STRING name;
    OBJECT_ATTRIBUTES obj_attr;
    IO_STATUS_BLOCK io_status;

    auto& child_native = child.native;

    if (parent.name == nullptr)
    {
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    name.Buffer = const_cast<PWSTR>(parent.name);
    name.Length = static_cast<USHORT>(wcslen(parent.name) * sizeof(WCHAR));
    name.MaximumLength = name.Length + sizeof(WCHAR);

    InitializeObjectAttributes(
        &obj_attr,
        &name,
        OBJ_CASE_INSENSITIVE,
        parent.native.dir_handle,
        nullptr);

    NTSTATUS status = NtOpenFile(
        &child_native.dir_handle,
        FILE_LIST_DIRECTORY | SYNCHRONIZE,
        &obj_attr,
        &io_status,
        FILE_SHARE_READ |
        FILE_SHARE_WRITE |
        FILE_SHARE_DELETE,
        FILE_DIRECTORY_FILE |
        FILE_SYNCHRONOUS_IO_NONALERT);

    if (!NT_SUCCESS(status))
    {
        errors::assign_system_error_code(RtlNtStatusToDosError(status), err_code);
        return false;
    }

    child_native.buffer_offset = 0;
    child_native.valid_bytes = 0;
    child_native.restart_scan = true;
    child_native.name_storage.clear();
    child.name = nullptr;
    child.inode = 0;
    child.type = file_types::NIL;

    return true;
}

bool read_directory(directory_stream& directory_strm, std::error_code* err_code) noexcept
{
    auto& native = directory_strm.native;

    try
    {
        for (;;)
        {
            if (native.buffer_offset >= native.valid_bytes)
            {
                IO_STATUS_BLOCK io_status;

                NTSTATUS status = NtQueryDirectoryFile(
                    native.dir_handle,
                    nullptr,
                    nullptr,
                    nullptr,
                    &io_status,
                    native.buffer,
                    sizeof(native.buffer),
                    static_cast<FILE_INFORMATION_CLASS>(detail::FileIdBothDirectoryInformation_),
                    FALSE,
                    nullptr,
                    native.restart_scan);

                native.restart_scan = false;

                if (status == STATUS_NO_MORE_FILES)
                {
                    return false;
                }
                if (!NT_SUCCESS(status))
                {
                    errors::assign_system_error_code(RtlNtStatusToDosError(status), err_code);
                    return false;
                }

                native.valid_bytes = static_cast<std::size_t>(io_status.Information);
                native.buffer_offset = 0;
            }

            auto* entry = reinterpret_cast<detail::file_id_both_dir_information*>(
                native.buffer + native.buffer_offset);

            std::size_t name_len = entry->FileNameLength / sizeof(WCHAR);

            if ((name_len == 1 && entry->FileName[0] == L'.') ||
                (name_len == 2 && entry->FileName[0] == L'.' && entry->FileName[1] == L'.'))
            {
                if (entry->NextEntryOffset != 0)
                {
                    native.buffer_offset += entry->NextEntryOffset;
                }
                else
                {
                    native.buffer_offset = native.valid_bytes;
                }

                continue;
            }

            native.name_storage.assign(entry->FileName, name_len);
            directory_strm.name = native.name_storage.c_str();
            directory_strm.inode = static_cast<inode_t>(entry->FileId.QuadPart);

            if (entry->FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
            {
                directory_strm.type = file_types::SYMLINK;
            }
            else if (entry->FileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                directory_strm.type = file_types::DIRECTORY;
            }
            else
            {
                directory_strm.type = file_types::REGULAR_FILE;
            }

            if (entry->NextEntryOffset != 0)
            {
                native.buffer_offset += entry->NextEntryOffset;
            }
            else
            {
                native.buffer_offset = native.valid_bytes;
            }

            return true;
        }
    }
    catch (const std::bad_alloc&)
    {
        errors::assign_system_error_code(ERROR_NOT_ENOUGH_MEMORY, err_code);
    }
    catch (...)
    {
        errors::assign_system_error_code(ERROR_BAD_ARGUMENTS, err_code);
    }

    return false;
}

bool remove_directory(const path_char_t* directory_path, std::error_code* err_code) noexcept
{
    (void)SetFileAttributesW(directory_path, FILE_ATTRIBUTE_NORMAL);

    if (!RemoveDirectoryW(directory_path))
    {
        errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}

bool remove_path(const path_char_t* file_path, std::error_code* err_code) noexcept
{
    std::error_code local_error;

    if (is_directory(file_path, symlink_mode::NO_RESOLVE, &local_error))
    {
        return remove_directory(file_path, err_code);
    }

    if (local_error)
    {
        if (err_code != nullptr)
        {
            *err_code = local_error;
        }

        return false;
    }

    return remove_regular_file(file_path, err_code);
}

bool remove_regular_file(const path_char_t* regular_file_path, std::error_code* err_code) noexcept
{
    (void)SetFileAttributesW(regular_file_path, FILE_ATTRIBUTE_NORMAL);

    if (!DeleteFileW(regular_file_path))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        return false;
    }

    return true;
}

bool set_current_directory(const path_char_t* directory_path, std::error_code* err_code) noexcept
{
    if (!SetCurrentDirectoryW(directory_path))
    {
        errors::assign_system_error_code(GetLastError(), err_code);
        return false;
    }

    return true;
}

}

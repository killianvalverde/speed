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
 * @brief       filesystem operations source.
 * @author      Killian Valverde
 * @date        2017/05/26
 */

#include "../../../platform/platform.hpp"
#ifdef SPEED_WINAPI

#include "operations.hpp"

#include <accctrl.h>
#include <aclapi.h>
#include <io.h>
#include <sddl.h>
#include <shlguid.h>
#include <shobjidl_core.h>
#include <winioctl.h>

#include <cstdlib>
#include <memory>

#include "../../../../cryptography/cryptography.hpp"

namespace speed::system::detail::winapi::filesystem {

bool access(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        access_modes access_mods,
        std::error_code* err_code
) noexcept
{
    HANDLE file_handl;
    DWORD access_desird = 0;
    PACL p_dacl = nullptr;
    PSECURITY_DESCRIPTOR p_security_descriptr = nullptr;
    HANDLE tokn = nullptr;
    HANDLE impersonation_tokn = nullptr;
    PRIVILEGE_SET privilege_st{};
    DWORD privilege_st_sz = sizeof(PRIVILEGE_SET);
    DWORD granted_accss = 0;
    BOOL access_sttus = false;
    GENERIC_MAPPING generic_mappng = {FILE_GENERIC_READ, FILE_GENERIC_WRITE,
            FILE_GENERIC_EXECUTE, FILE_ALL_ACCESS};
    
    if (!get_file_handle(file_pth, resolve_symlnk, READ_CONTROL, file_handl, err_code))
    {
        return false;
    }
    
    if (access_mods == access_modes::EXISTS)
    {
        ::CloseHandle(file_handl);
        return true;
    }
    
    if ((access_mods & access_modes::READ) != access_modes::NIL)
    {
        access_desird |= GENERIC_READ;
    }
    if ((access_mods & access_modes::WRITE) != access_modes::NIL)
    {
        access_desird |= GENERIC_WRITE;
    }
    if ((access_mods & access_modes::EXECUTE) != access_modes::NIL)
    {
        access_desird |= GENERIC_EXECUTE;
    }

    if (::GetSecurityInfo(file_handl, SE_FILE_OBJECT,
            DACL_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION,
            nullptr, nullptr, &p_dacl, nullptr, &p_security_descriptr) != ERROR_SUCCESS ||
        !::OpenProcessToken(GetCurrentProcess(),
            TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_IMPERSONATE, &tokn) ||
        !::DuplicateToken(tokn, SecurityImpersonation, &impersonation_tokn))
    {
        goto cleanup_and_fail;
    }

    ::MapGenericMask(&access_desird, &generic_mappng);

    if (!::AccessCheck(p_security_descriptr, impersonation_tokn, access_desird, &generic_mappng,
            &privilege_st, &privilege_st_sz, &granted_accss, &access_sttus))
    {
        goto cleanup_and_fail;
    }

    goto cleanup_and_return;

cleanup_and_fail:
    system::errors::assign_system_error_code((int)GetLastError(), err_code);

cleanup_and_return:
    if (p_security_descriptr != nullptr)
    {
        ::LocalFree(p_security_descriptr);
    }
    if (tokn != nullptr)
    {
        ::CloseHandle(tokn);
    }
    if (impersonation_tokn != nullptr)
    {
        ::CloseHandle(impersonation_tokn);
    }
    ::CloseHandle(file_handl);

    return access_sttus == TRUE;
}

bool chdir(const path_char_t* directory_pth, std::error_code* err_code) noexcept
{
    if (!::SetCurrentDirectoryW(directory_pth))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}

bool check_file(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        access_modes access_mods,
        file_types file_typs,
        std::error_code* err_code
) noexcept
{
    return (is_file_type(file_pth, resolve_symlnk, file_typs, err_code) &&
            access(file_pth, resolve_symlnk, access_mods, err_code));
}

bool closedir(directory_entity& directory_ent, std::error_code* err_code) noexcept
{
    auto& directory_ent_ext = directory_ent._ext;
    
    if (!::FindClose(directory_ent_ext.dir_handl))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }
    
    return true;
}

bool get_file_handle(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        DWORD desired_accss,
        HANDLE& file_handl,
        std::error_code* err_code
) noexcept
{
    DWORD flgs = FILE_FLAG_BACKUP_SEMANTICS;
    
    if (!resolve_symlnk)
    {
        flgs |= FILE_FLAG_OPEN_REPARSE_POINT;
    }
    
    file_handl = ::CreateFileW(
            file_pth,
            desired_accss,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            nullptr,
            OPEN_EXISTING,
            flgs,
            nullptr);
    
    if (file_handl == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }
    
    return true;
}

bool file_exists(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    return access(file_pth, resolve_symlnk, access_modes::EXISTS, err_code);
}

inode_t get_file_inode(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    HANDLE file_handl;
    BY_HANDLE_FILE_INFORMATION file_info;
    
    if (!get_file_handle(file_pth, resolve_symlnk, 0, file_handl, err_code))
    {
        return ~0ull;
    }

    if (!::GetFileInformationByHandle(file_handl, &file_info))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        ::CloseHandle(file_handl);
        return ~0ull;
    }

    ::CloseHandle(file_handl);
    return ((std::uint64_t)file_info.nFileIndexHigh << 32) | file_info.nFileIndexLow;
}

inode_t get_file_inode(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    auto& directory_ent_ext = directory_ent._ext;
    wchar_t search_pth[MAX_PATH];
    errno_t std_err;
    
    if ((std_err = ::wcscpy_s(search_pth, MAX_PATH, directory_ent_ext.pth)) != 0 ||
        (std_err = ::wcscat_s(search_pth, MAX_PATH, directory_ent.nme)) != 0)
    {
        system::errors::assign_system_error_code((int)std_err, err_code);
        return false;
    }
    
    return get_file_inode(search_pth, resolve_symlnk, err_code);
}

system::process::uid_t get_file_uid(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    HANDLE file_handl = nullptr;
    PSID owner_sid;
    PSECURITY_DESCRIPTOR sec_desc = nullptr;
    LPSTR str_sid = nullptr;
    system::process::uid_t uid = ~0ull;
    
    if (!get_file_handle(file_pth, resolve_symlnk, GENERIC_READ, file_handl, err_code))
    {
        return ~0ull;
    }

    if (::GetSecurityInfo(file_handl, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &owner_sid,
            nullptr, nullptr, nullptr, &sec_desc) != ERROR_SUCCESS ||
        !::ConvertSidToStringSidA(owner_sid, &str_sid))
    {
        goto cleanup_and_fail;
    }

    uid = cryptography::city_hash_64((const char*)str_sid);
    goto cleanup_and_return;
    
cleanup_and_fail:
    system::errors::assign_system_error_code((int)GetLastError(), err_code);

cleanup_and_return:
    if (str_sid != nullptr)
    {
        ::LocalFree(str_sid);
    }
    if (sec_desc != nullptr)
    {
        ::LocalFree(sec_desc);
    }
    ::CloseHandle(file_handl);
    
    return uid;
}

system::process::gid_t get_file_gid(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    HANDLE file_handl;
    DWORD status;
    PSECURITY_DESCRIPTOR security_desc = nullptr;
    PSID group_sid = nullptr;
    BOOL group_defaultd = FALSE;
    LPSTR sid_cstr = nullptr;
    system::process::gid_t gid = ~0ull;
    
    if (!get_file_handle(file_pth, resolve_symlnk, READ_CONTROL, file_handl, err_code))
    {
        return ~0ull;
    }

    status = ::GetSecurityInfo(file_handl, SE_FILE_OBJECT, GROUP_SECURITY_INFORMATION,
            nullptr, nullptr, nullptr, nullptr, &security_desc);
    if (status != ERROR_SUCCESS)
    {
        system::errors::assign_system_error_code((int) status, err_code);
        ::CloseHandle(file_handl);
        return ~0ull;
    }
    
    if (::GetSecurityDescriptorGroup(security_desc, &group_sid, &group_defaultd) &&
        ::ConvertSidToStringSidA(group_sid, &sid_cstr))
    {
        gid = speed::cryptography::city_hash_64((const char*) sid_cstr);
        ::LocalFree(sid_cstr);
    }
    else
    {
        system::errors::assign_system_error_code((int) ::GetLastError(), err_code);
    }

    ::LocalFree(security_desc);
    ::CloseHandle(file_handl);
    return gid;
}

std::size_t get_file_size(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    HANDLE file_handl;
    LARGE_INTEGER file_sz;
    
    if (!get_file_handle(file_pth, resolve_symlnk, 0, file_handl, err_code))
    {
        return ~0ull;
    }
    
    if (!::GetFileSizeEx(file_handl, &file_sz))
    {
        system::errors::assign_system_error_code((int) ::GetLastError(), err_code);
        ::CloseHandle(file_handl);
        return ~0ull;
    }

    ::CloseHandle(file_handl);
    return static_cast<std::size_t>(file_sz.QuadPart);
}

bool get_modification_time(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        system::time::system_time& system_tme,
        std::error_code* err_code
) noexcept
{
    HANDLE file_handl;
    FILETIME last_write_tme;
    SYSTEMTIME utc_system_tme;
    SYSTEMTIME local_system_tme;
    
    if (!get_file_handle(file_pth, resolve_symlnk, 0, file_handl, err_code))
    {
        return false;
    }

    if (!::GetFileTime(file_handl, nullptr, nullptr, &last_write_tme) ||
        !::FileTimeToSystemTime(&last_write_tme, &utc_system_tme) ||
        !::SystemTimeToTzSpecificLocalTime(nullptr, &utc_system_tme, &local_system_tme))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        ::CloseHandle(file_handl);
        return false;
    }
    
    system_tme.set_years(local_system_tme.wYear)
              .set_months(local_system_tme.wMonth)
              .set_days(local_system_tme.wDay)
              .set_hours(local_system_tme.wHour)
              .set_minutes(local_system_tme.wMinute)
              .set_seconds(local_system_tme.wSecond);
    
    ::CloseHandle(file_handl);
    return true;
}

const path_char_t* get_temporal_path() noexcept
{
    static path_char_t temp_pth[MAX_PATH];
    DWORD len = ::GetTempPathW(MAX_PATH, temp_pth);

    if (len == 0 || len > MAX_PATH)
    {
        return nullptr;
    }

    return temp_pth;
}

bool is_block_device(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    return is_file_type(file_pth, resolve_symlnk, file_types::BLOCK_DEVICE, err_code);
}

bool is_block_device(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    return is_file_type(directory_ent, resolve_symlnk, file_types::BLOCK_DEVICE, err_code);
}

bool is_character_device(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    return is_file_type(file_pth, resolve_symlnk, file_types::CHARACTER_DEVICE, err_code);
}

bool is_character_device(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    return is_file_type(directory_ent, resolve_symlnk, file_types::CHARACTER_DEVICE, err_code);
}

bool is_directory(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    return is_file_type(file_pth, resolve_symlnk, file_types::DIRECTORY, err_code);
}

bool is_directory(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    return is_file_type(directory_ent, resolve_symlnk, file_types::DIRECTORY, err_code);
}

bool is_file_type(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        file_types fle_type,
        std::error_code* err_code
) noexcept
{
    HANDLE file_handl;
    
    if (!get_file_handle(file_pth, resolve_symlnk, 0, file_handl, err_code))
    {
        return false;
    }
    
    if ((fle_type & file_types::BLOCK_DEVICE) != file_types::NIL)
    {
        DISK_GEOMETRY disk_geo;
        DWORD bytes_ret = 0;
        if (::DeviceIoControl(file_handl, IOCTL_DISK_GET_DRIVE_GEOMETRY, nullptr, 0, &disk_geo,
                    sizeof(disk_geo), &bytes_ret, nullptr))
        {
            ::CloseHandle(file_handl);
            return true;
        }
        system::errors::assign_system_error_code((int)::GetLastError(), err_code);
    }
    if ((fle_type & file_types::CHARACTER_DEVICE) != file_types::NIL)
    {
        DWORD file_typ = ::GetFileType(file_handl);
        if (file_typ == FILE_TYPE_CHAR)
        {
            ::CloseHandle(file_handl);
            return true;
        }
    }
    if ((fle_type & file_types::DIRECTORY) != file_types::NIL)
    {
        BY_HANDLE_FILE_INFORMATION info;
        if (!::GetFileInformationByHandle(file_handl, &info))
        {
            system::errors::assign_system_error_code((int)::GetLastError(), err_code);
        }
        else if ((info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                 !(info.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT))
        {
            ::CloseHandle(file_handl);
            return true;
        }
    }
    if ((fle_type & file_types::PIPE) != file_types::NIL)
    {
        DWORD file_typ = ::GetFileType(file_handl);
        if (file_typ == FILE_TYPE_PIPE)
        {
            ::CloseHandle(file_handl);
            return true;
        }
    }
    if ((fle_type & file_types::REGULAR_FILE) != file_types::NIL)
    {
        BY_HANDLE_FILE_INFORMATION info;
        if (!::GetFileInformationByHandle(file_handl, &info))
        {
            system::errors::assign_system_error_code((int)::GetLastError(), err_code);
        }
        else if (!(info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                 !(info.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT))
        {
            ::CloseHandle(file_handl);
            return true;
        }
    }
    if ((fle_type & file_types::SOCKET) != file_types::NIL)
    {
        system::errors::assign_system_error_code(ERROR_CALL_NOT_IMPLEMENTED, err_code);
    }
    if ((fle_type & file_types::SYMLINK) != file_types::NIL)
    {
        BYTE repase_data_buf[MAXIMUM_REPARSE_DATA_BUFFER_SIZE];
        DWORD bytes_ret = 0;
        
        if (::DeviceIoControl(file_handl, FSCTL_GET_REPARSE_POINT, nullptr, 0, repase_data_buf,
                sizeof(repase_data_buf), &bytes_ret, nullptr) &&
            *(DWORD*)repase_data_buf == IO_REPARSE_TAG_SYMLINK)
        {
            ::CloseHandle(file_handl);
            return true;
        }
        system::errors::assign_system_error_code((int)::GetLastError(), err_code);
    }
    
    ::CloseHandle(file_handl);
    return false;
}

bool is_file_type(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        file_types fle_type,
        std::error_code* err_code
) noexcept
{
    DWORD file_attrs = directory_ent._ext.find_dat.dwFileAttributes;
    DWORD reparse_tg = directory_ent._ext.find_dat.dwReserved0;

    if (resolve_symlnk &&
        (file_attrs & FILE_ATTRIBUTE_REPARSE_POINT) &&
        reparse_tg == IO_REPARSE_TAG_SYMLINK)
    {
        WIN32_FILE_ATTRIBUTE_DATA attr_data;
        path_char_t pth[MAX_PATH] = {};
        
        stringutils::cstr_copy(pth, directory_ent._ext.pth);
        stringutils::cstr_remove_trailing_if(pth, [](wchar_t ch) { return ch == L'\\'; });
        stringutils::cstr_concat(pth, "\\", MAX_PATH);
        stringutils::cstr_concat(pth, directory_ent.nme, MAX_PATH);
        
        if (!::GetFileAttributesExW(pth, GetFileExInfoStandard, &attr_data))
        {
            system::errors::assign_system_error_code((int)::GetLastError(), err_code);
            return false;
        }

        file_attrs = attr_data.dwFileAttributes;
    }
    
    if ((fle_type & file_types::DIRECTORY) != file_types::NIL &&
        (file_attrs & FILE_ATTRIBUTE_DIRECTORY) && !(file_attrs & FILE_ATTRIBUTE_REPARSE_POINT))
    {
        return true;
    }
    if ((fle_type & file_types::REGULAR_FILE) != file_types::NIL &&
        !(file_attrs & FILE_ATTRIBUTE_DIRECTORY) && !(file_attrs & FILE_ATTRIBUTE_REPARSE_POINT))
    {
        return true;
    }
    if ((fle_type & file_types::SYMLINK) != file_types::NIL &&
        (file_attrs & FILE_ATTRIBUTE_REPARSE_POINT) &&
        directory_ent._ext.find_dat.dwReserved0 == IO_REPARSE_TAG_SYMLINK)
    {
        return true;
    }
    if ((fle_type & (file_types::BLOCK_DEVICE | file_types::CHARACTER_DEVICE | file_types::PIPE |
                file_types::SOCKET)) != file_types::NIL)
    {
        system::errors::assign_system_error_code(ERROR_CALL_NOT_IMPLEMENTED, err_code);
    }

    return false;
}

bool is_pipe(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    return is_file_type(file_pth, resolve_symlnk, file_types::PIPE, err_code);
}

bool is_pipe(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    return is_file_type(directory_ent, resolve_symlnk, file_types::PIPE, err_code);
}

bool is_regular_file(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    return is_file_type(file_pth, resolve_symlnk, file_types::REGULAR_FILE, err_code);
}

bool is_regular_file(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    return is_file_type(directory_ent, resolve_symlnk, file_types::REGULAR_FILE, err_code);
}

bool is_socket(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    return is_file_type(file_pth, resolve_symlnk, file_types::SOCKET, err_code);
}

bool is_socket(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    return is_file_type(directory_ent, resolve_symlnk, file_types::SOCKET, err_code);
}

bool is_symlink(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    return is_file_type(file_pth, resolve_symlnk, file_types::SYMLINK, err_code);
}

bool is_symlink(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    return is_file_type(directory_ent, resolve_symlnk, file_types::SYMLINK, err_code);
}

bool mkdir(const path_char_t* directory_pth, std::error_code* err_code) noexcept
{
    if (!::CreateDirectoryW(directory_pth, nullptr))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}

bool mkdir_recursively(const path_char_t* directory_pth, std::error_code* err_code) noexcept
{
    wchar_t parnt_path[MAX_PATH] = {0};
    std::size_t pth_len;
    size_t slash_pos[MAX_PATH];
    size_t slash_pos_sz = 0;
    wchar_t* last_chr;

    pth_len = stringutils::cstr_length(directory_pth);

    if (pth_len >= MAX_PATH ||
        pth_len == 0 ||
        access(directory_pth, false, access_modes::EXISTS, err_code))
    {
        return false;
    }

    stringutils::cstr_copy(parnt_path, directory_pth);
    stringutils::cstr_remove_trailing_if(parnt_path, [](wchar_t ch) { return ch == L'\\'; });

    do
    {
        last_chr = stringutils::cstr_cut(parnt_path, L'\\', true);

        if (last_chr == nullptr)
        {
            if (!mkdir(parnt_path, err_code))
            {
                return false;
            }

            break;
        }

        pth_len = last_chr - parnt_path;
        slash_pos[slash_pos_sz++] = pth_len;

        if (pth_len > 0 && parnt_path[pth_len - 1] == L':')
        {
            pth_len = 0;
        }
    }
    while (!access(parnt_path, false, access_modes::EXISTS, err_code) && pth_len > 0);

    while (slash_pos_sz > 0)
    {
        parnt_path[slash_pos[--slash_pos_sz]] = L'\\';

        if (!mkdir(parnt_path, err_code))
        {
            return false;
        }
    }

    return true;
}

bool opendir(
        directory_entity& directory_ent,
        const path_char_t* directory_pth,
        std::error_code* err_code
) noexcept
{
    std::size_t directory_pth_len;
    bool slash_insertd = false;
    auto& directory_ent_ext = directory_ent._ext;
    
    directory_pth_len = stringutils::cstr_length(directory_pth);
    if (directory_pth_len >= MAX_PATH - 3)
    {
        system::errors::assign_system_error_code(ERANGE, err_code);
        return false;
    }
    
    stringutils::cstr_copy(directory_ent_ext.pth, directory_pth);
    if (directory_pth_len < 1 || directory_pth[directory_pth_len - 1] != '\\')
    {
        stringutils::cstr_concat(directory_ent_ext.pth, "\\*");
        slash_insertd = true;
    }
    else
    {
        stringutils::cstr_concat(directory_ent_ext.pth, "*");
    }
    
    directory_ent_ext.dir_handl = ::FindFirstFileW(
            directory_ent_ext.pth, &directory_ent_ext.find_dat);
    
    if (slash_insertd)
    {
        directory_ent_ext.pth[directory_pth_len + 1] = '\0';
    }
    else
    {
        directory_ent_ext.pth[directory_pth_len] = '\0';
    }
    
    if (directory_ent_ext.dir_handl == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int) GetLastError(), err_code);
        return false;
    }
    
    directory_ent_ext.read_dne = false;
    return true;
}

bool readdir(directory_entity& directory_ent, std::error_code* err_code) noexcept
{
    auto& directory_ent_ext = directory_ent._ext;
    DWORD last_err;

    if (directory_ent_ext.read_dne &&
        !::FindNextFileW(directory_ent_ext.dir_handl, &directory_ent_ext.find_dat))
    {
        last_err = ::GetLastError();
        if (last_err != ERROR_NO_MORE_FILES)
        {
            system::errors::assign_system_error_code((int)last_err, err_code);
        }

        return false;
    }

    directory_ent_ext.read_dne = true;
    directory_ent.nme = directory_ent_ext.find_dat.cFileName;

    return true;
}

bool rmdir(const path_char_t* directory_pth, std::error_code* err_code) noexcept
{
    ::SetFileAttributesW(directory_pth, FILE_ATTRIBUTE_NORMAL);
    if (!::RemoveDirectoryW(directory_pth))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}

bool shortcut(
        const path_char_t* target_pth,
        const path_char_t* shortcut_pth,
        std::error_code* err_code
) noexcept
{
    HRESULT res = -1;
    IShellLinkW* shell_lnk = nullptr;
    IPersistFile* persist_fle = nullptr;
    wchar_t absolute_target_pth[MAX_PATH] = {};
    wchar_t wshortcut_pth[MAX_PATH] = {};
    std::size_t shortcut_pth_len;
    errno_t err;

    if (!::GetFullPathNameW(target_pth, MAX_PATH, absolute_target_pth, nullptr))
    {
        system::errors::assign_system_error_code(ERROR_BAD_PATHNAME, err_code);
        goto exit;
    }

    ::CoInitialize(nullptr);
    res = ::CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLinkW,
            (LPVOID*)&shell_lnk);
    if (!SUCCEEDED(res))
    {
        goto exit_with_error;
    }

    shell_lnk->SetPath(absolute_target_pth);
    shell_lnk->SetDescription(nullptr);

    res = shell_lnk->QueryInterface(IID_IPersistFile, (LPVOID*)&persist_fle);
    if (!SUCCEEDED(res))
    {
        goto exit_with_error;
    }

    shortcut_pth_len = stringutils::cstr_length(shortcut_pth);
    if (shortcut_pth_len + 5 >= MAX_PATH)
    {
        system::errors::assign_system_error_code(ERROR_BAD_PATHNAME, err_code);
        goto exit;
    }

    stringutils::cstr_copy(wshortcut_pth, shortcut_pth);
    stringutils::cstr_copy(wshortcut_pth + shortcut_pth_len, L".lnk");
    res = persist_fle->Save(wshortcut_pth, true);

    goto exit;

exit_with_error:
    system::errors::assign_system_error_code((int)GetLastError(), err_code);

exit:
    if (shell_lnk != nullptr)
    {
        shell_lnk->Release();
    }
    if (persist_fle != nullptr)
    {
        persist_fle->Release();
    }

    return res == 0;
}

bool symlink(
        const path_char_t* target_pth,
        const path_char_t* link_pth,
        std::error_code* err_code
) noexcept
{
    DWORD flgs = SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE;
    DWORD file_attr;

    file_attr = ::GetFileAttributesW(target_pth);
    if (file_attr == INVALID_FILE_ATTRIBUTES)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    if (file_attr & FILE_ATTRIBUTE_DIRECTORY)
    {
        flgs |= SYMBOLIC_LINK_FLAG_DIRECTORY;
    }

    if (!::CreateSymbolicLinkW(link_pth, target_pth, flgs))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}

bool touch(const path_char_t* regular_file_pth, std::error_code* err_code) noexcept
{
    HANDLE file_handle = ::CreateFileW(regular_file_pth, GENERIC_WRITE, 0, nullptr, CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handle == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    ::CloseHandle(file_handle);
    return true;
}

bool unlink(const path_char_t* regular_file_pth, std::error_code* err_code) noexcept
{
    if (is_directory(regular_file_pth, true, err_code))
    {
        return rmdir(regular_file_pth, err_code);
    }
    
    if (!::DeleteFileW(regular_file_pth))
    {
        system::errors::assign_system_error_code((int)::GetLastError(), err_code);
        return false;
    }

    return true;
}

}

#endif

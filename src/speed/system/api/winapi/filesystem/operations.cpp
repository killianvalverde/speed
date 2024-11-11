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
 * @file        speed/system/api/winapi/filesystem/operations.cpp
 * @brief       filesystem operations source.
 * @author      Killian Valverde
 * @date        2017/05/26
 */

#include "../../../compatibility/compatibility.hpp"
#ifdef SPEED_WINAPI

#include <accctrl.h>
#include <aclapi.h>
#include <io.h>
#include <sddl.h>
#include <shlguid.h>
#include <shobjidl_core.h>

#include <cstdlib>
#include <memory>

#include "../../../../cryptography/cryptography.hpp"
#include "operations.hpp"


namespace speed::system::api::winapi::filesystem {


bool access(
        const char* file_pth,
        system::filesystem::access_modes access_mods,
        std::error_code* err_code
) noexcept
{
    DWORD access_desird = 0;
    PACL p_dacl = nullptr;
    PSECURITY_DESCRIPTOR p_security_descriptr = nullptr;
    HANDLE tokn = nullptr;
    HANDLE impersonation_tokn = nullptr;
    PRIVILEGE_SET privilege_st;
    DWORD privilege_st_sz = sizeof(PRIVILEGE_SET);
    DWORD granted_accss = 0;
    BOOL access_sttus = false;
    GENERIC_MAPPING generic_mappng = {FILE_GENERIC_READ, FILE_GENERIC_WRITE,
                                      FILE_GENERIC_EXECUTE, FILE_ALL_ACCESS };

    if (access_mods == system::filesystem::access_modes::NIL)
    {
        return true;
    }
    if (access_mods == system::filesystem::access_modes::EXISTS)
    {
        return ::GetFileAttributesA(file_pth) != INVALID_FILE_ATTRIBUTES;
    }
    if ((access_mods & system::filesystem::access_modes::READ) !=
                system::filesystem::access_modes::NIL)
    {
        access_desird |= GENERIC_READ;
    }
    if ((access_mods & system::filesystem::access_modes::WRITE) !=
                system::filesystem::access_modes::NIL)
    {
        access_desird |= GENERIC_WRITE;
    }
    if ((access_mods & system::filesystem::access_modes::EXECUTE) !=
                system::filesystem::access_modes::NIL)
    {
        access_desird |= GENERIC_EXECUTE;
    }

    if (::GetNamedSecurityInfoA(file_pth, SE_FILE_OBJECT,
                DACL_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION,
                nullptr, nullptr, &p_dacl, nullptr, &p_security_descriptr) != ERROR_SUCCESS ||
        !::OpenProcessToken(GetCurrentProcess(),
                TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_IMPERSONATE, &tokn) ||
        !::DuplicateToken(tokn, SecurityImpersonation, &impersonation_tokn))
    {
        goto exit_with_error;
    }

    ::MapGenericMask(&access_desird, &generic_mappng);

    if (!::AccessCheck(p_security_descriptr, impersonation_tokn, access_desird, &generic_mappng,
                       &privilege_st, &privilege_st_sz, &granted_accss, &access_sttus))
    {
        goto exit_with_error;
    }

    goto exit;

exit_with_error:
    system::errors::assign_system_error_code((int)GetLastError(), err_code);

exit:
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

    return (bool)access_sttus;
}


bool access(
        const wchar_t* file_pth,
        system::filesystem::access_modes access_mods,
        std::error_code* err_code
) noexcept
{
    DWORD access_desird = 0;
    PACL p_dacl = nullptr;
    PSECURITY_DESCRIPTOR p_security_descriptr = nullptr;
    HANDLE tokn = nullptr;
    HANDLE impersonation_tokn = nullptr;
    PRIVILEGE_SET privilege_st;
    DWORD privilege_st_sz = sizeof(PRIVILEGE_SET);
    DWORD granted_accss = 0;
    BOOL access_sttus = false;
    GENERIC_MAPPING generic_mappng = {FILE_GENERIC_READ, FILE_GENERIC_WRITE,
                                      FILE_GENERIC_EXECUTE, FILE_ALL_ACCESS };

    if (access_mods == system::filesystem::access_modes::EXISTS)
    {
        return GetFileAttributesW(file_pth) != INVALID_FILE_ATTRIBUTES;
    }
    if ((access_mods & system::filesystem::access_modes::READ) !=
                system::filesystem::access_modes::NIL)
    {
        access_desird |= GENERIC_READ;
    }
    if ((access_mods & system::filesystem::access_modes::WRITE) !=
                system::filesystem::access_modes::NIL)
    {
        access_desird |= GENERIC_WRITE;
    }
    if ((access_mods & system::filesystem::access_modes::EXECUTE) !=
                system::filesystem::access_modes::NIL)
    {
        access_desird |= GENERIC_EXECUTE;
    }

    if (::GetNamedSecurityInfoW(file_pth, SE_FILE_OBJECT,
                DACL_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION,
                nullptr, nullptr, &p_dacl, nullptr, &p_security_descriptr) != ERROR_SUCCESS ||
        !::OpenProcessToken(GetCurrentProcess(),
                TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_IMPERSONATE, &tokn) ||
        !::DuplicateToken(tokn, SecurityImpersonation, &impersonation_tokn))
    {
        goto exit_with_error;
    }

    ::MapGenericMask(&access_desird, &generic_mappng);

    if (!AccessCheck(p_security_descriptr, impersonation_tokn, access_desird, &generic_mappng,
                     &privilege_st, &privilege_st_sz, &granted_accss, &access_sttus))
    {
        goto exit_with_error;
    }

    goto exit;

exit_with_error:
    system::errors::assign_system_error_code((int)GetLastError(), err_code);

exit:
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

    return (bool)access_sttus;
}


bool access(
        const char* file_pth,
        system::filesystem::access_modes access_mods,
        system::filesystem::file_types file_typs,
        std::error_code* err_code
) noexcept
{
    return (is_file_type(file_pth, file_typs, err_code) &&
            access(file_pth, access_mods, err_code));
}


bool access(
        const wchar_t* file_pth,
        system::filesystem::access_modes access_mods,
        system::filesystem::file_types file_typs,
        std::error_code* err_code
) noexcept
{
    return (is_file_type(file_pth, file_typs, err_code) &&
            access(file_pth, access_mods, err_code));
}


bool can_directory_be_created(const char* directory_pth, std::error_code* err_code) noexcept
{
    char parent_pth[MAX_PATH] = {0};
    std::size_t directory_path_len = stringutils::strlen(directory_pth);
    char* last_char_p;

    if (directory_path_len >= MAX_PATH ||
        directory_path_len == 0 ||
        file_exists(directory_pth, err_code))
    {
        return false;
    }

    speed::stringutils::strcpy(parent_pth, directory_pth);
    speed::stringutils::strdisclastif(parent_pth, '\\');
    last_char_p = speed::stringutils::strcut(parent_pth, '\\');
    directory_path_len = last_char_p == nullptr ? 0 : parent_pth - last_char_p + 1;

    if (directory_path_len == 0)
    {
        parent_pth[0] = '.';
        parent_pth[1] = '\0';
    }

    return access(parent_pth, system::filesystem::access_modes::WRITE |
            system::filesystem::access_modes::EXECUTE, err_code);
}


bool can_directory_be_created(const wchar_t* directory_pth, std::error_code* err_code) noexcept
{
    wchar_t parent_pth[MAX_PATH] = {0};
    std::size_t directory_path_len = stringutils::strlen(directory_pth);
    wchar_t* p_last_char;

    if (directory_path_len >= MAX_PATH ||
        directory_path_len == 0 ||
        file_exists(directory_pth, err_code))
    {
        return false;
    }

    speed::stringutils::strcpy(parent_pth, directory_pth);
    speed::stringutils::strdisclastif(parent_pth, L'\\');
    p_last_char = speed::stringutils::strcut(parent_pth, L'\\');
    directory_path_len = p_last_char == nullptr ? 0 : parent_pth - p_last_char + 1;

    if (directory_path_len == 0)
    {
        parent_pth[0] = L'.';
        parent_pth[1] = L'\0';
    }

    return access(parent_pth, system::filesystem::access_modes::WRITE |
            system::filesystem::access_modes::EXECUTE, err_code);
}


bool can_regular_file_be_created(const char* regular_file_pth, std::error_code* err_code) noexcept
{
    char parent_pth[MAX_PATH] = {0};
    std::size_t path_len = stringutils::strlen(regular_file_pth);
    char* p_last_char;

    if (path_len >= MAX_PATH || path_len == 0)
    {
        return false;
    }

    if (file_exists(regular_file_pth, err_code))
    {
        return access(regular_file_pth, system::filesystem::access_modes::WRITE,
                      system::filesystem::file_types::REGULAR_FILE, err_code);
    }

    speed::stringutils::strcpy(parent_pth, regular_file_pth);
    speed::stringutils::strdisclastif(parent_pth, '\\');
    p_last_char = speed::stringutils::strcut(parent_pth, '\\');
    path_len = p_last_char == nullptr ? 0 : parent_pth - p_last_char + 1;

    if (path_len == 0)
    {
        parent_pth[0] = '.';
        parent_pth[1] = '\0';
    }

    return access(parent_pth, system::filesystem::access_modes::WRITE |
            system::filesystem::access_modes::EXECUTE, err_code);
}


bool can_regular_file_be_created(const wchar_t* regular_file_pth, std::error_code* err_code) noexcept
{
    wchar_t parent_pth[MAX_PATH] = {0};
    std::size_t path_len = stringutils::strlen(regular_file_pth);
    wchar_t* p_last_char;

    if (path_len >= MAX_PATH || path_len == 0)
    {
        return false;
    }

    if (file_exists(regular_file_pth, err_code))
    {
        return access(regular_file_pth, system::filesystem::access_modes::WRITE,
                      system::filesystem::file_types::REGULAR_FILE, err_code);
    }

    speed::stringutils::strcpy(parent_pth, regular_file_pth);
    speed::stringutils::strdisclastif(parent_pth, L'\\');
    p_last_char = speed::stringutils::strcut(parent_pth, L'\\');
    path_len = p_last_char == nullptr ? 0 : parent_pth - p_last_char + 1;

    if (path_len == 0)
    {
        parent_pth[0] = L'.';
        parent_pth[1] = L'\0';
    }

    return access(parent_pth, system::filesystem::access_modes::WRITE |
            system::filesystem::access_modes::EXECUTE, err_code);
}


bool chdir(const char* directory_pth, std::error_code* err_code) noexcept
{
    if (!::SetCurrentDirectoryA(directory_pth))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}


bool chdir(const wchar_t* directory_pth, std::error_code* err_code) noexcept
{
    if (!::SetCurrentDirectoryW(directory_pth))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}


bool closedir(
        system::filesystem::directory_entity* directory_ent,
        std::error_code* err_code
) noexcept
{
    auto* directory_ent_ext = &directory_ent->__priv;
    
    if (!::FindClose(directory_ent_ext->dir_handl))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }
    
    return true;
}


bool closedir(
        system::filesystem::wdirectory_entity* directory_ent,
        std::error_code* err_code
) noexcept
{
    auto* directory_ent_ext = &directory_ent->__priv;
    
    if (!::FindClose(directory_ent_ext->dir_handl))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }
    
    return true;
}


bool file_exists(const char* file_pth, std::error_code* err_code) noexcept
{
    return access(file_pth, system::filesystem::access_modes::EXISTS, err_code);
}


bool file_exists(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    return access(file_pth, system::filesystem::access_modes::EXISTS, err_code);
}


system::filesystem::inode_t get_file_inode(const char* file_pth, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    BY_HANDLE_FILE_INFORMATION file_info;

    file_handl = ::CreateFileA(file_pth, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
                               nullptr);
    
    if (file_handl == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
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


system::filesystem::inode_t get_file_inode(
        const wchar_t* file_pth,
        std::error_code* err_code
) noexcept
{
    HANDLE file_handl;
    BY_HANDLE_FILE_INFORMATION file_info;

    file_handl = ::CreateFileW(file_pth, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
                               nullptr);

    if (file_handl == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
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


system::filesystem::inode_t get_file_inode(
        system::filesystem::directory_entity* directory_ent,
        std::error_code* err_code
) noexcept
{
    auto* directory_ent_ext = &directory_ent->__priv;
    char search_pth[MAX_PATH];
    errno_t std_err;
    
    if ((std_err = ::strcpy_s(search_pth, MAX_PATH, directory_ent_ext->pth)) != 0 ||
        (std_err = ::strcat_s(search_pth, MAX_PATH, directory_ent->nme)) != 0)
    {
        system::errors::assign_system_error_code((int)std_err, err_code);
        return false;
    }
    
    return get_file_inode(search_pth, err_code);
}


system::filesystem::inode_t get_file_inode(
        system::filesystem::wdirectory_entity* directory_ent,
        std::error_code* err_code
) noexcept
{
    auto* directory_ent_ext = &directory_ent->__priv;
    wchar_t search_pth[MAX_PATH];
    errno_t std_err;
    
    if ((std_err = ::wcscpy_s(search_pth, MAX_PATH, directory_ent_ext->pth)) != 0 ||
        (std_err = ::wcscat_s(search_pth, MAX_PATH, directory_ent->nme)) != 0)
    {
        system::errors::assign_system_error_code((int)std_err, err_code);
        return false;
    }
    
    return get_file_inode(search_pth, err_code);
}


system::process::uid_t get_file_uid(const char* file_pth, std::error_code* err_code) noexcept
{
    PSID owner_sid;
    PSECURITY_DESCRIPTOR sec_desc;
    char* sid_cstr;
    system::process::uid_t uid;

    if (::GetNamedSecurityInfoA(file_pth, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &owner_sid,
                                nullptr, nullptr, nullptr, &sec_desc) != ERROR_SUCCESS ||
        !::ConvertSidToStringSidA(owner_sid, &sid_cstr))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        if (sec_desc != nullptr)
        {
            ::LocalFree(sec_desc);
        }
        return ~0ull;
    }
    
    uid = speed::cryptography::city_hash_64((const char*)sid_cstr);

    ::LocalFree(sid_cstr);
    ::LocalFree(sec_desc);

    return uid;
}


system::process::uid_t get_file_uid(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    PSID owner_sid;
    PSECURITY_DESCRIPTOR sec_desc;
    char* sid_cstr;
    system::process::uid_t uid;

    if (::GetNamedSecurityInfoW(file_pth, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &owner_sid,
                                nullptr, nullptr, nullptr, &sec_desc) != ERROR_SUCCESS ||
        !::ConvertSidToStringSidA(owner_sid, &sid_cstr))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        if (sec_desc != nullptr)
        {
            ::LocalFree(sec_desc);
        }
        return ~0ull;
    }

    uid = speed::cryptography::city_hash_64((const char*)sid_cstr);

    ::LocalFree(sid_cstr);
    ::LocalFree(sec_desc);

    return uid;
}


system::process::gid_t get_file_gid(const char* file_pth, std::error_code* err_code) noexcept
{
    PSECURITY_DESCRIPTOR security_desc;
    DWORD security_desc_sz;
    PSID group_sid = nullptr;
    BOOL group_defaultd = FALSE;
    LPSTR sid_cstr;
    system::process::gid_t gid = ~0ull;

    if (!::GetFileSecurityA(file_pth, GROUP_SECURITY_INFORMATION, nullptr, 0, &security_desc_sz) &&
        GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return ~0ull;
    }

    security_desc = (PSECURITY_DESCRIPTOR)malloc(security_desc_sz);
    if (!security_desc)
    {
        system::errors::assign_system_error_code(ERROR_NOT_ENOUGH_MEMORY, err_code);
        return ~0ull;
    }

    if (!::GetFileSecurityA(file_pth, GROUP_SECURITY_INFORMATION, security_desc, security_desc_sz,
                            &security_desc_sz) ||
        !::GetSecurityDescriptorGroup(security_desc, &group_sid, &group_defaultd))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        free(security_desc);
        return ~0ull;
    }

    if (::ConvertSidToStringSidA(group_sid, &sid_cstr))
    {
        gid = speed::cryptography::city_hash_64((const char*)sid_cstr);
        ::LocalFree(sid_cstr);
    }
    else
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
    }

    free(security_desc);
    return gid;
}


system::process::gid_t get_file_gid(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    PSECURITY_DESCRIPTOR security_desc;
    DWORD security_desc_sz;
    PSID group_sid = nullptr;
    BOOL group_defaultd = FALSE;
    LPSTR sid_cstr;
    system::process::gid_t gid = ~0ull;

    if (!::GetFileSecurityW(file_pth, GROUP_SECURITY_INFORMATION, nullptr, 0, &security_desc_sz) &&
        GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return ~0ull;
    }

    security_desc = (PSECURITY_DESCRIPTOR)malloc(security_desc_sz);
    if (!security_desc)
    {
        system::errors::assign_system_error_code(ERROR_NOT_ENOUGH_MEMORY, err_code);
        return ~0ull;
    }

    if (!::GetFileSecurityW(file_pth, GROUP_SECURITY_INFORMATION, security_desc, security_desc_sz,
                            &security_desc_sz) ||
        !::GetSecurityDescriptorGroup(security_desc, &group_sid, &group_defaultd))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        free(security_desc);
        return ~0ull;
    }

    if (::ConvertSidToStringSidA(group_sid, &sid_cstr))
    {
        gid = speed::cryptography::city_hash_64((const char*)sid_cstr);
        ::LocalFree(sid_cstr);
    }
    else
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
    }

    free(security_desc);
    return gid;
}


std::size_t get_file_size(const char* file_pth, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    DWORD file_sz;
    
    file_handl = ::CreateFileA(file_pth, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    
    if (file_handl == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return ~0ull;
    }

    file_sz = ::GetFileSize(file_handl, nullptr);
    if (file_sz == INVALID_FILE_SIZE)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        ::CloseHandle(file_handl);
        return ~0ull;
    }
    
    ::CloseHandle(file_handl);
    return file_sz;
}


std::size_t get_file_size(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    DWORD file_sz;
    
    file_handl = ::CreateFileW(file_pth, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    
    if (file_handl == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return ~0ull;
    }

    file_sz = ::GetFileSize(file_handl, nullptr);
    if (file_sz == INVALID_FILE_SIZE)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        ::CloseHandle(file_handl);
        return ~0ull;
    }
    
    ::CloseHandle(file_handl);
    return file_sz;
}


bool get_modification_time(
        const char* file_pth,
        system::time::system_time* system_tme,
        std::error_code* err_code
) noexcept
{
    HANDLE handl;
    FILETIME last_write_tme;
    SYSTEMTIME utc_system_tme;
    SYSTEMTIME local_system_tme;
    
    handl = ::CreateFileA(file_pth, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                          OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
                          nullptr);
    
    if (handl == INVALID_HANDLE_VALUE ||
        ::GetFileTime(handl, nullptr, nullptr, &last_write_tme) == 0 ||
        ::FileTimeToSystemTime(&last_write_tme, &utc_system_tme) == 0 ||
        ::SystemTimeToTzSpecificLocalTime(nullptr, &utc_system_tme, &local_system_tme) == 0)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }
    
    system_tme->set_years(local_system_tme.wYear)
               .set_months(local_system_tme.wMonth)
               .set_days(local_system_tme.wDay)
               .set_hours(local_system_tme.wHour)
               .set_minutes(local_system_tme.wMinute)
               .set_seconds(local_system_tme.wSecond);
    
    ::CloseHandle(handl);
    
    return true;
}


bool get_modification_time(
        const wchar_t* file_pth,
        system::time::system_time* system_tme,
        std::error_code* err_code
) noexcept
{
    HANDLE handl;
    FILETIME last_write_tme;
    SYSTEMTIME utc_system_tme;
    SYSTEMTIME local_system_tme;
    
    handl = ::CreateFileW(file_pth, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                          OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
                          nullptr);

    if (handl == INVALID_HANDLE_VALUE ||
        ::GetFileTime(handl, nullptr, nullptr, &last_write_tme) == 0 ||
        ::FileTimeToSystemTime(&last_write_tme, &utc_system_tme) == 0 ||
        ::SystemTimeToTzSpecificLocalTime(nullptr, &utc_system_tme, &local_system_tme) == 0)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }
    
    system_tme->set_years(local_system_tme.wYear)
               .set_months(local_system_tme.wMonth)
               .set_days(local_system_tme.wDay)
               .set_hours(local_system_tme.wHour)
               .set_minutes(local_system_tme.wMinute)
               .set_seconds(local_system_tme.wSecond);
    
    CloseHandle(handl);
    
    return true;
}


const char* get_temporal_path() noexcept
{
    static char temp_pth[MAX_PATH];
    DWORD len = ::GetTempPathA(MAX_PATH, temp_pth);

    if (len == 0 || len > MAX_PATH)
    {
        return nullptr;
    }

    return temp_pth;
}


bool is_block_device(const char* file_pth, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    DWORD file_typ;

    file_handl = ::CreateFileA(file_pth, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handl == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    file_typ = ::GetFileType(file_handl);
    ::CloseHandle(file_handl);

    return file_typ == FILE_TYPE_DISK;
}


bool is_block_device(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    DWORD file_typ;

    file_handl = ::CreateFileW(file_pth, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handl == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    file_typ = ::GetFileType(file_handl);
    ::CloseHandle(file_handl);

    return file_typ == FILE_TYPE_DISK;
}


bool is_character_device(const char* file_pth, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    DWORD file_typ;

    file_handl = ::CreateFileA(file_pth, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handl == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    file_typ = ::GetFileType(file_handl);
    ::CloseHandle(file_handl);

    return file_typ == FILE_TYPE_CHAR;
}


bool is_character_device(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    DWORD file_typ;

    file_handl = ::CreateFileW(file_pth, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handl == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    file_typ = ::GetFileType(file_handl);
    ::CloseHandle(file_handl);

    return file_typ == FILE_TYPE_CHAR;
}


bool is_directory(const char* file_pth, std::error_code* err_code) noexcept
{
    DWORD attr = ::GetFileAttributesA(file_pth);

    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}


bool is_directory(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    DWORD attr = ::GetFileAttributesW(file_pth);

    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}


bool is_file_type(
        const char* file_pth,
        system::filesystem::file_types fle_type,
        std::error_code* err_code
) noexcept
{
    if ((fle_type & system::filesystem::file_types::BLOCK_DEVICE) !=
                system::filesystem::file_types::NIL && is_block_device(file_pth, err_code))
    {
        return true;
    }
    if ((fle_type & system::filesystem::file_types::CHARACTER_DEVICE) !=
                system::filesystem::file_types::NIL && is_character_device(file_pth, err_code))
    {
        return true;
    }
    if ((fle_type & system::filesystem::file_types::DIRECTORY) !=
                system::filesystem::file_types::NIL && is_directory(file_pth, err_code))
    {
        return true;
    }
    if ((fle_type & system::filesystem::file_types::PIPE) !=
                system::filesystem::file_types::NIL && is_pipe(file_pth, err_code))
    {
        return true;
    }
    if ((fle_type & system::filesystem::file_types::REGULAR_FILE) !=
                system::filesystem::file_types::NIL && is_regular_file(file_pth, err_code))
    {
        return true;
    }
    if ((fle_type & system::filesystem::file_types::SOCKET) !=
                system::filesystem::file_types::NIL && is_socket(file_pth, err_code))
    {
        return true;
    }
    if ((fle_type & system::filesystem::file_types::SYMLINK) !=
                system::filesystem::file_types::NIL && is_symlink(file_pth, err_code))
    {
        return true;
    }

    return false;
}


bool is_file_type(
        const wchar_t* file_pth,
        system::filesystem::file_types fle_type,
        std::error_code* err_code
) noexcept
{
    if ((fle_type & system::filesystem::file_types::BLOCK_DEVICE) !=
                system::filesystem::file_types::NIL && is_block_device(file_pth, err_code))
    {
        return true;
    }
    if ((fle_type & system::filesystem::file_types::CHARACTER_DEVICE) !=
                system::filesystem::file_types::NIL && is_character_device(file_pth, err_code))
    {
        return true;
    }
    if ((fle_type & system::filesystem::file_types::DIRECTORY) !=
                system::filesystem::file_types::NIL && is_directory(file_pth, err_code))
    {
        return true;
    }
    if ((fle_type & system::filesystem::file_types::PIPE) !=
                system::filesystem::file_types::NIL && is_pipe(file_pth, err_code))
    {
        return true;
    }
    if ((fle_type & system::filesystem::file_types::REGULAR_FILE) !=
                system::filesystem::file_types::NIL && is_regular_file(file_pth, err_code))
    {
        return true;
    }
    if ((fle_type & system::filesystem::file_types::SOCKET) !=
                system::filesystem::file_types::NIL && is_socket(file_pth, err_code))
    {
        return true;
    }
    if ((fle_type & system::filesystem::file_types::SYMLINK) !=
                system::filesystem::file_types::NIL && is_symlink(file_pth, err_code))
    {
        return true;
    }

    return false;
}


bool is_pipe(const char* file_pth, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    DWORD file_typ;

    file_handl = ::CreateFileA(file_pth, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handl == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    file_typ = ::GetFileType(file_handl);
    ::CloseHandle(file_handl);

    return file_typ == FILE_TYPE_PIPE;
}


bool is_pipe(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    DWORD file_typ;

    file_handl = ::CreateFileW(file_pth, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handl == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    file_typ = ::GetFileType(file_handl);
    ::CloseHandle(file_handl);

    return file_typ == FILE_TYPE_PIPE;
}


bool is_regular_file(const char* file_pth, std::error_code* err_code) noexcept
{
    DWORD attr = ::GetFileAttributesA(file_pth);

    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return (attr & FILE_ATTRIBUTE_DIRECTORY) == 0;
}


bool is_regular_file(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    DWORD attr = ::GetFileAttributesW(file_pth);

    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return (attr & FILE_ATTRIBUTE_DIRECTORY) == 0;
}


bool is_socket(const char* file_pth, std::error_code* err_code) noexcept
{
    // TODO: This function requires a header that make problems.
    // #include <winsock2.h>
    // HANDLE file_handl;
    // WSANETWORKEVENTS evnts;
    // int sockt;
    //
    // file_handl = CreateFileA(fle_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
    //                          OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    // if (file_handl == INVALID_HANDLE_VALUE)
    // {
    //     assign_system_error_code((int)GetLastError(), err_code);
    //     return false;
    // }
    //
    // sockt = _open_osfhandle((intptr_t)file_handl, 0);
    // if (WSAEnumNetworkEvents((SOCKET)sockt, nullptr, &evnts) == SOCKET_ERROR)
    // {
    //     assign_system_error_code(WSAGetLastError(), err_code);
    //     CloseHandle(file_handl);
    //     return false;
    // }
    //
    // CloseHandle(file_handl);
    // return true;

    return false;
}


bool is_socket(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    // TODO: This function requires a header that make problems.
    // #include <winsock2.h>
    // HANDLE file_handl;
    // WSANETWORKEVENTS evnts;
    // int sockt;
    //
    // file_handl = CreateFileW(fle_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
    //                          OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    // if (file_handl == INVALID_HANDLE_VALUE)
    // {
    //     assign_system_error_code((int)GetLastError(), err_code);
    //     return false;
    // }
    //
    // sockt = _open_osfhandle((intptr_t)file_handl, 0);
    // if (WSAEnumNetworkEvents((SOCKET)sockt, nullptr, &evnts) == SOCKET_ERROR)
    // {
    //     assign_system_error_code(WSAGetLastError(), err_code);
    //     CloseHandle(file_handl);
    //     return false;
    // }
    //
    // CloseHandle(file_handl);
    // return true;

    return false;
}


bool is_symlink(const char* file_pth, std::error_code* err_code) noexcept
{
    WIN32_FIND_DATA find_dat;
    DWORD file_attrs;
    HANDLE file_handl;

    file_attrs = ::GetFileAttributesA(file_pth);
    if (file_attrs == INVALID_FILE_ATTRIBUTES)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    if (file_attrs & FILE_ATTRIBUTE_REPARSE_POINT)
    {
        file_handl = ::FindFirstFileA(file_pth, &find_dat);
        if (file_handl != INVALID_HANDLE_VALUE)
        {
            ::FindClose(file_handl);

            return find_dat.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT &&
                   find_dat.dwReserved0 == IO_REPARSE_TAG_SYMLINK;
        }
    }

    return false;
}


bool is_symlink(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    WIN32_FIND_DATAW find_dat;
    DWORD file_attrs;
    HANDLE file_handl;

    file_attrs = ::GetFileAttributesW(file_pth);
    if (file_attrs == INVALID_FILE_ATTRIBUTES)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    if (file_attrs & FILE_ATTRIBUTE_REPARSE_POINT)
    {
        file_handl = ::FindFirstFileW(file_pth, &find_dat);
        if (file_handl != INVALID_HANDLE_VALUE)
        {
            ::FindClose(file_handl);

            return find_dat.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT &&
                   find_dat.dwReserved0 == IO_REPARSE_TAG_SYMLINK;
        }
    }

    return false;
}


bool mkdir(const char* directory_pth, std::error_code* err_code) noexcept
{
    if (!::CreateDirectoryA(directory_pth, nullptr))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}


bool mkdir(const wchar_t* directory_pth, std::error_code* err_code) noexcept
{
    if (!::CreateDirectoryW(directory_pth, nullptr))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}


bool mkdir_recursively(
        const char* directory_pth,
        std::error_code* err_code
) noexcept
{
    char parnt_path[MAX_PATH] = {0};
    std::size_t pth_len;
    size_t slash_pos[MAX_PATH];
    size_t slash_pos_sz = 0;
    char* last_chr;

    pth_len = stringutils::strlen(directory_pth);

    if (pth_len >= MAX_PATH ||
        pth_len == 0 ||
        access(directory_pth, system::filesystem::access_modes::EXISTS, err_code))
    {
        return false;
    }

    stringutils::strcpy(parnt_path, directory_pth);
    stringutils::strdisclastif(parnt_path, '\\');

    do
    {
        last_chr = stringutils::strcut(parnt_path, '\\', true);

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

        if (pth_len > 0 && parnt_path[pth_len - 1] == ':')
        {
            pth_len = 0;
        }

    } while (!access(parnt_path, system::filesystem::access_modes::EXISTS, err_code) &&
             pth_len > 0);

    while (slash_pos_sz > 0)
    {
        parnt_path[slash_pos[--slash_pos_sz]] = '\\';

         if (!mkdir(parnt_path, err_code))
         {
             return false;
         }
    }

    return true;
}


bool mkdir_recursively(
        const wchar_t* directory_pth,
        std::error_code* err_code
) noexcept
{
    wchar_t parnt_path[MAX_PATH] = {0};
    std::size_t pth_len;
    size_t slash_pos[MAX_PATH];
    size_t slash_pos_sz = 0;
    wchar_t* last_chr;

    pth_len = stringutils::strlen(directory_pth);

    if (pth_len >= MAX_PATH ||
        pth_len == 0 ||
        access(directory_pth, system::filesystem::access_modes::EXISTS, err_code))
    {
        return false;
    }

    stringutils::strcpy(parnt_path, directory_pth);
    stringutils::strdisclastif(parnt_path, L'\\');

    do
    {
        last_chr = stringutils::strcut(parnt_path, L'\\', true);

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

    } while (!access(parnt_path, system::filesystem::access_modes::EXISTS, err_code) &&
             pth_len > 0);

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
        system::filesystem::directory_entity* directory_ent,
        const char* directory_pth,
        std::error_code* err_code
) noexcept
{
    std::size_t directory_pth_len;
    bool slash_insertd = false;
    auto* directory_ent_ext = &directory_ent->__priv;
    
    directory_pth_len = speed::stringutils::strlen(directory_pth);
    if (directory_pth_len >= MAX_PATH - 3)
    {
        system::errors::assign_system_error_code(ERANGE, err_code);
        return false;
    }
    
    speed::stringutils::strcpy(directory_ent_ext->pth, directory_pth);
    if (directory_pth_len < 1 || directory_pth[directory_pth_len - 1] != '\\')
    {
        speed::stringutils::strcat(directory_ent_ext->pth, "\\*");
        slash_insertd = true;
    }
    else
    {
        speed::stringutils::strcat(directory_ent_ext->pth, "*");
    }
    
    directory_ent_ext->dir_handl = ::FindFirstFileA(
            directory_ent_ext->pth, &directory_ent_ext->find_dat);
    
    if (slash_insertd)
    {
        directory_ent_ext->pth[directory_pth_len + 1] = '\0';
    }
    else
    {
        directory_ent_ext->pth[directory_pth_len] = '\0';
    }
    
    if (directory_ent_ext->dir_handl == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int) GetLastError(), err_code);
        return false;
    }
    
    directory_ent_ext->read_dne = false;
    
    return true;
}


bool opendir(
        system::filesystem::wdirectory_entity* directory_ent,
        const wchar_t* directory_pth,
        std::error_code* err_code
) noexcept
{
    std::size_t directory_pth_len;
    bool slash_insertd = false;
    auto* directory_ent_ext = &directory_ent->__priv;
    
    directory_pth_len = speed::stringutils::strlen(directory_pth);
    if (directory_pth_len >= MAX_PATH - 3)
    {
        system::errors::assign_system_error_code(ERANGE, err_code);
        return false;
    }
    
    speed::stringutils::strcpy(directory_ent_ext->pth, directory_pth);
    if (directory_pth_len < 1 || directory_pth[directory_pth_len - 1] != '\\')
    {
        speed::stringutils::strcat(directory_ent_ext->pth, "\\*");
        slash_insertd = true;
    }
    else
    {
        speed::stringutils::strcat(directory_ent_ext->pth, "*");
    }
    
    directory_ent_ext->dir_handl = ::FindFirstFileW(
            directory_ent_ext->pth, &directory_ent_ext->find_dat);
    
    if (slash_insertd)
    {
        directory_ent_ext->pth[directory_pth_len + 1] = '\0';
    }
    else
    {
        directory_ent_ext->pth[directory_pth_len] = '\0';
    }
    
    if (directory_ent_ext->dir_handl == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int) GetLastError(), err_code);
        return false;
    }
    
    directory_ent_ext->read_dne = false;
    
    return true;
}


bool readdir(
        system::filesystem::directory_entity* directory_ent,
        std::error_code* err_code
) noexcept
{
    auto* directory_ent_ext = &directory_ent->__priv;
    DWORD last_err;

    if (directory_ent_ext->read_dne &&
        !::FindNextFileA(directory_ent_ext->dir_handl, &directory_ent_ext->find_dat))
    {
        last_err = ::GetLastError();
        if (last_err != ERROR_NO_MORE_FILES)
        {
            system::errors::assign_system_error_code((int)last_err, err_code);
        }

        return false;
    }

    directory_ent_ext->read_dne = true;
    directory_ent->nme = directory_ent_ext->find_dat.cFileName;
    
    return true;
}


bool readdir(
        system::filesystem::wdirectory_entity* directory_ent,
        std::error_code* err_code
) noexcept
{
    auto* directory_ent_ext = &directory_ent->__priv;
    DWORD last_err;

    if (directory_ent_ext->read_dne &&
        !::FindNextFileW(directory_ent_ext->dir_handl, &directory_ent_ext->find_dat))
    {
        last_err = ::GetLastError();
        if (last_err != ERROR_NO_MORE_FILES)
        {
            system::errors::assign_system_error_code((int)last_err, err_code);
        }

        return false;
    }

    directory_ent_ext->read_dne = true;
    directory_ent->nme = directory_ent_ext->find_dat.cFileName;

    return true;
}


bool rmdir(const char* directory_pth, std::error_code* err_code) noexcept
{
    ::SetFileAttributesA(directory_pth, 0x80);
    if (!::RemoveDirectoryA(directory_pth))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}


bool rmdir(const wchar_t* directory_pth, std::error_code* err_code) noexcept
{
    ::SetFileAttributesW(directory_pth, 0x80);
    if (!::RemoveDirectoryW(directory_pth))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}


bool shortcut(
        const char* target_pth,
        const char* shortcut_pth,
        std::error_code* err_code
) noexcept
{
    HRESULT res = -1;
    IShellLinkA* shell_lnk = nullptr;
    IPersistFile* persist_fle = nullptr;
    char absolute_target_pth[MAX_PATH] = {};
    wchar_t wshortcut_pth[MAX_PATH] = {};
    std::size_t converted_chars = 0;
    errno_t err;

    if (!::GetFullPathNameA(target_pth, MAX_PATH, absolute_target_pth, nullptr))
    {
        system::errors::assign_system_error_code(ERROR_BAD_PATHNAME, err_code);
        goto exit;
    }

    ::CoInitialize(nullptr);
    res = ::CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLink,
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

    err = mbstowcs_s(&converted_chars, wshortcut_pth, MAX_PATH, shortcut_pth, _TRUNCATE);
    if (err != 0 || converted_chars == 0)
    {
        goto exit_with_error;
    }

    if (converted_chars + 4 >= MAX_PATH)
    {
        system::errors::assign_system_error_code(ERROR_BAD_PATHNAME, err_code);
        goto exit;
    }

    speed::stringutils::strcpy(wshortcut_pth + converted_chars - 1, L".lnk");
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


bool shortcut(
        const wchar_t* target_pth,
        const wchar_t* shortcut_pth,
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

    shortcut_pth_len = speed::stringutils::strlen(shortcut_pth);
    if (shortcut_pth_len + 5 >= MAX_PATH)
    {
        system::errors::assign_system_error_code(ERROR_BAD_PATHNAME, err_code);
        goto exit;
    }

    speed::stringutils::strcpy(wshortcut_pth, shortcut_pth);
    speed::stringutils::strcpy(wshortcut_pth + shortcut_pth_len, L".lnk");
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


bool symlink(const char* target_pth, const char* link_pth, std::error_code* err_code) noexcept
{
    DWORD flgs = 0;
    DWORD file_attr;

    file_attr = ::GetFileAttributesA(target_pth);
    if (file_attr == INVALID_FILE_ATTRIBUTES)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    if (file_attr & FILE_ATTRIBUTE_DIRECTORY)
    {
        flgs = SYMBOLIC_LINK_FLAG_DIRECTORY;
    }

    if (!::CreateSymbolicLinkA(link_pth, target_pth, flgs))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}


bool symlink(const wchar_t* target_pth, const wchar_t* link_pth, std::error_code* err_code) noexcept
{
    DWORD flgs = 0;
    DWORD file_attr;

    file_attr = ::GetFileAttributesW(target_pth);
    if (file_attr == INVALID_FILE_ATTRIBUTES)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    if (file_attr & FILE_ATTRIBUTE_DIRECTORY)
    {
        flgs = SYMBOLIC_LINK_FLAG_DIRECTORY;
    }

    if (!::CreateSymbolicLinkW(link_pth, target_pth, flgs))
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}


bool touch(const char* regular_file_pth, std::error_code* err_code) noexcept
{
    HANDLE file_handle = ::CreateFileA(regular_file_pth, GENERIC_WRITE, 0, nullptr, CREATE_NEW,
                                       FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handle == INVALID_HANDLE_VALUE)
    {
        system::errors::assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    ::CloseHandle(file_handle);

    return true;
}


bool touch(const wchar_t* regular_file_pth, std::error_code* err_code) noexcept
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


bool unlink(const char* regular_file_pth, std::error_code* err_code) noexcept
{
    if (!::DeleteFileA(regular_file_pth))
    {
        system::errors::assign_system_error_code((int)::GetLastError(), err_code);
        return false;
    }

    return true;
}


bool unlink(const wchar_t* regular_file_pth, std::error_code* err_code) noexcept
{
    if (!::DeleteFileW(regular_file_pth))
    {
        system::errors::assign_system_error_code((int)::GetLastError(), err_code);
        return false;
    }

    return true;
}


}


#endif

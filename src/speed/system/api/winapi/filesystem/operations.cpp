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

#include "../../../../stringutils/stringutils.hpp"
#include "directory_entity_extension.hpp"
#include "operations.hpp"


namespace speed::system::api::winapi::filesystem {


bool access(
        const char* fle_path,
        access_modes acss_modes,
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

    if (acss_modes == access_modes::NIL)
    {
        return true;
    }
    if (acss_modes == access_modes::EXISTS)
    {
        return GetFileAttributesA(fle_path) != INVALID_FILE_ATTRIBUTES;
    }
    if ((acss_modes & access_modes::READ) != access_modes::NIL)
    {
        access_desird |= GENERIC_READ;
    }
    if ((acss_modes & access_modes::WRITE) != access_modes::NIL)
    {
        access_desird |= GENERIC_WRITE;
    }
    if ((acss_modes & access_modes::EXECUTE) != access_modes::NIL)
    {
        access_desird |= GENERIC_EXECUTE;
    }

    if (::GetNamedSecurityInfoA(fle_path, SE_FILE_OBJECT,
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
    assign_system_error_code((int)GetLastError(), err_code);

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

    return access_sttus;
}


bool access(
        const wchar_t* fle_path,
        access_modes acss_modes,
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

    if (acss_modes == access_modes::EXISTS)
    {
        return GetFileAttributesW(fle_path) != INVALID_FILE_ATTRIBUTES;
    }
    if ((acss_modes & access_modes::READ) != access_modes::NIL)
    {
        access_desird |= GENERIC_READ;
    }
    if ((acss_modes & access_modes::WRITE) != access_modes::NIL)
    {
        access_desird |= GENERIC_WRITE;
    }
    if ((acss_modes & access_modes::EXECUTE) != access_modes::NIL)
    {
        access_desird |= GENERIC_EXECUTE;
    }

    if (::GetNamedSecurityInfoW(fle_path, SE_FILE_OBJECT,
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
    assign_system_error_code((int)GetLastError(), err_code);

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

    return access_sttus;
}


bool access(
        const char* fle_path,
        access_modes acss_modes,
        file_types fle_type,
        std::error_code* err_code
) noexcept
{
    return (is_file_type(fle_path, fle_type, err_code) &&
            access(fle_path, acss_modes, err_code));
}


bool access(
        const wchar_t* fle_path,
        access_modes acss_modes,
        file_types fle_type,
        std::error_code* err_code
) noexcept
{
    return (is_file_type(fle_path, fle_type, err_code) &&
            access(fle_path, acss_modes, err_code));
}


bool can_directory_be_created(const char* dir_path, std::error_code* err_code) noexcept
{
    char parent_pth[MAX_PATH] = {0};
    std::size_t dir_path_len = stringutils::strlen(dir_path);
    char* last_char_p;

    if (dir_path_len >= MAX_PATH ||
        dir_path_len == 0 ||
        access(dir_path, access_modes::EXISTS, err_code))
    {
        return false;
    }

    stringutils::strcpy(parent_pth, dir_path);
    stringutils::strdisclastif(parent_pth, '\\');
    last_char_p = stringutils::strcut(parent_pth, '\\');
    dir_path_len = last_char_p == nullptr ? 0 : parent_pth - last_char_p + 1;

    if (dir_path_len == 0)
    {
        parent_pth[0] = '.';
        parent_pth[1] = '\0';
    }

    return access(parent_pth, access_modes::WRITE | access_modes::EXECUTE, err_code);
}


bool can_directory_be_created(const wchar_t* dir_path, std::error_code* err_code) noexcept
{
    wchar_t parent_pth[MAX_PATH] = {0};
    std::size_t dir_path_len = stringutils::strlen(dir_path);
    wchar_t* last_char_p;

    if (dir_path_len >= MAX_PATH ||
        dir_path_len == 0 ||
        access(dir_path, access_modes::EXISTS, err_code))
    {
        return false;
    }

    stringutils::strcpy(parent_pth, dir_path);
    stringutils::strdisclastif(parent_pth, L'\\');
    last_char_p = stringutils::strcut(parent_pth, L'\\');
    dir_path_len = last_char_p == nullptr ? 0 : parent_pth - last_char_p + 1;

    if (dir_path_len == 0)
    {
        parent_pth[0] = L'.';
        parent_pth[1] = L'\0';
    }

    return access(parent_pth, access_modes::WRITE | access_modes::EXECUTE, err_code);
}


bool can_regular_file_be_created(const char* reg_file_path, std::error_code* err_code) noexcept
{
    char parent_pth[MAX_PATH] = {0};
    std::size_t path_len = stringutils::strlen(reg_file_path);
    char* last_char_p;

    if (path_len >= MAX_PATH || path_len == 0)
    {
        return false;
    }

    if (access(reg_file_path, access_modes::EXISTS, err_code))
    {
        return access(reg_file_path, access_modes::WRITE, file_types::REGULAR_FILE, err_code);
    }

    stringutils::strcpy(parent_pth, reg_file_path);
    stringutils::strdisclastif(parent_pth, '\\');
    last_char_p = stringutils::strcut(parent_pth, '\\');
    path_len = last_char_p == nullptr ? 0 : parent_pth - last_char_p + 1;

    if (path_len == 0)
    {
        parent_pth[0] = '.';
        parent_pth[1] = '\0';
    }

    return access(parent_pth, access_modes::WRITE | access_modes::EXECUTE, err_code);
}


bool can_regular_file_be_created(const wchar_t* reg_file_path, std::error_code* err_code) noexcept
{
    wchar_t parent_pth[MAX_PATH] = {0};
    std::size_t path_len = stringutils::strlen(reg_file_path);
    wchar_t* last_char_p;

    if (path_len >= MAX_PATH || path_len == 0)
    {
        return false;
    }

    if (access(reg_file_path, access_modes::EXISTS, err_code))
    {
        return access(reg_file_path, access_modes::WRITE, file_types::REGULAR_FILE, err_code);
    }

    stringutils::strcpy(parent_pth, reg_file_path);
    stringutils::strdisclastif(parent_pth, L'\\');
    last_char_p = stringutils::strcut(parent_pth, L'\\');
    path_len = last_char_p == nullptr ? 0 : parent_pth - last_char_p + 1;

    if (path_len == 0)
    {
        parent_pth[0] = L'.';
        parent_pth[1] = L'\0';
    }

    return access(parent_pth, access_modes::WRITE | access_modes::EXECUTE, err_code);
}


bool chdir(const char* dir_path, std::error_code* err_code) noexcept
{
    if (!::SetCurrentDirectoryA(dir_path))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}


bool chdir(const wchar_t* dir_path, std::error_code* err_code) noexcept
{
    if (!::SetCurrentDirectoryW(dir_path))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}


bool closedir(directory_entity* dir_ent, std::error_code* err_code) noexcept
{
    bool success = false;
    auto* dir_ent_ext = (directory_entity_extension*)dir_ent->ext;

    if (dir_ent_ext != nullptr)
    {
        if (!::FindClose(dir_ent_ext->dir_handl))
        {
            assign_system_error_code((int)GetLastError(), err_code);
        }
        else
        {
            success = true;
        }

        dir_ent_ext->~directory_entity_extension();
        free(dir_ent_ext);
        dir_ent->ext = nullptr;
    }
    else
    {
        assign_system_error_code(EINVAL, err_code);
    }

    return success;
}


bool closedir(wdirectory_entity* dir_ent, std::error_code* err_code) noexcept
{
    bool success = false;
    auto* dir_ent_ext = (wdirectory_entity_extension*)dir_ent->ext;

    if (dir_ent_ext != nullptr)
    {
        if (!::FindClose(dir_ent_ext->dir_handl))
        {
            assign_system_error_code((int)GetLastError(), err_code);
        }
        else
        {
            success = true;
        }

        dir_ent_ext->~wdirectory_entity_extension();
        free(dir_ent_ext);
        dir_ent->ext = nullptr;
    }
    else
    {
        assign_system_error_code(EINVAL, err_code);
    }

    return success;
}


bool file_exists(const char* fle_path, std::error_code* err_code) noexcept
{
    return access(fle_path, access_modes::EXISTS, err_code);
}


bool file_exists(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    return access(fle_path, access_modes::EXISTS, err_code);
}


// bool get_first_actual_directory(char* pth, std::error_code* err_code) noexcept
// {
//     std::size_t pth_len;
//
//     stringutils::strdisclastif(pth, '\\');
//     pth_len = stringutils::strlen(pth);
//
//     if (pth_len == 0)
//     {
//         assign_system_error_code(EINVAL, err_code);
//         return false;
//     }
//
//     while (pth_len > 0)
//     {
//         if (access(pth, access_modes::EXISTS, file_types::DIRECTORY, err_code))
//         {
//             return true;
//         }
//
//         if (stringutils::strcut(pth, '\\') == nullptr)
//         {
//             break;
//         }
//
//         pth_len = stringutils::strlen(pth);
//
//         if (pth_len != 1)
//         {
//             stringutils::strdisclastif(pth, '\\');
//             --pth_len;
//         }
//     }
//
//     pth[0] = '.';
//     pth[1] = '\0';
//
//     return true;
// }


// bool get_first_actual_directory(wchar_t* pth, std::error_code* err_code) noexcept
// {
//     std::size_t pth_len;
//
//     stringutils::strdisclastif(pth, '\\');
//     pth_len = stringutils::strlen(pth);
//
//     if (pth_len == 0)
//     {
//         assign_system_error_code(EINVAL, err_code);
//         return false;
//     }
//
//     while (pth_len > 0)
//     {
//         if (access(pth, access_modes::EXISTS, file_types::DIRECTORY, err_code))
//         {
//             return true;
//         }
//
//         if (stringutils::strcut(pth, '\\') == nullptr)
//         {
//             break;
//         }
//
//         pth_len = stringutils::strlen(pth);
//
//         if (pth_len != 1)
//         {
//             stringutils::strdisclastif(pth, '\\');
//             --pth_len;
//         }
//     }
//
//     pth[0] = '.';
//     pth[1] = '\0';
//
//     return true;
// }


uint64_t get_file_inode(const char* fle_path, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    BY_HANDLE_FILE_INFORMATION file_info;

    file_handl = ::CreateFileA(fle_path, GENERIC_READ,  FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
                               nullptr);

    if (file_handl == INVALID_HANDLE_VALUE)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return ~0ull;
    }

    if (!::GetFileInformationByHandle(file_handl, &file_info))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        ::CloseHandle(file_handl);
        return ~0ull;
    }

    ::CloseHandle(file_handl);

    return ((std::uint64_t)file_info.nFileIndexHigh << 32) | file_info.nFileIndexLow;
}


uint64_t get_file_inode(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    BY_HANDLE_FILE_INFORMATION file_info;

    file_handl = ::CreateFileW(fle_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
                               nullptr);

    if (file_handl == INVALID_HANDLE_VALUE)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return ~0ull;
    }

    if (!::GetFileInformationByHandle(file_handl, &file_info))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        ::CloseHandle(file_handl);
        return ~0ull;
    }

    ::CloseHandle(file_handl);

    return ((std::uint64_t)file_info.nFileIndexHigh << 32) | file_info.nFileIndexLow;
}


int get_file_uid(const char* fle_path, std::error_code* err_code) noexcept
{
    PSID owner_sid = nullptr;
    PSECURITY_DESCRIPTOR sec_desc = nullptr;
    char* sid_cstr = nullptr;
    int uid;

    if (::GetNamedSecurityInfoA(fle_path, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &owner_sid,
                nullptr, nullptr, nullptr, &sec_desc) != ERROR_SUCCESS ||
        !::ConvertSidToStringSidA(owner_sid, &sid_cstr))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        if (sec_desc != nullptr)
        {
            ::LocalFree(sec_desc);
        }
        return -1;
    }

    // TODO: Compute a 32 bits hash using the windows api.
    uid = (int)std::hash<std::string>{}(sid_cstr);

    ::LocalFree(sid_cstr);
    ::LocalFree(sec_desc);

    return uid;
}


int get_file_uid(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    PSID owner_sid = nullptr;
    PSECURITY_DESCRIPTOR sec_desc = nullptr;
    char* sid_wstr = nullptr;
    int uid;

    if (::GetNamedSecurityInfoW(fle_path, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &owner_sid,
                nullptr, nullptr, nullptr, &sec_desc) != ERROR_SUCCESS ||
        !::ConvertSidToStringSidA(owner_sid, &sid_wstr))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        if (sec_desc != nullptr)
        {
            ::LocalFree(sec_desc);
        }
        return -1;
    }

    // TODO: Compute a 32 bits hash using the windows api.
    uid = (int)std::hash<std::string>{}(sid_wstr);

    ::LocalFree(sid_wstr);
    ::LocalFree(sec_desc);

    return uid;
}


int get_file_gid(const char* fle_path, std::error_code* err_code) noexcept
{
    // TODO: Correct this function.
    PSECURITY_DESCRIPTOR pSD;
    DWORD size = 0;
    PSID pGroupSID = nullptr;
    BOOL bGroupDefaulted = FALSE;
    char name[256];
    char domain[256];
    DWORD name_len = sizeof(name);
    DWORD domain_len = sizeof(domain);
    SID_NAME_USE sid_type;

    if (!GetFileSecurityA(fle_path, GROUP_SECURITY_INFORMATION, nullptr, 0, &size) &&
        GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return -1;
    }

    pSD = (PSECURITY_DESCRIPTOR)malloc(size);
    if (!pSD)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return -1;
    }

    if (!GetFileSecurityA(fle_path, GROUP_SECURITY_INFORMATION, pSD, size, &size))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        free(pSD);
        return -1;
    }

    if (!GetSecurityDescriptorGroup(pSD, &pGroupSID, &bGroupDefaulted))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        free(pSD);
        return -1;
    }

    if (!LookupAccountSidA(nullptr, pGroupSID, name, &name_len, domain, &domain_len, &sid_type))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        free(pSD);
        return -1;
    }

    free(pSD);

    return (int)std::hash<std::string>()(std::string(domain) + "\\" + std::string(name));
}


int get_file_gid(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    // TODO: Correct this function.
    PSECURITY_DESCRIPTOR pSD;
    DWORD size = 0;
    PSID pGroupSID = nullptr;
    BOOL bGroupDefaulted = FALSE;
    char name[256];
    char domain[256];
    DWORD name_len = sizeof(name);
    DWORD domain_len = sizeof(domain);
    SID_NAME_USE sid_type;

    if (!GetFileSecurityW(fle_path, GROUP_SECURITY_INFORMATION, nullptr, 0, &size) &&
        GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return -1;
    }

    pSD = (PSECURITY_DESCRIPTOR)malloc(size);
    if (!pSD)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return -1;
    }

    if (!GetFileSecurityW(fle_path, GROUP_SECURITY_INFORMATION, pSD, size, &size))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        free(pSD);
        return -1;
    }

    if (!GetSecurityDescriptorGroup(pSD, &pGroupSID, &bGroupDefaulted))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        free(pSD);
        return -1;
    }

    if (!LookupAccountSidA(nullptr, pGroupSID, name, &name_len, domain, &domain_len, &sid_type))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        free(pSD);
        return -1;
    }

    free(pSD);

    return (int)std::hash<std::string>()(std::string(domain) + "\\" + std::string(name));
}


std::size_t get_file_size(const char* fle_path, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    DWORD file_sz;
    
    file_handl = ::CreateFileA(fle_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file_handl == INVALID_HANDLE_VALUE)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return ~0ull;
    }

    file_sz = ::GetFileSize(file_handl, nullptr);
    if (file_sz == INVALID_FILE_SIZE)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        ::CloseHandle(file_handl);
        return ~0ull;
    }
    
    ::CloseHandle(file_handl);
    return file_sz;
}


std::size_t get_file_size(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    DWORD file_sz;
    
    file_handl = ::CreateFileW(fle_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file_handl == INVALID_HANDLE_VALUE)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return ~0ull;
    }

    file_sz = ::GetFileSize(file_handl, nullptr);
    if (file_sz == INVALID_FILE_SIZE)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        ::CloseHandle(file_handl);
        return ~0ull;
    }
    
    ::CloseHandle(file_handl);
    return file_sz;
}


bool get_modification_time(
        const char* fle_path,
        system_time* system_tme,
        std::error_code* err_code
) noexcept
{
    HANDLE handl;
    FILETIME last_write_tme;
    SYSTEMTIME utc_system_tme;
    SYSTEMTIME local_system_tme;
    
    handl = ::CreateFileA(fle_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                          OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
                          nullptr);

    if (handl == INVALID_HANDLE_VALUE ||
        ::GetFileTime(handl, nullptr, nullptr, &last_write_tme) == 0 ||
        ::FileTimeToSystemTime(&last_write_tme, &utc_system_tme) == 0 ||
        ::SystemTimeToTzSpecificLocalTime(nullptr, &utc_system_tme, &local_system_tme) == 0)
    {
        assign_system_error_code((int)GetLastError(), err_code);
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


bool get_modification_time(
        const wchar_t* fle_path,
        system_time* system_tme,
        std::error_code* err_code
) noexcept
{
    HANDLE handl;
    FILETIME last_write_tme;
    SYSTEMTIME utc_system_tme;
    SYSTEMTIME local_system_tme;
    
    handl = ::CreateFileW(fle_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                          OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
                          nullptr);

    if (handl == INVALID_HANDLE_VALUE ||
        ::GetFileTime(handl, nullptr, nullptr, &last_write_tme) == 0 ||
        ::FileTimeToSystemTime(&last_write_tme, &utc_system_tme) == 0 ||
        ::SystemTimeToTzSpecificLocalTime(nullptr, &utc_system_tme, &local_system_tme) == 0)
    {
        assign_system_error_code((int)GetLastError(), err_code);
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
    // TODO: Make this as a template in order to allow both kind of path (c/w).
    static char temp_pth[MAX_PATH];
    DWORD len = ::GetTempPathA(MAX_PATH, temp_pth);

    if (len == 0 || len > MAX_PATH)
    {
        return nullptr;
    }

    return temp_pth;
}


bool is_block_device(const char* fle_path, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    DWORD file_typ;

    file_handl = ::CreateFileA(fle_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handl == INVALID_HANDLE_VALUE)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    file_typ = ::GetFileType(file_handl);
    CloseHandle(file_handl);

    return file_typ == FILE_TYPE_DISK;
}


bool is_block_device(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    DWORD file_typ;

    file_handl = ::CreateFileW(fle_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handl == INVALID_HANDLE_VALUE)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    file_typ = ::GetFileType(file_handl);
    CloseHandle(file_handl);

    return file_typ == FILE_TYPE_DISK;
}


bool is_character_device(const char* fle_path, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    DWORD file_typ;

    file_handl = ::CreateFileA(fle_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handl == INVALID_HANDLE_VALUE)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    file_typ = ::GetFileType(file_handl);
    CloseHandle(file_handl);

    return file_typ == FILE_TYPE_CHAR;
}


bool is_character_device(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    DWORD file_typ;

    file_handl = ::CreateFileW(fle_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handl == INVALID_HANDLE_VALUE)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    file_typ = ::GetFileType(file_handl);
    CloseHandle(file_handl);

    return file_typ == FILE_TYPE_CHAR;
}


bool is_directory(const char* fle_path, std::error_code* err_code) noexcept
{
    DWORD attr = ::GetFileAttributesA(fle_path);

    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}


bool is_directory(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    DWORD attr = ::GetFileAttributesW(fle_path);

    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}


bool is_fifo(const char* fle_path, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    DWORD file_typ;

    file_handl = ::CreateFileA(fle_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handl == INVALID_HANDLE_VALUE)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    file_typ = ::GetFileType(file_handl);
    CloseHandle(file_handl);

    return file_typ == FILE_TYPE_PIPE;
}


bool is_fifo(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    HANDLE file_handl;
    DWORD file_typ;

    file_handl = ::CreateFileW(fle_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handl == INVALID_HANDLE_VALUE)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    file_typ = ::GetFileType(file_handl);
    CloseHandle(file_handl);

    return file_typ == FILE_TYPE_PIPE;
}


bool is_file_type(const char* fle_path, file_types fle_type, std::error_code* err_code) noexcept
{
    if ((fle_type & file_types::BLOCK_DEVICE) != file_types::NIL &&
        is_block_device(fle_path, err_code))
    {
        return true;
    }
    if ((fle_type & file_types::CHARACTER_DEVICE) != file_types::NIL &&
        is_character_device(fle_path, err_code))
    {
        return true;
    }
    if ((fle_type & file_types::DIRECTORY) != file_types::NIL &&
        is_directory(fle_path, err_code))
    {
        return true;
    }
    if ((fle_type & file_types::FIFO) != file_types::NIL &&
        is_fifo(fle_path, err_code))
    {
        return true;
    }
    if ((fle_type & file_types::REGULAR_FILE) != file_types::NIL &&
        is_regular_file(fle_path, err_code))
    {
        return true;
    }
    if ((fle_type & file_types::SOCKET) != file_types::NIL &&
        is_socket(fle_path, err_code))
    {
        return true;
    }
    if ((fle_type & file_types::SYMLINK) != file_types::NIL &&
        is_symlink(fle_path, err_code))
    {
        return true;
    }

    return false;
}


bool is_file_type(const wchar_t* fle_path, file_types fle_type, std::error_code* err_code) noexcept
{
    if ((fle_type & file_types::BLOCK_DEVICE) != file_types::NIL &&
        is_block_device(fle_path, err_code))
    {
        return true;
    }
    if ((fle_type & file_types::CHARACTER_DEVICE) != file_types::NIL &&
        is_character_device(fle_path, err_code))
    {
        return true;
    }
    if ((fle_type & file_types::DIRECTORY) != file_types::NIL &&
        is_directory(fle_path, err_code))
    {
        return true;
    }
    if ((fle_type & file_types::FIFO) != file_types::NIL &&
        is_fifo(fle_path, err_code))
    {
        return true;
    }
    if ((fle_type & file_types::REGULAR_FILE) != file_types::NIL &&
        is_regular_file(fle_path, err_code))
    {
        return true;
    }
    if ((fle_type & file_types::SOCKET) != file_types::NIL &&
        is_socket(fle_path, err_code))
    {
        return true;
    }
    if ((fle_type & file_types::SYMLINK) != file_types::NIL &&
        is_symlink(fle_path, err_code))
    {
        return true;
    }

    return false;
}


bool is_regular_file(const char* fle_path, std::error_code* err_code) noexcept
{
    DWORD attr = ::GetFileAttributesA(fle_path);

    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return (attr & FILE_ATTRIBUTE_DIRECTORY) == 0;
}


bool is_regular_file(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    DWORD attr = ::GetFileAttributesW(fle_path);

    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return (attr & FILE_ATTRIBUTE_DIRECTORY) == 0;
}


bool is_socket(const char* fle_path, std::error_code* err_code) noexcept
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


bool is_socket(const wchar_t* fle_path, std::error_code* err_code) noexcept
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


bool is_symlink(const char* fle_path, std::error_code* err_code) noexcept
{
    WIN32_FIND_DATA find_dat;
    DWORD file_attrs;
    HANDLE file_handl;

    file_attrs = ::GetFileAttributesA(fle_path);
    if (file_attrs == INVALID_FILE_ATTRIBUTES)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    if (file_attrs & FILE_ATTRIBUTE_REPARSE_POINT)
    {
        file_handl = ::FindFirstFileA(fle_path, &find_dat);
        if (file_handl != INVALID_HANDLE_VALUE)
        {
            ::FindClose(file_handl);

            return find_dat.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT &&
                   find_dat.dwReserved0 == IO_REPARSE_TAG_SYMLINK;
        }
    }

    return false;
}


bool is_symlink(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    WIN32_FIND_DATAW find_dat;
    DWORD file_attrs;
    HANDLE file_handl;

    file_attrs = ::GetFileAttributesW(fle_path);
    if (file_attrs == INVALID_FILE_ATTRIBUTES)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    if (file_attrs & FILE_ATTRIBUTE_REPARSE_POINT)
    {
        file_handl = ::FindFirstFileW(fle_path, &find_dat);
        if (file_handl != INVALID_HANDLE_VALUE)
        {
            ::FindClose(file_handl);

            return find_dat.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT &&
                   find_dat.dwReserved0 == IO_REPARSE_TAG_SYMLINK;
        }
    }

    return false;
}


bool mkdir(const char* dir_path, std::uint32_t mods, std::error_code* err_code) noexcept
{
    // TODO: Change the mods after the directory creation to match them with the argument
    if (!::CreateDirectoryA(dir_path, nullptr))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}


bool mkdir(const wchar_t* dir_path, std::uint32_t mods, std::error_code* err_code) noexcept
{
    // TODO: Change the mods after the directory creation to match them with the argument
    if (!::CreateDirectoryW(dir_path, nullptr))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}


bool mkdir_recursively(
        const char* dir_path,
        std::uint32_t mods,
        std::error_code* err_code
) noexcept
{
    // TODO: Change the mods after the directory creation to match them with the argument.
    char parnt_path[MAX_PATH] = {0};
    std::size_t pth_len;
    size_t slash_pos[MAX_PATH];
    size_t slash_pos_sz = 0;
    char* last_chr;

    pth_len = stringutils::strlen(dir_path);

    if (pth_len >= MAX_PATH ||
        pth_len == 0 ||
        access(dir_path, access_modes::EXISTS, err_code))
    {
        return false;
    }

    stringutils::strcpy(parnt_path, dir_path);
    stringutils::strdisclastif(parnt_path, '\\');

    do
    {
        last_chr = stringutils::strcut(parnt_path, '\\', true);

        if (last_chr == nullptr)
        {
            if (!mkdir(parnt_path, mods, err_code))
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

    } while (!access(parnt_path, access_modes::EXISTS, err_code) && pth_len > 0);

    while (slash_pos_sz > 0)
    {
        parnt_path[slash_pos[--slash_pos_sz]] = '\\';

         if (!mkdir(parnt_path, mods, err_code))
         {
             return false;
         }
    }

    return true;
}


bool mkdir_recursively(
        const wchar_t* dir_path,
        std::uint32_t mods,
        std::error_code* err_code
) noexcept
{
    // TODO: Change the mods after the directory creation to match them with the argument
    wchar_t parnt_path[MAX_PATH] = {0};
    std::size_t pth_len;
    size_t slash_pos[MAX_PATH];
    size_t slash_pos_sz = 0;
    wchar_t* last_chr;

    pth_len = stringutils::strlen(dir_path);

    if (pth_len >= MAX_PATH ||
        pth_len == 0 ||
        access(dir_path, access_modes::EXISTS, err_code))
    {
        return false;
    }

    stringutils::strcpy(parnt_path, dir_path);
    stringutils::strdisclastif(parnt_path, L'\\');

    do
    {
        last_chr = stringutils::strcut(parnt_path, L'\\', true);

        if (last_chr == nullptr)
        {
            if (!mkdir(parnt_path, mods, err_code))
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

    } while (!access(parnt_path, access_modes::EXISTS, err_code) && pth_len > 0);

    while (slash_pos_sz > 0)
    {
        parnt_path[slash_pos[--slash_pos_sz]] = L'\\';

        if (!mkdir(parnt_path, mods, err_code))
        {
            return false;
        }
    }

    return true;
}


bool opendir(
        directory_entity* dir_ent,
        const char* dir_pth,
        std::error_code* err_code
) noexcept
{
    // TODO: Stop using strings and make it noexcept.
    std::string search_pth;
    directory_entity_extension* dir_ent_ext;
    std::size_t dir_pth_len;

    dir_ent_ext = (directory_entity_extension*)malloc(sizeof(directory_entity_extension));
    dir_ent->ext = dir_ent_ext;
    if (dir_ent_ext == nullptr)
    {
        assign_system_error_code(ERROR_NOT_ENOUGH_MEMORY, err_code);
        return false;
    }

    ::new ((void*)(&dir_ent_ext->pth)) decltype(dir_ent_ext->pth)(dir_pth);
    dir_pth_len = dir_ent_ext->pth.length();
    if (dir_pth_len < 1 || dir_pth[dir_pth_len - 1] != '\\')
    {
        dir_ent_ext->pth += '\\';
    }

    search_pth = dir_ent_ext->pth + '*';
    dir_ent_ext->dir_handl = FindFirstFileA(search_pth.c_str(), &dir_ent_ext->find_dat);
    if (dir_ent_ext->dir_handl == INVALID_HANDLE_VALUE)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        free(dir_ent->ext);
        dir_ent->ext = nullptr;
        return false;
    }

    dir_ent_ext->read_dne = false;

    return true;
}


bool opendir(
        wdirectory_entity* dir_ent,
        const wchar_t* dir_pth,
        std::error_code* err_code
) noexcept
{
    // TODO: Stop using strings and make it noexcept.
    std::wstring search_pth;
    wdirectory_entity_extension* dir_ent_ext;
    std::size_t dir_pth_len;

    dir_ent_ext = (wdirectory_entity_extension*)malloc(sizeof(wdirectory_entity_extension));
    dir_ent->ext = dir_ent_ext;
    if (dir_ent_ext == nullptr)
    {
        assign_system_error_code(ERROR_NOT_ENOUGH_MEMORY, err_code);
        return false;
    }

    ::new ((void*)(&dir_ent_ext->pth)) decltype(dir_ent_ext->pth)(dir_pth);
    dir_pth_len = dir_ent_ext->pth.length();
    if (dir_pth_len < 1 || dir_pth[dir_pth_len - 1] != '\\')
    {
        dir_ent_ext->pth += L'\\';
    }

    search_pth = dir_ent_ext->pth + L'*';
    dir_ent_ext->dir_handl = FindFirstFileW(search_pth.c_str(), &dir_ent_ext->find_dat);
    if (dir_ent_ext->dir_handl == INVALID_HANDLE_VALUE)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        free(dir_ent->ext);
        dir_ent->ext = nullptr;
        return false;
    }

    dir_ent_ext->read_dne = false;

    return true;
}


bool readdir(directory_entity* dir_ent, std::error_code* err_code) noexcept
{
    // TODO: Stop using strings and make it noexcept.
    auto* dir_ent_ext = (directory_entity_extension*)dir_ent->ext;
    DWORD last_err;
    std::string search_pth;

    if (dir_ent_ext->read_dne && !::FindNextFileA(dir_ent_ext->dir_handl, &dir_ent_ext->find_dat))
    {
        last_err = ::GetLastError();
        if (last_err != ERROR_NO_MORE_FILES)
        {
            assign_system_error_code((int)last_err, err_code);
        }

        return false;
    }

    dir_ent_ext->read_dne = true;
    dir_ent->nme = dir_ent_ext->find_dat.cFileName;
    search_pth = dir_ent_ext->pth + dir_ent->nme;
    dir_ent->ino = get_file_inode(search_pth.c_str(), err_code);

    return true;
}


bool readdir(wdirectory_entity* dir_ent, std::error_code* err_code) noexcept
{
    // TODO: Stop using strings and make it noexcept.
    auto* dir_ent_ext = (wdirectory_entity_extension*)dir_ent->ext;
    DWORD last_err;
    std::wstring search_pth;

    if (dir_ent_ext->read_dne && !::FindNextFileW(dir_ent_ext->dir_handl, &dir_ent_ext->find_dat))
    {
        last_err = ::GetLastError();
        if (last_err != ERROR_NO_MORE_FILES)
        {
            assign_system_error_code((int)last_err, err_code);
        }

        return false;
    }

    dir_ent_ext->read_dne = true;
    dir_ent->nme = dir_ent_ext->find_dat.cFileName;
    search_pth = dir_ent_ext->pth + dir_ent->nme;
    dir_ent->ino = get_file_inode(search_pth.c_str(), err_code);

    return true;
}


bool rmdir(const char* dir_path, std::error_code* err_code) noexcept
{
    ::SetFileAttributesA(dir_path, 0x80);
    if (!::RemoveDirectoryA(dir_path))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}


bool rmdir(const wchar_t* dir_path, std::error_code* err_code) noexcept
{
    ::SetFileAttributesW(dir_path, 0x80);
    if (!::RemoveDirectoryW(dir_path))
    {
        assign_system_error_code((int)GetLastError(), err_code);
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
        assign_system_error_code(ERROR_BAD_PATHNAME, err_code);
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
        assign_system_error_code(ERROR_BAD_PATHNAME, err_code);
        goto exit;
    }

    speed::stringutils::strcpy(wshortcut_pth + converted_chars - 1, L".lnk");
    res = persist_fle->Save(wshortcut_pth, true);

    goto exit;

exit_with_error:
    assign_system_error_code((int)GetLastError(), err_code);

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
        assign_system_error_code(ERROR_BAD_PATHNAME, err_code);
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
        assign_system_error_code(ERROR_BAD_PATHNAME, err_code);
        goto exit;
    }

    speed::stringutils::strcpy(wshortcut_pth, shortcut_pth);
    speed::stringutils::strcpy(wshortcut_pth + shortcut_pth_len, L".lnk");
    res = persist_fle->Save(wshortcut_pth, true);

    goto exit;

exit_with_error:
    assign_system_error_code((int)GetLastError(), err_code);

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
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    if (file_attr & FILE_ATTRIBUTE_DIRECTORY)
    {
        flgs = SYMBOLIC_LINK_FLAG_DIRECTORY;
    }

    if (!::CreateSymbolicLinkA(link_pth, target_pth, flgs))
    {
        assign_system_error_code((int)GetLastError(), err_code);
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
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    if (file_attr & FILE_ATTRIBUTE_DIRECTORY)
    {
        flgs = SYMBOLIC_LINK_FLAG_DIRECTORY;
    }

    if (!::CreateSymbolicLinkW(link_pth, target_pth, flgs))
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    return true;
}


bool touch(const char* regular_file_pth, std::uint32_t mods, std::error_code* err_code) noexcept
{
    // TODO: Change the mods after the regular file creation to match them with the argument
    HANDLE file_handle = ::CreateFileA(regular_file_pth, GENERIC_WRITE, 0, nullptr, CREATE_NEW,
                                       FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handle == INVALID_HANDLE_VALUE)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    ::CloseHandle(file_handle);

    return true;
}


bool touch(const wchar_t* regular_file_pth, std::uint32_t mods, std::error_code* err_code) noexcept
{
    // TODO: Change the mods after the regular file creation to match them with the argument
    HANDLE file_handle = ::CreateFileW(regular_file_pth, GENERIC_WRITE, 0, nullptr, CREATE_NEW,
                                       FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handle == INVALID_HANDLE_VALUE)
    {
        assign_system_error_code((int)GetLastError(), err_code);
        return false;
    }

    ::CloseHandle(file_handle);

    return true;
}


bool unlink(const char* reg_file_path, std::error_code* err_code) noexcept
{
    if (!::DeleteFileA(reg_file_path))
    {
        assign_system_error_code((int)::GetLastError(), err_code);
        return false;
    }

    return true;
}


bool unlink(const wchar_t* reg_file_path, std::error_code* err_code) noexcept
{
    if (!::DeleteFileW(reg_file_path))
    {
        assign_system_error_code((int)::GetLastError(), err_code);
        return false;
    }

    return true;
}


}


#endif

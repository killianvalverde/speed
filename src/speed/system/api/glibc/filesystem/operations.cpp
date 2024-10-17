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
 * @file        speed/system/api/glibc/filesystem/operations.cpp
 * @brief       filesystem operations source.
 * @author      Killian Valverde
 * @date        2017/05/26
 */

#include "../../../compatibility/compatibility.hpp"
#ifdef SPEED_GLIBC

#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
#include <wchar.h>

#include "../../../../stringutils/stringutils.hpp"
#include "directory_entity_extension.hpp"
#include "operations.hpp"


namespace speed::system::api::glibc::filesystem {


bool access(
        const char* fle_path,
        access_modes acss_modes,
        std::error_code* err_code
) noexcept
{
    decltype(F_OK) mode_buildr = 0;

    if (acss_modes == access_modes::NIL)
    {
        return true;
    }
    if ((acss_modes & access_modes::EXISTS) != access_modes::NIL)
    {
        mode_buildr |= F_OK;
    }
    if ((acss_modes & access_modes::READ) != access_modes::NIL)
    {
        mode_buildr |= R_OK;
    }
    if ((acss_modes & access_modes::WRITE) != access_modes::NIL)
    {
        mode_buildr |= W_OK;
    }
    if ((acss_modes & access_modes::EXECUTE) != access_modes::NIL)
    {
        mode_buildr |= X_OK;
    }
    
    if (::access(fle_path, mode_buildr) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}


bool access(
        const wchar_t* fle_path,
        access_modes acss_modes,
        std::error_code* err_code
) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(fle_path, c_str))
    {
        return false;
    }

    return access(c_str, acss_modes, err_code);
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
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(fle_path, c_str))
    {
        return false;
    }

    return access(c_str, acss_modes, fle_type, err_code);
}


bool can_directory_be_created(const char* dir_path, std::error_code* err_code) noexcept
{
    char parent_pth[PATH_MAX] = {0};
    std::size_t dir_path_len = stringutils::strlen(dir_path);
    char* last_char_p;

    if (dir_path_len >= PATH_MAX ||
        dir_path_len == 0 ||
        access(dir_path, access_modes::EXISTS, err_code))
    {
        return false;
    }

    stringutils::strcpy(parent_pth, dir_path);
    stringutils::strdisclastif(parent_pth, '/');
    last_char_p = stringutils::strcut(parent_pth, '/');
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
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(dir_path, c_str))
    {
        return false;
    }

    return can_directory_be_created(c_str, err_code);
}


bool can_regular_file_be_created(const char* reg_file_path, std::error_code* err_code) noexcept
{
    char parent_pth[PATH_MAX] = {0};
    std::size_t path_len = stringutils::strlen(reg_file_path);
    char* last_char_p;

    if (path_len >= PATH_MAX || path_len == 0)
    {
        return false;
    }

    if (access(reg_file_path, access_modes::EXISTS, err_code))
    {
        return access(reg_file_path, access_modes::WRITE, file_types::REGULAR_FILE, err_code);
    }

    stringutils::strcpy(parent_pth, reg_file_path);
    stringutils::strdisclastif(parent_pth, '/');
    last_char_p = stringutils::strcut(parent_pth, '/');
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
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(reg_file_path, c_str))
    {
        return false;
    }

    return can_regular_file_be_created(c_str, err_code);
}


bool chdir(const char* dir_path, std::error_code* err_code) noexcept
{
    errno = 0;
    
    if (::chdir(dir_path) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}


bool chdir(const wchar_t* dir_path, std::error_code* err_code) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(dir_path, c_str))
    {
        return false;
    }

    return chdir(c_str, err_code);
}


bool closedir(directory_entity* dir_ent, std::error_code* err_code) noexcept
{
    errno = 0;
    bool succss = false;

    if (dir_ent->ext != nullptr)
    {
        if (::closedir(((directory_entity_extension*)dir_ent->ext)->dir) == -1)
        {
            assign_system_error_code(errno, err_code);
        }
        else
        {
            succss = true;
        }

        free(dir_ent->ext);
        dir_ent->ext = nullptr;
    }
    else
    {
        assign_system_error_code(EINVAL, err_code);
    }
    
    return succss;
}


bool closedir(wdirectory_entity* dir_ent, std::error_code* err_code) noexcept
{
    errno = 0;
    bool succss = false;

    if (dir_ent->ext != nullptr)
    {
        if (::closedir(((directory_entity_extension*)dir_ent->ext)->dir) == -1)
        {
            assign_system_error_code(errno, err_code);
        }
        else
        {
            succss = true;
        }

        free(dir_ent->ext);
        dir_ent->ext = nullptr;
    }
    else
    {
        assign_system_error_code(EINVAL, err_code);
    }

    return succss;
}


bool get_cstr_path_from_wstr(
        const wchar_t* w_str,
        char* c_str
) noexcept
{
    std::size_t len = wcstombs(nullptr, w_str, 0) + 1;

    if (len > PATH_MAX)
    {
        return false;
    }

    return wcstombs(c_str, w_str, len) != (std::size_t)-1;
}


bool get_first_actual_directory(char* pth, std::error_code* err_code) noexcept
{
    std::size_t pth_len;
    errno = 0;
    
    stringutils::strdisclastif(pth, '/');
    pth_len = stringutils::strlen(pth);
    
    if (pth_len == 0)
    {
        assign_system_error_code(EINVAL, err_code);
        return false;
    }
    
    while (pth_len > 0)
    {
        if (access(pth, access_modes::EXISTS, file_types::DIRECTORY, err_code))
        {
            return true;
        }
    
        if (stringutils::strcut(pth, '/') == nullptr)
        {
            break;
        }
        
        pth_len = stringutils::strlen(pth);
        
        if (pth_len != 1)
        {
            stringutils::strdisclastif(pth, '/');
            --pth_len;
        }
    }
    
    pth[0] = '.';
    pth[1] = '\0';
    
    return true;
}


std::uint64_t get_file_inode(const char* fle_path, std::error_code* err_code) noexcept
{
    struct ::stat stt;
    errno = 0;
    
    if (::lstat(fle_path, &stt) == -1)
    {
        assign_system_error_code(errno, err_code);
        return ~0ul;
    }
    
    return stt.st_ino;
}


std::uint64_t get_file_inode(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(fle_path, c_str))
    {
        return false;
    }

    return get_file_inode(c_str, err_code);
}


int get_file_uid(const char* fle_path, std::error_code* err_code) noexcept
{
    struct ::stat stt;
    errno = 0;
    
    if (::stat(fle_path, &stt) == -1)
    {
        assign_system_error_code(errno, err_code);
        return -1;
    }
    
    return stt.st_uid;
}


int get_file_uid(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(fle_path, c_str))
    {
        return false;
    }

    return get_file_uid(c_str, err_code);
}


int get_file_gid(const char* fle_path, std::error_code* err_code) noexcept
{
    struct ::stat stt;
    errno = 0;
    
    if (::stat(fle_path, &stt) == -1)
    {
        assign_system_error_code(errno, err_code);
        return -1;
    }
    
    return stt.st_gid;
}


int get_file_gid(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(fle_path, c_str))
    {
        return false;
    }

    return get_file_gid(c_str, err_code);
}


const char* get_temporal_path() noexcept
{
    return "/tmp";
}


bool get_wstr_path_from_cstr(
        const char* c_str,
        wchar_t* w_str
) noexcept
{
    size_t len = mbstowcs(nullptr, c_str, 0) + 1;

    if (len > PATH_MAX)
    {
        return false;
    }

    return mbstowcs(w_str, c_str, len) != static_cast<std::size_t>(-1);
}


bool is_block_device(const char* fle_path, std::error_code* err_code) noexcept
{
    struct ::stat infos;
    errno = 0;
    
    if (::stat(fle_path, &infos) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return S_ISBLK(infos.st_mode);
}


bool is_block_device(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(fle_path, c_str))
    {
        return false;
    }

    return is_block_device(c_str, err_code);
}


bool is_character_device(const char* fle_path, std::error_code* err_code) noexcept
{
    struct ::stat infos;
    errno = 0;
    
    if (::stat(fle_path, &infos) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return S_ISCHR(infos.st_mode);
}


bool is_character_device(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(fle_path, c_str))
    {
        return false;
    }

    return is_character_device(c_str, err_code);
}


bool is_directory(const char* fle_path, std::error_code* err_code) noexcept
{
    struct ::stat infos;
    errno = 0;
    
    if (::stat(fle_path, &infos) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return S_ISDIR(infos.st_mode);
}


bool is_directory(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(fle_path, c_str))
    {
        return false;
    }

    return is_directory(c_str, err_code);
}


bool is_fifo(const char* fle_path, std::error_code* err_code) noexcept
{
    struct ::stat infos;
    errno = 0;
    
    if (::stat(fle_path, &infos) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return S_ISFIFO(infos.st_mode);
}


bool is_fifo(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(fle_path, c_str))
    {
        return false;
    }

    return is_fifo(c_str, err_code);
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
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(fle_path, c_str))
    {
        return false;
    }

    return is_file_type(c_str, fle_type, err_code);
}


bool is_regular_file(const char* fle_path, std::error_code* err_code) noexcept
{
    struct ::stat infos;
    errno = 0;
    
    if (::stat(fle_path, &infos) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return S_ISREG(infos.st_mode);
}


bool is_regular_file(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(fle_path, c_str))
    {
        return false;
    }

    return is_regular_file(c_str, err_code);
}


bool is_socket(const char* fle_path, std::error_code* err_code) noexcept
{
    struct ::stat infos;
    errno = 0;
    
    if (::stat(fle_path, &infos) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return S_ISSOCK(infos.st_mode);
}


bool is_socket(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(fle_path, c_str))
    {
        return false;
    }

    return is_socket(c_str, err_code);
}


bool is_symlink(const char* fle_path, std::error_code* err_code) noexcept
{
    struct ::stat infos;
    errno = 0;
    
    if (::lstat(fle_path, &infos) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return S_ISLNK(infos.st_mode);
}


bool is_symlink(const wchar_t* fle_path, std::error_code* err_code) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(fle_path, c_str))
    {
        return false;
    }

    return is_symlink(c_str, err_code);
}


bool mkdir(const char* dir_path, std::uint32_t mods, std::error_code* err_code) noexcept
{
    errno = 0;
    
    if (::mkdir(dir_path, mods) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}


bool mkdir(const wchar_t* dir_path, std::uint32_t mods, std::error_code* err_code) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(dir_path, c_str))
    {
        return false;
    }

    return mkdir(c_str, mods, err_code);
}


bool mkdir_recursively(
        const char* dir_path,
        std::uint32_t mods,
        std::error_code* err_code
) noexcept
{
    static_assert(PATH_MAX >= 255, "PATH_MAX has to be at least 255.");
    
    char parnt_path[PATH_MAX] = {0};
    std::size_t pth_len;
    size_t slash_pos[PATH_MAX];
    size_t slash_pos_sz = 0;
    char* lst_ch;
    errno = 0;
    
    pth_len = stringutils::strlen(dir_path);
    
    if (pth_len >= PATH_MAX ||
        pth_len == 0 ||
        access(dir_path, access_modes::EXISTS, err_code))
    {
        assign_system_error_code(EINVAL, err_code);
        return false;
    }
    
    stringutils::strcpy(parnt_path, dir_path);
    stringutils::strdisclastif(parnt_path, '/');
    
    do
    {
        lst_ch = stringutils::strcut(parnt_path, '/', true);
        
        if (lst_ch == nullptr)
        {
            if (!mkdir(parnt_path, mods, err_code))
            {
                return false;
            }
            
            break;
        }

        pth_len = lst_ch - parnt_path;
        slash_pos[slash_pos_sz++] = pth_len;
        
    } while (!access(parnt_path, access_modes::EXISTS, err_code) && pth_len > 0);
    
    while (slash_pos_sz > 0)
    {
        parnt_path[slash_pos[--slash_pos_sz]] = '/';
        
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
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(dir_path, c_str))
    {
        return false;
    }

    return mkdir_recursively(c_str, mods, err_code);
}


bool opendir(
        directory_entity* dir_ent,
        const char* dir_pth,
        std::error_code* err_code
) noexcept
{
    errno = 0;

    dir_ent->ext = (directory_entity_extension*)malloc(sizeof(directory_entity_extension));
    if (dir_ent->ext == nullptr)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    if ((((directory_entity_extension*)dir_ent->ext)->dir = ::opendir(dir_pth)) == nullptr)
    {
        assign_system_error_code(errno, err_code);
        free(dir_ent->ext);
        dir_ent->ext = nullptr;
        return false;
    }
    
    return true;
}


bool opendir(
        wdirectory_entity* dir_ent,
        const wchar_t* dir_pth,
        std::error_code* err_code
) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(dir_pth, c_str))
    {
        return false;
    }

    dir_ent->ext = (directory_entity_extension*)malloc(sizeof(directory_entity_extension));
    if (dir_ent->ext == nullptr)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }

    if ((((directory_entity_extension*)dir_ent->ext)->dir = ::opendir(c_str)) == nullptr)
    {
        assign_system_error_code(errno, err_code);
        free(dir_ent->ext);
        dir_ent->ext = nullptr;
        return false;
    }

    return true;
}


bool readdir(directory_entity* dir_ent, std::error_code* err_code) noexcept
{
    errno = 0;
    ((directory_entity_extension*)dir_ent->ext)->entry =
            ::readdir(((directory_entity_extension*)dir_ent->ext)->dir);
    
    if (((directory_entity_extension*)dir_ent->ext)->entry == nullptr)
    {
        if (errno != 0)
        {
            assign_system_error_code(errno, err_code);
        }
        
        return false;
    }
    
    dir_ent->ino = ((directory_entity_extension*)dir_ent->ext)->entry->d_ino;
    dir_ent->nme = ((directory_entity_extension*)dir_ent->ext)->entry->d_name;
    
    return true;
}


bool readdir(wdirectory_entity* dir_ent, std::error_code* err_code) noexcept
{
    wchar_t w_str[PATH_MAX] = {};

    errno = 0;
    ((directory_entity_extension*)dir_ent->ext)->entry =
            ::readdir(((directory_entity_extension*)dir_ent->ext)->dir);

    if (((directory_entity_extension*)dir_ent->ext)->entry == nullptr)
    {
        if (errno != 0)
        {
            assign_system_error_code(errno, err_code);
        }

        return false;
    }

    dir_ent->ino = ((directory_entity_extension*)dir_ent->ext)->entry->d_ino;
    get_wstr_path_from_cstr(((directory_entity_extension*)dir_ent->ext)->entry->d_name, w_str);
    ((directory_entity_extension*)dir_ent->ext)->wpath = w_str;
    dir_ent->nme = &w_str[0];

    return true;
}


bool rmdir(const char* dir_path, std::error_code* err_code) noexcept
{
    if (::rmdir(dir_path) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}


bool rmdir(const wchar_t* dir_path, std::error_code* err_code) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(dir_path, c_str))
    {
        return false;
    }

    return rmdir(c_str, err_code);
}


bool shortcut(
        const char* target_pth,
        const char* shortcut_pth,
        std::error_code* err_code
) noexcept
{
    return symlink(target_pth, shortcut_pth, err_code);
}


bool shortcut(
        const wchar_t* target_pth,
        const wchar_t* shortcut_pth,
        std::error_code* err_code
) noexcept
{
    return symlink(target_pth, shortcut_pth, err_code);
}


bool symlink(const char* trg, const char* lnk_pth, std::error_code* err_code) noexcept
{
    errno = 0;
    
    if (::symlink(trg, lnk_pth) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}


bool symlink(const wchar_t* trg, const wchar_t* lnk_pth, std::error_code* err_code) noexcept
{
    char c_str_trg[PATH_MAX] = {};
    char c_str_lnk[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(trg, c_str_trg) || !get_cstr_path_from_wstr(lnk_pth, c_str_lnk))
    {
        return false;
    }

    return symlink(c_str_trg, c_str_lnk, err_code);
}


bool touch(const char* regfle_path, std::uint32_t mods, std::error_code* err_code) noexcept
{
    errno = 0;
    
    if (::mknod(regfle_path, mods, S_IFREG) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}


bool touch(const wchar_t* regfle_path, std::uint32_t mods, std::error_code* err_code) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(regfle_path, c_str))
    {
        return false;
    }

    return touch(c_str, mods, err_code);
}


bool unlink(const char* reg_file_path, std::error_code* err_code) noexcept
{
    if (::unlink(reg_file_path) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }

    return true;
}


bool unlink(const wchar_t* reg_file_path, std::error_code* err_code) noexcept
{
    char c_str[PATH_MAX] = {};
    if (!get_cstr_path_from_wstr(reg_file_path, c_str))
    {
        return false;
    }

    return unlink(c_str, err_code);
}


}


#endif

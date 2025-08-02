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
#ifdef SPEED_GLIBC

#include "operations.hpp"

#include <fcntl.h>
#include <cstdlib>
#include <ctime>

#include "../../../../stringutils/stringutils.hpp"

namespace speed::system::detail::glibc::filesystem {

bool access(
        const struct ::stat& stt,
        access_modes access_mods,
        std::error_code* err_code
) noexcept
{
    ::uid_t euid;
    ::gid_t egid;
    ::mode_t fle_permissions;
    ::mode_t relevant_bts = 0;
    bool succss = true;

    if (access_mods == access_modes::EXISTS || access_mods == access_modes::NIL)
    {
        return true;
    }
    
    euid = ::geteuid();
    egid = ::getegid();
    fle_permissions = stt.st_mode;
    
    if (stt.st_uid == euid)
    {
        relevant_bts = (fle_permissions >> 6) & 0x7;
    }
    else if (stt.st_gid == egid)
    {
        relevant_bts = (fle_permissions >> 3) & 0x7;
    }
    else
    {
        relevant_bts = fle_permissions & 0x7;
    }

    if ((access_mods & access_modes::READ) != access_modes::NIL && !(relevant_bts & 0b100))
    {
        succss = false;
    }
    if ((access_mods & access_modes::WRITE) != access_modes::NIL && !(relevant_bts & 0b010))
    {
        succss = false;
    }
    if ((access_mods & access_modes::EXECUTE) != access_modes::NIL && !(relevant_bts & 0b001))
    {
        succss = false;
    }
    if (!succss)
    {
        system::errors::assign_system_error_code(EACCES, err_code);
        return false;
    }

    return true;
}

bool access(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        access_modes access_mods,
        std::error_code* err_code
) noexcept
{
    struct ::stat stt;
    
    return (stat(file_pth, resolve_symlnk, stt, err_code) &&
            access(stt, access_mods, err_code));
}

bool chdir(const path_char_t* directory_pth, std::error_code* err_code) noexcept
{
    if (::chdir(directory_pth) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}

bool check_file(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        access_modes access_mods,
        file_types file_typ,
        std::error_code* err_code
) noexcept
{
    struct ::stat stt;
    
    return (stat(file_pth, resolve_symlnk, stt, err_code) &&
            access(stt, access_mods, err_code) &&
            is_file_type(stt, file_typ));
}

bool closedir(directory_entity& directory_ent, std::error_code* err_code) noexcept
{
    const auto& directory_ent_ext = directory_ent._ext;
    
    if (::closedir(directory_ent_ext.dir) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
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
    struct ::stat stt;
    return stat(file_pth, resolve_symlnk, stt, err_code) ? stt.st_ino : ~0ull;
}

inode_t get_file_inode(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    if (!resolve_symlnk || !is_symlink(directory_ent, false, err_code))
    {
        return directory_ent._ext.entry->d_ino;
    }
    
    struct ::stat stt;
    auto& directory_ent_ext = directory_ent._ext;
    auto* entry = directory_ent_ext.entry;
    if (::fstatat(::dirfd(directory_ent_ext.dir), entry->d_name, &stt, AT_SYMLINK_NOFOLLOW) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return ~0ull;
    }
    
    return stt.st_ino;
}

uid_t get_file_uid(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    struct ::stat stt;
    return stat(file_pth, resolve_symlnk, stt, err_code) ? stt.st_uid : -1;
}

gid_t get_file_gid(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    struct ::stat stt;
    return stat(file_pth, resolve_symlnk, stt, err_code) ? stt.st_gid : -1;
}

std::size_t get_file_size(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code
) noexcept
{
    struct ::stat stt;
    return stat(file_pth, resolve_symlnk, stt, err_code) ? (std::size_t)stt.st_size : ~0ull;
}

bool get_modification_time(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        system::time::system_time& system_tme,
        std::error_code* err_code
) noexcept
{
    struct ::stat stt;
    std::tm* local_tme;
    
    if (!stat(file_pth, resolve_symlnk, stt, err_code))
    {
        return false;
    }
    
    local_tme = std::localtime(&stt.st_mtime);
    
    system_tme.set_years(local_tme->tm_year + 1900)
              .set_months(local_tme->tm_mon + 1)
              .set_days(local_tme->tm_mday)
              .set_hours(local_tme->tm_hour)
              .set_minutes(local_tme->tm_min)
              .set_seconds(local_tme->tm_sec);

    return true;
}

const path_char_t* get_temporal_path() noexcept
{
    return "/tmp/";
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

bool is_file_type(const struct ::stat& stt, file_types file_typ) noexcept
{
    if (file_typ == file_types::NIL)
    {
        return true;
    }
    if ((file_typ & file_types::BLOCK_DEVICE) != file_types::NIL && S_ISBLK(stt.st_mode))
    {
        return true;
    }
    if ((file_typ & file_types::CHARACTER_DEVICE) != file_types::NIL && S_ISCHR(stt.st_mode))
    {
        return true;
    }
    if ((file_typ & file_types::DIRECTORY) != file_types::NIL && S_ISDIR(stt.st_mode))
    {
        return true;
    }
    if ((file_typ & file_types::PIPE) != file_types::NIL && S_ISFIFO(stt.st_mode))
    {
        return true;
    }
    if ((file_typ & file_types::REGULAR_FILE) != file_types::NIL && S_ISREG(stt.st_mode))
    {
        return true;
    }
    if ((file_typ & file_types::SOCKET) != file_types::NIL && S_ISSOCK(stt.st_mode))
    {
        return true;
    }
    if ((file_typ & file_types::SYMLINK) != file_types::NIL && S_ISLNK(stt.st_mode))
    {
        return true;
    }

    return false;
}

bool is_file_type(unsigned char d_type, file_types file_typ) noexcept
{
    if (file_typ == file_types::NIL)
    {
        return true;
    }
    if ((file_typ & file_types::BLOCK_DEVICE) != file_types::NIL && d_type == DT_BLK)
    {
        return true;
    }
    if ((file_typ & file_types::CHARACTER_DEVICE) != file_types::NIL && d_type == DT_CHR)
    {
        return true;
    }
    if ((file_typ & file_types::DIRECTORY) != file_types::NIL && d_type == DT_DIR)
    {
        return true;
    }
    if ((file_typ & file_types::PIPE) != file_types::NIL && d_type == DT_FIFO)
    {
        return true;
    }
    if ((file_typ & file_types::REGULAR_FILE) != file_types::NIL && d_type == DT_REG)
    {
        return true;
    }
    if ((file_typ & file_types::SOCKET) != file_types::NIL && d_type == DT_SOCK)
    {
        return true;
    }
    if ((file_typ & file_types::SYMLINK) != file_types::NIL && d_type == DT_LNK)
    {
        return true;
    }

    return false;
}

bool is_file_type(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        file_types file_typ,
        std::error_code* err_code
) noexcept
{
    struct ::stat stt;
    return stat(file_pth, resolve_symlnk, stt, err_code) && is_file_type(stt, file_typ);
}

bool is_file_type(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        file_types file_typ,
        std::error_code* err_code
) noexcept
{
    struct ::stat stt;
    auto& directory_ent_ext = directory_ent._ext;
    auto* entry = directory_ent_ext.entry;
    
    if (entry->d_type != DT_UNKNOWN && (entry->d_type != DT_LNK || !resolve_symlnk))
    {
        return is_file_type(entry->d_type, file_typ);
    }
    
    if (::fstatat(::dirfd(directory_ent_ext.dir), entry->d_name, &stt,
            resolve_symlnk ? 0 : AT_SYMLINK_NOFOLLOW) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return is_file_type(stt, file_typ);
}

bool is_pipe(const path_char_t* file_pth, bool resolve_symlnk, std::error_code* err_code) noexcept
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
    if (::mkdir(directory_pth, 0755) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}

bool mkdir_recursively(
        const path_char_t* directory_pth,
        std::error_code* err_code
) noexcept
{
    char parnt_path[PATH_MAX] = {0};
    std::size_t pth_len;
    size_t slash_pos[PATH_MAX];
    size_t slash_pos_sz = 0;
    char* lst_ch;
    
    pth_len = stringutils::cstr_length(directory_pth);
    
    if (pth_len >= PATH_MAX ||
        pth_len == 0 ||
        access(directory_pth, false, access_modes::EXISTS, err_code))
    {
        system::errors::assign_system_error_code(EINVAL, err_code);
        return false;
    }
    
    stringutils::cstr_copy(parnt_path, directory_pth);
    stringutils::cstr_remove_trailing_if(parnt_path, [](char ch) { return ch == '/'; });
    
    do
    {
        lst_ch = stringutils::cstr_cut(parnt_path, '/', true);
        
        if (lst_ch == nullptr)
        {
            if (!mkdir(parnt_path, err_code))
            {
                return false;
            }
            
            break;
        }

        pth_len = lst_ch - parnt_path;
        slash_pos[slash_pos_sz++] = pth_len;
        
    }
    while (!access(parnt_path, false, access_modes::EXISTS, err_code) && pth_len > 0);
    
    while (slash_pos_sz > 0)
    {
        parnt_path[slash_pos[--slash_pos_sz]] = '/';
        
         if (!mkdir(parnt_path, err_code))
         {
             return false;
         }
    }
    
    return true;
}

bool opendir(
        directory_entity& directory_ent,
        const char* directory_pth,
        std::error_code* err_code
) noexcept
{
    if ((directory_ent._ext.dir = ::opendir(directory_pth)) == nullptr)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}

bool readdir(
        directory_entity& directory_ent,
        std::error_code* err_code
) noexcept
{
    auto& directory_ent_ext = directory_ent._ext;
    
    errno = 0;
    directory_ent_ext.entry = ::readdir(directory_ent_ext.dir);
    if (directory_ent_ext.entry == nullptr)
    {
        if (errno != 0)
        {
            system::errors::assign_system_error_code(errno, err_code);
        }
        
        return false;
    }
    
    directory_ent.nme = directory_ent_ext.entry->d_name;
    
    return true;
}

bool rmdir(const path_char_t* directory_pth, std::error_code* err_code) noexcept
{
    if (::rmdir(directory_pth) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
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
    return symlink(target_pth, shortcut_pth, err_code);
}

bool stat(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        struct ::stat& stt,
        std::error_code* err_code
) noexcept
{
    if (resolve_symlnk)
    {
        if (::stat(file_pth, &stt) == -1)
        {
            system::errors::assign_system_error_code(errno, err_code);
            return false;
        }
    }
    else if (::lstat(file_pth, &stt) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}

bool symlink(
        const path_char_t* target_pth,
        const path_char_t* link_pth,
        std::error_code* err_code
) noexcept
{
    if (::symlink(target_pth, link_pth) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}

bool touch(const path_char_t* regular_file_pth, std::error_code* err_code) noexcept
{
    if (::mknod(regular_file_pth, 0755, S_IFREG) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}

bool unlink(const path_char_t* regular_file_pth, std::error_code* err_code) noexcept
{
    if (::unlink(regular_file_pth) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }

    return true;
}

}

#endif

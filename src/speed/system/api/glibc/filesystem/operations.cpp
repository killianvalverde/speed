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
 * @file        operations.cpp
 * @brief       filesystem operations source.
 * @author      Killian Valverde
 * @date        2017/05/26
 */

#include "../../../compatibility/compatibility.hpp"
#ifdef SPEED_GLIBC

#include "operations.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <cstdlib>

#include "../../../../stringutils/stringutils.hpp"
#include "../../../codecs/codecs.hpp"

namespace speed::system::api::glibc::filesystem {

bool access(
        const char* file_pth,
        system::filesystem::access_modes access_mods,
        std::error_code* err_code
) noexcept
{
    decltype(F_OK) mode_buildr = 0;

    if (access_mods == system::filesystem::access_modes::NIL)
    {
        return true;
    }
    if ((access_mods & system::filesystem::access_modes::EXISTS) !=
                system::filesystem::access_modes::NIL)
    {
        mode_buildr |= F_OK;
    }
    if ((access_mods & system::filesystem::access_modes::READ) !=
                system::filesystem::access_modes::NIL)
    {
        mode_buildr |= R_OK;
    }
    if ((access_mods & system::filesystem::access_modes::WRITE) !=
                system::filesystem::access_modes::NIL)
    {
        mode_buildr |= W_OK;
    }
    if ((access_mods & system::filesystem::access_modes::EXECUTE) !=
                system::filesystem::access_modes::NIL)
    {
        mode_buildr |= X_OK;
    }
    
    if (::access(file_pth, mode_buildr) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}

bool access(
        const wchar_t* file_pth,
        system::filesystem::access_modes access_mods,
        std::error_code* err_code
) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(file_pth, &str))
    {
        return access(str.c_str(), access_mods, err_code);
    }
    return false;
}

bool access(
        const char* file_pth,
        system::filesystem::access_modes access_mods,
        system::filesystem::file_types file_typ,
        std::error_code* err_code
) noexcept
{
    return (is_file_type(file_pth, file_typ, err_code) &&
            access(file_pth, access_mods, err_code));
}

bool access(
        const wchar_t* file_pth,
        system::filesystem::access_modes access_mods,
        system::filesystem::file_types file_typ,
        std::error_code* err_code
) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(file_pth, &str))
    {
        return access(str.c_str(), access_mods, file_typ, err_code);
    }
    return false;
}

bool can_directory_be_created(const char* directory_pth, std::error_code* err_code) noexcept
{
    char parent_pth[PATH_MAX] = {0};
    std::size_t dir_path_len = stringutils::strlen(directory_pth);
    char* last_char_p;

    if (dir_path_len >= PATH_MAX ||
        dir_path_len == 0 ||
        access(directory_pth, system::filesystem::access_modes::EXISTS, err_code))
    {
        return false;
    }

    stringutils::strcpy(parent_pth, directory_pth);
    stringutils::strrmlast(parent_pth, '/');
    last_char_p = stringutils::strcut(parent_pth, '/');
    dir_path_len = last_char_p == nullptr ? 0 : parent_pth - last_char_p + 1;

    if (dir_path_len == 0)
    {
        parent_pth[0] = '.';
        parent_pth[1] = '\0';
    }

    return access(parent_pth, system::filesystem::access_modes::WRITE |
            system::filesystem::access_modes::EXECUTE, err_code);
}

bool can_directory_be_created(const wchar_t* directory_pth, std::error_code* err_code) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(directory_pth, &str))
    {
        return can_directory_be_created(str.c_str(), err_code);
    }
    return false;
}

bool can_regular_file_be_created(const char* regular_file_pth, std::error_code* err_code) noexcept
{
    char parent_pth[PATH_MAX] = {0};
    std::size_t path_len = stringutils::strlen(regular_file_pth);
    char* last_char_p;

    if (path_len >= PATH_MAX || path_len == 0)
    {
        return false;
    }

    if (access(regular_file_pth, system::filesystem::access_modes::EXISTS, err_code))
    {
        return access(regular_file_pth, system::filesystem::access_modes::WRITE,
                      system::filesystem::file_types::REGULAR_FILE, err_code);
    }

    stringutils::strcpy(parent_pth, regular_file_pth);
    stringutils::strrmlast(parent_pth, '/');
    last_char_p = stringutils::strcut(parent_pth, '/');
    path_len = last_char_p == nullptr ? 0 : parent_pth - last_char_p + 1;

    if (path_len == 0)
    {
        parent_pth[0] = '.';
        parent_pth[1] = '\0';
    }

    return access(parent_pth, system::filesystem::access_modes::WRITE |
            system::filesystem::access_modes::EXECUTE, err_code);
}

bool can_regular_file_be_created(
        const wchar_t* regular_file_pth,
        std::error_code* err_code
) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(regular_file_pth, &str))
    {
        return can_regular_file_be_created(str.c_str(), err_code);
    }
    return false;
}

bool chdir(const char* directory_pth, std::error_code* err_code) noexcept
{
    if (::chdir(directory_pth) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}

bool chdir(const wchar_t* directory_pth, std::error_code* err_code) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(directory_pth, &str))
    {
        return chdir(str.c_str(), err_code);
    }
    return false;
}

bool closedir(
        system::filesystem::directory_entity* directory_ent,
        std::error_code* err_code
) noexcept
{
    auto* directory_ent_ext = &directory_ent->__priv;
    
    if (::closedir(directory_ent_ext->dir) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
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
    
    if (::closedir(directory_ent_ext->dir) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
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

system::filesystem::inode_t get_file_inode(
        const char* file_pth,
        std::error_code* err_code
) noexcept
{
    struct ::stat stt;
    errno = 0;
    
    if (::lstat(file_pth, &stt) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return ~0ull;
    }
    
    return stt.st_ino;
}

system::filesystem::inode_t get_file_inode(
        const wchar_t* file_pth,
        std::error_code* err_code
) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(file_pth, &str))
    {
        return get_file_inode(str.c_str(), err_code);
    }
    return false;
}

system::filesystem::inode_t get_file_inode(
        system::filesystem::directory_entity* directory_ent,
        std::error_code* err_code
) noexcept
{
    return directory_ent->__priv.ino;
}

system::filesystem::inode_t get_file_inode(
        system::filesystem::wdirectory_entity* directory_ent,
        std::error_code* err_code
) noexcept
{
    return directory_ent->__priv.ino;
}

uid_t get_file_uid(const char* file_pth, std::error_code* err_code) noexcept
{
    struct ::stat stt;
    errno = 0;
    
    if (::stat(file_pth, &stt) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return -1;
    }
    
    return stt.st_uid;
}

uid_t get_file_uid(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(file_pth, &str))
    {
        return get_file_uid(str.c_str(), err_code);
    }
    return false;
}

gid_t get_file_gid(const char* file_pth, std::error_code* err_code) noexcept
{
    struct ::stat stt;
    errno = 0;
    
    if (::stat(file_pth, &stt) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return -1;
    }
    
    return stt.st_gid;
}

gid_t get_file_gid(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(file_pth, &str))
    {
        return get_file_gid(str.c_str(), err_code);
    }
    return false;
}

std::size_t get_file_size(const char* file_pth, std::error_code* err_code) noexcept
{
    struct stat file_stt;

    if (::stat(file_pth, &file_stt) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return ~0ull;
    }

    return (std::size_t)file_stt.st_size;
}

std::size_t get_file_size(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(file_pth, &str))
    {
        return get_file_size(str.c_str(), err_code);
    }
    return false;
}

bool get_modification_time(
        const char* file_pth,
        system::time::system_time* system_tme,
        std::error_code* err_code
) noexcept
{
    struct ::stat stt;
    std::tm* local_tme;
    
    if (stat(file_pth, &stt) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    local_tme = std::localtime(&stt.st_mtime);
    
    system_tme->set_years(local_tme->tm_year + 1900)
               .set_months(local_tme->tm_mon + 1)
               .set_days(local_tme->tm_mday)
               .set_hours(local_tme->tm_hour)
               .set_minutes(local_tme->tm_min)
               .set_seconds(local_tme->tm_sec);

    return true;
}

bool get_modification_time(
        const wchar_t* file_pth,
        system::time::system_time* system_tme,
        std::error_code* err_code
) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(file_pth, &str))
    {
        return get_modification_time(str.c_str(), system_tme, err_code);
    }
    return false;
}

const char* get_temporal_path() noexcept
{
    return "/tmp/";
}

bool is_block_device(const char* file_pth, std::error_code* err_code) noexcept
{
    struct ::stat infos;
    errno = 0;
    
    if (::stat(file_pth, &infos) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return S_ISBLK(infos.st_mode);
}

bool is_block_device(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(file_pth, &str))
    {
        return is_block_device(str.c_str(), err_code);
    }
    return false;
}

bool is_character_device(const char* file_pth, std::error_code* err_code) noexcept
{
    struct ::stat infos;
    errno = 0;
    
    if (::stat(file_pth, &infos) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return S_ISCHR(infos.st_mode);
}

bool is_character_device(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(file_pth, &str))
    {
        return is_character_device(str.c_str(), err_code);
    }
    return false;
}

bool is_directory(const char* file_pth, std::error_code* err_code) noexcept
{
    struct ::stat infos;
    errno = 0;
    
    if (::stat(file_pth, &infos) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return S_ISDIR(infos.st_mode);
}

bool is_directory(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(file_pth, &str))
    {
        return is_directory(str.c_str(), err_code);
    }
    return false;
}

bool is_directory(
        system::filesystem::directory_entity* directory_ent,
        std::error_code* err_code
) noexcept
{
    auto* directory_ent_ext = &directory_ent->__priv;
    auto* entry = directory_ent_ext->entry;
    
    if (entry->d_type != DT_UNKNOWN)
    {
        return entry->d_type == DT_DIR;
    }
    
    struct ::stat st;
    ::fstatat(::dirfd(directory_ent_ext->dir), entry->d_name, &st, AT_SYMLINK_NOFOLLOW);
    
    return S_ISDIR(st.st_mode);
}

bool is_directory(
        system::filesystem::wdirectory_entity* directory_ent,
        std::error_code* err_code
) noexcept
{
    auto* directory_ent_ext = &directory_ent->__priv;
    auto* entry = directory_ent_ext->entry;
    
    if (entry->d_type != DT_UNKNOWN)
    {
        return entry->d_type == DT_DIR;
    }
    
    struct ::stat st;
    ::fstatat(::dirfd(directory_ent_ext->dir), entry->d_name, &st, AT_SYMLINK_NOFOLLOW);
    
    return S_ISDIR(st.st_mode);
}

bool is_file_type(
        const char* file_pth,
        system::filesystem::file_types file_typ,
        std::error_code* err_code
) noexcept
{
    if ((file_typ & system::filesystem::file_types::BLOCK_DEVICE) !=
                system::filesystem::file_types::NIL && is_block_device(file_pth, err_code))
    {
        return true;
    }
    if ((file_typ & system::filesystem::file_types::CHARACTER_DEVICE) !=
                system::filesystem::file_types::NIL && is_character_device(file_pth, err_code))
    {
        return true;
    }
    if ((file_typ & system::filesystem::file_types::DIRECTORY) !=
                system::filesystem::file_types::NIL && is_directory(file_pth, err_code))
    {
        return true;
    }
    if ((file_typ & system::filesystem::file_types::PIPE) !=
                system::filesystem::file_types::NIL && is_pipe(file_pth, err_code))
    {
        return true;
    }
    if ((file_typ & system::filesystem::file_types::REGULAR_FILE) !=
                system::filesystem::file_types::NIL && is_regular_file(file_pth, err_code))
    {
        return true;
    }
    if ((file_typ & system::filesystem::file_types::SOCKET) !=
                system::filesystem::file_types::NIL && is_socket(file_pth, err_code))
    {
        return true;
    }
    if ((file_typ & system::filesystem::file_types::SYMLINK) !=
                system::filesystem::file_types::NIL && is_symlink(file_pth, err_code))
    {
        return true;
    }

    return false;
}

bool is_file_type(
        const wchar_t* file_pth,
        system::filesystem::file_types file_typ,
        std::error_code* err_code
) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(file_pth, &str))
    {
        return is_file_type(str.c_str(), file_typ, err_code);
    }
    return false;
}

bool is_pipe(const char* file_pth, std::error_code* err_code) noexcept
{
    struct ::stat infos;
    
    if (::stat(file_pth, &infos) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return S_ISFIFO(infos.st_mode);
}

bool is_pipe(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(file_pth, &str))
    {
        return is_pipe(str.c_str(), err_code);
    }
    return false;
}

bool is_regular_file(const char* file_pth, std::error_code* err_code) noexcept
{
    struct ::stat infos;
    
    if (::stat(file_pth, &infos) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return S_ISREG(infos.st_mode);
}

bool is_regular_file(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(file_pth, &str))
    {
        return is_regular_file(str.c_str(), err_code);
    }
    return false;
}

bool is_socket(const char* file_pth, std::error_code* err_code) noexcept
{
    struct ::stat infos;
    
    if (::stat(file_pth, &infos) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return S_ISSOCK(infos.st_mode);
}

bool is_socket(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(file_pth, &str))
    {
        return is_socket(str.c_str(), err_code);
    }
    return false;
}

bool is_symlink(const char* file_pth, std::error_code* err_code) noexcept
{
    struct ::stat infos;
    
    if (::lstat(file_pth, &infos) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return S_ISLNK(infos.st_mode);
}

bool is_symlink(const wchar_t* file_pth, std::error_code* err_code) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(file_pth, &str))
    {
        return is_symlink(str.c_str(), err_code);
    }
    return false;
}

bool mkdir(const char* directory_pth, std::error_code* err_code) noexcept
{
    if (::mkdir(directory_pth, 0755) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}

bool mkdir(const wchar_t* directory_pth, std::error_code* err_code) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(directory_pth, &str))
    {
        return mkdir(str.c_str(), err_code);
    }
    return false;
}

bool mkdir_recursively(
        const char* directory_pth,
        std::error_code* err_code
) noexcept
{
    char parnt_path[PATH_MAX] = {0};
    std::size_t pth_len;
    size_t slash_pos[PATH_MAX];
    size_t slash_pos_sz = 0;
    char* lst_ch;
    
    pth_len = stringutils::strlen(directory_pth);
    
    if (pth_len >= PATH_MAX ||
        pth_len == 0 ||
        access(directory_pth, system::filesystem::access_modes::EXISTS, err_code))
    {
        system::errors::assign_system_error_code(EINVAL, err_code);
        return false;
    }
    
    stringutils::strcpy(parnt_path, directory_pth);
    stringutils::strrmlast(parnt_path, '/');
    
    do
    {
        lst_ch = stringutils::strcut(parnt_path, '/', true);
        
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
        
    } while (!access(parnt_path, system::filesystem::access_modes::EXISTS, err_code) &&
             pth_len > 0);
    
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

bool mkdir_recursively(
        const wchar_t* directory_pth,
        std::error_code* err_code
) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(directory_pth, &str))
    {
        return mkdir(str.c_str(), err_code);
    }
    return false;
}

bool opendir(
        system::filesystem::directory_entity* directory_ent,
        const char* directory_pth,
        std::error_code* err_code
) noexcept
{
    auto* directory_ent_ext = &directory_ent->__priv;
    
    if ((directory_ent_ext->dir = ::opendir(directory_pth)) == nullptr)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}

bool opendir(
        system::filesystem::wdirectory_entity* directory_ent,
        const wchar_t* directory_pth,
        std::error_code* err_code
) noexcept
{
    std::string str;
    if (!codecs::convert_w_str_to_string(directory_pth, &str))
    {
        return false;
    }

    auto* directory_ent_ext = &directory_ent->__priv;

    if ((directory_ent_ext->dir = ::opendir(str.c_str())) == nullptr)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }

    return true;
}

bool readdir(
        system::filesystem::directory_entity* directory_ent,
        std::error_code* err_code
) noexcept
{
    auto* directory_ent_ext = &directory_ent->__priv;
    
    errno = 0;
    directory_ent_ext->entry = ::readdir(directory_ent_ext->dir);
    if (directory_ent_ext->entry == nullptr)
    {
        if (errno != 0)
        {
            system::errors::assign_system_error_code(errno, err_code);
        }
        
        return false;
    }
    
    directory_ent_ext->ino = directory_ent_ext->entry->d_ino;
    directory_ent->nme = directory_ent_ext->entry->d_name;
    
    return true;
}

bool readdir(
        system::filesystem::wdirectory_entity* directory_ent,
        std::error_code* err_code
) noexcept
{
    auto* directory_ent_ext = &directory_ent->__priv;

    errno = 0;
    directory_ent_ext->entry = ::readdir(directory_ent_ext->dir);
    if (directory_ent_ext->entry == nullptr)
    {
        if (errno != 0)
        {
            system::errors::assign_system_error_code(errno, err_code);
        }
        
        return false;
    }

    directory_ent_ext->ino = directory_ent_ext->entry->d_ino;
    if (!codecs::convert_c_str_to_wstring(directory_ent_ext->entry->d_name,
                                          &directory_ent_ext->name_holdr))
    {
        return false;
    }
    directory_ent->nme = &directory_ent_ext->name_holdr[0];

    return true;
}

bool rmdir(const char* directory_pth, std::error_code* err_code) noexcept
{
    if (::rmdir(directory_pth) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}

bool rmdir(const wchar_t* directory_pth, std::error_code* err_code) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(directory_pth, &str))
    {
        return rmdir(str.c_str(), err_code);
    }
    return false;
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

bool symlink(const char* target_pth, const char* link_pth, std::error_code* err_code) noexcept
{
    if (::symlink(target_pth, link_pth) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}

bool symlink(const wchar_t* target_pth, const wchar_t* link_pth, std::error_code* err_code) noexcept
{
    std::string trg_str;
    std::string lnk_str;
    if (codecs::convert_w_str_to_string(target_pth, &trg_str) &&
        codecs::convert_w_str_to_string(link_pth, &lnk_str))
    {
        return symlink(trg_str.c_str(), lnk_str.c_str(), err_code);
    }
    return false;
}

bool touch(const char* regular_file_pth, std::error_code* err_code) noexcept
{
    if (::mknod(regular_file_pth, 0755, S_IFREG) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}

bool touch(const wchar_t* regular_file_pth, std::error_code* err_code) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(regular_file_pth, &str))
    {
        return touch(str.c_str(), err_code);
    }
    return false;
}

bool unlink(const char* regular_file_pth, std::error_code* err_code) noexcept
{
    if (::unlink(regular_file_pth) == -1)
    {
        system::errors::assign_system_error_code(errno, err_code);
        return false;
    }

    return true;
}

bool unlink(const wchar_t* regular_file_pth, std::error_code* err_code) noexcept
{
    std::string str;
    if (codecs::convert_w_str_to_string(regular_file_pth, &str))
    {
        return unlink(str.c_str(), err_code);
    }
    return false;
}

}

#endif

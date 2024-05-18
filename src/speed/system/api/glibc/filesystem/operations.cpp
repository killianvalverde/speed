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

#include "../../../type_traits/type_traits.hpp"
#ifdef SPEED_GLIBC

#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>

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
    bool check = false;
    
    if ((acss_modes & access_modes::EXISTS) != access_modes::NIL)
    {
        check = true;
        mode_buildr |= F_OK;
    }
    if ((acss_modes & access_modes::READ) != access_modes::NIL)
    {
        check = true;
        mode_buildr |= R_OK;
    }
    if ((acss_modes & access_modes::WRITE) != access_modes::NIL)
    {
        check = true;
        mode_buildr |= W_OK;
    }
    if ((acss_modes & access_modes::EXECUTE) != access_modes::NIL)
    {
        check = true;
        mode_buildr |= X_OK;
    }
    
    if (check && ::access(fle_path, mode_buildr) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
}


bool access(
        const char* fle_path,
        access_modes acss_modes,
        file_type fle_type,
        std::error_code* err_code
) noexcept
{
    return (is_file_type(fle_path, fle_type, err_code) && 
            access(fle_path, acss_modes, err_code));
}


bool can_directory_be_created(const char* dir_path, std::error_code* err_code)
{
    static_assert(PATH_MAX >= 255, "PATH_MAX has to be at least 255.");
    
    char parnt_path[PATH_MAX] = {0};
    std::size_t dir_path_len;
    errno = 0;
    
    dir_path_len = stringutils::strlen(dir_path);
    
    if (dir_path_len >= PATH_MAX ||
        dir_path_len == 0 ||
        file_exists(dir_path))
    {
        return false;
    }
    
    stringutils::strcpy(parnt_path, dir_path);
    
    if (!get_first_actual_directory(parnt_path, err_code))
    {
        return false;
    }
    
    return access(parnt_path, access_modes::WRITE | access_modes::EXECUTE, err_code);
}


bool can_regular_file_be_created(const char* reg_file_path, std::error_code* err_code)
{
    static_assert(PATH_MAX >= 255, "PATH_MAX has to be at least 255.");
    
    char parnt_path[PATH_MAX] = {0};
    std::size_t reg_file_path_len;
    errno = 0;
    
    reg_file_path_len = stringutils::strlen(reg_file_path);
    
    if (reg_file_path_len >= PATH_MAX ||
        reg_file_path_len == 0)
    {
        return false;
    }
    
    if (file_exists(reg_file_path))
    {
        return is_regular_file(reg_file_path, err_code) &&
               access(reg_file_path, access_modes::WRITE, err_code);
    }
    
    stringutils::strcpy(parnt_path, reg_file_path);
    
    if (!get_first_actual_directory(parnt_path, err_code))
    {
        return false;
    }
    
    return access(parnt_path, access_modes::WRITE | access_modes::EXECUTE, err_code);
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


bool file_exists(const char* fle_path, std::error_code* err_code) noexcept
{
    return access(fle_path, access_modes::EXISTS, err_code);
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
        if (file_exists(pth, err_code) && is_directory(pth, err_code))
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


uint64_t get_file_inode(const char* fle_path, std::error_code* err_code) noexcept
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


const char* get_tmp_path() noexcept
{
    return "/tmp";
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


bool is_file_type(const char* fle_path, file_type fle_type, std::error_code* err_code) noexcept
{
    bool scs;
    
    switch (fle_type)
    {
        case file_type::NIL:
            scs = true;
            break;
        
        case file_type::BLOCK_DEVICE:
            scs = is_block_device(fle_path, err_code);
            break;
        
        case file_type::CHARACTER_DEVICE:
            scs = is_character_device(fle_path, err_code);
            break;
        
        case file_type::DIRECTORY:
            scs = is_directory(fle_path, err_code);
            break;
        
        case file_type::FIFO:
            scs = is_fifo(fle_path, err_code);
            break;
        
        case file_type::REGULAR_FILE:
            scs = is_regular_file(fle_path, err_code);
            break;
        
        case file_type::SOCKET:
            scs = is_socket(fle_path, err_code);
            break;
        
        case file_type::SYMLINK:
            scs = is_symlink(fle_path, err_code);
            break;
        
        default:
            scs = false;
    }
    
    return scs;
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
        file_exists(dir_path))
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
        
        slash_pos[slash_pos_sz] = lst_ch - parnt_path;
        ++slash_pos_sz;
        
        pth_len = stringutils::strlen(parnt_path);
        
    } while (!file_exists(parnt_path, err_code) && pth_len > 0);
    
    while (slash_pos_sz > 0)
    {
        parnt_path[slash_pos[slash_pos_sz - 1]] = '/';
        --slash_pos_sz;
        
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


bool rmdir(const char* dir_path, std::error_code* err_code) noexcept
{
    errno = 0;
    
    if (::rmdir(dir_path) == -1)
    {
        assign_system_error_code(errno, err_code);
        return false;
    }
    
    return true;
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


}


#endif

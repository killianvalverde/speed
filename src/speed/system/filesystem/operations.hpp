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
 * @file        speed/system/filesystem/operations.hpp
 * @brief       filesystem operations header.
 * @author      Killian Valverde
 * @date        2017/05/26
 */

#ifndef SPEED_SYSTEM_FILESYSTEM_OPERATIONS_HPP
#define SPEED_SYSTEM_FILESYSTEM_OPERATIONS_HPP

#include <cstdint>

#include "../api/api.hpp"
#include "../type_traits/type_traits.hpp"
#include "access_modes.hpp"
#include "directory_entity.hpp"
#include "file_type.hpp"


namespace speed::system::filesystem {


/**
 * @brief       Checks whether the calling process can access the file path. If pathname is a
 *              symbolic link, it is dereferenced.
 * @param       fle_path : The file path.
 * @param       acss_modes : Specifies the accessibility check(s) to be performed.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the true is returned, otherwise false is returned.
 */
inline bool access(
        const char* fle_path,
        access_modes acss_modes,
        std::error_code* err_code = nullptr
)
{
    return SPEED_SELECT_API(filesystem::access, false, fle_path, acss_modes, err_code);
}


/**
 * @brief       Checks whether the calling process can access the file path. If pathname is a
 *              symbolic link, it is dereferenced.
 * @param       fle_path : The file path.
 * @param       acss_modes : Specifies the accessibility check(s) to be performed.
 * @param       fle_type : The file type.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the true is returned, otherwise false is returned.
 */
inline bool access(
        const char* fle_path,
        access_modes acss_modes,
        file_type fle_type,
        std::error_code* err_code = nullptr
)
{
    return SPEED_SELECT_API(filesystem::access, false, fle_path, acss_modes, fle_type, err_code);
}


/**
 * @brief       Change the current execution directory.
 * @param       dir_path : The path of the new current directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool chdir(const char* dir_path, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::chdir, false, dir_path, err_code);
}


/**
 * @brief       Closes the directory stream.
 * @param       dir_ent : The directory entity.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful a pointer to the directory is returned, otherwise nullptr
 *              is returned.
 */
inline bool closedir(directory_entity* dir_ent, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::closedir, false, dir_ent, err_code);
}


/**
 * @brief       Get the inode number of the specified file.
 * @param       fle_path : The file to get the inode number.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the inode number of the sepcified file is returned, otherwise -1 is
 *              returned.
 */
inline uint64_t get_file_inode(const char* fle_path, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::get_file_inode, -1, fle_path, err_code);
}


/**
 * @brief       Get the UID of the specified file.
 * @param       fle_path : The file to get the UID.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the UID of the sepcified file is returned, otherwise -1 is returned.
 */
inline int get_file_uid(const char* fle_path, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::get_file_uid, -1, fle_path, err_code);
}


/**
 * @brief       Get the GID of the specified file.
 * @param       fle_path : The file to get the GID.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the GID of the sepcified file is returned, otherwise -1 is returned..
 */
inline int get_file_gid(const char* fle_path, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::get_file_gid, -1, fle_path, err_code);
}


/**
 * @brief       Get the tmp system path.
 * @return      If function was successful the tmp system path is returned otherwise a null pointer
 *              is returned.
 */
inline const char* get_tmp_path() noexcept
{
    return SPEED_SELECT_API(filesystem::get_tmp_path, nullptr);
}


/**
 * @brief       Checks if the given path corresponds to a block device.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_block_device(const char* fle_path, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::is_block_device, false, fle_path, err_code);
}


/**
 * @brief       Checks if the given path corresponds to a character device.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_character_device(const char* fle_path, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::is_character_device, false, fle_path, err_code);
}


/**
 * @brief       Checks if the given path corresponds to a directory.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_directory(const char* fle_path, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::is_directory, false, fle_path, err_code);
}


/**
 * @brief       Checks if the given path corresponds to a named pipe.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_fifo(const char* fle_path, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::is_fifo, false, fle_path, err_code);
}


/**
 * @brief       Checks if the given path corresponds to a specified file type.
 * @param       fle_path : Path to check.
 * @param       fle_type : The specified file type.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_file_type(
        const char* fle_path, 
        file_type fle_type, 
        std::error_code* err_code
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_file_type, false, fle_path, fle_type, err_code);
}


/**
 * @brief       Checks if the given path corresponds to a regular file.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_regular_file(const char* fle_path, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::is_regular_file, false, fle_path, err_code);
}


/**
 * @brief       Checks if the given path corresponds to a socket.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_socket(const char* fle_path, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::is_socket, false, fle_path, err_code);
}


/**
 * @brief       Checks if the given path corresponds to a socket.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_symlink(const char* fle_path, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::is_symlink, false, fle_path, err_code);
}


// TODO: Maybe the modes should have their own data type.
/**
 * @brief       Attempts to create a directory.
 * @param       dir_path : The path of the new directory.
 * @param       mods : Specifies the mode for the new directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool mkdir(
        const char* dir_path,
        std::uint32_t mods = 0755,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::mkdir, false, dir_path, mods, err_code);
}


/**
 * @brief       Attemps to create a directory path.
 * @param       dir_path : The path of directories to create.
 * @param       mods : Specifies the mode for the new directories.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool mkdir_recursively(
        const char* dir_path,
        std::uint32_t mods = 0755,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::mkdir_recursively, false, dir_path, mods, err_code);
}


/**
 * @brief       Opens a directory stream corresponding to the directory name, and returns a pointer
 *              to the directory stream.
 * @param       dir_ent : The directory entity.
 * @param       dir_pth : The path of the directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool opendir(
        directory_entity* dir_ent,
        const char *dir_pth,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::opendir, false, dir_ent, dir_pth, err_code);
}


/**
 * @brief       Read the next directory entry in the directory stream.
 * @param       dir_ent : The current directory entity.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool readdir(directory_entity* dir_ent, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::readdir, false, dir_ent, err_code);
}


/**
 * @brief       Delete the specified directory.
 * @param       dir_path : The path of the directory to delete.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool rmdir(const char* dir_path, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::rmdir, false, dir_path, err_code);
}


/**
 * @brief       Creates a symbolic link named lnk_pth which contains the string trg.
 * @param       trg : The string to contain in the symlink.
 * @param       lnk_pth : The symbolilc link name.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool symlink(
        const char* trg,
        const char* lnk_pth,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::symlink, false, trg, lnk_pth, err_code);
}


// TODO: Test this function.
/**
 * @brief       Attempts to create a regular file.
 * @param       regfle_path : The path of the new regular file.
 * @param       mods : Specifies the mode for the new regular file.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool touch(
        const char* regfle_path,
        std::uint32_t mods = 0755,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::touch, false, regfle_path, mods, err_code);
}


}


#endif

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
 * @file        operations.hpp
 * @brief       filesystem operations header.
 * @author      Killian Valverde
 * @date        2017/05/26
 */

#ifndef SPEED_SYSTEM_FILESYSTEM_OPERATIONS_HPP
#define SPEED_SYSTEM_FILESYSTEM_OPERATIONS_HPP

#include <cstdint>

#include "../platform/platform.hpp"
#include "../detail/detail.hpp"
#include "../process/process.hpp"
#include "../time/time.hpp"
#include "access_modes.hpp"
#include "directory_entity.hpp"
#include "file_types.hpp"
#include "types.hpp"

namespace speed::system::filesystem {

/**
 * @brief       Checks the accessibility of a file with specified access modes.
 * @param       file_pth : The path to the file to check.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       access_mods : The access modes to test (e.g., read, write, execute).
 * @param       err_code : Optional pointer to an error code object to receive error details.
 * @return      `true` if the file can be accessed with the specified modes, otherwise `false`.
 */
inline bool access(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        access_modes access_mods,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::access, false, file_pth, resolve_symlnk, access_mods,
            err_code);
}

/**
 * @brief       Change the current execution directory.
 * @param       directory_pth : The path of the new current directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool chdir(const path_char_t* directory_pth, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::chdir, false, directory_pth, err_code);
}

/**
 * @brief       Checks whether the calling process can access the file path. If pathname is a
 *              symbolic link, it is dereferenced.
 * @param       file_pth : The file path.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       access_mods : Specifies the accessibility check(s) to be performed.
 * @param       file_typs : File types that the file must match with at least once.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the true is returned, otherwise false is returned.
 */
inline bool check_file(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        access_modes access_mods,
        file_types file_typs,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::check_file, false, file_pth, resolve_symlnk, access_mods,
            file_typs, err_code);
}

/**
 * @brief       Closes the directory stream.
 * @param       directory_ent : The directory entity.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful a pointer to the directory is returned, otherwise nullptr
 *              is returned.
 */
inline bool closedir(directory_entity& directory_ent, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::closedir, false, directory_ent, err_code);
}

/**
 * @brief       Check if a file exists.
 * @param       file_pth : The path of the file to check.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool file_exists(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::file_exists, false, file_pth, resolve_symlnk, err_code);
}

/**
 * @brief       Get the inode number of the specified file.
 * @param       file_pth : The file to get the inode number.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the inode number of the sepcified file is returned, otherwise -1 is
 *              returned.
 */
inline inode_t get_file_inode(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::get_file_inode, -1, file_pth, resolve_symlnk, err_code);
}

/**
 * @brief       Get the inode number of the specified file.
 * @param       directory_ent : The directory entity current file to get the inode number.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the inode number of the sepcified file is returned, otherwise -1 is
 *              returned.
 */
inline inode_t get_file_inode(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::get_file_inode, -1, directory_ent, resolve_symlnk,
            err_code);
}

/**
 * @brief       Get the UID of the specified file.
 * @param       file_pth : The file to get the UID.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the UID of the sepcified file is returned, otherwise -1 is returned.
 */
inline process::uid_t get_file_uid(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::get_file_uid, -1, file_pth, resolve_symlnk, err_code);
}

/**
 * @brief       Get the GID of the specified file.
 * @param       file_pth : The file to get the GID.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the GID of the sepcified file is returned, otherwise -1 is returned..
 */
inline process::gid_t get_file_gid(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::get_file_gid, -1, file_pth, resolve_symlnk, err_code);
}

/**
 * @brief       Get the size in bytes of a specified file path.
 * @param       file_pth : The path of the file to get the size.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the file size in bytes in returned, otherwise -1 is returned.
 */
inline std::size_t get_file_size(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::get_file_size, ~0ull, file_pth, resolve_symlnk, err_code);
}

/**
 * @brief       Get the modification time of the specified file.
 * @param       file_pth : The file to get the modification time.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       system_tme : The object in which store the modification time.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool get_modification_time(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        time::system_time& system_tme,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::get_modification_time, false, file_pth, resolve_symlnk,
            system_tme, err_code);
}

/**
 * @brief       Get a temporal path.
 * @return      If function was successful a temporal path is returned otherwise a null pointer
 *              is returned.
 */
inline const path_char_t* get_temporal_path() noexcept
{
    return SPEED_SELECT_API(filesystem::get_temporal_path, nullptr);
}

/**
 * @brief       Checks if the given path corresponds to a block device.
 * @param       file_pth : Path to check.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_block_device(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_block_device, false, file_pth, resolve_symlnk,
            err_code);
}

/**
 * @brief       Checks if the given directory_entity corresponds to a block device.
 * @param       directory_ent : Directory entity to check.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_block_device(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_block_device, false, directory_ent, resolve_symlnk,
            err_code);
}

/**
 * @brief       Checks if the given path corresponds to a character device.
 * @param       file_pth : Path to check.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_character_device(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_character_device, false, file_pth, resolve_symlnk,
            err_code);
}

/**
 * @brief       Checks if the given directory entity corresponds to a character device.
 * @param       directory_ent : Directory entity to check.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_character_device(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_character_device, false, directory_ent, resolve_symlnk,
            err_code);
}

/**
 * @brief       Checks if the given path corresponds to a directory.
 * @param       file_pth : Path to check.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_directory(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_directory, false, file_pth, resolve_symlnk, err_code);
}

/**
 * @brief       Checks if the given path corresponds to a directory.
 * @param       directory_ent : The directory entity.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_directory(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_directory, false, directory_ent, resolve_symlnk,
            err_code);
}

/**
 * @brief       Checks if the given path corresponds to a specified file type.
 * @param       file_pth : Path to check.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       file_typ : The specified file type. If more than one flag is set, the function will
 *              verify if at least one of the type is matching.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_file_type(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        file_types file_typ,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_file_type, false, file_pth, resolve_symlnk, file_typ,
            err_code);
}

/**
 * @brief       Checks if the given path corresponds to a specified file type.
 * @param       directory_ent : The directory entity.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       file_typ : The specified file type. If more than one flag is set, the function will
 *              verify if at least one of the type is matching.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_file_type(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        file_types file_typ,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_file_type, false, directory_ent, resolve_symlnk,
            file_typ, err_code);
}

/**
 * @brief       Checks if the given path corresponds to a named pipe.
 * @param       file_pth : Path to check.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_pipe(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_pipe, false, file_pth, resolve_symlnk, err_code);
}

/**
 * @brief       Checks if the given directory entity corresponds to a named pipe.
 * @param       directory_ent : Directory entity to check.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_pipe(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_pipe, false, directory_ent, resolve_symlnk, err_code);
}

/**
 * @brief       Checks if the given path corresponds to a regular file.
 * @param       file_pth : Path to check.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_regular_file(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_regular_file, false, file_pth, resolve_symlnk,
            err_code);
}

/**
 * @brief       Checks if the given directory entity corresponds to a regular file.
 * @param       directory_ent : Directory entity to check.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_regular_file(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_regular_file, false, directory_ent, resolve_symlnk,
            err_code);
}

/**
 * @brief       Checks if the given path corresponds to a socket.
 * @param       file_pth : Path to check.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_socket(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_socket, false, file_pth, resolve_symlnk, err_code);
}

/**
 * @brief       Checks if the given directory entity corresponds to a socket.
 * @param       directory_ent : Directory entity to check.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_socket(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_socket, false, directory_ent, resolve_symlnk, err_code);
}

/**
 * @brief       Checks if the given path corresponds to a symlink.
 * @param       file_pth : Path to check.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_symlink(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_symlink, false, file_pth, resolve_symlnk, err_code);
}

/**
 * @brief       Checks if the given directory entity corresponds to a symlink.
 * @param       directory_ent : Directory entity to check.
 * @param       resolve_symlnk : If `true`, symbolic links will be resolved before checking access.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool is_symlink(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::is_symlink, false, directory_ent, resolve_symlnk,
            err_code);
}

/**
 * @brief       Attempts to create a directory.
 * @param       directory_pth : The path of the new directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool mkdir(
        const path_char_t* directory_pth,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::mkdir, false, directory_pth, err_code);
}

/**
 * @brief       Attemps to create a directory path.
 * @param       directory_pth : The path of directories to create.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool mkdir_recursively(
        const path_char_t* directory_pth,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::mkdir_recursively, false, directory_pth, err_code);
}

/**
 * @brief       Opens a directory stream corresponding to the directory name, and returns a pointer
 *              to the directory stream.
 * @param       directory_ent : The directory entity.
 * @param       directory_pth : The path of the directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool opendir(
        directory_entity& directory_ent,
        const path_char_t* directory_pth,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::opendir, false, directory_ent, directory_pth, err_code);
}

/**
 * @brief       Read the next directory entry in the directory stream.
 * @param       directory_ent : The current directory entity.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool readdir(directory_entity& directory_ent, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::readdir, false, directory_ent, err_code);
}

/**
 * @brief       Delete the specified directory.
 * @param       directory_pth : The path of the directory to delete.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool rmdir(const path_char_t* directory_pth, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(filesystem::rmdir, false, directory_pth, err_code);
}

/**
 * @brief       Creates a shortcut stored at shortcut_pth that points at target_pth.
 * @param       target_pth : Path of the target.
 * @param       shortcut_pth : Path where the shortcut is stored including the file name.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool shortcut(
        const path_char_t* target_pth,
        const path_char_t* shortcut_pth,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::shortcut, false, target_pth, shortcut_pth, err_code);
}

/**
 * @brief       Creates a symbolic link named lnk_pth which contains the string trg.
 * @param       target_pth : The string to contain in the symlink.
 * @param       link_pth : The symbolilc link name.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool symlink(
        const path_char_t* target_pth,
        const path_char_t* link_pth,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::symlink, false, target_pth, link_pth, err_code);
}

/**
 * @brief       Attempts to create a regular file.
 * @param       regular_file_pth : The path of the new regular file.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool touch(
        const path_char_t* regular_file_pth,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::touch, false, regular_file_pth, err_code);
}

/**
 * @brief       Delete the specified regular file.
 * @param       regular_file_pth : The path of the regular file to delete.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool unlink(
        const path_char_t* regular_file_pth,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(filesystem::unlink, false, regular_file_pth, err_code);
}

}

#endif

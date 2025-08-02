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

#ifndef SPEED_SYSTEM_DETAIL_WINAPI_FILESYSTEM_OPERATIONS_HPP
#define SPEED_SYSTEM_DETAIL_WINAPI_FILESYSTEM_OPERATIONS_HPP

#include "../../../platform/platform.hpp"
#ifdef SPEED_WINAPI

#include "../../../errors/errors.hpp"
#include "../../../filesystem/access_modes.hpp"
#include "../../../filesystem/directory_entity.hpp"
#include "../../../filesystem/file_types.hpp"
#include "../../../filesystem/types.hpp"
#include "../../../process/types.hpp"
#include "../../../time/system_time.hpp"
#include "types.hpp"

namespace speed::system::detail::winapi::filesystem {

/**
 * @brief       Checks whether the calling process can access the file path. If pathname is a
 *              symbolic link, it is dereferenced.
 * @param       file_pth : The file path.
 * @param       access_mods : Specifies the accessibility check(s) to be performed.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the true is returned, otherwise false is returned.
 */
bool access(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        access_modes access_mods,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Change the current execution directory.
 * @param       directory_pth : The path of the new current directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool chdir(const path_char_t* directory_pth, std::error_code* err_code = nullptr) noexcept;

/**
 * @brief       Checks whether the calling process can access the file path. If pathname is a
 *              symbolic link, it is dereferenced.
 * @param       file_pth : The file path.
 * @param       access_mods : Specifies the accessibility check(s) to be performed.
 * @param       file_typs : The file type.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the true is returned, otherwise false is returned.
 */
bool check_file(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        access_modes access_mods,
        file_types file_typs,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Closes the directory stream.
 * @param       directory_ent : The directory entity.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool closedir(
        directory_entity& directory_ent,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Check if a file exists.
 * @param       file_pth : The path of the file to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool file_exists(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

bool get_file_handle(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        DWORD desired_accss,
        HANDLE& file_handl,
        std::error_code* err_code
) noexcept;

/**
 * @brief       Get the inode number of the specified file.
 * @param       file_pth : The file to get the inode number.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the inode number of the sepcified file is returned, otherwise -1 is
 *              returned.
 */
inode_t get_file_inode(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Get the inode number of the specified file.
 * @param       directory_ent : The directory entity current file to get the inode number.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the inode number of the sepcified file is returned, otherwise -1 is
 *              returned.
 */
inode_t get_file_inode(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Get the UID of the specified file.
 * @param       file_pth : The file to get the UID.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the UID of the sepcified file is returned, otherwise -1 is returned.
 */
system::process::uid_t get_file_uid(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Get the GID of the specified file.
 * @param       file_pth : The file to get the GID.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the GID of the sepcified file is returned, otherwise -1 is returned.
 */
system::process::gid_t get_file_gid(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Get the size in bytes of a specified file path.
 * @param       file_pth : The path of the file to get the size.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the file size in bytes in returned, otherwise -1 is returned.
 */
std::size_t get_file_size(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Get the modification time of the specified file.
 * @param       file_pth : The file to get the modification time.
 * @param       system_tme : The object in which store the modification time.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool get_modification_time(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        system::time::system_time& system_tme,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Get a temporal path.
 * @return      A temporal path.
 */
const path_char_t* get_temporal_path() noexcept;

/**
 * @brief       Checks if the given path corresponds to a block device.
 * @param       file_pth : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_block_device(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

bool is_block_device(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Checks if the given path corresponds to a character device.
 * @param       file_pth : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_character_device(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

bool is_character_device(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Checks if the given path corresponds to a directory.
 * @param       file_pth : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_directory(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Checks if the given path corresponds to a directory.
 * @param       directory_ent : The directory entity.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_directory(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Checks if the given path corresponds to a specified file type.
 * @param       file_pth : Path to check.
 * @param       fle_type : The specified file type.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_file_type(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        file_types fle_type,
        std::error_code* err_code = nullptr
) noexcept;

bool is_file_type(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        file_types file_typ,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Checks if the given path corresponds to a named pipe.
 * @param       file_pth : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_pipe(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

bool is_pipe(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Checks if the given path corresponds to a regular file.
 * @param       file_pth : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_regular_file(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

bool is_regular_file(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Checks if the given path corresponds to a socket.
 * @param       file_pth : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_socket(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

bool is_socket(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Checks if the given path corresponds to a socket.
 * @param       file_pth : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_symlink(
        const path_char_t* file_pth,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

bool is_symlink(
        const directory_entity& directory_ent,
        bool resolve_symlnk,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Attempts to create a directory.
 * @param       directory_pth : The path of the new directory.
 * @param       mods : Specifies the mode for the new directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool mkdir(
        const path_char_t* directory_pth,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Attemps to create a directory path.
 * @param       directory_pth : The path of directories to create.
 * @param       mods : Specifies the mode for the new directories.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool mkdir_recursively(
        const path_char_t* directory_pth,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Opens a directory stream corresponding to the directory name, and returns a pointer
 *              to the directory stream.
 * @param       directory_ent : The directory entity.
 * @param       directory_pth : The path of the directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool opendir(
        directory_entity& directory_ent,
        const path_char_t* directory_pth,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Read the next directory entry in the directory stream.
 * @param       directory_ent : The current directory entity.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool readdir(
        directory_entity& directory_ent,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Delete the specified directory.
 * @param       directory_pth : The path of the directory to delete.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool rmdir(const path_char_t* directory_pth, std::error_code* err_code = nullptr) noexcept;

/**
 * @brief       Creates a shortcut stored at shortcut_pth that points at target_pth.
 * @param       target_pth : Path of the target.
 * @param       shortcut_pth : Path where the shortcut is stored including the file name.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool shortcut(
        const path_char_t* target_pth,
        const path_char_t* shortcut_pth,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Creates a symbolic link named link_pth which contains the string target_pth.
 * @param       target_pth : The string to contain in the symlink.
 * @param       link_pth : The symbolilc link path.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool symlink(
        const path_char_t* target_pth,
        const path_char_t* link_pth,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Attempts to create a regular file.
 * @param       regular_file_pth : The path of the new regular file.
 * @param       mods : Specifies the mode for the new regular file.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool touch(
        const path_char_t* regular_file_pth,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Delete the specified regular file.
 * @param       regular_file_pth : The path of the regular file to delete.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool unlink(const path_char_t* regular_file_pth, std::error_code* err_code = nullptr) noexcept;

}

#endif
#endif

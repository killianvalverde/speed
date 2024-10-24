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
 * @file        speed/system/api/glibc/filesystem/operations.hpp
 * @brief       filesystem operations header.
 * @author      Killian Valverde
 * @date        2017/05/26
 */

#ifndef SPEED_SYSTEM_API_GLIBC_FILESYSTEM_OPERATIONS_HPP
#define SPEED_SYSTEM_API_GLIBC_FILESYSTEM_OPERATIONS_HPP

#include "../../../compatibility/compatibility.hpp"
#ifdef SPEED_GLIBC

#include <dirent.h>

#include "../../../errors/errors.hpp"
#include "../../../filesystem/access_modes.hpp"
#include "../../../filesystem/basic_directory_entity.hpp"
#include "../../../filesystem/file_types.hpp"
#include "../../../time/system_time.hpp"


namespace speed::system::api::glibc::filesystem {


using namespace speed::system::errors;
using speed::system::filesystem::access_modes;
using speed::system::filesystem::directory_entity;
using speed::system::filesystem::wdirectory_entity;
using speed::system::filesystem::file_types;
using speed::system::time::system_time;


/**
 * @brief       Checks whether the calling process can access the file path. If pathname is a
 *              symbolic link, it is dereferenced.
 * @param       fle_path : The file path.
 * @param       acss_modes : Specifies the accessibility check(s) to be performed.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the true is returned, otherwise false is returned.
 */
bool access(
        const char* fle_path,
        access_modes acss_modes,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Checks whether the calling process can access the file path. If pathname is a
 *              symbolic link, it is dereferenced.
 * @param       fle_path : The file path.
 * @param       acss_modes : Specifies the accessibility check(s) to be performed.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the true is returned, otherwise false is returned.
 */
bool access(
        const wchar_t* fle_path,
        access_modes acss_modes,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Checks whether the calling process can access the file path. If pathname is a
 *              symbolic link, it is dereferenced.
 * @param       fle_path : The file path.
 * @param       acss_modes : Specifies the accessibility check(s) to be performed.
 * @param       fle_type : The file type.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the true is returned, otherwise false is returned.
 */
bool access(
        const char* fle_path,
        access_modes acss_modes,
        file_types fle_type,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Checks whether the calling process can access the file path. If pathname is a
 *              symbolic link, it is dereferenced.
 * @param       fle_path : The file path.
 * @param       acss_modes : Specifies the accessibility check(s) to be performed.
 * @param       fle_type : The file type.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the true is returned, otherwise false is returned.
 */
bool access(
        const wchar_t* fle_path,
        access_modes acss_modes,
        file_types fle_type,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Check whether a specified directory can be created.
 * @param       dir_path : The specified directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the true is returned, otherwise false is returned.
 */
bool can_directory_be_created(const char* dir_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Check whether a specified directory can be created.
 * @param       dir_path : The specified directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the true is returned, otherwise false is returned.
 */
bool can_directory_be_created(
        const wchar_t* dir_path,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Check whether a specified regular file can be created.
 * @param       reg_file_path : The specified regular files.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the true is returned, otherwise false is returned.
 */
bool can_regular_file_be_created(
        const char* reg_file_path,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Check whether a specified regular file can be created.
 * @param       reg_file_path : The specified regular files.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the true is returned, otherwise false is returned.
 */
bool can_regular_file_be_created(
        const wchar_t* reg_file_path,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Change the current execution directory.
 * @param       dir_path : The path of the new current directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool chdir(const char* dir_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Change the current execution directory.
 * @param       dir_path : The path of the new current directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool chdir(const wchar_t* dir_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Closes the directory stream.
 * @param       dir_ent : The directory entity.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool closedir(directory_entity* dir_ent, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Closes the directory stream.
 * @param       dir_ent : The directory entity.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool closedir(wdirectory_entity* dir_ent, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Check if a file exists.
 * @param       fle_path : The path of the file to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool file_exists(const char* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Check if a file exists.
 * @param       fle_path : The path of the file to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool file_exists(const wchar_t* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Set a c_str from a w_str path.
 * @param       w_str : w_str path.
 * @param       c_str : Buffer in which set the data. It is expected to be PATH_MAX size.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
static bool get_cstr_path_from_wstr(
        const wchar_t* w_str,
        char* c_str
) noexcept;


/**
 * @brief       Get through the first directory that exists in a specified path.
 * @param       pth : Input and output argument, it is used to specify the path and to get the
 *              result. The length of the string has to be at least 2 characters, otherwise it has
 *              undefinied behaviour.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
static bool get_first_actual_directory(char* pth, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Get the inode number of the specified file.
 * @param       fle_path : The file to get the inode number.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the inode number of the sepcified file is returned, otherwise -1 is
 *              returned.
 */
std::uint64_t get_file_inode(const char* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Get the inode number of the specified file.
 * @param       fle_path : The file to get the inode number.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the inode number of the sepcified file is returned, otherwise -1 is
 *              returned.
 */
std::uint64_t get_file_inode(const wchar_t* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Get the UID of the specified file.
 * @param       fle_path : The file to get the UID.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the UID of the sepcified file is returned, otherwise -1 is returned.
 */
int get_file_uid(const char* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Get the UID of the specified file.
 * @param       fle_path : The file to get the UID.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the UID of the sepcified file is returned, otherwise -1 is returned.
 */
int get_file_uid(const wchar_t* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Get the GID of the specified file.
 * @param       fle_path : The file to get the GID.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the GID of the sepcified file is returned, otherwise -1 is returned.
 */
int get_file_gid(const char* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Get the GID of the specified file.
 * @param       fle_path : The file to get the GID.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the GID of the sepcified file is returned, otherwise -1 is returned.
 */
int get_file_gid(const wchar_t* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Get the size in bytes of a specified file path.
 * @param       fle_path : The path of the file to get the size.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the file size in bytes in returned, otherwise -1 is returned.
 */
std::size_t get_file_size(const char* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Get the size in bytes of a specified file path.
 * @param       fle_path : The path of the file to get the size.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      On success the file size in bytes in returned, otherwise -1 is returned.
 */
std::size_t get_file_size(const wchar_t* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Get the modification time of the specified file.
 * @param       fle_path : The file to get the modification time.
 * @param       system_tme : The object in which store the modification time.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool get_modification_time(
        const char* fle_path,
        system_time* system_tme,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Get the modification time of the specified time.
 * @param       fle_path : The file to get the modification time.
 * @param       system_tme : The object in which store the modification time.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool get_modification_time(
        const wchar_t* fle_path,
        system_time* system_tme,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Get the tmp system path.
 * @return      The tmp system path.
 */
const char* get_temporal_path() noexcept;


/**
 * @brief       Set a c_str from a w_str path.
 * @param       c_str : str path.
 * @param       w_str : Buffer in which set the data. It is expected to be PATH_MAX size.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
static bool get_wstr_path_from_cstr(
        const char* c_str,
        wchar_t* w_str
) noexcept;


/**
 * @brief       Checks if the given path corresponds to a block device.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_block_device(const char* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Checks if the given path corresponds to a block device.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_block_device(const wchar_t* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Checks if the given path corresponds to a character device.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_character_device(const char* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Checks if the given path corresponds to a character device.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_character_device(const wchar_t* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Checks if the given path corresponds to a directory.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_directory(const char* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Checks if the given path corresponds to a directory.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_directory(const wchar_t* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Checks if the given path corresponds to a named pipe.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_fifo(const char* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Checks if the given path corresponds to a named pipe.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_fifo(const wchar_t* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Checks if the given path corresponds to a specified file type.
 * @param       fle_path : Path to check.
 * @param       fle_type : The specified file type.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_file_type(
        const char* fle_path,
        file_types fle_type,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Checks if the given path corresponds to a specified file type.
 * @param       fle_path : Path to check.
 * @param       fle_type : The specified file type.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_file_type(
        const wchar_t* fle_path,
        file_types fle_type,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Checks if the given path corresponds to a regular file.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_regular_file(const char* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Checks if the given path corresponds to a regular file.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_regular_file(const wchar_t* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Checks if the given path corresponds to a socket.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_socket(const char* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Checks if the given path corresponds to a socket.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_socket(const wchar_t* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Checks if the given path corresponds to a socket.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_symlink(const char* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Checks if the given path corresponds to a socket.
 * @param       fle_path : Path to check.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool is_symlink(const wchar_t* fle_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Attempts to create a directory.
 * @param       dir_path : The path of the new directory.
 * @param       mods : Specifies the mode for the new directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool mkdir(
        const char* dir_path,
        std::uint32_t mods = 0755,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Attempts to create a directory.
 * @param       dir_path : The path of the new directory.
 * @param       mods : Specifies the mode for the new directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool mkdir(
        const wchar_t* dir_path,
        std::uint32_t mods = 0755,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Attemps to create a directory path.
 * @param       dir_path : The path of directories to create.
 * @param       mods : Specifies the mode for the new directories.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool mkdir_recursively(
        const char* dir_path,
        std::uint32_t mods = 0755,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Attemps to create a directory path.
 * @param       dir_path : The path of directories to create.
 * @param       mods : Specifies the mode for the new directories.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool mkdir_recursively(
        const wchar_t* dir_path,
        std::uint32_t mods = 0755,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Opens a directory stream corresponding to the directory name, and returns a pointer
 *              to the directory stream.
 * @param       dir_ent : The directory entity.
 * @param       dir_pth : The path of the directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool opendir(
        directory_entity* dir_ent,
        const char* dir_pth,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Opens a directory stream corresponding to the directory name, and returns a pointer
 *              to the directory stream.
 * @param       dir_ent : The directory entity.
 * @param       dir_pth : The path of the directory.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool opendir(
        wdirectory_entity* dir_ent,
        const wchar_t* dir_pth,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Read the next directory entry in the directory stream.
 * @param       dir_ent : The current directory entity.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool readdir(directory_entity* dir_ent, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Read the next directory entry in the directory stream.
 * @param       dir_ent : The current directory entity.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool readdir(wdirectory_entity* dir_ent, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Delete the specified directory.
 * @param       dir_path : The path of the directory to delete.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool rmdir(const char* dir_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Delete the specified directory.
 * @param       dir_path : The path of the directory to delete.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool rmdir(const wchar_t* dir_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Creates a shortcut stored at shortcut_pth that points at target_pth.
 * @param       target_pth : Path of the target.
 * @param       shortcut_pth : Path where the shortcut is stored including the file name.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool shortcut(
        const char* target_pth,
        const char* shortcut_pth,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Creates a shortcut stored at shortcut_pth that points at target_pth.
 * @param       target_pth : Path of the target.
 * @param       shortcut_pth : Path where the shortcut is stored including the file name.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool shortcut(
        const wchar_t* target_pth,
        const wchar_t* shortcut_pth,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Creates a symbolic link named lnk_pth which contains the string trg.
 * @param       trg : The string to contain in the symlink.
 * @param       lnk_pth : The symbolilc link name.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool symlink(const char* trg, const char* lnk_pth, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Creates a symbolic link named lnk_pth which contains the string trg.
 * @param       trg : The string to contain in the symlink.
 * @param       lnk_pth : The symbolilc link name.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool symlink(
        const wchar_t* trg,
        const wchar_t* lnk_pth,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Attempts to create a regular file.
 * @param       regfle_path : The path of the new regular file.
 * @param       mods : Specifies the mode for the new regular file.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool touch(
        const char* regfle_path,
        std::uint32_t mods = 0755,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Attempts to create a regular file.
 * @param       regfle_path : The path of the new regular file.
 * @param       mods : Specifies the mode for the new regular file.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool touch(
        const wchar_t* regfle_path,
        std::uint32_t mods = 0755,
        std::error_code* err_code = nullptr
) noexcept;


/**
 * @brief       Delete the specified regular file.
 * @param       reg_file_path : The path of the regular file to delete.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool unlink(const char* reg_file_path, std::error_code* err_code = nullptr) noexcept;


/**
 * @brief       Delete the specified regular file.
 * @param       reg_file_path : The path of the regular file to delete.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool unlink(const wchar_t* reg_file_path, std::error_code* err_code = nullptr) noexcept;


}


#endif
#endif

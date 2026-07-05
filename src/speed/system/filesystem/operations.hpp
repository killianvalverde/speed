/* speed - Generic C++ library.
 * Copyright (C) 2015-2026 Killian Valverde.
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
 * @file operations.hpp
 * @brief Core operations for the system::filesystem submodule.
 * @author Killian Valverde
 * @date 2017-05-26
 */

#pragma once

#include "../platform/platform.hpp"

#include <cstdint>

#include "../process/process.hpp"
#include "../time/time.hpp"
#include "access_modes.hpp"
#include "directory_stream.hpp"
#include "file_types.hpp"
#include "symlink_mode.hpp"
#include "symlink_target_type.hpp"
#include "types.hpp"

namespace speed::system::filesystem {

/**
 * @brief Closes an open directory stream.
 *
 * Releases all resources associated with the specified directory stream.
 *
 * @param directory_strm Directory stream to close.
 * @param err_code Optional destination for error information.
 * @return true if the directory stream was closed successfully; otherwise false.
 */
bool close_directory(
    directory_stream& directory_strm,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Creates a directory.
 *
 * Creates a new directory at the specified path.
 *
 * @param directory_path Path of the directory to create.
 * @param err_code Optional destination for error information.
 * @return true if the directory was created successfully; otherwise false.
 */
bool create_directory(
    const path_char_t* directory_path,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Creates a directory hierarchy.
 *
 * Creates all nonexistent directories contained in the specified path.
 * Existing directory components are ignored.
 *
 * @param directory_path Path of the directory hierarchy to create.
 * @param err_code Optional destination for error information.
 * @return true if the directory hierarchy exists after the call; otherwise false.
 */
bool create_directories(
    const path_char_t* directory_path,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Creates an empty regular file.
 *
 * Fails if a filesystem object already exists at the specified path.
 *
 * @param regular_file_path Path of the regular file to create.
 * @param err_code Optional destination for error information.
 * @return true if the file was created successfully; otherwise false.
 */
bool create_regular_file(
    const path_char_t* regular_file_path,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Creates a shortcut.
 *
 * Creates a platform-specific shortcut referring to the specified target.
 *
 * @param target_path Path of the target object.
 * @param shortcut_path Path of the shortcut to create.
 * @param err_code Optional destination for error information.
 * @return true if the shortcut was created successfully; otherwise false.
 */
bool create_shortcut(
    const path_char_t* target_path,
    const path_char_t* shortcut_path,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Creates a symbolic link.
 *
 * @param target_path Path of the target object.
 * @param link_path Path of the symbolic link to create.
 * @param target_type Type of object referenced by the symbolic link.
 * @param err_code Optional destination for error information.
 * @return true if the symbolic link was created successfully; otherwise false.
 */
bool create_symlink(
    const path_char_t* target_path,
    const path_char_t* link_path,
    symlink_target_type target_type,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Checks whether a filesystem object exists.
 *
 * The behavior with respect to symbolic links is controlled by symlink_md.
 *
 * @param file_path Path of the filesystem object.
 * @param symlink_md Symbolic link resolution mode.
 * @param err_code Optional destination for error information.
 * @return true if the filesystem object exists; otherwise false.
 */
bool file_exists(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Checks access permissions for a filesystem object.
 *
 * Determines whether the requested access modes are permitted for the
 * specified filesystem object.
 *
 * @param file_path Path of the filesystem object.
 * @param symlink_md Symbolic link resolution mode.
 * @param access_mds Access modes to verify.
 * @param err_code Optional destination for error information.
 * @return true if all requested access modes are permitted; otherwise false.
 */
bool file_has_access(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    access_modes access_mds,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Checks whether a filesystem object satisfies the specified criteria.
 *
 * Verifies both file type and access requirements.
 *
 * @param file_path Path of the filesystem object.
 * @param symlink_md Symbolic link resolution mode.
 * @param access_mds Required access modes.
 * @param file_ts Accepted file types.
 * @param err_code Optional destination for error information.
 * @return true if all specified requirements are satisfied; otherwise false.
 */
bool file_matches(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    access_modes access_mds,
    file_types file_ts,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Retrieves the inode identifier of a filesystem object.
 *
 * @param file_path Path of the filesystem object.
 * @param symlink_md Symbolic link resolution mode.
 * @param err_code Optional destination for error information.
 * @return Inode identifier on success; otherwise an invalid inode value.
 */
inode_t get_file_inode(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Retrieves the owner identifier of a filesystem object.
 *
 * @param file_path Path of the filesystem object.
 * @param symlink_md Symbolic link resolution mode.
 * @param err_code Optional destination for error information.
 * @return User identifier of the owner on success; otherwise an empty user identifier.
 */
process::user_id get_file_user_id(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Retrieves the size of a filesystem object.
 *
 * @param file_path Path of the filesystem object.
 * @param symlink_md Symbolic link resolution mode.
 * @param err_code Optional destination for error information.
 * @return Size of the filesystem object in bytes on success; otherwise an invalid size value.
 */
file_size_t get_file_size(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Retrieves timestamps associated with a filesystem object.
 *
 * Any output pointer may be null.
 *
 * @param file_path Path of the filesystem object.
 * @param symlink_md Symbolic link resolution mode.
 * @param creation_time Receives the creation timestamp.
 * @param last_access_time Receives the last-access timestamp.
 * @param modification_time Receives the last-modification timestamp.
 * @param err_code Optional destination for error information.
 * @return true if the timestamps were retrieved successfully; otherwise false.
 *
 * @note Creation time may be unavailable on some platforms. In such cases, the creation
 *       timestamp is cleared.
 */
bool get_file_times(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    time::calendar_time* creation_time,
    time::calendar_time* last_access_time,
    time::calendar_time* modification_time,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Retrieves the system temporary directory path.
 *
 * @param err_code Optional destination for error information.
 * @return Temporary directory path on success; otherwise an empty string.
 */
std::basic_string<path_char_t> get_temporary_path(std::error_code* err_code = nullptr) noexcept;

/**
 * @brief Checks whether a filesystem object is a block device.
 *
 * @param file_path Path of the filesystem object.
 * @param symlink_md Symbolic link resolution mode.
 * @param err_code Optional destination for error information.
 * @return true if the filesystem object is a block device; otherwise false.
 */
bool is_block_device(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Checks whether a filesystem object is a character device.
 *
 * @param file_path Path of the filesystem object.
 * @param symlink_md Symbolic link resolution mode.
 * @param err_code Optional destination for error information.
 * @return true if the filesystem object is a character device; otherwise false.
 */
bool is_character_device(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Checks whether a filesystem object is a directory.
 *
 * @param file_path Path of the filesystem object.
 * @param symlink_md Symbolic link resolution mode.
 * @param err_code Optional destination for error information.
 * @return true if the filesystem object is a directory; otherwise false.
 */
bool is_directory(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Checks whether a filesystem object matches one of the specified file types.
 *
 * @param file_path Path of the filesystem object.
 * @param symlink_md Symbolic link resolution mode.
 * @param file_ts Accepted file types.
 * @param err_code Optional destination for error information.
 * @return true if the filesystem object matches at least one specified file type; otherwise false.
 */
bool is_file_type(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    file_types file_ts,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Checks whether a filesystem object is a pipe.
 *
 * @param file_path Path of the filesystem object.
 * @param symlink_md Symbolic link resolution mode.
 * @param err_code Optional destination for error information.
 * @return true if the filesystem object is a pipe; otherwise false.
 */
bool is_pipe(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Checks whether a filesystem object is a regular file.
 *
 * @param file_path Path of the filesystem object.
 * @param symlink_md Symbolic link resolution mode.
 * @param err_code Optional destination for error information.
 * @return true if the filesystem object is a regular file; otherwise false.
 */
bool is_regular_file(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Checks whether a filesystem object is a socket.
 *
 * @param file_path Path of the filesystem object.
 * @param symlink_md Symbolic link resolution mode.
 * @param err_code Optional destination for error information.
 * @return true if the filesystem object is a socket; otherwise false.
 */
bool is_socket(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Checks whether a filesystem object is a symbolic link.
 *
 * @param file_path Path of the filesystem object.
 * @param symlink_md Symbolic link resolution mode.
 * @param err_code Optional destination for error information.
 * @return true if the filesystem object is a symbolic link; otherwise false.
 */
bool is_symlink(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Opens a directory stream.
 *
 * @param directory_strm Receives the opened directory stream.
 * @param directory_path Path of the directory to open.
 * @param err_code Optional destination for error information.
 * @return true if the directory was opened successfully; otherwise false.
 */
bool open_directory(
    directory_stream& directory_strm,
    const path_char_t* directory_path,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Opens a child directory stream.
 *
 * Opens the directory referenced by the current entry of an existing directory stream.
 *
 * @param child Receives the opened child directory stream.
 * @param parent Parent directory stream.
 * @param err_code Optional destination for error information.
 * @return true if the child directory was opened successfully; otherwise false.
 */
bool open_directory(
    directory_stream& child,
    const directory_stream& parent,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Reads the next directory entry.
 *
 * Updates the public members of the specified directory stream with information describing
 * the next entry.
 *
 * The "." and ".." entries are skipped.
 *
 * @param directory_strm Directory stream to read.
 * @param err_code Optional destination for error information.
 * @return true if an entry was read successfully; otherwise false.
 *
 * @note When false is returned, inspect err_code to distinguish end-of-directory from
 *       an error condition.
 */
bool read_directory(directory_stream& directory_strm, std::error_code* err_code = nullptr) noexcept;

/**
 * @brief Removes a directory.
 *
 * @param directory_path Path of the directory to remove.
 * @param err_code Optional destination for error information.
 * @return true if the directory was removed successfully; otherwise false.
 */
bool remove_directory(
    const path_char_t* directory_path,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Removes a filesystem object.
 *
 * Removes either a regular file or a directory.
 *
 * @param file_path Path of the filesystem object to remove.
 * @param err_code Optional destination for error information.
 * @return true if the filesystem object was removed successfully; otherwise false.
 */
bool remove_path(const path_char_t* file_path, std::error_code* err_code = nullptr) noexcept;

/**
 * @brief Removes a regular file.
 *
 * @param regular_file_path Path of the regular file to remove.
 * @param err_code Optional destination for error information.
 * @return true if the regular file was removed successfully; otherwise false.
 */
bool remove_regular_file(
    const path_char_t* regular_file_path,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Changes the current working directory.
 *
 * @param directory_path Path of the new working directory.
 * @param err_code Optional destination for error information.
 * @return true if the current working directory was changed successfully; otherwise false.
 */
bool set_current_directory(
    const path_char_t* directory_path,
    std::error_code* err_code = nullptr
) noexcept;

}

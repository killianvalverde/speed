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
 * @file operations.cpp
 * @brief Core operations implementation for the system::filesystem submodule.
 * @author Killian Valverde
 * @date 2017-05-26
 */

#include "../../../platform/platform.hpp"
#include "../../operations.hpp"

#include <cstdlib>
#include <ctime>
#include <limits>

#include <fcntl.h>

#include "../../../../strings/strings.hpp"
#include "../../../errors/errors.hpp"

namespace speed::system::filesystem {

static bool assign_calendar_time(
    std::int64_t seconds,
    std::uint32_t nanoseconds,
    time::calendar_time* calendar_time
) noexcept
{
    if (calendar_time == nullptr)
    {
        return true;
    }

    std::tm tm{};
    auto native_time = static_cast<std::time_t>(seconds);

    if (gmtime_r(&native_time, &tm) == nullptr)
    {
        return false;
    }

    calendar_time
        ->set_year(tm.tm_year + 1900)
        .set_month(tm.tm_mon + 1)
        .set_day(tm.tm_mday)
        .set_hour(tm.tm_hour)
        .set_minute(tm.tm_min)
        .set_second(tm.tm_sec)
        .set_millisecond(static_cast<std::uint16_t>(nanoseconds / 1'000'000));

    return true;
}

static bool get_stat(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    struct stat& st,
    std::error_code* err_code
) noexcept
{
    int result = symlink_md == symlink_mode::NO_RESOLVE
        ? lstat(file_path, &st)
        : stat(file_path, &st);

    if (result == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    return true;
}

bool close_directory(directory_stream& directory_strm, std::error_code* err_code) noexcept
{
    auto& native = directory_strm.native;
    if (native.dir == nullptr)
    {
        return true;
    }

    if (closedir(native.dir) == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    native.dir = nullptr;
    native.dir_fd = -1;
    native.entry = nullptr;
    directory_strm.name = nullptr;
    directory_strm.inode = 0;
    directory_strm.type = file_types::NIL;

    return true;
}

bool create_directory(const path_char_t* directory_path, std::error_code* err_code) noexcept
{
    if (mkdir(directory_path, 0755) == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    return true;
}

bool create_directories(const path_char_t* directory_path, std::error_code* err_code) noexcept
{
    if (directory_path == nullptr || *directory_path == '\0')
    {
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    try
    {
        std::basic_string current_path(directory_path);

        while (current_path.size() > 1 && current_path.back() == '/')
        {
            current_path.pop_back();
        }
        if (current_path == "/")
        {
            return true;
        }

        if (current_path.empty())
        {
            errors::assign_errc(std::errc::invalid_argument, err_code);
            return false;
        }

        auto create_directory_impl = [&](const path_char_t* path) noexcept
        {
            if (mkdir(path, 0755) == 0)
            {
                return true;
            }

            int err = errno;
            if (err == EEXIST)
            {
                return is_directory(path, symlink_mode::RESOLVE, nullptr);
            }

            errors::assign_errno_error_code(err, err_code);
            return false;
        };

        std::size_t start_pos = 0;
        if (!current_path.empty() && current_path.front() == '/')
        {
            start_pos = 1;
        }

        for (std::size_t i = start_pos; i < current_path.size(); ++i)
        {
            if (current_path[i] == '/')
            {
                path_char_t saved = current_path[i];
                current_path[i] = '\0';

                if (current_path[0] != '\0' && !create_directory_impl(current_path.c_str()))
                {
                    return false;
                }

                current_path[i] = saved;
            }
        }

        return create_directory_impl(current_path.c_str());
    }
    catch (const std::bad_alloc&)
    {
        errors::assign_errc(std::errc::not_enough_memory, err_code);
        return false;
    }
}

bool create_regular_file(
    const path_char_t* regular_file_path,
    std::error_code* err_code
) noexcept
{
    int file_descriptor = open(regular_file_path, O_CREAT | O_EXCL | O_WRONLY, 0644);
    if (file_descriptor == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    close(file_descriptor);
    return true;
}

bool create_shortcut(
    const path_char_t* target_path,
    const path_char_t* shortcut_path,
    std::error_code* err_code
) noexcept
{
    return create_symlink(target_path, shortcut_path, symlink_target_type::AUTO, err_code);
}

bool create_symlink(
    const path_char_t* target_path,
    const path_char_t* link_path,
    symlink_target_type target_type,
    std::error_code* err_code
) noexcept
{
    (void)target_type;

    if (symlink(target_path, link_path) == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    return true;
}

bool file_exists(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    struct stat st{};
    return get_stat(file_path, symlink_md, st, err_code);
}

bool file_has_access(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    access_modes access_mds,
    std::error_code* err_code
) noexcept
{
    if (access_mds == access_modes::NIL)
    {
        return true;
    }

    int mode = 0;

    if ((access_mds & access_modes::READ) != access_modes::NIL)
    {
        mode |= R_OK;
    }

    if ((access_mds & access_modes::WRITE) != access_modes::NIL)
    {
        mode |= W_OK;
    }

    if ((access_mds & access_modes::EXECUTE) != access_modes::NIL)
    {
        mode |= X_OK;
    }

    int flags = AT_EACCESS;
    if (symlink_md == symlink_mode::NO_RESOLVE)
    {
        flags |= AT_SYMLINK_NOFOLLOW;
    }

    if (faccessat(AT_FDCWD, file_path, mode, flags) == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    return true;
}

bool file_matches(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    access_modes access_mds,
    file_types file_ts,
    std::error_code* err_code
) noexcept
{
    return is_file_type(file_path, symlink_md, file_ts, err_code) &&
        file_has_access(file_path, symlink_md, access_mds, err_code);
}

inode_t get_file_inode(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    struct stat st{};

    if (!get_stat(file_path, symlink_md, st, err_code))
    {
        return std::numeric_limits<inode_t>::max();
    }

    return static_cast<inode_t>(st.st_ino);
}

process::user_id get_file_user_id(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    struct stat st{};
    process::user_id uid;

    if (!get_stat(file_path, symlink_md, st, err_code))
    {
        return uid;
    }

    uid.assign(st.st_uid);
    return uid;
}

file_size_t get_file_size(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    struct stat st{};

    if (!get_stat(file_path, symlink_md, st, err_code))
    {
        return std::numeric_limits<file_size_t>::max();
    }

    return static_cast<file_size_t>(st.st_size);
}

bool get_file_times(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    time::calendar_time* creation_time,
    time::calendar_time* last_access_time,
    time::calendar_time* modification_time,
    std::error_code* err_code
) noexcept
{
    if (creation_time != nullptr)
    {
        *creation_time = {};
    }

#if defined(__linux__)
    int flags = symlink_md == symlink_mode::NO_RESOLVE ? AT_SYMLINK_NOFOLLOW : 0;
    struct statx stx{};

    if (statx(AT_FDCWD, file_path, flags, STATX_ATIME | STATX_MTIME | STATX_BTIME, &stx) == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    if ((stx.stx_mask & STATX_BTIME) != 0)
    {
        if (!assign_calendar_time(stx.stx_btime.tv_sec, stx.stx_btime.tv_nsec, creation_time))
        {
            errors::assign_errc(std::errc::result_out_of_range, err_code);
            return false;
        }
    }

    if (!assign_calendar_time(stx.stx_atime.tv_sec, stx.stx_atime.tv_nsec, last_access_time))
    {
        errors::assign_errc(std::errc::result_out_of_range, err_code);
        return false;
    }
    if (!assign_calendar_time(stx.stx_mtime.tv_sec, stx.stx_mtime.tv_nsec, modification_time))
    {
        errors::assign_errc(std::errc::result_out_of_range, err_code);
        return false;
    }

    return true;
#else
    struct stat st{};

    if (!get_stat(file_path, symlink_md, st, err_code))
    {
        return false;
    }
#if defined(__APPLE__) || \
    defined(__FreeBSD__) || \
    defined(__NetBSD__) || \
    defined(__OpenBSD__) || \
    defined(__CYGWIN__)
    if (!assign_calendar_time(st.st_birthtim.tv_sec, st.st_birthtim.tv_nsec, creation_time))
    {
        errors::assign_errc(std::errc::result_out_of_range, err_code);
        return false;
    }
#endif
    if (!assign_calendar_time(st.st_atim.tv_sec, st.st_atim.tv_nsec, last_access_time))
    {
        errors::assign_errc(std::errc::result_out_of_range, err_code);
        return false;
    }
    if (!assign_calendar_time(st.st_mtim.tv_sec, st.st_mtim.tv_nsec, modification_time))
    {
        errors::assign_errc(std::errc::result_out_of_range, err_code);
        return false;
    }

    return true;
#endif
}

std::basic_string<path_char_t> get_temporary_path(std::error_code* err_code) noexcept
{
    try
    {
        const path_char_t* tmpdir = std::getenv("TMPDIR");

        if (tmpdir != nullptr && *tmpdir != '\0')
        {
            return tmpdir;
        }

        return "/tmp";
    }
    catch (const std::bad_alloc&)
    {
        errors::assign_errc(std::errc::not_enough_memory, err_code);
        return {};
    }
}

bool is_block_device(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    return is_file_type(file_path, symlink_md, file_types::BLOCK_DEVICE, err_code);
}

bool is_character_device(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    return is_file_type(file_path, symlink_md, file_types::CHARACTER_DEVICE, err_code);
}

bool is_directory(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    return is_file_type(file_path, symlink_md, file_types::DIRECTORY, err_code);
}

bool is_file_type(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    file_types file_ts,
    std::error_code* err_code
) noexcept
{
    struct stat st{};

    if (!get_stat(file_path, symlink_md, st, err_code))
    {
        return false;
    }

    if (file_ts == file_types::NIL)
    {
        return true;
    }

    const mode_t mode = st.st_mode;

    if ((file_ts & file_types::BLOCK_DEVICE) != file_types::NIL && S_ISBLK(mode))
    {
        return true;
    }
    if ((file_ts & file_types::CHARACTER_DEVICE) != file_types::NIL && S_ISCHR(mode))
    {
        return true;
    }
    if ((file_ts & file_types::DIRECTORY) != file_types::NIL && S_ISDIR(mode))
    {
        return true;
    }
    if ((file_ts & file_types::PIPE) != file_types::NIL && S_ISFIFO(mode))
    {
        return true;
    }
    if ((file_ts & file_types::REGULAR_FILE) != file_types::NIL && S_ISREG(mode))
    {
        return true;
    }
    if ((file_ts & file_types::SOCKET) != file_types::NIL && S_ISSOCK(mode))
    {
        return true;
    }
    if ((file_ts & file_types::SYMLINK) != file_types::NIL && S_ISLNK(mode))
    {
        return true;
    }

    return false;
}

bool is_pipe(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    return is_file_type(file_path, symlink_md, file_types::PIPE, err_code);
}

bool is_regular_file(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    return is_file_type(file_path, symlink_md, file_types::REGULAR_FILE, err_code);
}

bool is_socket(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    return is_file_type(file_path, symlink_md, file_types::SOCKET, err_code);
}

bool is_symlink(
    const path_char_t* file_path,
    symlink_mode symlink_md,
    std::error_code* err_code
) noexcept
{
    return is_file_type(file_path, symlink_md, file_types::SYMLINK, err_code);
}

bool open_directory(
    directory_stream& directory_strm,
    const path_char_t* directory_path,
    std::error_code* err_code
) noexcept
{
    auto& native = directory_strm.native;

    int fd = open(directory_path, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
    if (fd == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    DIR* dir = fdopendir(fd);
    if (dir == nullptr)
    {
        errors::assign_errno_error_code(errno, err_code);
        close(fd);
        return false;
    }

    native.dir = dir;
    native.dir_fd = fd;
    native.entry = nullptr;

    directory_strm.name = nullptr;
    directory_strm.inode = 0;
    directory_strm.type = file_types::NIL;

    return true;
}

bool open_directory(
    directory_stream& child,
    const directory_stream& parent,
    std::error_code* err_code
) noexcept
{
    if (parent.name == nullptr)
    {
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    auto& native = child.native;

    int fd = openat(parent.native.dir_fd, parent.name, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
    if (fd == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    DIR* dir = fdopendir(fd);
    if (dir == nullptr)
    {
        errors::assign_errno_error_code(errno, err_code);
        close(fd);
        return false;
    }

    native.dir = dir;
    native.dir_fd = fd;
    native.entry = nullptr;

    child.name = nullptr;
    child.inode = 0;
    child.type = file_types::NIL;

    return true;
}

bool read_directory(directory_stream& directory_strm, std::error_code* err_code) noexcept
{
    auto& native = directory_strm.native;

    errno = 0;

    for (;;)
    {
        dirent* entry = readdir(native.dir);
        if (entry == nullptr)
        {
            if (errno != 0)
            {
                errors::assign_errno_error_code(errno, err_code);
            }

            return false;
        }

        if ((entry->d_name[0] == '.' && entry->d_name[1] == '\0') ||
            (entry->d_name[0] == '.' && entry->d_name[1] == '.' && entry->d_name[2] == '\0'))
        {
            continue;
        }

        native.entry = entry;
        directory_strm.name = entry->d_name;
        directory_strm.inode = static_cast<inode_t>(entry->d_ino);

        switch (entry->d_type)
        {
        case DT_BLK:
            directory_strm.type = file_types::BLOCK_DEVICE;
            break;

        case DT_CHR:
            directory_strm.type = file_types::CHARACTER_DEVICE;
            break;

        case DT_DIR:
            directory_strm.type = file_types::DIRECTORY;
            break;

        case DT_FIFO:
            directory_strm.type = file_types::PIPE;
            break;

        case DT_LNK:
            directory_strm.type = file_types::SYMLINK;
            break;

        case DT_REG:
            directory_strm.type = file_types::REGULAR_FILE;
            break;

        case DT_SOCK:
            directory_strm.type = file_types::SOCKET;
            break;

        default:
            {
                struct stat st{};

                if (fstatat(native.dir_fd, entry->d_name, &st, AT_SYMLINK_NOFOLLOW) == -1)
                {
                    errors::assign_errno_error_code(errno, err_code);
                    return false;
                }

                const mode_t mode = st.st_mode;
                if (S_ISBLK(mode))
                {
                    directory_strm.type = file_types::BLOCK_DEVICE;
                }
                else if (S_ISCHR(mode))
                {
                    directory_strm.type = file_types::CHARACTER_DEVICE;
                }
                else if (S_ISDIR(mode))
                {
                    directory_strm.type = file_types::DIRECTORY;
                }
                else if (S_ISFIFO(mode))
                {
                    directory_strm.type = file_types::PIPE;
                }
                else if (S_ISLNK(mode))
                {
                    directory_strm.type = file_types::SYMLINK;
                }
                else if (S_ISREG(mode))
                {
                    directory_strm.type = file_types::REGULAR_FILE;
                }
                else if (S_ISSOCK(mode))
                {
                    directory_strm.type = file_types::SOCKET;
                }
                else
                {
                    directory_strm.type = file_types::NIL;
                }

                break;
            }
        }

        return true;
    }
}

bool remove_directory(const path_char_t* directory_path, std::error_code* err_code) noexcept
{
    if (rmdir(directory_path) == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    return true;
}

bool remove_path(const path_char_t* file_path, std::error_code* err_code) noexcept
{
    std::error_code local_error;

    if (is_directory(file_path, symlink_mode::NO_RESOLVE, &local_error))
    {
        return remove_directory(file_path, err_code);
    }

    if (local_error)
    {
        if (err_code != nullptr)
        {
            *err_code = local_error;
        }

        return false;
    }

    return remove_regular_file(file_path, err_code);
}

bool remove_regular_file(const path_char_t* regular_file_path, std::error_code* err_code) noexcept
{
    if (unlink(regular_file_path) == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    return true;
}

bool set_current_directory(const path_char_t* directory_path, std::error_code* err_code) noexcept
{
    if (chdir(directory_path) == -1)
    {
        errors::assign_errno_error_code(errno, err_code);
        return false;
    }

    return true;
}

}

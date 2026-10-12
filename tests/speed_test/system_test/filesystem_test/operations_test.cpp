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
 * @file operations_test.cpp
 * @brief Unit tests for system::filesystem operations.
 * @author Killian Valverde
 * @date 2026-10-11
 */

#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <limits>
#include <string>
#include <system_error>

#include <gtest/gtest.h>

#include "speed/system/filesystem/filesystem.hpp"

namespace std_fs = std::filesystem;
namespace fs = speed::system::filesystem;

namespace {

class temporary_workspace
{
public:
    temporary_workspace()
    {
        static std::atomic_uint64_t counter{0};

        path_ = std_fs::temp_directory_path() / ("speed_filesystem_test_" +
            std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + "_" +
            std::to_string(counter.fetch_add(1)));

        std_fs::create_directories(path_);
    }

    ~temporary_workspace()
    {
        std::error_code err_code;
        std_fs::remove_all(path_, err_code);
    }

    [[nodiscard]] std_fs::path path(const std_fs::path& relative = {}) const
    {
        return path_ / relative;
    }

private:
    std_fs::path path_;
};

}

TEST(system_filesystem_create_directory, creates_directory)
{
    temporary_workspace workspace;
    const auto path = workspace.path("directory");

    std::error_code err_code;
    const bool result = fs::create_directory(path.c_str(), &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_TRUE(std_fs::is_directory(path));
}

TEST(system_filesystem_create_directories, creates_nested_directories)
{
    temporary_workspace workspace;
    const auto path = workspace.path("first/second/third");

    std::error_code err_code;
    const bool result = fs::create_directories(path.c_str(), &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_TRUE(std_fs::is_directory(path));
}

TEST(system_filesystem_create_regular_file, creates_empty_file)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    std::error_code err_code;
    const bool result = fs::create_regular_file(path.c_str(), &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_TRUE(std_fs::is_regular_file(path));
    EXPECT_EQ(std_fs::file_size(path), 0);
}

TEST(system_filesystem_create_shortcut, creates_shortcut)
{
    temporary_workspace workspace;
    const auto target = workspace.path("target.txt");

#ifdef SPEED_WINAPI
    const auto shortcut = workspace.path("shortcut.lnk");
    auto stem_shortcut = std_fs::path(shortcut).replace_extension();
#else
    const auto shortcut = workspace.path("shortcut");
#endif

    ASSERT_TRUE(std::ofstream(target).good());

    std::error_code err_code;
#ifdef SPEED_WINAPI
    const bool result = fs::create_shortcut(target.c_str(), stem_shortcut.c_str(), &err_code);
#else
    const bool result = fs::create_shortcut(target.c_str(), shortcut.c_str(), &err_code);
#endif

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_TRUE(std_fs::exists(shortcut));
}

TEST(system_filesystem_create_symlink, creates_symbolic_link)
{
    temporary_workspace workspace;
    const auto target = workspace.path("target.txt");
    const auto link = workspace.path("link.txt");

    ASSERT_TRUE(std::ofstream(target).good());

    std::error_code err_code;
    const bool result = fs::create_symlink(target.c_str(), link.c_str(),
        fs::symlink_target_type::FILE, &err_code);

#ifdef SPEED_WINAPI
    if (!result && err_code == std::errc::operation_not_permitted)
    {
        GTEST_SKIP() << "Symbolic link creation requires additional privileges.";
    }
#endif

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_TRUE(std_fs::is_symlink(link));
}

TEST(system_filesystem_file_exists, detects_existing_file)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(path).good());

    std::error_code err_code;
    const bool result = fs::file_exists(path.c_str(), fs::symlink_mode::RESOLVE, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
}

TEST(system_filesystem_file_has_access, detects_read_access)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(path).good());

    std::error_code err_code;
    const bool result = fs::file_has_access(path.c_str(), fs::symlink_mode::RESOLVE,
        fs::access_modes::READ, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
}

TEST(system_filesystem_file_matches, matches_file_requirements)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(path).good());

    std::error_code err_code;
    const bool result = fs::file_matches(path.c_str(), fs::symlink_mode::RESOLVE,
        fs::access_modes::READ, fs::file_types::REGULAR_FILE, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
}

TEST(system_filesystem_get_file_inode, returns_valid_inode)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(path).good());

    std::error_code err_code;
    const auto inode = fs::get_file_inode(path.c_str(), fs::symlink_mode::RESOLVE, &err_code);

    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_NE(inode, std::numeric_limits<fs::inode_t>::max());
}

TEST(system_filesystem_get_file_user_id, returns_file_owner)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(path).good());

    std::error_code err_code;
    const auto uid = fs::get_file_user_id(path.c_str(), fs::symlink_mode::RESOLVE, &err_code);

    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_FALSE(uid.is_empty());
}

TEST(system_filesystem_get_file_size, returns_correct_size)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    {
        std::ofstream file(path, std::ios::binary);
        ASSERT_TRUE(file.good());
        file << "Hello World";
    }

    std::error_code err_code;
    const auto size = fs::get_file_size(path.c_str(), fs::symlink_mode::RESOLVE, &err_code);

    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_EQ(size, 11);
}

TEST(system_filesystem_get_file_times, retrieves_file_timestamps)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(path).good());

    speed::system::time::calendar_time creation_time{};
    speed::system::time::calendar_time access_time{};
    speed::system::time::calendar_time modification_time{};

    std::error_code err_code;
    const bool result = fs::get_file_times(path.c_str(), fs::symlink_mode::RESOLVE, &creation_time,
        &access_time, &modification_time, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
}

TEST(system_filesystem_get_temporary_path, returns_existing_directory)
{
    std::error_code err_code;
    const auto path = fs::get_temporary_path(&err_code);

    EXPECT_FALSE(err_code) << err_code.message();
    ASSERT_FALSE(path.empty());
    EXPECT_TRUE(std_fs::is_directory(std_fs::path(path)));
}

TEST(system_filesystem_is_block_device, rejects_regular_file)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(path).good());

    EXPECT_FALSE(fs::is_block_device(path.c_str(), fs::symlink_mode::RESOLVE));
}

TEST(system_filesystem_is_character_device, rejects_regular_file)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(path).good());

    EXPECT_FALSE(fs::is_character_device(path.c_str(), fs::symlink_mode::RESOLVE));
}

TEST(system_filesystem_is_directory, detects_directory)
{
    temporary_workspace workspace;
    const auto path = workspace.path("directory");

    ASSERT_TRUE(std_fs::create_directory(path));

    EXPECT_TRUE(fs::is_directory(path.c_str(), fs::symlink_mode::RESOLVE));
}

TEST(system_filesystem_is_file_type, detects_regular_file_type)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(path).good());

    EXPECT_TRUE(fs::is_file_type(path.c_str(), fs::symlink_mode::RESOLVE,
        fs::file_types::REGULAR_FILE));
}

TEST(system_filesystem_is_pipe, rejects_regular_file)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(path).good());

    EXPECT_FALSE(fs::is_pipe(path.c_str(), fs::symlink_mode::RESOLVE));
}

TEST(system_filesystem_is_regular_file, detects_regular_file)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(path).good());

    EXPECT_TRUE(fs::is_regular_file(path.c_str(), fs::symlink_mode::RESOLVE));
}

TEST(system_filesystem_is_socket, rejects_regular_file)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(path).good());

    EXPECT_FALSE(fs::is_socket(path.c_str(), fs::symlink_mode::RESOLVE));
}

TEST(system_filesystem_is_symlink, detects_symbolic_link)
{
    temporary_workspace workspace;
    const auto target = workspace.path("target.txt");
    const auto link = workspace.path("link.txt");

    ASSERT_TRUE(std::ofstream(target).good());

    std::error_code err_code;
    std_fs::create_symlink(target, link, err_code);

#ifdef SPEED_WINAPI
    if (err_code == std::errc::operation_not_permitted)
    {
        GTEST_SKIP() << "Symbolic link creation requires additional privileges.";
    }
#endif

    ASSERT_FALSE(err_code) << err_code.message();

    EXPECT_TRUE(fs::is_symlink(link.c_str(), fs::symlink_mode::NO_RESOLVE));
}

TEST(system_filesystem_open_directory, opens_existing_directory)
{
    temporary_workspace workspace;
    fs::directory_stream stream;

    std::error_code err_code;
    const bool result = fs::open_directory(stream, workspace.path().c_str(), &err_code);

    ASSERT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();

    EXPECT_TRUE(fs::close_directory(stream));
}

TEST(system_filesystem_open_directory, opens_child_directory)
{
    temporary_workspace workspace;
    const auto child_path = workspace.path("child");

    ASSERT_TRUE(std_fs::create_directory(child_path));

    fs::directory_stream parent;
    fs::directory_stream child;
    std::error_code err_code;

    ASSERT_TRUE(fs::open_directory(parent, workspace.path().c_str()));

    bool found_child = false;

    while (fs::read_directory(parent, &err_code))
    {
        if (std_fs::path(parent.name).filename() == child_path.filename())
        {
            found_child = true;
            break;
        }
    }

    ASSERT_FALSE(err_code) << err_code.message();

    if (found_child)
    {
        const bool result = fs::open_directory(child, parent, &err_code);

        EXPECT_TRUE(result);
        EXPECT_FALSE(err_code) << err_code.message();

        if (result)
        {
            EXPECT_TRUE(fs::close_directory(child));
        }
    }

    EXPECT_TRUE(fs::close_directory(parent));
    EXPECT_TRUE(found_child);
}

TEST(system_filesystem_read_directory, reads_directory_entries)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(path).good());

    fs::directory_stream stream;
    std::error_code err_code;

    ASSERT_TRUE(fs::open_directory(stream, workspace.path().c_str()));

    bool found_file = false;

    while (fs::read_directory(stream, &err_code))
    {
        if (std_fs::path(stream.name).filename() == path.filename())
        {
            found_file = true;
            break;
        }
    }

    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_TRUE(found_file);
    EXPECT_TRUE(fs::close_directory(stream));
}

TEST(system_filesystem_close_directory, closes_open_directory)
{
    temporary_workspace workspace;
    fs::directory_stream stream;

    ASSERT_TRUE(fs::open_directory(stream, workspace.path().c_str()));

    std::error_code err_code;
    const bool result = fs::close_directory(stream, &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
}

TEST(system_filesystem_remove_directory, removes_empty_directory)
{
    temporary_workspace workspace;
    const auto path = workspace.path("directory");

    ASSERT_TRUE(std_fs::create_directory(path));

    std::error_code err_code;
    const bool result = fs::remove_directory(path.c_str(), &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_FALSE(std_fs::exists(path));
}

TEST(system_filesystem_remove_path, removes_existing_file)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(path).good());

    std::error_code err_code;
    const bool result = fs::remove_path(path.c_str(), &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_FALSE(std_fs::exists(path));
}

TEST(system_filesystem_remove_regular_file, removes_regular_file)
{
    temporary_workspace workspace;
    const auto path = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(path).good());

    std::error_code err_code;
    const bool result = fs::remove_regular_file(path.c_str(), &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_FALSE(std_fs::exists(path));
}

TEST(system_filesystem_set_current_directory, changes_working_directory)
{
    temporary_workspace workspace;
    const auto original_path = std_fs::current_path();
    const auto target_path = workspace.path();

    struct directory_guard
    {
        std_fs::path original;

        ~directory_guard()
        {
            std::error_code err_code;
            std_fs::current_path(original, err_code);
        }
    } guard{original_path};

    std::error_code err_code;
    const bool result = fs::set_current_directory(target_path.c_str(), &err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();

    if (result)
    {
        EXPECT_TRUE(std_fs::equivalent(std_fs::current_path(), target_path));
    }
}

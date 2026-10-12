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
 * @file validatable_path_test.cpp
 * @brief xxxxxx
 * @author Killian Valverde
 * @date 2024-05-20
 */

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>
#include <type_traits>
#include <utility>
#include <gtest/gtest.h>

#include "speed/filesystem/filesystem.hpp"

namespace fs = speed::filesystem;
namespace std_fs = std::filesystem;

namespace {

class temporary_workspace
{
public:
    temporary_workspace()
    {
        static std::size_t counter = 0;

        path_ = std_fs::temp_directory_path() / ("speed_validatable_path_test_" +
             std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + "_" +
             std::to_string(counter++));

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

TEST(filesystem_validatable_path, default_constructor)
{
    fs::validatable_path path;

    EXPECT_TRUE(path.empty());
    EXPECT_FALSE(path.is_valid());
}

TEST(filesystem_validatable_path, constructs_from_filesystem_path)
{
    temporary_workspace workspace;
    const fs::validatable_path path(workspace.path());

    EXPECT_EQ(static_cast<const std_fs::path&>(path), workspace.path());
}

TEST(filesystem_validatable_path, accepts_existing_file)
{
    temporary_workspace workspace;
    const auto file = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(file).good());

    const fs::validatable_path path(file);
    std::error_code err_code;

    EXPECT_TRUE(path.is_valid(&err_code));
    EXPECT_FALSE(err_code) << err_code.message();
}

TEST(filesystem_validatable_path, accepts_existing_directory)
{
    temporary_workspace workspace;
    const fs::validatable_path path(workspace.path());

    EXPECT_TRUE(path.is_valid());
}

TEST(filesystem_validatable_path, rejects_nonexistent_path)
{
    temporary_workspace workspace;
    const fs::validatable_path path(workspace.path("missing.txt"));

    EXPECT_FALSE(path.is_valid());
}

TEST(filesystem_validatable_path, validation_does_not_create_path)
{
    temporary_workspace workspace;
    const auto file = workspace.path("missing.txt");
    const fs::validatable_path path(file);

    EXPECT_FALSE(path.is_valid());
    EXPECT_FALSE(std_fs::exists(file));
}

TEST(filesystem_regular_file_path, accepts_regular_file)
{
    temporary_workspace workspace;
    const auto file = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(file).good());

    const fs::regular_file_path path(file);

    EXPECT_TRUE(path.is_valid());
}

TEST(filesystem_regular_file_path, rejects_directory)
{
    temporary_workspace workspace;
    const fs::regular_file_path path(workspace.path());

    EXPECT_FALSE(path.is_valid());
}

TEST(filesystem_regular_file_path, creates_missing_file)
{
    temporary_workspace workspace;
    const auto file = workspace.path("file.txt");

    const fs::regular_file_path path(file);
    std::error_code err_code;

    EXPECT_FALSE(path.is_valid());

    const bool result = path.ensure_exists(&err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_TRUE(std_fs::is_regular_file(file));
    EXPECT_TRUE(path.is_valid());
}

TEST(filesystem_regular_file_path, creates_missing_parent_directories)
{
    temporary_workspace workspace;
    const auto file = workspace.path("first/second/file.txt");

    const fs::regular_file_path path(file);
    std::error_code err_code;

    const bool result = path.ensure_exists(&err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_TRUE(std_fs::is_regular_file(file));
    EXPECT_TRUE(path.is_valid());
}

TEST(filesystem_regular_file_path, preserves_existing_file)
{
    temporary_workspace workspace;
    const auto file = workspace.path("file.txt");

    {
        std::ofstream stream(file);
        ASSERT_TRUE(stream.good());
        stream << "Original content";
    }

    const fs::regular_file_path path(file);

    EXPECT_TRUE(path.ensure_exists());

    std::ifstream stream(file);
    std::string content;

    std::getline(stream, content);

    EXPECT_EQ(content, "Original content");
}

TEST(filesystem_regular_file_path, does_not_replace_existing_directory)
{
    temporary_workspace workspace;
    const auto directory = workspace.path("existing");

    ASSERT_TRUE(std_fs::create_directory(directory));

    const fs::regular_file_path path(directory);

    EXPECT_TRUE(path.ensure_exists());
    EXPECT_FALSE(path.is_valid());
    EXPECT_TRUE(std_fs::is_directory(directory));
}

TEST(filesystem_directory_path, accepts_directory)
{
    temporary_workspace workspace;
    const fs::directory_path path(workspace.path());

    EXPECT_TRUE(path.is_valid());
}

TEST(filesystem_directory_path, rejects_regular_file)
{
    temporary_workspace workspace;
    const auto file = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(file).good());

    const fs::directory_path path(file);

    EXPECT_FALSE(path.is_valid());
}

TEST(filesystem_directory_path, creates_missing_directory)
{
    temporary_workspace workspace;
    const auto directory = workspace.path("new_directory");

    const fs::directory_path path(directory);
    std::error_code err_code;

    EXPECT_FALSE(path.is_valid());

    const bool result = path.ensure_exists(&err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_TRUE(std_fs::is_directory(directory));
    EXPECT_TRUE(path.is_valid());
}

TEST(filesystem_directory_path, creates_nested_directories)
{
    temporary_workspace workspace;
    const auto directory = workspace.path("first/second/third");

    const fs::directory_path path(directory);

    EXPECT_TRUE(path.ensure_exists());
    EXPECT_TRUE(std_fs::is_directory(directory));
    EXPECT_TRUE(path.is_valid());
}

TEST(filesystem_directory_path, preserves_existing_directory)
{
    temporary_workspace workspace;
    const fs::directory_path path(workspace.path());

    EXPECT_TRUE(path.ensure_exists());
    EXPECT_TRUE(path.is_valid());
}

TEST(filesystem_directory_path, does_not_replace_existing_file)
{
    temporary_workspace workspace;
    const auto file = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(file).good());

    const fs::directory_path path(file);

    EXPECT_TRUE(path.ensure_exists());
    EXPECT_FALSE(path.is_valid());
    EXPECT_TRUE(std_fs::is_regular_file(file));
}

TEST(filesystem_readable_path_decorator, accepts_readable_file)
{
    temporary_workspace workspace;
    const auto file = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(file).good());

    const fs::r_regular_file_path path(file);

    EXPECT_TRUE(path.is_valid());
}

TEST(filesystem_writable_path_decorator, accepts_writable_file)
{
    temporary_workspace workspace;
    const auto file = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(file).good());

    const fs::w_regular_file_path path(file);

    EXPECT_TRUE(path.is_valid());
}

TEST(filesystem_executable_path_decorator, accepts_executable_directory)
{
    temporary_workspace workspace;

    const fs::x_directory_path path(workspace.path());

    EXPECT_TRUE(path.is_valid());
}

TEST(filesystem_validatable_path, accepts_readable_writable_file)
{
    temporary_workspace workspace;
    const auto file = workspace.path("file.txt");

    ASSERT_TRUE(std::ofstream(file).good());

    const fs::rw_regular_file_path path(file);

    EXPECT_TRUE(path.is_valid());
}

TEST(filesystem_validatable_path, accepts_readable_writable_directory)
{
    temporary_workspace workspace;

    const fs::rw_directory_path path(workspace.path());

    EXPECT_TRUE(path.is_valid());
}

TEST(filesystem_validatable_path, accepts_readable_writable_executable_directory)
{
    temporary_workspace workspace;

    const fs::rwx_directory_path path(workspace.path());

    EXPECT_TRUE(path.is_valid());
}

TEST(filesystem_validatable_path, rejects_wrong_type_with_access_requirements)
{
    temporary_workspace workspace;

    const fs::rw_regular_file_path path(workspace.path());

    EXPECT_FALSE(path.is_valid());
}

TEST(filesystem_output_path_decorator, constructs_without_creating_file)
{
    temporary_workspace workspace;
    const auto file = workspace.path("output.txt");

    const fs::output_regular_file_path path(file);

    EXPECT_FALSE(std_fs::exists(file));
    EXPECT_FALSE(path.is_valid());
}

TEST(filesystem_output_path_decorator, creates_output_regular_file)
{
    temporary_workspace workspace;
    const auto file = workspace.path("nested/output.txt");

    const fs::output_regular_file_path path(file);
    std::error_code err_code;

    const bool result = path.ensure_exists(&err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_TRUE(std_fs::is_regular_file(file));
    EXPECT_TRUE(path.is_valid());
}

TEST(filesystem_output_path_decorator, constructs_without_creating_directory)
{
    temporary_workspace workspace;
    const auto directory = workspace.path("output");

    const fs::output_directory_path path(directory);

    EXPECT_FALSE(std_fs::exists(directory));
    EXPECT_FALSE(path.is_valid());
}

TEST(filesystem_output_path_decorator, creates_output_directory)
{
    temporary_workspace workspace;
    const auto directory = workspace.path("nested/output");

    const fs::output_directory_path path(directory);
    std::error_code err_code;

    const bool result = path.ensure_exists(&err_code);

    EXPECT_TRUE(result);
    EXPECT_FALSE(err_code) << err_code.message();
    EXPECT_TRUE(std_fs::is_directory(directory));
    EXPECT_TRUE(path.is_valid());
}

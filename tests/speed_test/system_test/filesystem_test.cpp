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
 * @file        filesystem_test.cpp
 * @brief       filesystem unit test.
 * @author      Killian Valverde
 * @date        2018/06/01
 */

#include <gtest/gtest.h>

#include "speed/filesystem/filesystem.hpp"
#include "speed/system/system.hpp"

class system_filesystem : public ::testing::Test
{
protected:
    system_filesystem()
    {
    }

    void SetUp() override
    {
    }

    const speed::system::filesystem::path_char_t* cur_dir_pth =
            speed::filesystem::pick_path(".", L".");
    
    const speed::system::filesystem::path_char_t* parent_dir_pth =
            speed::filesystem::pick_path("..", L"..");
    
    const speed::system::filesystem::path_char_t* dir_nme =
            speed::filesystem::pick_path(
                    "6bd55a4e9d240fe0bcc137ad1eaeaf67517cfe4c",
                    L"6bd55a4e9d240fe0bcc137ad1eaeaf67517cfe4c");
    
    const speed::system::filesystem::path_char_t* regfile_nme =
            speed::filesystem::pick_path(
                    "f57475c9df7a624a05e842b5f852336015de44ba",
                    L"f57475c9df7a624a05e842b5f852336015de44ba");
    
    const speed::system::filesystem::path_char_t* shortcut_nme =
            speed::filesystem::pick_path(
                    "f57475c9df7a624a05e842b5f852336015de44ba"
                    SPEED_SYSTEM_FILESYSTEM_SHORTCUT_EXTENSION_CSTR,
                    L"f57475c9df7a624a05e842b5f852336015de44ba"
                    SPEED_SYSTEM_FILESYSTEM_SHORTCUT_EXTENSION_CSTR);
};

TEST_F(system_filesystem, access)
{
    ASSERT_TRUE(speed::system::filesystem::access(cur_dir_pth, false,
            speed::system::filesystem::am_t::EXISTS));
}

TEST_F(system_filesystem, chdir)
{
    ASSERT_TRUE(speed::system::filesystem::chdir(cur_dir_pth));
}

TEST_F(system_filesystem, check_file)
{
    ASSERT_TRUE(speed::system::filesystem::check_file(cur_dir_pth, false,
            speed::system::filesystem::am_t::EXISTS, speed::system::filesystem::ft_t::DIRECTORY));
}

TEST_F(system_filesystem, closedir)
{
    speed::system::filesystem::directory_entity dir_ent;

    ASSERT_TRUE(speed::system::filesystem::opendir(dir_ent, cur_dir_pth));
    ASSERT_TRUE(speed::system::filesystem::closedir(dir_ent));
}

TEST_F(system_filesystem, file_exists)
{
    ASSERT_TRUE(speed::system::filesystem::file_exists(cur_dir_pth, false));
}

TEST_F(system_filesystem, get_file_inode)
{
    ASSERT_TRUE(speed::system::filesystem::get_file_inode(cur_dir_pth, false) != ~0ull);
    ASSERT_TRUE(speed::system::filesystem::get_file_inode(parent_dir_pth, false) != ~0ull);
    ASSERT_TRUE(speed::system::filesystem::get_file_inode(regfile_nme, false) == ~0ull);
}

TEST_F(system_filesystem, get_file_uid)
{
    auto file_uid = speed::system::filesystem::get_file_uid(cur_dir_pth, false);
    auto user_uid = speed::system::process::get_uid();
    ASSERT_TRUE(file_uid == user_uid && file_uid != -1);
}

TEST_F(system_filesystem, get_file_gid)
{
    auto file_gid = speed::system::filesystem::get_file_gid(cur_dir_pth, false);
    auto user_gid = speed::system::process::get_gid();
    ASSERT_TRUE(file_gid == user_gid && file_gid != -1);
}

TEST_F(system_filesystem, get_modification_time)
{
    speed::system::time::system_time system_tme;
    ASSERT_TRUE(speed::system::filesystem::get_modification_time(cur_dir_pth, false, system_tme));
}

TEST_F(system_filesystem, get_temporal_path)
{
    ASSERT_TRUE(speed::system::filesystem::get_temporal_path() != nullptr);
}

TEST_F(system_filesystem, is_block_device)
{
    ASSERT_TRUE(!speed::system::filesystem::is_block_device(cur_dir_pth, false));
}

TEST_F(system_filesystem, is_character_device)
{
    ASSERT_TRUE(!speed::system::filesystem::is_character_device(cur_dir_pth, false));
}

TEST_F(system_filesystem, is_directory)
{
    ASSERT_TRUE(speed::system::filesystem::is_directory(cur_dir_pth, false));
    ASSERT_TRUE(!speed::system::filesystem::is_directory(regfile_nme, false));
}

TEST_F(system_filesystem, is_fifo)
{
    ASSERT_TRUE(!speed::system::filesystem::is_pipe(cur_dir_pth, false));
}

TEST_F(system_filesystem, is_regular_file)
{
    ASSERT_TRUE(!speed::system::filesystem::is_regular_file(regfile_nme, false));
    ASSERT_TRUE(speed::system::filesystem::touch(regfile_nme));
    ASSERT_TRUE(speed::system::filesystem::is_regular_file(regfile_nme, false));
    ASSERT_TRUE(speed::system::filesystem::unlink(regfile_nme));
}

TEST_F(system_filesystem, is_socket)
{
    ASSERT_TRUE(!speed::system::filesystem::is_socket(cur_dir_pth, false));
}

TEST_F(system_filesystem, is_symlink)
{
    ASSERT_TRUE(!speed::system::filesystem::is_symlink(cur_dir_pth, false));
    ASSERT_TRUE(speed::system::filesystem::symlink(cur_dir_pth, regfile_nme));
    ASSERT_TRUE(speed::system::filesystem::is_symlink(regfile_nme, false));
    ASSERT_TRUE(speed::system::filesystem::unlink(regfile_nme));
}

TEST_F(system_filesystem, mkdir)
{
    ASSERT_TRUE(speed::system::filesystem::mkdir(dir_nme));
}

TEST_F(system_filesystem, mkdir_recursively)
{
    // ASSERT_TRUE(speed::system::filesystem::mkdir_recursively(
    //         "f57475c9df7a624a05e842b5f852336015de44ba"
    //         SPEED_SYSTEM_FILESYSTEM_SLASH_CSTR
    //         "87e54df0169fce4cec5c064d8f1dfa6162388226"));
}

TEST_F(system_filesystem, opendir)
{
    speed::system::filesystem::directory_entity dir_ent;

    ASSERT_TRUE(speed::system::filesystem::opendir(dir_ent,cur_dir_pth));
    ASSERT_TRUE(speed::system::filesystem::closedir(dir_ent));
}

TEST_F(system_filesystem, readdir)
{
    speed::system::filesystem::directory_entity dir_ent;

    ASSERT_TRUE(speed::system::filesystem::opendir(dir_ent, cur_dir_pth));
    ASSERT_TRUE(speed::system::filesystem::readdir(dir_ent));
    ASSERT_TRUE(speed::system::filesystem::readdir(dir_ent));
    ASSERT_TRUE(speed::system::filesystem::closedir(dir_ent));
}

TEST_F(system_filesystem, rmdir)
{
    ASSERT_TRUE(speed::system::filesystem::rmdir(dir_nme));
}

TEST_F(system_filesystem, shortcut)
{
    ASSERT_TRUE(speed::system::filesystem::shortcut(cur_dir_pth, regfile_nme));
    ASSERT_TRUE(speed::system::filesystem::unlink(shortcut_nme));
}

TEST_F(system_filesystem, touch)
{
    ASSERT_TRUE(speed::system::filesystem::touch(regfile_nme));
}

TEST_F(system_filesystem, unlink)
{
    ASSERT_TRUE(speed::system::filesystem::unlink(regfile_nme));
}

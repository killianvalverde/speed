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
 * @file        speed/filesystem/directory_iteration.cpp
 * @brief       directory_iteration class methods definition.
 * @author      Killian Valverde
 * @date        2018/11/25
 */

#include "../stringutils/stringutils.hpp"

#include "directory_iteration.hpp"


namespace speed::filesystem {


//directory_iteration::iterator::iterator(directory_iteration* compo)
//        : stck_()
//        , vistd_inos_()
//        , cur_pth_()
//        , end_(false)
//        , opend_(false)
//        , compo_(compo)
//{
//    stck_.emplace();
//    if (system::opendir(&stck_.top(), compo_->dir_pth_.c_str()))
//    {
//        opend_ = true;
//        this->operator++();
//    }
//    else
//    {
//        end_ = true;
//    }
//}


//directory_iteration::iterator::~iterator()
//{
//    //if (opend_)
//    //{
//    //    system::closedir(&dir_ent_);
//    //}
//}


//directory_iteration::iterator::self_type& directory_iteration::iterator::operator ++()
//{
//    //system::basic_directory_entity* cur_dir_ent = &stck_.top();
//    //
//    //if (speed::system::is_directory(cur_pth_.c_str()))
//    //{
//    //    stck_.emplace();
//    //    cur_dir_ent = &stck_.top();
//    //    system::opendir(cur_dir_ent, cur_pth_.c_str());
//    //}
//    //
//    //do
//    //{
//    //    system::readdir(cur_dir_ent);
//    //} while (cur_dir_ent.
//    //        speed::stringutils::strcmp(cur_dir_ent->nme, ".") == 0 ||
//    //         speed::stringutils::strcmp(cur_dir_ent->nme, "..") == 0);
//    //
//    //
//    //while (!system::readdir(&dir_ent_))
//    //{
//    //    end_ = true;
//    //}
//}


//directory_iteration::directory_iteration(
//        std::filesystem::path dir_pth,
//        system::file_types fle_typs,
//        std::uint64_t deep_lv,
//        std::string regx_to_match
//)
//        : dir_pth_(std::move(dir_pth))
//        , fle_typs_(fle_typs)
//        , deep_lv_(deep_lv)
//        , regx_(std::move(regx_to_match))
//{
//}


}

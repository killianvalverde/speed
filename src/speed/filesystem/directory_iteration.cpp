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
 * @file        directory_iteration.cpp
 * @brief       directory_iteration class methods definition.
 * @author      Killian Valverde
 * @date        2024/10/15
 */

#include "directory_iteration.hpp"

#include "../stringutils/stringutils.hpp"

namespace speed::filesystem {

directory_iteration::const_iterator::const_iterator(const directory_iteration* composit)
       : cur_dir_()
       , cur_fle_()
       , directory_entity_stck_()
       , vistd_inos_()
       , composit_(composit)
       , end_(false)
       , current_recursivity_levl_(0)
{
    if (composit == nullptr)
    {
        end_ = true;
        return;
    }

    cur_dir_ = composit->root_pth_;
    cur_fle_ = cur_dir_;

    if (open_directory())
    {
        this->operator++();
    }
}

directory_iteration::const_iterator::~const_iterator() noexcept
{
    while (!directory_entity_stck_.empty())
    {
        speed::system::filesystem::closedir(&directory_entity_stck_.top());
        directory_entity_stck_.pop();
    }

    end_ = true;
}

directory_iteration::const_iterator::self_type& directory_iteration::const_iterator::operator ++()
{
start:
    directory_entity& cur_dir_ent = directory_entity_stck_.top();

    if (!read_directory())
    {
        close_directory();

        if (end_)
        {
            return *this;
        }
    }
    else
    {
        cur_fle_.replace_filename(cur_dir_ent.nme);

        if (speed::system::filesystem::is_directory(&cur_dir_ent))
        {
            cur_dir_ /= cur_dir_ent.nme;

            if (open_directory())
            {
                goto start;
            }
        }
    }

    if (!is_file_valid())
    {
        goto start;
    }

    return *this;
}

bool directory_iteration::const_iterator::operator ==(const self_type& rhs) const noexcept
{
    if (end_ && rhs.end_)
    {
        return true;
    }
    if (composit_ != rhs.composit_ || cur_fle_ != rhs.cur_fle_)
    {
        return false;
    }

    return true;
}

bool directory_iteration::const_iterator::open_directory()
{
    speed::system::filesystem::inode_t ino;
    
    if (composit_->inode_trackr_)
    {
        ino = speed::system::filesystem::get_file_inode(cur_dir_.c_str());
    }

    if (current_recursivity_levl_ > composit_->max_recursivity_levl_ ||
        (composit_->inode_trackr_ && vistd_inos_.contains(ino)) ||
        (!composit_->follow_symbolic_lnks_ &&
                speed::system::filesystem::is_symlink(cur_dir_.c_str())))
    {
        cur_fle_ = cur_dir_;
        cur_dir_ = cur_dir_.parent_path();
        return false;
    }

    ++current_recursivity_levl_;
    cur_fle_ /= ".";
    directory_entity_stck_.emplace();

    if (!speed::system::filesystem::opendir(&directory_entity_stck_.top(), cur_dir_.c_str()))
    {
        exit_directory();
        return false;
    }

    if (composit_->inode_trackr_)
    {
        vistd_inos_.emplace(ino);
    }
    
    return true;
}

bool directory_iteration::const_iterator::read_directory()
{
    bool succss;
    directory_entity& cur_dir_ent = directory_entity_stck_.top();

    do
    {
        succss = speed::system::filesystem::readdir(&cur_dir_ent);
    }
    while (succss && (speed::stringutils::cstr_compare(cur_dir_ent.nme, ".") == 0 ||
                    speed::stringutils::cstr_compare(cur_dir_ent.nme, "..") == 0));

    return succss;
}

void directory_iteration::const_iterator::close_directory()
{
    speed::system::filesystem::closedir(&directory_entity_stck_.top());
    exit_directory();
}

void directory_iteration::const_iterator::exit_directory()
{
    directory_entity_stck_.pop();
    cur_fle_ = cur_dir_;
    cur_dir_ = cur_dir_.parent_path();
    --current_recursivity_levl_;

    if (directory_entity_stck_.empty())
    {
        end_ = true;
    }
}

[[nodiscard]] bool directory_iteration::const_iterator::is_file_valid()
{
    if (!composit_->substring_to_mtch_.empty() && !find_substr())
    {
        return false;
    }
    if (!composit_->wildcard_to_mtch_.empty() && !matches_wildcard())
    {
        return false;
    }
    if (!composit_->regex_to_mtch_str_.empty() &&
        !std::regex_match(cur_fle_.filename().c_str(), composit_->regex_to_mtch_))
    {
        return false;
    }
    
    if (composit_->file_typs_ != speed::system::filesystem::file_types::NIL &&
        !speed::system::filesystem::is_file_type(cur_fle_.c_str(), composit_->file_typs_))
    {
        return false;
    }
    if (composit_->access_mods_ != speed::system::filesystem::access_modes::NIL &&
        !speed::system::filesystem::access(cur_fle_.c_str(), composit_->access_mods_))
    {
        return false;
    }

    return true;
}

[[nodiscard]] bool directory_iteration::const_iterator::find_substr() const
{
    const auto& pth = cur_fle_.filename();
    const char_type* substr = composit_->substring_to_mtch_.c_str();

    return composit_->case_insensitve_
            ? speed::stringutils::cstr_find_substr_icase(pth.c_str(), substr)
            : speed::stringutils::cstr_find_substr(pth.c_str(), substr);
}

[[nodiscard]] bool directory_iteration::const_iterator::matches_wildcard() const
{
    const auto& pth = cur_fle_.filename();
    const char_type* wildcrd = composit_->wildcard_to_mtch_.c_str();

    return composit_->case_insensitve_
            ? speed::stringutils::match_wildcard_icase(pth.c_str(), wildcrd)
            : speed::stringutils::match_wildcard(pth.c_str(), wildcrd);
}

void directory_iteration::update_regex()
{
    typename regex_type::flag_type flg;
    
    if (case_insensitve_)
    {
        flg = regex_type::ECMAScript | regex_type::icase;
    }
    else
    {
        flg = regex_type::ECMAScript;
    }
    
    regex_to_mtch_.assign(regex_to_mtch_str_, flg);
}

}

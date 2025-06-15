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
 * @file        indentation.cpp
 * @brief       indentation class methods definition.
 * @author      Killian Valverde
 * @date        2018/01/10
 */

#include "indentation.hpp"

namespace speed::iostream {

indentation::indentation(std::size_t tab_size, std::size_t curr_size) noexcept
        : tab_size_(tab_size)
        , curr_size_(curr_size)
{
}

indentation& indentation::operator ++() noexcept
{
    curr_size_ += tab_size_;
    return *this;
}

const indentation indentation::operator ++(int) noexcept
{
    indentation old_indent(*this);
    curr_size_ += tab_size_;
    
    return old_indent;
}

indentation& indentation::operator --() noexcept
{
    if (curr_size_ > tab_size_)
    {
        curr_size_ -= tab_size_;
    }
    else
    {
        curr_size_ = 0;
    }
    
    return *this;
}

const indentation indentation::operator --(int) noexcept
{
    indentation old_indent(*this);
    
    if (curr_size_ > tab_size_)
    {
        curr_size_ -= tab_size_;
    }
    else
    {
        curr_size_ = 0;
    }
    
    return old_indent;
}

}

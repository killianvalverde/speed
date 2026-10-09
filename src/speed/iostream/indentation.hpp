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
 * @file indentation.hpp
 * @brief indentation class header.
 * @author Killian Valverde
 * @date 2018-01-10
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <ostream>

namespace speed::iostream {

/**
 * @brief Stream manipulator that inserts a configurable number of spaces.
 */
class indentation
{
public:
    /**
     * @brief Constructs an indentation.
     * 
     * @param tab_sz The number of spaces added or removed by each increment or decrement.
     * @param curr_sz The initial indentation width, in spaces.
     */
    constexpr explicit indentation(std::size_t tab_sz = 4, std::size_t curr_sz = 0) noexcept
        : tab_sz_(tab_sz)
        , curr_sz_(curr_sz)
    {
    }

    /**
     * @brief Increases the indentation by one step.
     * 
     * @return A reference to this object.
     */
    constexpr indentation& operator++() noexcept
    {
        curr_sz_ += tab_sz_;
        return *this;
    }

    /**
     * @brief Increases the indentation by one step.
     * 
     * @return A copy of this object as it was before the increment.
     */
    [[nodiscard]] constexpr indentation operator++(int) noexcept
    {
        indentation old_indent(*this);
        ++*this;
        return old_indent;
    }

    /**
     * @brief Decreases the indentation by one step.
     *
     * The width never goes below zero: if the current width is smaller than one step, it is
     * set to zero.
     *
     * @return A reference to this object.
     */
    constexpr indentation& operator--() noexcept
    {
        curr_sz_ -= std::min(curr_sz_, tab_sz_);
        return *this;
    }

    /**
     * @brief Decreases the indentation by one step.
     *
     * The width never goes below zero: if the current width is smaller than one step, it is
     * set to zero.
     *
     * @return A copy of this object as it was before the decrement.
     */
    [[nodiscard]] constexpr indentation operator--(int) noexcept
    {
        indentation old_indent(*this);
        --*this;
        return old_indent;
    }

    /**
     * @brief Writes the current indentation to an output stream.
     *
     * Writes as many spaces as the current indentation width. The space character is obtained
     * through the stream's widen(), so it is correct for any character type. Writing stops
     * early if the stream enters a failed state.
     *
     * @tparam CharT The character type of the stream.
     * @tparam CharTraitsT The character traits type of the stream.
     * 
     * @param os The output stream to write to.
     * @param indent The indentation to write.
     * @return The output stream.
     */
    template<typename CharT, typename CharTraitsT>
    friend std::basic_ostream<CharT, CharTraitsT>& operator<<(
        std::basic_ostream<CharT, CharTraitsT>& os,
        const indentation& indent
    )
    {
        const CharT space = os.widen(' ');

        for (std::size_t i = 0; i < indent.curr_sz_ && os; ++i)
        {
            os.put(space);
        }

        return os;
    }

private:
    /** The number of spaces added or removed by each increment or decrement. */
    std::size_t tab_sz_;

    /** The current indentation width, in spaces. */
    std::size_t curr_sz_;
};

}

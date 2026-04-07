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
 * @file        indentation.hpp
 * @brief       indentation class header.
 * @author      Killian Valverde
 * @date        2018/01/10
 */

#ifndef SPEED_IOSTREAM_BASIC_INDENTATION_HPP
#define SPEED_IOSTREAM_BASIC_INDENTATION_HPP

#include <iostream>

namespace speed::iostream {

/**
 * @brief       Class used to print indentation.
 */
class indentation
{
public:
    /**
     * @brief       Constructor with parameters.
     * @param       tab_sz : The babulator size.
      * @param      curr_sz : The current size of the indentation.
     */
    explicit indentation(std::size_t tab_sz = 4, std::size_t curr_sz = 0) noexcept
            : tab_sz_(tab_sz)
            , curr_sz_(curr_sz)
    {
    }
    
    /**
     * @brief       Increase the indentation by the tab size.
     * @return      The object who call the method.
     */
    indentation& operator ++() noexcept
    {
        curr_sz_ += tab_sz_;
        return *this;
    }
    
    /**
     * @brief       Increase the indentation by the tab size.
     * @return      The object who call the method.
     */
    indentation operator ++(int) noexcept
    {
        indentation old_indent(*this);
        curr_sz_ += tab_sz_;

        return old_indent;
    }
    
    /**
     * @brief       Decrease the indentation by the tab size.
     * @return      The object who call the method.
     */
    indentation& operator --() noexcept
    {
        if (curr_sz_ > tab_sz_)
        {
            curr_sz_ -= tab_sz_;
        }
        else
        {
            curr_sz_ = 0;
        }

        return *this;
    }

    /**
     * @brief       Decrease the indentation by the tab size.
     * @return      The object who call the method.
     */
    indentation operator --(int) noexcept
    {
        indentation old_indent(*this);

        if (curr_sz_ > tab_sz_)
        {
            curr_sz_ -= tab_sz_;
        }
        else
        {
            curr_sz_ = 0;
        }

        return old_indent;
    }

    /**
     * @brief       Allows to use an object to print in standard output the value that constains the
     *              indentation.
     * @param       os : The object who call the function.
     * @param       indent : The indent to print.
     * @return      The object who call the function.
     */
    template<typename CharT_, typename CharTraitsT_>
    friend std::basic_ostream<CharT_, CharTraitsT_>& operator <<(
            std::basic_ostream<CharT_, CharTraitsT_>& os,
            const indentation& indent
    );

private:
    /** The balulator size. */
    std::size_t tab_sz_;
    
    /** The size of the current indentation. */
    std::size_t curr_sz_;
};

/**
 * @brief       Allows to use an object to print in standard output the value that constains the
 *              indentation.
 * @param       os : The object who call the function.
 * @param       indent : The indent to print.
 * @return      The object who call the function.
 */
template<typename CharT, typename CharTraitsT>
std::basic_ostream<CharT, CharTraitsT>& operator <<(
        std::basic_ostream<CharT, CharTraitsT>& os,
        const indentation& indent
)
{
    for (std::size_t i = 0; i < indent.curr_sz_; i++)
    {
        os.put(' ');
    }
    
    return os;
}

}

#endif

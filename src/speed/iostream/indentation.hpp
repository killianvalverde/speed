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
 * @file        speed/iostream/indentation.hpp
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
     * @param       tab_size : The babulator size.
      * @param      curr_size : The current size of the indentation.
     */
    indentation(std::size_t tab_size = 4, std::size_t curr_size = 0) noexcept;
    
    /**
     * @brief       Increase the indentation by the tab size.
     * @return      The object who call the method.
     */
    indentation& operator ++() noexcept;
    
    /**
     * @brief       Increase the indentation by the tab size.
     * @return      The object who call the method.
     */
    const indentation operator ++(int) noexcept;
    
    /**
     * @brief       Decrease the indentation by the tab size.
     * @return      The object who call the method.
     */
    indentation& operator --() noexcept;
    
    /**
     * @brief       Decrease the indentation by the tab size.
     * @return      The object who call the method.
     */
    const indentation operator --(int) noexcept;

    /**
     * @brief       Allows to use an object to print in standard output the value that constains the
     *              indentation.
     * @param       os : The object who call the function.
     * @param       indent : The indent to print.
     * @return      The object who call the function.
     */
    template<typename TpChar_, typename TpCharTraits_>
    friend std::basic_ostream<TpChar_, TpCharTraits_>& operator <<(
            std::basic_ostream<TpChar_, TpCharTraits_>& os,
            const indentation& indent
    );

private:
    /** The balulator size. */
    std::size_t tab_size_;
    
    /** The size of the current indentation. */
    std::size_t curr_size_;
};


/**
 * @brief       Allows to use an object to print in standard output the value that constains the
 *              indentation.
 * @param       os : The object who call the function.
 * @param       indent : The indent to print.
 * @return      The object who call the function.
 */
template<typename TpChar, typename TpCharTraits>
std::basic_ostream<TpChar, TpCharTraits>& operator <<(
        std::basic_ostream<TpChar, TpCharTraits>& os,
        const indentation& indent
)
{
    for (std::size_t i = 0; i < indent.curr_size_; i++)
    {
        os << ' ';
    }
    
    return os;
}


}


#endif

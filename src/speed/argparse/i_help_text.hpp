/* speed - Generic C++ library.
 * Copyright (C) 2015-2023 Killian Valverde.
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
 * @file        speed/argparse/i_help_text.hpp
 * @brief       i_help_text class header.
 * @author      Killian Green
 * @date        2016/03/12 - 20:35
 */

#ifndef SPEED_ARGPARSE_I_HELP_TEXT_HPP
#define SPEED_ARGPARSE_I_HELP_TEXT_HPP

#include <string>


namespace speed::argparse {


/**
 * @brief       Interface that represents a text that can be printed in the help menu.
 */
class i_help_text
{
public:
    /**
     * @brief       Print the argument information for help menu.
     * @param       max_line_len : The maximum description length that will be printed in a single
     *              line.
     * @param       newline_indent : The indentation used when a newline is found in a description.
     * @param       current_line_len : The length of the current line.
     */
    virtual void print_help_text(
            std::size_t max_line_len,
            std::size_t newline_indent,
            std::size_t current_line_len
    ) const = 0;

};


}


#endif

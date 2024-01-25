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
 * @file        speed/argparse/basic_help_text.hpp
 * @brief       basic_help_text class header.
 * @author      Killian Valverde
 * @date        2016/03/12 - 20:35
 */

#ifndef SPEED_ARGPARSE_BASIC_HELP_TEXT_HPP
#define SPEED_ARGPARSE_BASIC_HELP_TEXT_HPP

#include "../lowlevel.hpp"
#include "i_help_text.hpp"


namespace speed::argparse {


/**
 * @brief       Class that represents a text in the help information.
 */
template<typename TpAllocator>
class basic_help_text : public i_help_text
{
public:
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;
    
    /** Class that represents sequences of characters. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       desc : Argument description content.
     */
    template<
            typename TpString_,
            typename = std::enable_if_t<
                    !std::is_base_of<basic_help_text, std::decay_t<TpString_>>::value
            >
    >
    basic_help_text(TpString_&& desc)
            : txt_(std::forward<TpString_>(desc))
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_help_text(const basic_help_text& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_help_text(basic_help_text&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_help_text() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_help_text& operator =(const basic_help_text& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_help_text& operator =(basic_help_text&& rhs) noexcept = default;
    
    
    /**
     * @brief       Print the argument information for help menu.
     * @param       max_line_len : The maximum description length that will be printed in a
     *              single line.
     * @param       new_line_indent : The indentation used when a new line is found in a
     *              description.
     * @param       current_line_len : The length of the current line.
     */
    void print_help_text(
            std::size_t max_line_len,
            std::size_t new_line_indent,
            std::size_t current_line_len
    ) const override
    {
        if (!txt_.empty())
        {
            std::size_t len_to_next;
    
            for (auto it = txt_.cbegin(); it != txt_.cend(); it++)
            {
                if (*it == '\n')
                {
                    current_line_len = new_line_indent;
                    std::cout << '\n';
                    for (std::size_t i = 0; i < new_line_indent; i++)
                    {
                        std::cout << ' ';
                    }
                }
                else if (*it == ' ')
                {
                    len_to_next = 1;
            
                    auto aux_it = it;
                    aux_it++;
                    for (; aux_it != txt_.cend() && *aux_it != ' '; aux_it++)
                    {
                        len_to_next++;
                    }
            
                    speed::lowlevel::try_addm(&len_to_next, current_line_len);
                    if (len_to_next > max_line_len)
                    {
                        current_line_len = new_line_indent;
                        std::cout << '\n';
                        for (std::size_t i = 0; i < new_line_indent; i++)
                        {
                            std::cout << ' ';
                        }
                    }
                    else
                    {
                        std::cout << ' ';
                        current_line_len++;
                    }
                }
                else
                {
                    std::cout << *it;
                    current_line_len++;
                }
            }
        }
    
        std::cout << '\n';
    }

private:
    /** Argument description content. */
    string_type txt_;
};


/** Class that represents a text a the help menu. */
using help_text = basic_help_text<std::allocator<int>>;


}


#endif

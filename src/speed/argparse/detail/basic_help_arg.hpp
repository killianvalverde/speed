/* speed - Generic C++ library.
 * Copyright (C) 2015-2025 Killian Valverde.
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
 * @file        basic_help_arg.hpp
 * @brief       basic_help_arg class header.
 * @author      Killian Valverde
 * @date        2015/12/30
 */

#ifndef SPEED_ARGPARSE_DETAIL_BASIC_HELP_ARG_HPP
#define SPEED_ARGPARSE_DETAIL_BASIC_HELP_ARG_HPP

#include <regex>
#include <string>
#include <vector>

#include "forward_declarations.hpp"
#include "../basic_arg_parser.hpp"
#include "../exception.hpp"
#include "arg_flags.hpp"
#include "basic_arg_key.hpp"
#include "basic_base_arg.hpp"
#include "basic_key_arg.hpp"
#include "basic_value_arg.hpp"

namespace speed::argparse::detail {

/**
 * @brief       Class that represents arguments that print help information.
 */
template<typename AllocatorT>
class basic_help_arg : public basic_key_arg<AllocatorT>
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<T>;

    /** String type used in the class. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;

    /** Regex type used in the class. */
    using regex_type = std::basic_regex<char, std::regex_traits<char>>;

    /** Pair type used in the class. */
    template<typename T1, typename T2>
    using pair_type = std::pair<T1, T2>;

    /** Vector type used in the class. */
    template<typename T>
    using vector_type = std::vector<T, allocator_type<T>>;

    /** Type that represents a key for an argument. */
    using arg_key_type = basic_arg_key<AllocatorT>;

    /** Type that represents a value for an argument. */
    using arg_value_type = basic_arg_value<AllocatorT>;
    
    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<AllocatorT>;
    
    /** Type that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<AllocatorT>;
    
    /** Type that represents arguments that have values. */
    using value_arg_type = basic_value_arg<AllocatorT>;

    /** Type that represents a help menu. */
    using help_menu_type = basic_help_menu<AllocatorT>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<AllocatorT>;

    /**
     * @brief       Constructor with parameters.
     * @param       arg_parsr : Argument parser that holds this object.
     * @param       kys : Argument keys.
     */
    template<typename... StringTs_>
    explicit basic_help_arg(arg_parser_type* arg_parsr, StringTs_&&... kys)
            : base_arg_type(arg_parsr)
            , key_arg_type(arg_parsr, std::forward<StringTs_>(kys)...)
    {
        base_arg_type::set_flags(arg_flags::DEFAULT_HELP_ARG_FLAGS);
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_help_arg(const basic_help_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_help_arg(basic_help_arg&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_help_arg() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_help_arg& operator =(const basic_help_arg& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_help_arg& operator =(basic_help_arg&& rhs) noexcept = default;

    /**
     * @brief       Sets the help menu triggered by the argument.
     * @param       hlp_menu_triggerd : The help menu triggered.
     */
    template<typename StringT_>
    void set_help_menu_triggered(StringT_&& hlp_menu_triggerd)
    {
        hlp_menu_triggerd_ = &(base_arg_type::get_arg_parser()
                ->get_help_menu(std::forward<StringT_>(hlp_menu_triggerd)));
    }
    
    /**
     * @brief       Print the help menu assigned to this help arguement.
     */
    void print_help_menu_assigned()
    {
        if (hlp_menu_triggerd_ == nullptr)
        {
            set_help_menu_triggered(string_type());
        }
        
        hlp_menu_triggerd_->print();
    }

private:
    /** Help menus that can be triggered under conditions. */
    help_menu_type* hlp_menu_triggerd_ = nullptr;
};

}

#endif

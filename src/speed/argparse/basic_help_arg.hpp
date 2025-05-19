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
 * @file        basic_help_arg.hpp
 * @brief       basic_help_arg class header.
 * @author      Killian Valverde
 * @date        2015/12/30
 */

#ifndef SPEED_ARGPARSE_BASIC_HELP_ARG_HPP
#define SPEED_ARGPARSE_BASIC_HELP_ARG_HPP

#include <regex>
#include <string>
#include <vector>

#include "arg_flags.hpp"
#include "basic_arg_key.hpp"
#include "basic_arg_parser.hpp"
#include "basic_base_arg.hpp"
#include "basic_key_arg.hpp"
#include "basic_key_value_arg.hpp"
#include "basic_value_arg.hpp"
#include "exception.hpp"
#include "forward_declarations.hpp"

namespace speed::argparse {

/**
 * @brief       Class that represents arguments that print help information.
 */
template<typename TpAllocator>
class basic_help_arg : public basic_key_value_arg<TpAllocator>
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<TpAllocator>::template rebind_alloc<T>;

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
    using arg_key_type = basic_arg_key<TpAllocator>;

    /** Type that represents a value for an argument. */
    using arg_value_type = basic_arg_value<TpAllocator>;
    
    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;
    
    /** Type that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<TpAllocator>;
    
    /** Type that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpAllocator>;
    
    /** Type that represents arguments that have values. */
    using key_value_arg_type = basic_key_value_arg<TpAllocator>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;

    /**
     * @brief       Constructor with parameters.
     * @param       arg_parsr : Argument parser that holds this object.
     * @param       kys : Argument keys.
     */
    template<typename... Ts_>
    explicit basic_help_arg(arg_parser_type* arg_parsr, Ts_&&... kys)
            : base_arg_type(arg_parsr)
            , key_value_arg_type(arg_parsr, std::forward<Ts_>(kys)...)
    {
        base_arg_type::set_flags(arg_flags::DEFAULT_HELP_ARG_FLAGS);
        value_arg_type::set_minmax_values(0, 0);
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
     * @brief       Get the help menu id currently assigned.
     * @return      The help menu id currently assigned.
    */
    [[nodiscard]] string_type get_help_menu_id_assigned()
    {
        if (hlp_menus_triggers_.empty())
        {
            return string_type();
        }

        string_type vals = value_arg_type::get_values_as_string();

        for (auto& pr : hlp_menus_triggers_)
        {
            if (std::regex_match(vals, pr.first))
            {
                return pr.second;
            }
        }

        throw help_menu_not_found_exception();
    }

    /**
     * @brief       Set the help menus triggered by the help argument.
     * @param       triggs : Variadic number of two strings that indicate the regex resulting from
     *              concatenating the values and the help menu ID to print in that case.
     */
    template<typename... Ts_>
    void set_help_menus_triggered(Ts_&&... triggs)
    {
        hlp_menus_triggers_.clear();
        set_help_menus_triggered_helper(std::forward<Ts_>(triggs)...);
    }

private:
    /**
     * @brief       Help to set the help menus triggered by the help argument.
     * @param       regx_triggr : Regex resulting from concatenating the values of the argument.
     * @param       hlp_menu_id : The help menu to print.
     * @param       rest : Remaining arguments.
     */
    template<typename TpString1_, typename TpString2_, typename... Ts_>
    void set_help_menus_triggered_helper(
            TpString1_&& regx_triggr,
            TpString2_&& hlp_menu_id,
            Ts_&&... rest
    )
    {
        hlp_menus_triggers_.push_back({regex_type(std::forward<TpString1_>(regx_triggr)),
                                       std::forward<TpString2_>(hlp_menu_id)});

        set_help_menus_triggered_helper(std::forward<Ts_>(rest)...);
    }

    /**
     * @brief       Help to set the help menus triggered by the help argument.
     */
    void set_help_menus_triggered_helper()
    {
    }

private:
    /** Help menus that can be triggered under conditions. */
    vector_type<pair_type<regex_type, string_type>> hlp_menus_triggers_;
};

}

#endif

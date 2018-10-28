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
 * @file        speed/argparse/basic_keyless_arg.hpp
 * @brief       basic_keyless_arg class header.
 * @author      Killian Green
 * @date        2016/03/11 - 14:20
 */

#ifndef SPEED_ARGPARSE_BASIC_KEYLESS_ARG_HPP
#define SPEED_ARGPARSE_BASIC_KEYLESS_ARG_HPP

#include "basic_value_arg.hpp"


namespace speed::argparse {


/**
 * @brief       Class that represents arguments without keys.
 */
template<typename TpAllocator>
class basic_keyless_arg : public basic_value_arg<TpAllocator>
{
public:
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;
    
    /** Class that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;
    
    /** Class that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpAllocator>;
    
    /** Class that represents a value for an argument. */
    using arg_value_type = basic_arg_value<TpAllocator>;
    
    /** Class that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;
    
    /** Class that represents flags container */
    template<typename T>
    using flags_type = speed::containers::flags<T>;
    
    /** Class that represents sequences of characters. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;
    
    /** Class that represents a sequence container. */
    template<typename T>
    using vector_type = std::vector<T, allocator_type<T>>;
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       desc : Argument description content.
     * @param       err_id : The id used to reference an arguments during the error display.
     * @param       flgs : Flags that dictates the argument behavior.
     * @param       min_vals : Minimum number of values for an option.
     * @param       max_vals : Maximum number of values for an option.
     * @param       vals_types : Collection that has the values types that must have the values.
     * @param       regx_vector : Regular expressions that the values have to match. If the string
     *              is empty all values will match.
     * @param       usage_ky : The key used to reference an arguments during the help usage
     *              sentence display.
     * @param       help_ky : The key used to reference an arguments during the help display.
     * @param       compo : The composite object of this class.
     */
    template<
            typename TpString1_,
            typename TpString2_,
            typename TpString3_,
            typename TpString4_,
            typename TpArgValueTypesVector_ = vector_type<arg_value_types>,
            typename TpStringVector_ = vector_type<string_type>
    >
    basic_keyless_arg(
            TpString1_&& desc,
            TpString2_&& err_id,
            arg_flags flgs,
            std::size_t min_vals,
            std::size_t max_vals,
            TpArgValueTypesVector_&& vals_types,
            TpStringVector_&& regx_vector,
            TpString3_&& usage_ky,
            TpString4_&& help_ky,
            arg_parser_type* compo
    )
            : base_arg_type(std::forward<TpString1_>(desc),
                            std::forward<TpString2_>(err_id),
                            flgs,
                            compo)
            , value_arg_type(desc,
                             err_id,
                             flgs,
                             min_vals,
                             max_vals,
                             std::forward<TpArgValueTypesVector_>(vals_types),
                             std::forward<TpStringVector_>(regx_vector),
                             compo)
            , usage_ky_(std::forward<TpString3_>(usage_ky))
            , help_ky_(std::forward<TpString4_>(help_ky))
    {
        if (base_arg_type::error_id_is_empty() &&
            base_arg_type::flag_is_set(arg_flags::USE_FIRST_KEY_IF_ERROR_ID_EMPTY))
        {
            base_arg_type::set_error_id(help_ky_);
        }
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_keyless_arg(const basic_keyless_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_keyless_arg(basic_keyless_arg&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_keyless_arg() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_keyless_arg& operator =(const basic_keyless_arg& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_keyless_arg& operator =(basic_keyless_arg&& rhs) noexcept = default;
    
    /**
     * @brief       Get the usage key of the argument.
     * @return      The usage key of the argument.
     */
    inline const string_type& get_usage_key() const noexcept
    {
        return usage_ky_;
    }
    
    /**
     * @brief       Get the help key of the argument.
     * @return      The help key of the argument.
     */
    inline const string_type& get_help_key() const noexcept
    {
        return help_ky_;
    }
    
    /**
     * @brief       Allows knowing whether an keyless argument has a specified key.
     * @param       ky : The key to check.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    bool check_key(const string_type& ky) const noexcept override
    {
        return usage_ky_ == ky || help_ky_ == ky;
    }
    
    /**
     * @brief       Get the necessary length to print the help keys in standard output.
     * @return      The necessary length to print the help keys in standard output.
     */
    inline std::size_t get_short_keys_length() const noexcept override
    {
        if (base_arg_type::description_is_empty())
        {
            return 0;
        }
        
        return lowlevel::addm(help_ky_.length(), 2);
    }
    
    /**
     * @brief       Only used for polymorphic propose.
     * @return      0 is returned always.
     */
    inline std::size_t get_long_keys_length() const noexcept override
    {
        return 0;
    }
    
    /**
     * @brief       Print the argument usage key in standard output for usage sentence.
     */
    inline void print_usage_key() const
    {
        if (base_arg_type::flag_is_set(arg_flags::ALLWAYS_REQUIRED))
        {
            std::cout << " " << usage_ky_;
        }
        else
        {
            std::cout << " [" << usage_ky_ << "]";
        }
        
        if (value_arg_type::get_max_values() > 1)
        {
            std::cout << "...";
        }
    }
    
    /**
     * @brief       Print the argument information for help menu.
     * @param       args_indent : Indentation used to print arguments help description.
     * @param       max_line_len : The maximum description length that will be printed in a single
     *              line.
     * @param       new_line_indent : The indentation used when a new line is found in a
     *              description.
     * @param       short_id_len : The maximum length of the short keys.
     * @param       long_id_len : The maximum length of the long keys.
     */
    void print_help_text(
            std::size_t args_indent,
            std::size_t max_line_len,
            std::size_t new_line_indent,
            std::size_t short_id_len,
            std::size_t long_id_len
    ) const override
    {
        if (base_arg_type::description_is_empty())
        {
            return;
        }
        
        std::size_t current_id_len = speed::lowlevel::addm(help_ky_.length(), 2);
        std::size_t total_id_len = speed::lowlevel::addm(short_id_len, long_id_len);
        std::size_t i;
    
        for (i = args_indent; i > 0; i--)
        {
            std::cout << ' ';
        }
    
        std::cout << help_ky_ << "  ";
        
        if (current_id_len < total_id_len)
        {
            for (i = total_id_len - current_id_len; i > 0; i--)
            {
                std::cout << ' ';
            }
        }
    
        speed::lowlevel::try_addm(&args_indent, total_id_len);
        speed::lowlevel::try_addm(&new_line_indent, args_indent);
        
        base_arg_type::print_help_text(max_line_len, new_line_indent, args_indent);
    }
    
    /**
     * @brief       Print argument errors in standard output.
     * @param       prog_name : The name of the program.
     * @param       colrs_enable : If it is true, colors will be used during the print.
     */
    void print_errors(const string_type& prog_name, bool colrs_enable) const override
    {
        value_arg_type::print_errors(prog_name, colrs_enable);
    }

private:
    /** The key used to reference an arguments during the help usage sentence display. */
    string_type usage_ky_;
    
    /** The key used to reference an arguments during the help display. */
    string_type help_ky_;
    
    friend class basic_arg_parser<TpAllocator>;
};


/** Class that represents arguments without keys. */
using keyless_arg = basic_keyless_arg<std::allocator<int>>;


}


#endif

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
 * @file        speed/argparse/basic_arg_parser_setter.hpp
 * @brief       basic_arg_parser_setter class header.
 * @author      Killian Valverde
 * @date        2024/05/09
 */

#ifndef SPEED_ARGPARSE_BASIC_ARG_PARSER_SETTER_HPP
#define SPEED_ARGPARSE_BASIC_ARG_PARSER_SETTER_HPP

#include <utility>

#include "arg_parser_flags.hpp"
#include "basic_arg_parser.hpp"


namespace speed::argparse {


/**
 * @brief        Class allowing configuring an argument parser.
 */
template<typename TpAllocator>
class basic_arg_parser_setter
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<TpAllocator>::template rebind_alloc<T>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;

    /** Type that represents the actual type of the object. */
    using self_type = basic_arg_parser_setter;

    /**
     * @brief       Default constructor.
     */
    basic_arg_parser_setter() = default;

    /**
     * @brief       Constructor with parameters.
     * @param       arg_parsr : Argument parser that this object will configure.
     */
    explicit basic_arg_parser_setter(arg_parser_type* arg_parsr)
            : arg_parsr_(arg_parsr)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_arg_parser_setter(const basic_arg_parser_setter& rhs) = delete;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_arg_parser_setter(basic_arg_parser_setter&& rhs) noexcept = delete;
    
    /**
     * @brief       Destructor.
     */
    ~basic_arg_parser_setter() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_arg_parser_setter& operator =(const basic_arg_parser_setter& rhs) = delete;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_arg_parser_setter& operator =(basic_arg_parser_setter&& rhs) noexcept = delete;

    /**
     * @brief       Allows to specify whether the argument parser is allowed to use colors while
     *              printing. It is enabled by default.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& colors(bool enabl)
    {
        if (enabl)
        {
            arg_parsr_->set_flag(arg_parser_flags::USE_COLORS);
        }
        else
        {
            arg_parsr_->unset_flag(arg_parser_flags::USE_COLORS);
        }

        return *this;
    }

    /**
     * @brief       Allows to specify the error ID that the argument parser will be using while
     *              printing some error messages. By default it is "error".
     * @param       err_id : The new error id.
     * @return      The object who call the method.
     */
    template<typename TpString_>
    self_type& error_id(TpString_&& err_id) noexcept
    {
        arg_parsr_->set_error_id(std::forward<TpString_>(err_id));
        return *this;
    }

    /**
     * @brief       Allows to specify the new long prefixes. By default "--" is the only long
     *              prefix. Specifying a new set of long prefixes deletes the last long prefixes.
     * @param       prefxs : Set of new long prefixes to use.
     * @return      The object who call the method.
     */
    template<typename... Ts_>
    self_type& long_prefixes(Ts_&&... prefxs)
    {
        arg_parsr_->set_long_prefixes(std::forward<Ts_>(prefxs)...);
        return *this;
    }

    /**
     * @brief       Allow to specify the maximum amount of unrecognized arguements that the
     *              argument parser will track in order to print during the errors printing. The
     *              default value is 16 and it is recomented to keep this value low.
     * @param       max : The maximum amount of unrecognized
     * @return      The object who call the method.
     */
    self_type& maximum_unrecognized_args(std::size_t max) noexcept
    {
        arg_parsr_->set_maximum_unrecognized_args(max);
        return *this;
    }

    /**
     * @brief       Allows to specify whether after printing errors the argument parser has to
     *              kill the current process. It is enabled by default.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& pkill_after_printing_errors(bool enabl)
    {
        if (enabl)
        {
            arg_parsr_->set_flag(arg_parser_flags::PKILL_AFTER_PRINTING_ERRORS);
        }
        else
        {
            arg_parsr_->unset_flag(arg_parser_flags::PKILL_AFTER_PRINTING_ERRORS);
        }

        return *this;
    }

    /**
     * @brief       Allows to specify whether the errors will be automatically printed. It is
     *              enabled by default.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& print_errors(bool enabl)
    {
        if (enabl)
        {
            arg_parsr_->set_flag(arg_parser_flags::PRINT_ERRORS);
        }
        else
        {
            arg_parsr_->unset_flag(arg_parser_flags::PRINT_ERRORS);
        }

        return *this;
    }

    /**
     * @brief       Allows to specify whether the default help menu will be printed after printing
     *              errors. It is disabled by default.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& print_help_after_printing_errors(bool enabl)
    {
        if (enabl)
        {
            arg_parsr_->set_flag(arg_parser_flags::PRINT_HELP_AFTER_PRINTING_ERRORS);
        }
        else
        {
            arg_parsr_->unset_flag(arg_parser_flags::PRINT_HELP_AFTER_PRINTING_ERRORS);
        }

        return *this;
    }

    /**
     * @brief       Allows to specify the program name. If nothing is specify the argument parser
     *              will try to get the program name from argv. The program name is used for a
     *              large diversity of information printing.
     * @param       prog_name : The program name.
     * @return      The object who call the method.
     */
    template<typename TpString_>
    self_type& program_name(TpString_&& prog_name)
    {
        arg_parsr_->set_program_name(std::forward<TpString_>(prog_name));
        return *this;
    }

    /**
     * @brief       Allows to specify the new short prefixes. By default "-" is the only short
     *              prefix. Specifying a new set of short prefixes deletes the last short prefixes.
     * @param       prefxs : Set of new short prefixes to use.
     * @return      The object who call the method.
     */
    template<typename... Ts_>
    self_type& short_prefixes(Ts_&&... prefxs)
    {
        arg_parsr_->set_short_prefixes(std::forward<Ts_>(prefxs)...);
        return *this;
    }

private:
    /** Reference to the argument parser that produced this object. */
    arg_parser_type* arg_parsr_;
};


}


#endif

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
 * @file       basic_version_arg.hpp
 * @brief      basic_version_arg class header.
 * @author     Killian Valverde
 * @date       2015/12/30
 */

#ifndef SPEED_ARGPARSE_DETAIL_BASIC_VERSION_ARG_HPP
#define SPEED_ARGPARSE_DETAIL_BASIC_VERSION_ARG_HPP

#include <memory>
#include <string>

#include "../basic_arg_parser.hpp"
#include "arg_flags.hpp"
#include "basic_arg_key.hpp"
#include "basic_base_arg.hpp"
#include "basic_key_arg.hpp"

namespace speed::argparse::detail {

/**
 * @brief       Class that represents arguments to get the version information.
 */
template<typename AllocatorT>
class basic_version_arg : public basic_key_arg<AllocatorT>
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<T>;

    /** String type used in the class. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;

    /** Type that represents a key for an argument. */
    using arg_key_type = basic_arg_key<AllocatorT>;
    
    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<AllocatorT>;
    
    /** Type that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<AllocatorT>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<AllocatorT>;

    /**
     * @brief       Constructor with parameters.
     * @param       arg_parsr : Argument parser that holds this object.
     * @param       kys : Argument keys.
     */
    template<typename... StringTs_>
    explicit basic_version_arg(arg_parser_type* arg_parsr, StringTs_&&... kys)
            : base_arg_type(arg_parsr)
            , key_arg_type(arg_parsr, std::forward<StringTs_>(kys)...)
    {
        base_arg_type::set_flags(arg_flags::DEFAULT_VERSION_ARG_FLAGS);
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_version_arg(const basic_version_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_version_arg(basic_version_arg&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_version_arg() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_version_arg& operator =(const basic_version_arg& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_version_arg& operator =(basic_version_arg&& rhs) noexcept = default;

    /**
     * @brief       Set the version information.
     * @param       vers_information : The version information to set.
     */
    template<typename StringT_>
    inline void set_version_information(StringT_&& vers_information)
    {
        vers_information_ = std::forward<StringT_>(vers_information);
    }
    
    /**
     * @brief       Print version information.
     */
    inline void print_version_information()
    {
        base_arg_type::get_arg_parser()->get_ostream() << vers_information_ << std::endl;
    }

private:
    /** Version information content. */
    string_type vers_information_ = "v1.0.0";
};

}

#endif

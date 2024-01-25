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
 * @file        speed/argparse/basic_help_arg.hpp
 * @brief       basic_help_arg class header.
 * @author      Killian Valverde
 * @date        2015/12/30 - 20:40
 */

#ifndef SPEED_ARGPARSE_BASIC_HELP_ARG_HPP
#define SPEED_ARGPARSE_BASIC_HELP_ARG_HPP

#include "basic_key_arg.hpp"


namespace speed::argparse {


/**
 * @brief       Class that represents the option to get the help information.
 */
template<typename TpAllocator>
class basic_help_arg : public basic_key_arg<TpAllocator>
{
public:
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;
    
    /** Class that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;
    
    /** Class that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<TpAllocator>;
    
    /** Class that represents a key for an argument. */
    using arg_key_type = basic_arg_key<TpAllocator>;
    
    /** Class that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;
    
    /** Class that represents flags container */
    template<typename T>
    using flags_type = speed::containers::flags<T>;
    
    /** Class that represent sequences of characters. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;
    
    /** Class that represents a sequence container. */
    template<typename T>
    using vector_type = std::vector<T, allocator_type<T>>;
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       desc : Argument description content.
     * @param       err_id : The id used to reference an arguments during the error display.
     * @param       flgs : Flags that dictates the argument behavior.
     * @param       kys : Argument keys collection.
     * @param       compo : The composite object of this class.
     */
    template<
            typename TpString1_,
            typename TpString2_,
            typename TpArgKeyVector_ = vector_type<string_type
            >
    >
    basic_help_arg(
            TpString1_&& desc,
            TpString2_&& err_id,
            arg_flags flgs,
            TpArgKeyVector_&& kys,
            arg_parser_type* compo
    )
            : base_arg_type(std::forward<TpString1_>(desc),
                            std::forward<TpString2_>(err_id),
                            flgs,
                            compo)
            , key_arg_type(desc,
                           err_id,
                           flgs,
                           std::forward<TpArgKeyVector_>(kys),
                           compo)
    {
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
    
    friend class basic_arg_parser<TpAllocator>;
};


/** Class that represents the option to get the help information. */
using help_arg = basic_help_arg<std::allocator<int>>;
    
    
}


#endif

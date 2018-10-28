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
 * @file       speed/argparse/basic_version_arg.hpp
 * @brief      basic_version_arg class header.
 * @author     Killian
 * @date       2015/12/30 - 20:50
 */

#ifndef SPEED_ARGPARSE_BASIC_VERSION_ARG_HPP
#define SPEED_ARGPARSE_BASIC_VERSION_ARG_HPP

#include "basic_key_arg.hpp"


namespace speed::argparse {


/**
 * @brief       Class that represents the option to get the version information.
 */
template<typename TpAllocator>
class basic_version_arg : public basic_key_arg<TpAllocator>
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
     * @param       vers_information : Argument version information content.
     * @param       compo : The composite object of this class.
     */
    template<
            typename TpString1_,
            typename TpString2_,
            typename TpString3_,
            typename TpArgKeyVector_ = vector_type<string_type>
    >
    basic_version_arg(
            TpString1_&& desc,
            TpString2_&& err_id,
            arg_flags flgs,
            TpArgKeyVector_&& kys,
            TpString3_&& vers_information,
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
            , vers_information_(std::forward<TpString3_>(vers_information))
    {
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
     * @brief       Print version information in standard output.
     */
    inline void print_version_information() const
    {
        std::cout << vers_information_;
    }

private:
    /** Argument version information content. */
    string_type vers_information_;
    
    friend class basic_arg_parser<TpAllocator>;
};


/** Class that represents the option to get the version information. */
using version_arg = basic_version_arg<std::allocator<int>>;
    
    
}


#endif

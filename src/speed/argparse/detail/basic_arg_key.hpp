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
 * @file        basic_arg_key.hpp
 * @brief       basic_arg_key class header.
 * @author      Killian Valverde
 * @date        2017/01/05
 */

#ifndef SPEED_ARGPARSE_DETAIL_BASIC_ARG_KEY_HPP
#define SPEED_ARGPARSE_DETAIL_BASIC_ARG_KEY_HPP

#include <string>

#include "forward_declarations.hpp"
#include "../basic_arg_parser.hpp"

namespace speed::argparse::detail {

/**
 * @brief       Class that represents a key for an argument.
 */
template<typename AllocatorT>
class basic_arg_key
{
public:
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<T>;
    
    /** Class that represent sequences of characters. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<AllocatorT>;
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       ky : Key string to represent.
     * @param       arg_parsr : Argument parser that holds this object.
     */
    template<typename StringT_>
    basic_arg_key(StringT_&& ky, arg_parser_type& arg_parsr)
            : ky_(std::forward<StringT_>(ky))
            , arg_parsr_(arg_parsr)
            , is_prefx_long_(arg_parsr.is_key_prefix_long(ky_))
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_arg_key(const basic_arg_key& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_arg_key(basic_arg_key&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    virtual ~basic_arg_key() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_arg_key& operator =(const basic_arg_key& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_arg_key& operator =(basic_arg_key&& rhs) noexcept = default;
    
    /**
     * @brief       Allows knowing whether two objects are equal.
     * @param       rhs : Object to compare.
     * @return      If the objets are equal true is returned, otherwise false is returned.
     */
    bool operator ==(const basic_arg_key& rhs) const noexcept
    {
        return (ky_ == rhs.ky_);
    }
    
    /**
     * @brief       Allows knowing whether two objects are equal.
     * @param       ky : String to compare.
     * @return      If the objets are equal true is returned, otherwise false is returned.
     */
    bool operator ==(const string_type& ky) const noexcept
    {
        return (ky_ == ky);
    }
    
    /**
     * @brief       Allows knowing whether two objects are different.
     * @param       rhs : Object to compare.
     * @return      If the objets are different true is returned, otherwise false is returned.
     */
    bool operator !=(const basic_arg_key& rhs) const noexcept
    {
        return (ky_ != rhs.ky_);
    }
    
    /**
     * @brief       Allows knowing whether two objects are different.
     * @param       ky : String to compare.
     * @return      If the objets are different true is returned, otherwise false is returned.
     */
    bool operator !=(const string_type& ky) const noexcept
    {
        return (ky_ != ky);
    }
    
    /**
     * @brief       Get the key in a character string.
     * @return      The key in a character stirng.
     */
    const string_type& get_string() const noexcept
    {
        return ky_;
    }
    
    /**
     * @brief       Get the key lenght in number of characters.
     * @return      The key lenght in number of characters.
     */
    [[nodiscard]] std::size_t get_string_length() const noexcept
    {
        return ky_.length();
    }

    /**
     * @brief       Update the prefix type of the argument key.
     */
    void update_prefix_type() noexcept
    {
        is_prefx_long_ = arg_parsr_.is_key_prefix_long(ky_);
    }
    
    /**
     * @brief       Get the key lenght in number of characters.
     * @return      The key lenght in number of characters.
     */
    [[nodiscard]] std::size_t is_empty() const noexcept
    {
        return ky_.empty();
    }
    
    /**
     * @brief       Allows knowing whether the key prefix is a long prefix.
     * @return      A value that allows knowing whether the key prefix is a long prefix.
     */
    [[nodiscard]] bool is_prefix_long() const noexcept
    {
        return is_prefx_long_;
    }

private:
    /** The character string that represents the key. */
    string_type ky_;

    /** Reference to the argument parser that holds this object. */
    arg_parser_type& arg_parsr_;
    
    /** The value that allows knowing whether the key prefix is a long prefix. */
    bool is_prefx_long_;
};

/**
 * @brief       Allows to use an objects to print in standard output the value that constains the
 *              key.
 * @param       os : The object who call the function.
 * @param       arg_ky : The argument key to print in standard output.
 * @return      The object who call the function.
 */
template<typename CharT, typename CharTraitsT, typename AllocatorT>
std::basic_ostream<CharT, CharTraitsT>& operator <<(
        std::basic_ostream<CharT, CharTraitsT>& os,
        const basic_arg_key<AllocatorT>& arg_ky
)
{
    os << arg_ky.get_string();
    
    return os;
}

}

#endif

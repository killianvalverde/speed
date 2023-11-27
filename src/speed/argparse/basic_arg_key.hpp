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
 * @file        speed/argparse/basic_arg_key.hpp
 * @brief       basic_arg_key class header.
 * @author      Killian Green
 * @date        2017/01/05 - 20:57
 */

#ifndef SPEED_ARGPARSE_BASIC_ARG_KEY_HPP
#define SPEED_ARGPARSE_BASIC_ARG_KEY_HPP

#include <string>


namespace speed::argparse {


/**
 * @brief       Class that represents a key for an argument.
 */
template<typename TpAllocator>
class basic_arg_key
{
public:
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;
    
    /** Class that represent sequences of characters. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       ky : Key string to represent.
     * @param       is_prefx_long : Allows knowing whether the key prefix is a long prefix.
     */
    template<typename TpString_>
    basic_arg_key(TpString_&& ky, bool is_prefx_long)
            : ky_(std::forward<TpString_>(ky))
            , is_prefx_long_(is_prefx_long)
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
    basic_arg_key(basic_arg_key&& rhs) noexcept
            : ky_(std::move(rhs.ky_))
            , is_prefx_long_(rhs.is_prefx_long_)
    {
        rhs.is_prefx_long_ = false;
    }
    
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
    basic_arg_key& operator =(basic_arg_key&& rhs) noexcept
    {
        if (this != &rhs)
        {
            ky_ = std::move(rhs.ky_);
            std::swap(is_prefx_long_, rhs.is_prefx_long_);
        }
        
        return *this;
    }
    
    /**
     * @brief       Allows knowing whether two objects are equal.
     * @param       rhs : Object to compare.
     * @return      If the objets are equal true is returned, otherwise false is returned.
     */
    inline bool operator ==(const basic_arg_key& rhs) const noexcept
    {
        return (ky_ == rhs.ky_);
    }
    
    /**
     * @brief       Allows knowing whether two objects are equal.
     * @param       ky : String to compare.
     * @return      If the objets are equal true is returned, otherwise false is returned.
     */
    inline bool operator ==(const string_type& ky) const noexcept
    {
        return (ky_ == ky);
    }
    
    /**
     * @brief       Allows knowing whether two objects are different.
     * @param       rhs : Object to compare.
     * @return      If the objets are different true is returned, otherwise false is returned.
     */
    inline bool operator !=(const basic_arg_key& rhs) const noexcept
    {
        return (ky_ != rhs.ky_);
    }
    
    /**
     * @brief       Allows knowing whether two objects are different.
     * @param       ky : String to compare.
     * @return      If the objets are different true is returned, otherwise false is returned.
     */
    inline bool operator !=(const string_type& ky) const noexcept
    {
        return (ky_ != ky);
    }
    
    /**
     * @brief       Allows implicit conversion to string type.
     * @return      A const reference to the string equivalent of the object.
     */
    inline operator const string_type&() const noexcept
    {
        return ky_;
    }
    
    /**
     * @brief       Get the key in a character string.
     * @return      The key in a character stirng.
     */
    inline const string_type& get_string() const noexcept
    {
        return ky_;
    }
    
    /**
     * @brief       Get the key lenght in number of characters.
     * @return      The key lenght in number of characters.
     */
    inline std::size_t get_string_length() const noexcept
    {
        return ky_.length();
    }
    
    /**
     * @brief       Allows knowing whether the key prefix is a long prefix.
     * @return      A value that allows knowing whether the key prefix is a long prefix.
     */
    inline bool is_prefix_long() const noexcept
    {
        return is_prefx_long_;
    }

private:
    /** The character string that represents the key. */
    string_type ky_;
    
    /** The value that allows knowing whether the key prefix is a long prefix. */
    bool is_prefx_long_;
};


/**
 * @brief       Allows to use an objects to print in standard output the value that constains the
 *              key.
 * @param       os : The object who call the function.
 * @param       arg_ky : The argument key to print in standard output.
 * @return      The object who call the function.
 * @throw       std::basic_ostream::failure : It throws an exception if the resulting error state
 *              flag of the 'std::basic_ostream' object is not goodbit and member exceptions was set
 *              to throw for that state.
 */
template<typename TpChar, typename TpCharTraits, typename TpAllocator>
std::basic_ostream<TpChar, TpCharTraits>& operator <<(
        std::basic_ostream<TpChar, TpCharTraits>& os,
        const basic_arg_key<TpAllocator>& arg_ky
)
{
    os << arg_ky.get_string();
    
    return os;
}


/** Class that represents a key for an argument. */
using arg_key = basic_arg_key<std::allocator<int>>;


}


#endif

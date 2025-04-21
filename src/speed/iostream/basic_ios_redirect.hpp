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
 * @file        basic_ios_redirect.hpp
 * @brief       basic_ios_redirect class header.
 * @author      Killian Valverde
 * @date        2017/05/19
 */

#ifndef SPEED_IOSTREAM_BASIC_IOS_REDIRECT_HPP
#define SPEED_IOSTREAM_BASIC_IOS_REDIRECT_HPP

#include <sstream>

#include "../memory/memory.hpp"

namespace speed::iostream {

/**
 * @brief       Class used to redirect a input/output stream buffer to another one.
 */
template<
        typename TpChar,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAllocator = std::allocator<TpChar>
>
class basic_ios_redirect
{
public:
    /** Character type. */
    using char_type = TpChar;
    
    /** Character traits type, */
    using char_traits_type = TpCharTraits;
    
    /** Allocator type. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<TpAllocator>::template rebind_alloc<T>;
    
    /** Input/output stream type. */
    using ios_type = std::basic_ios<char_type, char_traits_type>;
    
    /** Stream buffer type. */
    using streambuf_type = std::basic_streambuf<char_type, char_traits_type>;
    
    /** String type. */
    using string_type = std::basic_string<char_type, char_traits_type, allocator_type<char_type>>;
    
    /** String stream type. */
    using stringstream_type = std::basic_stringstream<char_type, char_traits_type,
            allocator_type<char_type>>;

    /**
     * @brief       Constructor.
     * @param       ios : Input/output stream whose buffer will be redirect.
     */
    explicit basic_ios_redirect(ios_type& ios)
            : ios_(&ios)
            , old_streambuf_(nullptr)
            , stringstream_(nullptr)
            , stringstream_alloc_()
            , stringstream_constructed_(false)
    {
    }
    
    basic_ios_redirect(const basic_ios_redirect&) = delete;
    
    basic_ios_redirect(basic_ios_redirect&&) = delete;
    
    /**
     * @brief       Destructor.
     */
    ~basic_ios_redirect()
    {
        unredirect();
        
        if (stringstream_ != nullptr)
        {
            stringstream_alloc_.deallocate(stringstream_, 1);
            stringstream_ = nullptr;
        }
    }
    
    basic_ios_redirect& operator =(const basic_ios_redirect&) = delete;
    
    basic_ios_redirect& operator =(basic_ios_redirect&&) = delete;
    
    /**
     * @brief       Set a new stream buffer to use.
     * @param       new_streambuf : New stream buffer to use.
     */
    void redirect(streambuf_type *new_streambuf)
    {
        unredirect();
        old_streambuf_ = ios_->rdbuf(new_streambuf);
    }
    
    /**
     * @brief       Use the class embedded lazy stringstream to redirect the current input/output
     *              stream buffer.
     */
    void redirect_to_internal_stream()
    {
        unredirect();
        
        if (stringstream_ == nullptr)
        {
            stringstream_ = stringstream_alloc_.allocate(1);
        }

        speed::memory::construct_at(stringstream_);
        stringstream_constructed_ = true;
        
        old_streambuf_ = ios_->rdbuf(stringstream_->rdbuf());
    }
    
    /**
     * @brief       Redirect the current stream buffer to the old one.
     */
    void unredirect()
    {
        if (old_streambuf_ != nullptr)
        {
            ios_->rdbuf(old_streambuf_);
            old_streambuf_ = nullptr;
        }
        
        if (stringstream_constructed_)
        {
            speed::memory::destroy_at(stringstream_);
            stringstream_constructed_ = false;
        }
    }
    
    /**
     * @brief       Get the embedded string stream buffer as a string.
     * @return      The embedded string stream buffer as a string.
     */
    string_type get_internal_string()
    {
        if (stringstream_constructed_)
        {
            return stringstream_->str();
        }
        
        return string_type();
    }
    
    /**
     * @brief       Clear the embedded string stream buffer.
     */
    void clear_internal_stream()
    {
        if (stringstream_constructed_)
        {
            speed::memory::destroy_at(stringstream_);
            speed::memory::construct_at(stringstream_);
        }
    }

private:
    /** Input/output stream to redirect. */
    ios_type* ios_;
    
    /** Old stream buffer. */
    streambuf_type* old_streambuf_;
    
    /** The embedded string stream. */
    stringstream_type* stringstream_;

    /** Stringstream allocator. */
    allocator_type<stringstream_type> stringstream_alloc_;
    
    /** Allows to knowing whether the embedded string stream has been constructed. */
    bool stringstream_constructed_;
};

/** Class used to redirect a 8 bits input/output stream buffer to another one. */
using ios_redirect = basic_ios_redirect<char>;

/** Class used to redirect a 16 bits input/output stream buffer to another one. */
using wios_redirect = basic_ios_redirect<wchar_t>;

}

#endif

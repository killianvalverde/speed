/* speed - Generic C++ library.
 * Copyright (C) 2015-2026 Killian Valverde.
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
 * @file basic_ios_redirect.hpp
 * @brief Provides utilities for redirecting input and output streams.
 * @author Killian Valverde
 * @date 2017-05-19
 */

#pragma once

#include <ios>
#include <memory>
#include <optional>
#include <sstream>
#include <string>

#include "../memory/memory.hpp"

namespace speed::iostream {

/**
 * @brief Manages the redirection of a C++ stream buffer.
 *
 * Allows a stream to be redirected to an external stream buffer
 * or an internally managed string buffer. The original stream buffer
 * is automatically restored when the object is destroyed.
 *
 * @tparam CharT Character type used by the stream.
 * @tparam TraitsT Character traits type.
 * @tparam AllocatorT Allocator type used for internal string storage.
 */
template<
    typename CharT,
    typename TraitsT = std::char_traits<CharT>,
    typename AllocatorT = std::allocator<CharT>
>
class basic_ios_redirect
{
public:
    /** Character type used by the stream. */
    using char_type = CharT;

    /** Character traits type. */
    using traits_type = TraitsT;

    /** Allocator type used for internal string storage. */
    using allocator_type =
        typename std::allocator_traits<AllocatorT>::template rebind_alloc<char_type>;

    /** Type of the managed stream interface. */
    using ios_type = std::basic_ios<char_type, traits_type>;

    /** Type of the stream buffer. */
    using streambuf_type = std::basic_streambuf<char_type, traits_type>;

    /** Type of the internal string buffer. */
    using stringbuf_type = std::basic_stringbuf<char_type, traits_type, allocator_type>;

    /** Type of the string used for internal storage. */
    using string_type = std::basic_string<char_type, traits_type, allocator_type>;

    /** Type of the string view used for internal storage. */
    using string_view_type = std::basic_string_view<char_type, traits_type>;

    /**
     * @brief Constructs a stream redirection manager.
     *
     * @param ios Stream interface to manage.
     * @param allocator Allocator used for internal string storage.
     */
    explicit basic_ios_redirect(
        ios_type& ios,
        const allocator_type& allocator = allocator_type()
    )
        : ios_(ios), allocator_(allocator)
    {
    }

    /** 
     * @brief Copy construction is disabled. 
     */
    basic_ios_redirect(const basic_ios_redirect&) = delete;

    /** 
     * @brief Move construction is disabled. 
     */
    basic_ios_redirect(basic_ios_redirect&&) = delete;

    /**
     * @brief Destroys the manager and restores the original stream buffer.
     */
    ~basic_ios_redirect() noexcept
    {
        unredirect();
    }

    /** 
     * @brief Copy assignment is disabled. 
     */
    basic_ios_redirect& operator=(const basic_ios_redirect&) = delete;

     /** 
      * @brief Move assignment is disabled. 
      */
    basic_ios_redirect& operator=(basic_ios_redirect&&) = delete;

    /**
     * @brief Redirects the managed stream to another stream buffer.
     *
     * Saves the original stream buffer on the first successful redirection. Subsequent calls 
     * replace the current stream buffer without overwriting the original one.
     *
     * @param new_streambuf Stream buffer to redirect to.
     */
    void redirect(streambuf_type* new_streambuf)
    {
        auto* previous_streambuf = ios_.rdbuf();
        const auto previous_state = ios_.rdstate();

        try
        {
            ios_.rdbuf(new_streambuf);
        }
        catch (...)
        {
            set_buffer_and_state_noexcept(previous_streambuf, previous_state);
            throw;
        }

        if (!redirected_)
        {
            old_streambuf_ = previous_streambuf;
            redirected_ = true;
        }
    }

    /**
     * @brief Redirects the managed stream to an internal string buffer.
     *
     * Creates the internal buffer if necessary, or clears its contents if it already exists, 
     * before redirecting the stream.
     */
    void redirect_to_internal_stream()
    {
        if (!internal_buffer_)
        {
            internal_buffer_.emplace(std::ios_base::in | std::ios_base::out, allocator_);
        }
        else
        {
            internal_buffer_->str(string_type(allocator_));
        }

        redirect(std::addressof(*internal_buffer_));
    }

    /**
     * @brief Restores the original stream buffer.
     *
     * Restores the buffer saved during the first redirection and resets the stream state to 
     * std::ios_base::goodbit. Has no effect if the stream is not redirected.
     */
    void unredirect() noexcept
    {
        if (!redirected_)
        {
            return;
        }

        set_buffer_and_state_noexcept(
            old_streambuf_, std::ios_base::goodbit);

        old_streambuf_ = nullptr;
        redirected_ = false;
    }

    /**
     * @brief Checks whether the stream is currently redirected.
     *
     * @return true if redirection is active, false otherwise.
     */
    [[nodiscard]] bool is_redirected() const noexcept
    {
        return redirected_;
    }

    /**
     * @brief Retrieves a non-owning view of the internal string buffer.
     *
     * @return A view of the captured text, or an empty view if the
     *         internal buffer has not been created.
     *
     * @warning The returned view may be invalidated by subsequent
     *          modifications to the internal buffer or its destruction.
     */
    [[nodiscard]] string_view_type get_internal_string_view() const noexcept
    {
        if (internal_buffer_)
        {
            return internal_buffer_->view();
        }

        return {};
    }

    /**
     * @brief Clears the contents of the internal string buffer.
     *
     * If the managed stream currently uses the internal buffer, its error state is also cleared. 
     * Has no effect if the internal buffer has not been created.
     */
    void clear_internal_stream()
    {
        if (!internal_buffer_)
        {
            return;
        }

        internal_buffer_->str(string_type(allocator_));

        if (ios_.rdbuf() == std::addressof(*internal_buffer_))
        {
            ios_.clear();
        }
    }

private:
    /**
     * @brief Restores a stream buffer and stream state without propagating
     * exceptions from restoring the exception mask.
     *
     * Temporarily disables stream exceptions, replaces the stream buffer, restores the requested 
     * state, and attempts to restore the original exception mask.
     *
     * @param buffer Stream buffer to install.
     * @param state Stream state to restore.
     */
    void set_buffer_and_state_noexcept(
        streambuf_type* buffer,
        std::ios_base::iostate state
    ) noexcept
    {
        const auto exception_mask = ios_.exceptions();

        ios_.exceptions(std::ios_base::goodbit);
        ios_.rdbuf(buffer);
        ios_.clear(state);

        try
        {
            ios_.exceptions(exception_mask);
        }
        catch (...)
        {
        }
    }

    /** Reference to the managed stream interface. */
    ios_type& ios_;

    /** Allocator used for internal string storage. */
    allocator_type allocator_;

    /** Optional internal string buffer used for redirection. */
    std::optional<stringbuf_type> internal_buffer_;

    /** Original stream buffer saved during the first redirection. */
    streambuf_type* old_streambuf_ = nullptr;

    /** Indicates whether the stream is currently redirected. */
    bool redirected_ = false;
};

/** Stream redirection manager for narrow-character streams. */
using ios_redirect = basic_ios_redirect<char>;

/** Stream redirection manager for wide-character streams. */
using wios_redirect = basic_ios_redirect<wchar_t>;

}

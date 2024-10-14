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
 * @file        speed/exceptions/basic_message_exception.hpp
 * @brief       basic_message_exception class header.
 * @author      Killian Valverde
 * @date        2016/08/19
 */

#ifndef SPEED_EXCEPTIONS_BASIC_MESSAGE_EXCEPTION_HPP
#define SPEED_EXCEPTIONS_BASIC_MESSAGE_EXCEPTION_HPP

#include <type_traits>

#include "exception_base.hpp"


namespace speed::exceptions {


/**
 * @brief       Base for any speed exception class that needs to have a customizable message.
 */
template<typename TpAllocator>
class basic_message_exception : public exception_base
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<TpAllocator>::template rebind_alloc<T>;

    /** String type used in the class. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;

    /**
     * @brief       Constructor with parameters.
     * @param       mess : Message to return by the exception object.
     */
    template<
            typename TpString_,
            typename = std::enable_if_t<
                    !std::is_base_of<basic_message_exception, std::decay_t<TpString_>>::value
            >
    >
    explicit inline basic_message_exception(TpString_&& mess) noexcept
            : mess_(std::forward<TpString_>(mess))
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_message_exception(const basic_message_exception& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_message_exception(basic_message_exception&& rhs) = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_message_exception() override = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_message_exception& operator =(const basic_message_exception& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_message_exception& operator =(basic_message_exception&& rhs) = default;
    
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] inline const char* what() const noexcept override
    {
        return mess_.c_str();
    }

protected:
    /** Message of the exception. */
    string_type mess_;
};


/** Class used to parse arguments. */
using message_exception = basic_message_exception<std::allocator<char>>;


}


#endif

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
 * @file        speed/exception/exception_base.hpp
 * @brief       exception_base class header.
 * @author      Killian Valverde
 * @date        2016/08/19 11:39
 */

#ifndef SPEED_EXCEPTION_MESSAGE_EXCEPTION_BASE_HPP
#define SPEED_EXCEPTION_MESSAGE_EXCEPTION_BASE_HPP

#include "exception_base.hpp"


namespace speed::exception {


/**
 * @brief       Base for any speed exception class that needs to have a customizable message.
 */
class message_exception_base : public exception_base
{
public:
    /**
     * @brief       Constructor with parameters.
     * @param       str : Message to return by the exception object.
     */
    explicit message_exception_base(const char* const str = nullptr) noexcept
            : mess_(str)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    message_exception_base(const message_exception_base& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    message_exception_base(message_exception_base&& rhs) = default;
    
    /**
     * @brief       Destructor.
     */
    ~message_exception_base() override = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    message_exception_base& operator =(const message_exception_base& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    message_exception_base& operator =(message_exception_base&& rhs) = default;
    
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    inline const char* what() const noexcept override
    {
        return mess_;
    }

protected:
    /** Message of the exception. */
    const char *mess_;
};


}


#endif

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
 * @file        speed/exceptions/exception_base.hpp
 * @brief       exception_base class header.
 * @author      Killian Valverde
 * @date        2016/08/19
 */

#ifndef SPEED_EXCEPTIONS_EXCEPTION_BASE_HPP
#define SPEED_EXCEPTIONS_EXCEPTION_BASE_HPP

#include <exception>
#include <string>


namespace speed::exceptions {


/**
 * @brief       Base class for standard speed exceptions. All objects thrown by components speed are
 *              derived from this class. Therefore, all speed exceptions can be caught by catching
 *              this type by reference.
 */
class exception_base : public std::exception
{
public:
    /**
     * @brief       Constructor with parameters.
     */
    exception_base() = default;
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    exception_base(const exception_base& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    exception_base(exception_base&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~exception_base() override = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    exception_base& operator =(const exception_base& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    exception_base& operator =(exception_base&& rhs) noexcept = default;
    
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] const char* what() const noexcept override = 0;
};


}


#endif

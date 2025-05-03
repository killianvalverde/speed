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
 * @file        basic_arg_constraint_setter.hpp
 * @brief       basic_arg_constraint_setter class header.
 * @author      Killian Valverde
 * @date        2025/04/21
 */

#ifndef SPEED_ARGPARSE_BASIC_ARG_CONSTRAINT_SETTER_HPP
#define SPEED_ARGPARSE_BASIC_ARG_CONSTRAINT_SETTER_HPP

#include <string>
#include <vector>

#include "basic_arg_constraint.hpp"

namespace speed::argparse {

/**
 * @brief        Class allowing configuring an argument constraint setter.
 */
template<typename TpAllocator>
class basic_arg_constraint_setter
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<TpAllocator>::template rebind_alloc<T>;

    /** Type that represents an argument constraint. */
    using arg_constraint_type = basic_arg_constraint<TpAllocator>;

    /** Type that represents the actual type of the object. */
    using self_type = basic_arg_constraint_setter;

    /**
     * @brief       Constructor with parameters.
     * @param       arg_constrnt : Argument constraint that this object will configure.
     */
    explicit basic_arg_constraint_setter(arg_constraint_type* arg_constrnt)
            : arg_constrnt_(arg_constrnt)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_arg_constraint_setter(const basic_arg_constraint_setter& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_arg_constraint_setter(basic_arg_constraint_setter&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_arg_constraint_setter() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_arg_constraint_setter& operator =(const basic_arg_constraint_setter& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_arg_constraint_setter& operator =(basic_arg_constraint_setter&& rhs) noexcept = default;

    /**
     * @brief       Sets whether the argument is mutually exclusive.
     * @param       enabl : Specify whether the argument constraint should be marked as
     *              mutually exclusive (`true` to enable, `false` to disable).
     * @return      A reference to the current object (`self_type`) to allow method chaining.
     */
    self_type& mutually_exclusive(bool enabl)
    {
        arg_constrnt_->set_mutually_exclusive(enabl);
        return *this;
    }
    
    /**
     * @brief       Sets whether one or more arguments are required.
     * @param       enabl : Specify whether the argument constraint should require
     *              at least one argument (`true` to enable, `false` to disable).
     * @return      A reference to the current object (`self_type`) to allow method chaining.
     */
    self_type& one_or_more_required(bool enabl)
    {
        arg_constrnt_->set_one_or_more_required(enabl);
        return *this;
    }

private:
    /** Reference to the argument constraint that this object configures. */
    arg_constraint_type* arg_constrnt_;
};

}

#endif

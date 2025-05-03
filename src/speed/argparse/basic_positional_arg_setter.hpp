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
 * @file        basic_positional_arg_setter.hpp
 * @brief       basic_positional_arg_setter class header.
 * @author      Killian Valverde
 * @date        2024/05/06
 */

#ifndef SPEED_ARGPARSE_BASIC_POSITIONAL_ARG_SETTER_HPP
#define SPEED_ARGPARSE_BASIC_POSITIONAL_ARG_SETTER_HPP

#include <utility>

#include "../type_traits/type_traits.hpp"
#include "basic_base_arg_setter.hpp"
#include "basic_positional_arg.hpp"
#include "basic_value_arg_setter.hpp"

namespace speed::argparse {

/**
 * @brief        Class allowing configuring a positional argument.
 */
template<typename TpAllocator, typename TpActual = std::false_type>
class basic_positional_arg_setter : public speed::type_traits::basic_crtp_base<
        basic_value_arg_setter, basic_positional_arg_setter, TpActual, std::false_type, TpAllocator>
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<TpAllocator>::template rebind_alloc<T>;

    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;

    /** Type that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpAllocator>;

    /** Type that represents arguments without keys. */
    using positional_arg_type = basic_positional_arg<TpAllocator>;

    /** Type that represents the class allowing configuring a base argument. */
    using base_arg_setter_type = speed::type_traits::basic_crtp_base<basic_base_arg_setter,
            basic_positional_arg_setter, TpActual, std::false_type, TpAllocator>;

    /** Type that represents the class allowing configuring a value argument. */
    using value_arg_setter_type = speed::type_traits::basic_crtp_base<basic_value_arg_setter,
            basic_positional_arg_setter, TpActual, std::false_type, TpAllocator>;

    /** Type that represents the actual type of the object. */
    using self_type = speed::type_traits::basic_crtp_self<
            basic_positional_arg_setter, TpActual, std::false_type, TpAllocator>;

    /**
     * @brief       Default constructor.
     */
    basic_positional_arg_setter() = default;

    /**
     * @brief       Constructor with parameters.
     * @param       positionl_arg : positional argument that this object will configure.
     */
    explicit basic_positional_arg_setter(positional_arg_type* positionl_arg)
            : base_arg_setter_type(static_cast<base_arg_type*>(positionl_arg))
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_positional_arg_setter(const basic_positional_arg_setter& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_positional_arg_setter(basic_positional_arg_setter&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_positional_arg_setter() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_positional_arg_setter& operator =(const basic_positional_arg_setter& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_positional_arg_setter& operator =(basic_positional_arg_setter&& rhs) noexcept = default;
};

}

#endif

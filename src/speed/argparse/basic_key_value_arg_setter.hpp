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
 * @file        speed/argparse/basic_key_value_arg_setter.hpp
 * @brief       basic_key_value_arg_setter class header.
 * @author      Killian Valverde
 * @date        2024/05/06
 */

#ifndef SPEED_ARGPARSE_BASIC_KEY_VALUE_ARG_SETTER_HPP
#define SPEED_ARGPARSE_BASIC_KEY_VALUE_ARG_SETTER_HPP

#include <utility>

#include "../type_traits/type_traits.hpp"
#include "arg_flags.hpp"
#include "basic_base_arg_setter.hpp"
#include "basic_key_arg_setter.hpp"
#include "basic_key_value_arg.hpp"
#include "basic_value_arg_setter.hpp"


namespace speed::argparse {


/**
 * @brief        Class allowing configuring a key value argument.
 */
template<typename TpAllocator, typename TpActual = std::false_type>
class basic_key_value_arg_setter
        : public speed::type_traits::basic_crtp_base<basic_key_arg_setter,
                basic_key_value_arg_setter, TpActual, std::false_type, TpAllocator>
        , public speed::type_traits::basic_crtp_base<basic_value_arg_setter,
                basic_key_value_arg_setter, TpActual, std::false_type, TpAllocator>
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;

    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;

    /** Type that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<TpAllocator>;

    /** Type that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpAllocator>;

    /** Type that represents arguments that have keys and values. */
    using key_value_arg_type = basic_key_value_arg<TpAllocator>;

    /** Type that represents the class allowing configuring a base argument. */
    using base_arg_setter_type = speed::type_traits::basic_crtp_base<basic_base_arg_setter,
            basic_key_value_arg_setter, TpActual, std::false_type, TpAllocator>;

    /** Type that represents the class allowing configuring a key argument. */
    using key_arg_setter_type = speed::type_traits::basic_crtp_base<basic_key_arg_setter,
            basic_key_value_arg_setter, TpActual, std::false_type, TpAllocator>;

    /** Type that represents the class allowing configuring a value argument. */
    using value_arg_setter_type = speed::type_traits::basic_crtp_base<basic_value_arg_setter,
            basic_key_value_arg_setter, TpActual, std::false_type, TpAllocator>;

    /** Type that represents the actual type of the object. */
    using self_type = speed::type_traits::basic_crtp_self<basic_key_value_arg_setter,
            TpActual, std::false_type, TpAllocator>;

    /**
     * @brief       Default constructor.
     */
    basic_key_value_arg_setter() = default;

    /**
     * @brief       Constructor with parameters.
     * @param       ky_val_arg : Key value argument that this object will configure.
     */
    explicit basic_key_value_arg_setter(key_value_arg_type* ky_val_arg)
            : base_arg_setter_type(static_cast<base_arg_type*>(ky_val_arg))
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_key_value_arg_setter(const basic_key_value_arg_setter& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_key_value_arg_setter(basic_key_value_arg_setter&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_key_value_arg_setter() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_key_value_arg_setter& operator =(const basic_key_value_arg_setter& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_key_value_arg_setter& operator =(basic_key_value_arg_setter&& rhs) noexcept = default;

    /**
     * @brief       Allows to specify whether keys can have their values assigned by suing the
     *              '=' operator. This feature is enable by default for key value arguements.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& assignment_operator(bool enabl)
    {
        if (enabl)
        {
            base_arg_setter_type::bse_arg_->set_flag(arg_flags::ASSIGNMENT_OPERATOR);
        }
        else
        {
            base_arg_setter_type::bse_arg_->unset_flag(arg_flags::ASSIGNMENT_OPERATOR);
        }

        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to specify the name that will be assigned to each value. Each value
     *              will have assigned the name that corresponds to their index. If there is more
     *              values than names, the remaining values will be set the the last specified
     *              name. Values names are exclusivelly used during the help menu printing.
     * @param       vals_names : Set of names assigned to the values.
     * @return      The object who call the method.
     */
    template<typename... Ts_>
    self_type& values_names(Ts_&&... vals_names)
    {
        dynamic_cast<key_value_arg_type*>(base_arg_setter_type::bse_arg_)
                ->set_values_names(std::forward<Ts_>(vals_names)...);
        return dynamic_cast<self_type&>(*this);
    }
};


}


#endif

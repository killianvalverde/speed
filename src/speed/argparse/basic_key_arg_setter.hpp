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
 * @file        speed/argparse/basic_key_arg_setter.hpp
 * @brief       basic_key_arg_setter class header.
 * @author      Killian Valverde
 * @date        2024/05/06
 */

#ifndef SPEED_ARGPARSE_BASIC_KEY_ARG_SETTER_HPP
#define SPEED_ARGPARSE_BASIC_KEY_ARG_SETTER_HPP

#include <utility>

#include "../type_traits/type_traits.hpp"
#include "arg_flags.hpp"
#include "basic_base_arg_setter.hpp"
#include "basic_key_arg.hpp"


namespace speed::argparse {


/**
 * @brief        Class allowing configuring a key argument.
 */
template<typename TpAllocator, typename TpActual = std::false_type>
class basic_key_arg_setter : virtual public speed::type_traits::basic_crtp_base<
        basic_base_arg_setter, basic_key_arg_setter, TpActual, std::false_type, TpAllocator>
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;

    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;

    /** Type that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<TpAllocator>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;

    /** Type that represents the class allowing configuring a base argument. */
    using base_arg_setter_type = speed::type_traits::basic_crtp_base<
            basic_base_arg_setter, basic_key_arg_setter, TpActual, std::false_type, TpAllocator>;

    /** Type that represents the actual type of the object. */
    using self_type = speed::type_traits::basic_crtp_self<
            basic_key_arg_setter, TpActual, std::false_type, TpAllocator>;

    /**
     * @brief       Default constructor.
     */
    basic_key_arg_setter() = default;

    /**
     * @brief       Constructor with parameters.
     * @param       ky_arg : Key argument that this object will configure.
     */
    explicit basic_key_arg_setter(key_arg_type* ky_arg)
            : base_arg_setter_type(static_cast<base_arg_type*>(ky_arg))
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_key_arg_setter(const basic_key_arg_setter& rhs) = delete;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_key_arg_setter(basic_key_arg_setter&& rhs) noexcept = delete;
    
    /**
     * @brief       Destructor.
     */
    ~basic_key_arg_setter() override = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_key_arg_setter& operator =(const basic_key_arg_setter& rhs) = delete;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_key_arg_setter& operator =(basic_key_arg_setter&& rhs) noexcept = delete;

    /**
     * @brief       Allows to specify whether the arguement can be grupped with other arguments.
     *              As an example, grouping is the hability to turn this arguments "-l -a" into
     *              this argument "-la". By default this feature is enable in all king of
     *              arguements. On the other hand, in order to be able to group arguements, this
     *              arguements have to have a key that is a single character, as well as being
     *              preceded by a short prefix.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& grouping(bool enabl)
    {
        if (enabl)
        {
            base_arg_setter_type::bse_arg_->set_flag(arg_flags::GROUPING);
        }
        else
        {
            base_arg_setter_type::bse_arg_->unset_flag(arg_flags::GROUPING);
        }

        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to set an argument sub parser. A sub parser allows to execute a new
     *              arguement parser with its own configuration and arguments once a specific
     *              Key argument is found. The parsing will be executed in the next command line
     *              argument after the one relied with the current argument.
     * @param       ap : The arguement sub parser.
     * @return      The object who call the method.
     */
    self_type& sub_parser(arg_parser_type* ap)
    {
        dynamic_cast<key_arg_type*>(base_arg_setter_type::bse_arg_)
                ->set_sub_arg_parser(ap);
        return dynamic_cast<self_type&>(*this);
    }
};


}


#endif

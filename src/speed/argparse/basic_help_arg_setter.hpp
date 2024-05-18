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
 * @file        speed/argparse/basic_help_arg_setter.hpp
 * @brief       basic_help_arg_setter class header.
 * @author      Killian Valverde
 * @date        2024/05/06
 */

#ifndef SPEED_ARGPARSE_BASIC_HELP_ARG_SETTER_HPP
#define SPEED_ARGPARSE_BASIC_HELP_ARG_SETTER_HPP

#include <utility>

#include "../type_traits/type_traits.hpp"
#include "arg_flags.hpp"
#include "basic_base_arg_setter.hpp"
#include "basic_help_arg.hpp"
#include "basic_key_arg_setter.hpp"
#include "basic_key_value_arg_setter.hpp"
#include "basic_value_arg_setter.hpp"


namespace speed::argparse {


/**
 * @brief        Class allowing configuring a help argument.
 */
template<typename TpAllocator, typename TpActual = std::false_type>
class basic_help_arg_setter : public speed::type_traits::basic_crtp_base<
        basic_key_value_arg_setter, basic_help_arg_setter, TpActual, std::false_type, TpAllocator>
{
public:
    /** Class that represents the allocator used in the class. */
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

    /** Type that represents the option to get the help information. */
    using help_arg_type = basic_help_arg<TpAllocator>;

    /** Type that represents the class allowing configuring a base argument. */
    using base_arg_setter_type = speed::type_traits::basic_crtp_base<basic_base_arg_setter,
            basic_help_arg_setter, TpActual, std::false_type, TpAllocator>;

    /** Type that represents the class allowing configuring a key argument. */
    using key_arg_setter_type = speed::type_traits::basic_crtp_base<basic_key_arg_setter,
            basic_help_arg_setter, TpActual, std::false_type, TpAllocator>;

    /** Type that represents the class allowing configuring a value argument. */
    using value_arg_setter_type = speed::type_traits::basic_crtp_base<basic_value_arg_setter,
            basic_help_arg_setter, TpActual, std::false_type, TpAllocator>;

    /** Type that represents the class allowing configuring a key value argument. */
    using key_value_arg_setter_type = speed::type_traits::basic_crtp_base<
            basic_key_value_arg_setter, basic_help_arg_setter, TpActual, std::false_type,
            TpAllocator>;

    /** Type that represents the actual type of the object. */
    using self_type = speed::type_traits::basic_crtp_self<basic_help_arg_setter,
            TpActual, std::false_type, TpAllocator>;

    /**
     * @brief       Default constructor.
     */
    basic_help_arg_setter() = default;

    /**
     * @brief       Constructor with parameters.
     * @param       hlp_arg : Help argument that this object will configure.
     */
    explicit basic_help_arg_setter(help_arg_type* hlp_arg)
            : base_arg_setter_type(static_cast<base_arg_type*>(hlp_arg))
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_help_arg_setter(const basic_help_arg_setter& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_help_arg_setter(basic_help_arg_setter&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_help_arg_setter() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_help_arg_setter& operator =(const basic_help_arg_setter& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_help_arg_setter& operator =(basic_help_arg_setter&& rhs) noexcept = default;

    /**
     * @brief       Allows to specify a couple of regex / help menu id, that will make the
     *              association between the help argument and the help menus it triggers.
     *              The regex will check the concatenation of all the values assigned to the help
     *              arguement separated by a white space, and if it matches, the help menu
     *              specified will be triggered.
     * @param       triggs : Set of regex / help menu id to assign help menus to an argument.
     * @return      The object who call the method.
     */
    template<typename... Ts_>
    self_type& help_menus_triggered(Ts_&&... triggs)
    {
        dynamic_cast<help_arg_type*>(base_arg_setter_type::bse_arg_)
                ->set_help_menus_triggered(std::forward<Ts_>(triggs)...);
        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Terminates the program after triggering the printing of the help menu. This
     *              feature is activated by default.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& pkill_after_triggering(bool enabl)
    {
        if (enabl)
        {
            base_arg_setter_type::bse_arg_->set_flag(arg_flags::PKILL_AFTER_TRIGGERING);
        }
        else
        {
            base_arg_setter_type::bse_arg_->unset_flag(arg_flags::PKILL_AFTER_TRIGGERING);
        }

        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Triggers the printing of the help menu when the help arg is found.
     *              This feature is activated by default.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& trigger_help_printing(bool enabl)
    {
        if (enabl)
        {
            base_arg_setter_type::bse_arg_->set_flag(arg_flags::TRIGGER_HELP_PRINTING);
        }
        else
        {
            base_arg_setter_type::bse_arg_->unset_flag(arg_flags::TRIGGER_HELP_PRINTING);
        }

        return dynamic_cast<self_type&>(*this);
    }
};


}


#endif

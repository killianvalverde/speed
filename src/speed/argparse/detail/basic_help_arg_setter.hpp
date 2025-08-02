/* speed - Generic C++ library.
 * Copyright (C) 2015-2025 Killian Valverde.
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
 * @file        basic_help_arg_setter.hpp
 * @brief       basic_help_arg_setter class header.
 * @author      Killian Valverde
 * @date        2024/05/06
 */

#ifndef SPEED_ARGPARSE_DETAIL_BASIC_HELP_ARG_SETTER_HPP
#define SPEED_ARGPARSE_DETAIL_BASIC_HELP_ARG_SETTER_HPP

#include <utility>

#include "../../type_traits/type_traits.hpp"
#include "arg_flags.hpp"
#include "basic_base_arg_setter.hpp"
#include "basic_help_arg.hpp"
#include "basic_key_arg_setter.hpp"
#include "basic_key_value_arg_setter.hpp"
#include "basic_value_arg_setter.hpp"

namespace speed::argparse::detail {

/**
 * @brief        Class allowing configuring a help argument.
 */
template<typename AllocatorT, typename ActualT = std::false_type>
class basic_help_arg_setter : public type_traits::basic_crtp_base<basic_key_arg_setter,
        basic_help_arg_setter, ActualT, std::false_type, AllocatorT>
{
public:
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<T>;

    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<AllocatorT>;

    /** Type that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<AllocatorT>;

    /** Type that represents arguments that have values. */
    using value_arg_type = basic_value_arg<AllocatorT>;

    /** Type that represents arguments that have keys and values. */
    using key_value_arg_type = basic_key_value_arg<AllocatorT>;

    /** Type that represents the option to get the help information. */
    using help_arg_type = basic_help_arg<AllocatorT>;

    /** Type that represents the class allowing configuring a base argument. */
    using base_arg_setter_type = type_traits::basic_crtp_base<basic_base_arg_setter,
            basic_help_arg_setter, ActualT, std::false_type, AllocatorT>;

    /** Type that represents the class allowing configuring a key argument. */
    using key_arg_setter_type = type_traits::basic_crtp_base<basic_key_arg_setter,
            basic_help_arg_setter, ActualT, std::false_type, AllocatorT>;

    /** Type that represents the class allowing configuring a value argument. */
    using value_arg_setter_type = type_traits::basic_crtp_base<basic_value_arg_setter,
            basic_help_arg_setter, ActualT, std::false_type, AllocatorT>;

    /** Type that represents the class allowing configuring a key value argument. */
    using key_value_arg_setter_type = type_traits::basic_crtp_base<
            basic_key_value_arg_setter, basic_help_arg_setter, ActualT, std::false_type,
            AllocatorT>;

    /** Type that represents the actual type of the object. */
    using self_type = type_traits::basic_crtp_self<basic_help_arg_setter,
            ActualT, std::false_type, AllocatorT>;

    /**
     * @brief       Default constructor.
     */
    basic_help_arg_setter() = default;

    /**
     * @brief       Constructor with parameters.
     * @param       hlp_arg : Help argument that this object will configure.
     */
    basic_help_arg_setter(help_arg_type* hlp_arg)
            : base_arg_setter_type(static_cast<base_arg_type*>(hlp_arg))
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_help_arg_setter(const basic_help_arg_setter& rhs) = delete;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_help_arg_setter(basic_help_arg_setter&& rhs) noexcept = delete;
    
    /**
     * @brief       Destructor.
     */
    ~basic_help_arg_setter() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_help_arg_setter& operator =(const basic_help_arg_setter& rhs) = delete;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_help_arg_setter& operator =(basic_help_arg_setter&& rhs) noexcept = delete;

    /**
     * @brief       Sets the help menu triggered by the argument.
     * @param       hlp_menu_triggerd : The help menu triggered.
     * @return      The object who call the method.
     */
    template<typename StringT_>
    self_type& help_menu_triggered(StringT_&& hlp_menu_triggerd)
    {
        dynamic_cast<help_arg_type*>(base_arg_setter_type::bse_arg_)
                ->set_help_menu_triggered(std::forward<StringT_>(hlp_menu_triggerd));
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

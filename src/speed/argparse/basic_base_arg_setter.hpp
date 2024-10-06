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
 * @file        speed/argparse/basic_base_arg_setter.hpp
 * @brief       basic_base_arg_setter class header.
 * @author      Killian Valverde
 * @date        2024/05/06
 */

#ifndef SPEED_ARGPARSE_BASIC_BASE_ARG_SETTER_HPP
#define SPEED_ARGPARSE_BASIC_BASE_ARG_SETTER_HPP

#include <utility>

#include "arg_flags.hpp"
#include "basic_base_arg.hpp"


namespace speed::argparse {


/**
 * @brief        Class allowing configuring a base argument.
 */
template<typename TpAllocator, typename TpActual>
class basic_base_arg_setter
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;

    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;

    /** Type that represents the actual type of the object. */
    using self_type = TpActual;

    /**
     * @brief       Default constructor.
     */
    basic_base_arg_setter() = default;

    /**
     * @brief       Constructor with parameters.
     * @param       bse_arg : Base argument that this object will configure.
     */
    explicit basic_base_arg_setter(base_arg_type* bse_arg)
            : bse_arg_(bse_arg)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_base_arg_setter(const basic_base_arg_setter& rhs) = delete;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_base_arg_setter(basic_base_arg_setter&& rhs) noexcept = delete;
    
    /**
     * @brief       Destructor.
     */
    virtual ~basic_base_arg_setter() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_base_arg_setter& operator =(const basic_base_arg_setter& rhs) = delete;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_base_arg_setter& operator =(basic_base_arg_setter&& rhs) noexcept = delete;

    /**
     * @brief       Allows to specify a function to be called when the argument is found during
     *              the program call.
     * @param       callabl : Function to be called when the argument is found during
     *              the program call.
     * @return      The object who call the method.
     */
    template<typename TpCallable>
    self_type& action(TpCallable&& callabl)
    {
        bse_arg_->set_action(std::forward<TpCallable>(callabl));
        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to specify the argument description. The description is a string that
     *              describes what the argument does. If there is no description set for an
     *              argument, this argument will not be printed in the associated help menu.
     * @param       desc : The description of the argument.
     * @return      The object who call the method.
     */
    template<typename TpString_>
    self_type& description(TpString_&& desc)
    {
        bse_arg_->set_description(std::forward<TpString_>(desc));
        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to specify the argument error name. The error name is the string the
     *              arguement will be referenced during the print of its error messages.
     * @param       err_name : The error name of the argument.
     * @return      The object who call the method.
     */
    template<typename TpString_>
    self_type& error_name(TpString_&& err_name)
    {
        bse_arg_->set_error_name(std::forward<TpString_>(err_name));
        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to specify the help menus in which the arguement will be printed. By
     *              default every argument will be displayed in the default help menu. If you
     *              specify another set of help menus, the last ones will no longer be assigned.
     * @param       hlp_menus_ids : Set of help menus IDs assigned to the argument.
     * @return      The object who call the method.
     */
    template<typename... Ts_>
    self_type& help_menus_assigned(Ts_&&... hlp_menus_ids)
    {
        bse_arg_->set_help_menus_assigned(std::forward<Ts_>(hlp_menus_ids)...);
        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to specify whether is mandatory to found the argument in the program
     *              call. By defaul only keyless arguments are mandatory.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& mandatory(bool enabl)
    {
        if (enabl)
        {
            bse_arg_->set_flag(arg_flags::MANDATORY);
        }
        else
        {
            bse_arg_->unset_flag(arg_flags::MANDATORY);
        }

        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to specify an adress to a bool in order to store whether the arguement
     *              has been found during the program call.
     * @param       presence_sync : Adress to a bool in which to store the presence.
     * @return      The object who call the method.
     */
    self_type& store_presence(bool* presence_sync)
    {
        bse_arg_->set_presence_holder(presence_sync);
        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to specify whether finding this argument will make your program ifnore
     *              the presence of all the other arguments. This flag is only cosmetic to print
     *              correctly the usage message. By default only help and version arguments are
     *              terminal.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& terminal(bool enabl)
    {
        if (enabl)
        {
            bse_arg_->set_flag(arg_flags::TERMINAL);
        }
        else
        {
            bse_arg_->unset_flag(arg_flags::TERMINAL);
        }

        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to specify whether the argument can be found more than once during the
     *              program call. This option is enable dy default for all the arguements.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& unique_instance(bool enabl)
    {
        if (enabl)
        {
            bse_arg_->set_flag(arg_flags::UNIQUE_INSTANCE);
        }
        else
        {
            bse_arg_->unset_flag(arg_flags::UNIQUE_INSTANCE);
        }

        return dynamic_cast<self_type&>(*this);
    }


protected:
    /** Reference to the argument that this object configures. */
    base_arg_type* bse_arg_;
};


}


#endif

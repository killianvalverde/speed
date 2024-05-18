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
 * @file        speed/argparse/basic_help_menu_setter.hpp
 * @brief       basic_help_menu_setter class header.
 * @author      Killian Valverde
 * @date        2024/05/08
 */

#ifndef SPEED_ARGPARSE_BASIC_HELP_MENU_SETTER_HPP
#define SPEED_ARGPARSE_BASIC_HELP_MENU_SETTER_HPP

#include <string>
#include <vector>

#include "basic_help_menu.hpp"


namespace speed::argparse {


/**
 * @brief        Class allowing configuring a help menu.
 */
template<typename TpAllocator>
class basic_help_menu_setter
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;

    /** Type that represents a help menu. */
    using help_menu_type = basic_help_menu<TpAllocator>;

    /** Type that represents the actual type of the object. */
    using self_type = basic_help_menu_setter;

    /**
     * @brief       Default constructor.
     */
    basic_help_menu_setter() = default;

    /**
     * @brief       Constructor with parameters.
     * @param       hlp_menu : Help menu that this object will configure.
     */
    explicit basic_help_menu_setter(help_menu_type* hlp_menu)
            : hlp_menu_(hlp_menu)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_help_menu_setter(const basic_help_menu_setter& rhs) = delete;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_help_menu_setter(basic_help_menu_setter&& rhs) noexcept = delete;
    
    /**
     * @brief       Destructor.
     */
    ~basic_help_menu_setter() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_help_menu_setter& operator =(const basic_help_menu_setter& rhs) = delete;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_help_menu_setter& operator =(basic_help_menu_setter&& rhs) noexcept = delete;

    /**
     * @brief       Allows to specify the indentation that the arguments keys will have when
     *              printing them into the help menu.
     * @param       args_indent : The indentation that the arguements keys will have when
     *              printing hem into the help menu.
     * @return      The object who call the method.
     */
    self_type& arguments_indentation(std::size_t args_indent)
    {
        hlp_menu_->set_args_indentation(args_indent);
        return *this;
    }

    /**
     * @brief       Allows to specify the description printed for the help menu. The description is
     *              printed just after the usage.
     * @param       desc : The description printed.
     * @return      The object who call the method.
     */
    template<typename TpString_>
    self_type& description(TpString_&& desc)
    {
        hlp_menu_->set_description(std::forward<TpString_>(desc));
        return *this;
    }

    /**
     * @brief       Allows to specify the epilogue printed for the help menu. The epilogue is
     *              printed just at the end of the help menu.
     * @param       epilg : The epilogue printed.
     * @return      The object who call the method.
     */
    template<typename TpString_>
    self_type& epilogue(TpString_&& epilg)
    {
        hlp_menu_->set_epilog(std::forward<TpString_>(epilg));
        return *this;
    }

    /**
     * @brief       Allows to specify the maximum length that a line will have when printing the
     *              help menu. The maximum line length is 80 by default.
     * @param       max_line_len : The maximum line length.
     * @return      The object who call the method.
     */
    self_type& maximum_line_length(std::size_t max_line_len)
    {
        hlp_menu_->set_maximum_line_length(max_line_len);
        return *this;
    }

    /**
     * @brief       Allows to specify the indentation to make after printing a new line. The new
     *              line indentation is 2 by defaul.
     * @param       new_line_indent : The new line indentation.
     * @return      The object who call the method.
     */
    self_type& new_line_indentation(std::size_t new_line_indent)
    {
        hlp_menu_->set_new_line_indentation(new_line_indent);
        return *this;
    }

    /**
     * @brief       Allows to specify whether the arguements keys have to be printed. This feature
     *              is enabled by default.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& print_arguments_keys(bool enabl)
    {
        if (enabl)
        {
            hlp_menu_->set_flag(help_menu_flags::PRINT_ARGS_KEYS);
        }
        else
        {
            hlp_menu_->unset_flag(help_menu_flags::PRINT_ARGS_KEYS);
        }

        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to specify whether the commands have to be printed. Commands are
     *              arguments that are mandatory to appear in the program call. This feature
     *              is enabled by default.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& print_commands(bool enabl)
    {
        if (enabl)
        {
            hlp_menu_->set_flag(help_menu_flags::PRINT_COMMANDS);
        }
        else
        {
            hlp_menu_->unset_flag(help_menu_flags::PRINT_COMMANDS);
        }

        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to specify whether the options have to be printed. Options are
     *              arguments that are mandatory to appear in the program call. This feature
     *              is enabled by default.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& print_options(bool enabl)
    {
        if (enabl)
        {
            hlp_menu_->set_flag(help_menu_flags::PRINT_OPTIONS);
        }
        else
        {
            hlp_menu_->unset_flag(help_menu_flags::PRINT_OPTIONS);
        }

        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to specify whether the usage have to be printed. The usage is a brief
     *              description that is only one line long that sumarize the sintax of the program
     *              call. This feature is enabled by default.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& print_usage(bool enabl)
    {
        if (enabl)
        {
            hlp_menu_->set_flag(help_menu_flags::PRINT_USAGE);
        }
        else
        {
            hlp_menu_->unset_flag(help_menu_flags::PRINT_USAGE);
        }

        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows to specify whether the values have to be printed. The values are the
     *              keyless arguments. This feature is enabled by default.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& print_values(bool enabl)
    {
        if (enabl)
        {
            hlp_menu_->set_flag(help_menu_flags::PRINT_VALUES);
        }
        else
        {
            hlp_menu_->unset_flag(help_menu_flags::PRINT_VALUES);
        }

        return dynamic_cast<self_type&>(*this);
    }

private:
    /** Reference to the help menu that this object configures. */
    help_menu_type* hlp_menu_;
};


}


#endif

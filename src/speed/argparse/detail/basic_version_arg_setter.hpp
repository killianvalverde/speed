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
 * @file        basic_version_arg_setter.hpp
 * @brief       basic_version_arg_setter class header.
 * @author      Killian Valverde
 * @date        2024/05/06
 */

#ifndef SPEED_ARGPARSE_DETAIL_BASIC_VERSION_ARG_SETTER_HPP
#define SPEED_ARGPARSE_DETAIL_BASIC_VERSION_ARG_SETTER_HPP

#include <string>
#include <utility>

#include "../../type_traits/type_traits.hpp"
#include "arg_flags.hpp"
#include "basic_base_arg_setter.hpp"
#include "basic_key_arg_setter.hpp"
#include "basic_version_arg.hpp"

namespace speed::argparse::detail {

/**
 * @brief        Class allowing configuring a version argument.
 */
template<typename AllocatorT, typename ActualT = std::false_type>
class basic_version_arg_setter : public type_traits::basic_crtp_base<basic_key_arg_setter,
        basic_version_arg_setter, ActualT, std::false_type, AllocatorT>
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<T>;
    
    /** String type used in the class. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;

    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<AllocatorT>;

    /** Type that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<AllocatorT>;

    /** Type that represents the argument to get the version information. */
    using version_arg_type = basic_version_arg<AllocatorT>;

    /** Type that represents the class allowing configuring a base argument. */
    using base_arg_setter_type = type_traits::basic_crtp_base<basic_base_arg_setter,
            basic_version_arg_setter, ActualT, std::false_type, AllocatorT>;

    /** Type that represents the class allowing configuring a key argument. */
    using key_arg_setter_type = type_traits::basic_crtp_base<basic_key_arg_setter,
            basic_version_arg_setter, ActualT, std::false_type, AllocatorT>;

    /** Type that represents the actual type of the object. */
    using self_type = type_traits::basic_crtp_self<basic_version_arg_setter,
            ActualT, std::false_type, AllocatorT>;

    /**
     * @brief       Default constructor.
     */
    basic_version_arg_setter() = default;

    /**
     * @brief       Constructor with parameters.
     * @param       vers_arg : Version argument that this object will configure.
     */
    basic_version_arg_setter(version_arg_type* vers_arg)
            : base_arg_setter_type(static_cast<base_arg_type*>(vers_arg))
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_version_arg_setter(const basic_version_arg_setter& rhs) = delete;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_version_arg_setter(basic_version_arg_setter&& rhs) noexcept = delete;
    
    /**
     * @brief       Destructor.
     */
    ~basic_version_arg_setter() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_version_arg_setter& operator =(const basic_version_arg_setter& rhs) = delete;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_version_arg_setter& operator =(basic_version_arg_setter&& rhs) noexcept = delete;

    /**
     * @brief       Set a GPLv3 formated version information.
     * @param       prog_vers : The program version.
     * @param       dte : The date of the software release.
     * @param       authr : The author of the software.
     * @return      The object who call the method.
     */
    template<typename StringT1_, typename StringT2_, typename StringT3_>
    self_type& gplv3_version_information(
            const StringT1_& prog_vers,
            const StringT2_& dte,
            const StringT3_& authr
    )
    {
        string_type vers_info;

        auto& prog_name = base_arg_setter_type::bse_arg_->get_program_name();

        vers_info.append(prog_name.empty() ? "???" : prog_name);
        vers_info.append(" ");
        if (stringutils::get_length(prog_vers) == 0)
        {
            vers_info.append("v1.0.0");
        }
        else
        {
            vers_info.append(prog_vers);
        }

        vers_info.append("\nCopyright (C) ");
        if (stringutils::get_length(dte) == 0)
        {
            vers_info.append("20XX");
        }
        else
        {
            vers_info.append(dte);
        }

        vers_info.append(
                " Free Software Foundation, Inc.\n"
                "License GPLv3+: GNU GPL version 3 or later "
                "<http://gnu.org/licenses/gpl.html>.\n"
                "This is free software: you are free to change and redistribute it.\n"
                "There is NO WARRANTY, to the extent permitted by law.\n\n"
                "Written by ");
        if (stringutils::get_length(authr) == 0)
        {
            vers_info.append("Unknown");
        }
        else
        {
            vers_info.append(authr);
        }
        vers_info.append(".");

        return version_information(std::move(vers_info));
    }

    /**
     * @brief       Terminates the program after triggering the printing of the version
     *              information. This feature is activated by default.
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
     * @brief       Triggers the printing of the version information when the version arg is found.
     *              This feature is activated by default.
     * @param       enabl : Specify wheter the feature is enable.
     * @return      The object who call the method.
     */
    self_type& trigger_version_printing(bool enabl)
    {
        if (enabl)
        {
            base_arg_setter_type::bse_arg_->set_flag(arg_flags::TRIGGER_VERSION_PRINTING);
        }
        else
        {
            base_arg_setter_type::bse_arg_->unset_flag(arg_flags::TRIGGER_VERSION_PRINTING);
        }

        return dynamic_cast<self_type&>(*this);
    }

    /**
     * @brief       Allows specifying the version information.
     * @param       vers_info : Version information.
     * @return      The object who call the method.
     */
    template<typename StringT_>
    self_type& version_information(StringT_&& vers_info)
    {
        dynamic_cast<version_arg_type*>(base_arg_setter_type::bse_arg_)
                ->set_version_information(std::forward<StringT_>(vers_info));
        return dynamic_cast<self_type&>(*this);
    }
};

}

#endif

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
 * @file        arg_flags.hpp
 * @brief       arg_flags enum header.
 * @author      Killian Valverde
 * @date        2016/03/10
 */

#ifndef SPEED_ARGPARSE_DETAIL_ARG_FLAGS_HPP
#define SPEED_ARGPARSE_DETAIL_ARG_FLAGS_HPP

#include "../../scalars/scalars.hpp"

namespace speed::argparse::detail {

/**
 * @brief       Contains all argument flags constants.
 */
enum class arg_flags : std::uint16_t
{
    /** Null flag. */
    NIL = 0x0,
    
    /** The argument can be grouped (-la == -l -a). */
    GROUPING = 0x1,

    /** The values for the argument can have a prefix. */
    VALUES_WITH_PREFIX = 0x2,

    /** The values for the arugment can have other arguments keys as values. */
    KEYS_AS_VALUES = 0x4,
    
    /** Allows the usage of the equal operator (--tries=NUMBER). */
    ASSIGNMENT_OPERATOR = 0x8,

    /** Indicates that the argument will avoid the process of the rest of the arguments. */
    TERMINAL = 0x10,

    /** The argument will trigger the print of the help menu. Just used for help_args. */
    TRIGGER_HELP_PRINTING = 0x20,

    /** The argument will trigger the print of the version . Just used for version_args. */
    TRIGGER_VERSION_PRINTING = 0x40,

    /** Pkill the program when help or version args are found and triggers a print. */
    PKILL_AFTER_TRIGGERING = 0x80,
    
    /** All argument flags. */
    ALL = 0xFF,
    
    /** The default flags used for the key arguments. */
    DEFAULT_KEY_ARG_FLAGS = (
            GROUPING
    ),
    
    /** The default flags used for the key value arguments. */
    DEFAULT_KEY_VALUE_ARG_FLAGS = (
            GROUPING |
            ASSIGNMENT_OPERATOR
    ),
    
    /** The default flags used for the positional arguments. */
    DEFAULT_POSITIONAL_ARG_FLAGS = (
            NIL
    ),
    
    /** The default flags used for the help arguments. */
    DEFAULT_HELP_ARG_FLAGS = (
            TERMINAL |
            TRIGGER_HELP_PRINTING |
            PKILL_AFTER_TRIGGERING
    ),
    
    /** The default flags used for the version arguments. */
    DEFAULT_VERSION_ARG_FLAGS = (
            TERMINAL |
            TRIGGER_VERSION_PRINTING |
            PKILL_AFTER_TRIGGERING
    )
};

/** Contains all argument flags constants. */
using af_t = arg_flags;

}

/** @cond */
template<>
struct speed::scalars::is_flag_enum<speed::argparse::detail::arg_flags>
        : std::true_type {};
/** @endcond */

#endif

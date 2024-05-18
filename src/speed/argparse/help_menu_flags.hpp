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
 * @file        speed/argparse/help_menu_flags.hpp
 * @brief       help_menu_flags enum header.
 * @author      Killian Valverde
 * @date        2024/04/29
 */

#ifndef SPEED_ARGPARSE_HELP_MENU_FLAGS_HPP
#define SPEED_ARGPARSE_HELP_MENU_FLAGS_HPP

#include "../lowlevel/lowlevel.hpp"


namespace speed::argparse {


/**
 * @brief       Contains all help menu flags constants.
 */
enum class help_menu_flags : std::uint16_t
{
    /** Null flag. */
    NIL = 0x0,

    /** Whether the usage has to be printed. */
    PRINT_USAGE = 0x1,

    /** Whether the description has to be printed. */
    PRINT_DESCRIPTION = 0x2,

    /** Whether the arguements keys has to be printed. */
    PRINT_ARGS_KEYS = 0x4,

    /** Whether the options has to be printed. */
    PRINT_OPTIONS = 0x8,

    /** Whether the commands has to be printed. */
    PRINT_COMMANDS = 0x10,

    /** Whether the values has to be printed. */
    PRINT_VALUES = 0x20,

    /** Whether the epilogue has to be printed. */
    PRINT_EPILOGUE = 0x40,
    
    /** All help menu flags. */
    ALL = 0x7F,
    
    /** The default flags used for the help menu. */
    DEFAULT = (
            PRINT_USAGE |
            PRINT_DESCRIPTION |
            PRINT_OPTIONS |
            PRINT_ARGS_KEYS |
            PRINT_EPILOGUE
    )
};


/** Contains all help menu flags constants. Alias of 'help_menu_flags' enumeration. */
using hmf_t = help_menu_flags;


}


/** @cond */
namespace speed::lowlevel {
template<>
struct enable_bitwise_operators<speed::argparse::help_menu_flags>
{
    static constexpr bool enable_with_same_type = true;
};
}
/** @endcond */


#endif

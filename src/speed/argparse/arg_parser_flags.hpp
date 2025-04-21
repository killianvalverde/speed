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
 * @file        arg_parser_flags.hpp
 * @brief       arg_parser_flags enum header.
 * @author      Killian Valverde
 * @date        2016/08/07
 */

#ifndef SPEED_ARGPARSE_ARG_PARSER_FLAGS_HPP
#define SPEED_ARGPARSE_ARG_PARSER_FLAGS_HPP

#include "../lowlevel/lowlevel.hpp"

namespace speed::argparse {

/**
 * @brief       Contains all argument parser flags constants.
 */
enum class arg_parser_flags : std::uint8_t
{
    /** Null flag. */
    NIL = 0x0,
    
    /** Use colors to print arguments errors. */
    USE_COLORS = 0x1,
    
    /** If there are errors the errors will be printed in the standard output. */
    PRINT_ERRORS = 0x2,
    
    /** If errors are printed, the default help menu will be printed just after. */
    PRINT_HELP_AFTER_PRINTING_ERRORS = 0x4,
    
    /** Exit the program when the args errors are printed. */
    PKILL_AFTER_PRINTING_ERRORS = 0x8,
    
    /** All argument parser flags. */
    ALL = 0xF,
    
    /** The default flags used by the argument parser. */
    DEFAULT_ARG_PARSER_FLAGS = (
            USE_COLORS |
            PRINT_ERRORS |
            PKILL_AFTER_PRINTING_ERRORS
    ),
};

/** Contains all argument parser flags constants. Alias of 'arg_parser_flags' enumeration. */
using apf_t = arg_parser_flags;

}

/** @cond */
namespace speed::lowlevel {
template<>
struct enable_bitwise_operators<speed::argparse::arg_parser_flags>
{
    static constexpr bool enable_with_same_type = true;
};
}
/** @endcond */

#endif

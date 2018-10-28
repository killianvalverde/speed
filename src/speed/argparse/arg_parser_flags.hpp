/* speed - Generic C++ library.
 * Copyright (C) 2015-2023 Killian Valverde.
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
 * @file        speed/argparse/arg_parser_flags.hpp
 * @brief       arg_parser_flags enum header.
 * @author      Killian Green
 * @date        2016/08/07 - 01:08
 */

#ifndef SPEED_ARGPARSE_ARG_PARSER_FLAGS_HPP
#define SPEED_ARGPARSE_ARG_PARSER_FLAGS_HPP

#include "../lowlevel.hpp"


namespace speed::argparse {


/**
 * @brief       Contains all argument parser flags constants.
 */
enum class arg_parser_flags : std::uint16_t
{
    /** Null flag. */
    NIL = 0x0,
    
    /** Print the help information when a help argument is found. */
    PRINT_HELP_WHEN_HELP_ARG_FOUND = 0x1,
    
    /** The common 'usage' sentence will be printed in the beginning of the help information. */
    PRINT_USAGE_WHEN_PRINT_HELP = 0x2,
    
    /** The common 'Options:' sentence will be printed in the before the first argument. */
    PRINT_OPTIONS_SENTENCE_WHEN_PRINT_HELP = 0x4,
    
    /** When the help information will be printed, the arguments keys or help ids will be printed
     * before the description. */
    PRINT_ARGS_ID_WHEN_PRINT_HELP = 0x8,
    
    /** Exit the program when the help information is printed. */
    EXIT_ON_PRINT_HELP = 0x10,
    
    /** Print the version information when a version argument is found. */
    PRINT_VERSION_WHEN_VERSION_ARG_FOUND = 0x20,
    
    /** Exit the program when the version information is printed. */
    EXIT_ON_PRINT_VERSION = 0x40,
    
    /** If there are errors the errors will be printed in the standard output. */
    PRINT_ERRORS = 0x80,
    
    /** If there are arguments errors the 'try program_name --help' sentece will appear. */
    PRINT_TRY_HELP_WHEN_PRINT_ERRORS = 0x100,
    
    /** If there are arguments errors the help information will be printed in the standard
     * output. */
    PRINT_HELP_WHEN_PRINT_ERRORS = 0x200,
    
    /** Use colors to print arguments errors. */
    USE_COLORS_WHEN_PRINT_ERRORS = 0x400,
    
    /** Exit the program when the args errors are printed. */
    EXIT_ON_PRINT_ARGS_ERRORS = 0x800,
    
    /** The foreign arguments values cannot be added if the value contains one of the prefixes. */
    KEYLESS_ARG_VALUE_CANNOT_BE_ADDED_WITH_PREFIX = 0x1000,
    
    /** All argument parser flags. */
    ALL = 0x1FFF,
    
    /** The default flags used by the argument parser. */
    DEFAULT_ARG_PARSER_FLAGS = (
            PRINT_HELP_WHEN_HELP_ARG_FOUND |
            PRINT_USAGE_WHEN_PRINT_HELP |
            PRINT_OPTIONS_SENTENCE_WHEN_PRINT_HELP |
            PRINT_ARGS_ID_WHEN_PRINT_HELP |
            EXIT_ON_PRINT_HELP |
            PRINT_VERSION_WHEN_VERSION_ARG_FOUND |
            EXIT_ON_PRINT_VERSION |
            PRINT_ERRORS |
            PRINT_TRY_HELP_WHEN_PRINT_ERRORS |
            USE_COLORS_WHEN_PRINT_ERRORS |
            EXIT_ON_PRINT_ARGS_ERRORS |
            KEYLESS_ARG_VALUE_CANNOT_BE_ADDED_WITH_PREFIX
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

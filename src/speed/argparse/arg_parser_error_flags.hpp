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
 * @file        speed/argparse/arg_parser_error_flags.hpp
 * @brief       arg_parser_error_flags enum header.
 * @author      Killian Valverde
 * @date        2016/08/07
 */

#ifndef SPEED_ARGPARSE_ARG_PARSER_ERROR_FLAGS_HPP
#define SPEED_ARGPARSE_ARG_PARSER_ERROR_FLAGS_HPP

#include "../lowlevel/lowlevel.hpp"


namespace speed::argparse {


/**
 * @brief       Contains all argument parser error flags constants.
 */
enum class arg_parser_error_flags : std::uint8_t
{
    /** Null flag. */
    NIL = 0x0,
    
    /** The argument parser has arguments errors. */
    ARGS_ERROR = 0x1,
    
    /** The argument parser has found arguments that are unrecognized. */
    UNRECOGNIZED_ARGS_ERROR = 0x2,
    
    /** The argument parser has arguments whose constraints are violated. */
    ARGS_CONSTRAINTS_ERROR = 0x4,
    
    /** All argument parser error flags. */
    ALL = 0x7
};


/** Contains all argument parser error flags constants. */
using apef_t = arg_parser_error_flags;


}


/** @cond */
namespace speed::lowlevel {
template<>
struct enable_bitwise_operators<speed::argparse::arg_parser_error_flags>
{
    static constexpr bool enable_with_same_type = true;
};
}
/** @endcond */


#endif

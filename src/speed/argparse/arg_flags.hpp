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
 * @file        speed/argparse/arg_flags.hpp
 * @brief       arg_flags enum header.
 * @author      Killian Valverde
 * @date        2016/03/10 - 15:56
 */

#ifndef SPEED_ARGPARSE_ARG_FLAGS_HPP
#define SPEED_ARGPARSE_ARG_FLAGS_HPP

#include "../lowlevel.hpp"


namespace speed::argparse {


/**
 * @brief       Contains all argument flags constants.
 */
enum class arg_flags : std::uint16_t
{
    /** Null flag. */
    NIL = 0x0,
    
    /** The argument allways has to be present in the program call. */
    ALLWAYS_REQUIRED = 0x1,
    
    /** The argument must appear just once in the program call. */
    APPEAR_JUST_ONCE = 0x2,
    
    /** The argument will cause the program exit. Flag just used for the display of the help
     * information, it has no effect in the parser behaviour. */
    IS_TERMINAL = 0x4,
    
    /** The argument can be chained (-la == -l -a). The minimum and the maximum number of values for
     * an option has to be the same. */
    ALLOW_CHAIN = 0x8,
    
    /** The argument allows the equal operator (--tries=NUMBER). */
    ALLOW_EQ_OPERATOR = 0x10,
    
    /** The first key of an argument will be used to print the error id if the error id is empty. */
    USE_FIRST_KEY_IF_ERROR_ID_EMPTY = 0x20,
    
    /** Print the argument error id if there is an error with a path. */
    PRINT_ERROR_ID_WHEN_PATH_ERROR = 0x40,
    
    /** All argument flags. */
    ALL = 0x7F,
    
    /** The default flags used for the arguments. */
    DEFAULT_ARG_FLAGS = (
            APPEAR_JUST_ONCE |
            ALLOW_CHAIN |
            ALLOW_EQ_OPERATOR |
            USE_FIRST_KEY_IF_ERROR_ID_EMPTY
    ),
    
    DEFAULT_KEYLESS_ARG_FLAGS = (
            APPEAR_JUST_ONCE |
            ALLWAYS_REQUIRED |
            USE_FIRST_KEY_IF_ERROR_ID_EMPTY
    )
};


/** Contains all argument flags constants. Alias of 'arg_flags' enumeration. */
using af_t = arg_flags;


}


/** @cond */
namespace speed::lowlevel {
template<>
struct enable_bitwise_operators<speed::argparse::arg_flags>
{
    static constexpr bool enable_with_same_type = true;
};
}
/** @endcond */


#endif

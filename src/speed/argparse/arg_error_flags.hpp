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
 * @file        arg_error_flags.hpp
 * @brief       arg_error_flags enum header.
 * @author      Killian Valverde
 * @date        2016/03/10
 */

#ifndef SPEED_ARGPARSE_ARG_ERROR_FLAGS_HPP
#define SPEED_ARGPARSE_ARG_ERROR_FLAGS_HPP

#include <cstdint>

#include "../lowlevel/lowlevel.hpp"

namespace speed::argparse {

/**
 * @brief       Contains all argument error flags constants.
 */
enum class arg_error_flags : std::uint8_t
{
    /** Null flag. */
    NIL = 0x0,
    
    /** The minimum number of times the argument needs to be found was not obtained. */
    MIN_OCCURRENCES_ERROR = 0x1,
    
    /** The maximum number of times the argument needs to be found was exceeded. */
    MAX_OCCURRENCES_ERROR = 0x2,
    
    /** The minimum number of values for an argument was not obtained. */
    MIN_VALUES_ERROR = 0x4,
    
    /** The maximum number of values for an argument was exceeded. */
    MAX_VALUES_ERROR = 0x8,
    
    /** The argument values have errors. */
    VALUES_ERROR = 0x10,
    
    /** All argument error flags. */
    ALL = 0x1F
};

/** Contains all argument error flags constants. */
using aef_t = arg_error_flags;

}

/** @cond */
namespace speed::lowlevel {
template<>
struct enable_bitwise_operators<speed::argparse::arg_error_flags>
{
    static constexpr bool enable_with_same_type = true;
};
}
/** @endcond */

#endif

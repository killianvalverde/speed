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
 * @file        arg_constraint_flags.hpp
 * @brief       arg_constraint_flags enum header.
 * @author      Killian Valverde
 * @date        2025/04/20
 */

#ifndef SPEED_ARGPARSE_ARG_CONSTRAINT_FLAGS_HPP
#define SPEED_ARGPARSE_ARG_CONSTRAINT_FLAGS_HPP

#include "../lowlevel/lowlevel.hpp"

namespace speed::argparse {

/**
 * @brief       Contains all argument constraint flags constants.
 */
enum class arg_constraint_flags : std::uint8_t
{
    /** Null flag. */
    NIL = 0x0,
    
    /** One of the arguments has to be present in the program call. */
    ONE_OR_MORE_REQUIRED = 0x1,
    
    /** All the arguments are mutually exclusive. */
    MUTUALLY_EXCLUSIVE = 0x2,
    
    /** All argument flags. */
    ALL = 0x3
};

/** Contains all argument constraint flags constants. */
using acf_t = arg_constraint_flags;

}

/** @cond */
namespace speed::lowlevel {
template<>
struct enable_bitwise_operators<speed::argparse::arg_constraint_flags>
{
    static constexpr bool enable_with_same_type = true;
};
}
/** @endcond */

#endif

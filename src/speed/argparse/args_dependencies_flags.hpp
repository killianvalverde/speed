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
 * @file        speed/argparse/args_dependencies_flags.hpp
 * @brief       args_dependencies_flags enum header.
 * @author      Killian
 * @date        2017/10/21 - 01:21
 */

#ifndef SPEED_ARGPARSE_ARGS_DEPENDENCIES_FLAGS_HPP
#define SPEED_ARGPARSE_ARGS_DEPENDENCIES_FLAGS_HPP

#include "../lowlevel.hpp"


namespace speed::argparse {


/**
 * @brief       Contains all arguments relational constraints types.
 */
enum class args_dependencies_flags : std::uint8_t
{
    /** Null flag. */
    NIL = 0x0,
    
    /** At least one of the arguments have to be found during the parsing in the argv container. */
    AT_LEAST_ONE_FOUND = 0x1,
    
    /** All flags. */
    ALL = 0x1,
};


/** Contains all arguments relational constraints types. Alias of 'args_dependencies_flags'
 * enumeration. */
using adf_t = args_dependencies_flags;


}


/** @cond */
namespace speed::lowlevel {
template<>
struct enable_bitwise_operators<speed::argparse::args_dependencies_flags>
{
    static constexpr bool enable_with_same_type = true;
};
}
/** @endcond */


#endif

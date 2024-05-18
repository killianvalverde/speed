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
 * @file        speed/system/filesystem/access_modes.hpp
 * @brief       access_modes header.
 * @author      Killian Valverde
 * @date        2019/03/22
 */

#ifndef SPEED_SYSTEM_FILESYSTEM_ACCESS_MODES_HPP
#define SPEED_SYSTEM_FILESYSTEM_ACCESS_MODES_HPP

#include <cstdint>

#include "../../lowlevel/lowlevel.hpp"


namespace speed::system::filesystem {


/**
 * @brief       Represents the files access modes.
 */
enum class access_modes : std::uint8_t
{
    /** Any access mode. */
    NIL = 0,
    
    /** The file exists. */
    EXISTS = 0x1,
    
    /** Read is available. */
    READ = 0x2,
    
    /** Write is available. */
    WRITE = 0x4,
    
    /** Execute is available. */
    EXECUTE = 0x8,
    
    /** All access modes. */
    FULL = 0xF
};


/** Represents the files access modes. */
using am_t = access_modes;


}


/** @cond */
namespace speed::lowlevel {
template<>
struct enable_bitwise_operators<speed::system::filesystem::access_modes>
{
    static constexpr bool enable_with_same_type = true;
};
}
/** @endcond */


#endif

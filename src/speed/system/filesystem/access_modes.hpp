/* speed - Generic C++ library.
 * Copyright (C) 2015-2026 Killian Valverde.
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
 * @file access_modes.hpp
 * @brief File access mode flags.
 * @author Killian Valverde
 * @date 2019-03-22
 */

#pragma once

#include <cstdint>

#include "../../enums/enums.hpp"

namespace speed::system::filesystem {

/**
 * @brief File access mode flags.
 *
 * Defines the access modes that can be queried or required for a filesystem object.
 * Multiple values may be combined using bitwise operators.
 */
enum class access_modes : std::uint8_t
{
    /** No access mode. */
    NIL = 0,

    /** Read access. */
    READ = 0x1,

    /** Write access. */
    WRITE = 0x2,

    /** Execute access. */
    EXECUTE = 0x4,

    /** Combination of all access mode flags. */
    ALL = 0x7
};

/** Alias for access_modes. */
using am_t = access_modes;

}

/** @cond */
template<>
struct speed::scalars::is_flag_enum<speed::system::filesystem::access_modes>
        : std::true_type {};
/** @endcond */

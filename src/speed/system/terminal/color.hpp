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
 * @file color.hpp
 * @brief Terminal color definitions.
 * @author Killian Valverde
 * @date 2019-03-22
 */

#pragma once

#include <cstdint>

namespace speed::system::terminal {

/**
 * @brief Represents terminal colors.
 *
 * This enumeration defines standard colors supported by terminal environments.
 */
enum class color : std::uint8_t
{
    /** No color. */
    NIL,

    /** Default terminal color. */
    DEFAULT,

    /** Black color. */
    BLACK,

    /** Red color. */
    RED,

    /** Green color. */
    GREEN,

    /** Yellow color. */
    YELLOW,

    /** Blue color. */
    BLUE,

    /** Magenta color. */
    MAGENTA,

    /** Cyan color. */
    CYAN,

    /** White color. */
    WHITE,

    /** Bright black color. */
    BRIGHT_BLACK,

    /** Bright red color. */
    BRIGHT_RED,

    /** Bright green color. */
    BRIGHT_GREEN,

    /** Bright yellow color. */
    BRIGHT_YELLOW,

    /** Bright blue color. */
    BRIGHT_BLUE,

    /** Bright magenta color. */
    BRIGHT_MAGENTA,

    /** Bright cyan color. */
    BRIGHT_CYAN,

    /** Bright white color. */
    BRIGHT_WHITE
};

/** Represents terminal colors. */
using c_t = color;

}

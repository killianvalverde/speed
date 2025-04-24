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
 * @file        text_attribute.hpp
 * @brief       terminal_text_attribute header.
 * @author      Killian Valverde
 * @date        2019/03/22
 */

#ifndef SPEED_SYSTEM_TERMINAL_TEXT_ATTRIBUTE_HPP
#define SPEED_SYSTEM_TERMINAL_TEXT_ATTRIBUTE_HPP

#include <cstdint>

namespace speed::system::terminal {

/**
 * @brief       Represents terminal text attributes.
 */
enum class text_attribute : std::uint8_t
{
    /** Nil text attribute. */
    NIL,
    
    /** Default text attribute. */
    DEFAULT,
    
    /** Black text attribute. */
    BLACK,
    
    /** Blue text attribute. */
    BLUE,
    
    /** Brown text attribute. */
    BROWN,
    
    /** Cyan text attribute. */
    CYAN,
    
    /** Gray text attribute. */
    GRAY,
    
    /** Green text attribute. */
    GREEN,
    
    /** Light blue text attribute. */
    LIGHT_BLUE,
    
    /** Light cyan text attribute. */
    LIGHT_CYAN,
    
    /** Light gray text attribute. */
    LIGHT_GRAY,
    
    /** Light green text attribute. */
    LIGHT_GREEN,
    
    /** Light purple text attribute. */
    LIGHT_PURPLE,
    
    /** Light red text attribute. */
    LIGHT_RED,
    
    /** Purple text attribute. */
    PURPLE,
    
    /** Red text attribute. */
    RED,
    
    /** White text attribute. */
    WHITE,
    
    /** Yellow text attribute. */
    YELLOW,
};

/** Represents terminal text attributes. */
using ta_t = text_attribute;

}

#endif

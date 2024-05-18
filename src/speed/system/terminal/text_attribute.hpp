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
 * @file        speed/system/terminal/text_attribute.hpp
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
    
    /** Default text attribute */
    DEFAULT,
    
    /** Black text. */
    BLACK,
    
    /** Red text. */
    RED,
    
    /** Green text. */
    GREEN,
    
    /** Brown text. */
    BROWN,
    
    /** Blue text. */
    BLUE,
    
    /** Purple text. */
    PURPLE,
    
    /** Cyan text. */
    CYAN,
    
    /** Light gray text. */
    LIGHT_GRAY,
    
    /** Dark gray text. */
    DARK_GRAY,
    
    /** Light red text. */
    LIGHT_RED,
    
    /** Light green text. */
    LIGHT_GREEN,
    
    /** Yellow text. */
    YELLOW,
    
    /** Light blue text. */
    LIGHT_BLUE,
    
    /** Light purple text. */
    LIGHT_PURPLE,
    
    /** Light cyan text. */
    LIGHT_CYAN,
    
    /** White text. */
    WHITE
};


/** Represents terminal text attributes. */
using ta_t = text_attribute;


}


#endif

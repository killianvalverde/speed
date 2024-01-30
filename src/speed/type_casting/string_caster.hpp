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
 * @file        speed/type_casting/string_caster.hpp
 * @brief       string_caster main header.
 * @author      Killian Valverde
 * @date        2024/01/30
 */

#ifndef SPEED_TYPE_CASTING_STRING_CASTER_HPP
#define SPEED_TYPE_CASTING_STRING_CASTER_HPP


#include "basic_type_caster.hpp"


namespace speed::type_casting {


/**
 * @brief       Classs used as a string casting strategy holder.
 */
template<typename TpTarget, typename TpComponent = i_type_caster<std::string>>
using string_caster = basic_type_caster<TpTarget, std::string, TpComponent>;


}


#endif

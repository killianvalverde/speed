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
 * @file        speed/type_casting/try_cast_basic_string_to_type.hpp
 * @brief       try_cast_basic_string_to_type main header.
 * @author      Killian Valverde
 * @date        2016/08/23
 */

#ifndef SPEED_TYPE_CASTING_TRY_CAST_BASIC_STRING_TO_TYPE_HPP
#define SPEED_TYPE_CASTING_TRY_CAST_BASIC_STRING_TO_TYPE_HPP

#include "error_category.hpp"
#include "try_cast_c_string_to_basic_regex.hpp"
#include "try_cast_c_string_to_basic_string.hpp"
#include "try_cast_c_string_to_floating_point.hpp"
#include "try_cast_c_string_to_integral_signed.hpp"
#include "try_cast_c_string_to_integral_unsigned.hpp"
#include "try_cast_c_string_to_path.hpp"
#include "try_cast_c_string_to_secure_path.hpp"


namespace speed::type_casting {


/** @cond */
namespace __private {


template<typename TpTarget, typename TpChar, typename TpCharTraits, typename TpCharAlloc>
inline bool __try_type_cast(
        const std::basic_string<TpChar, TpCharTraits, TpCharAlloc>& arg, 
        TpTarget* res, 
        std::error_code* err_code
) noexcept
{
    return __try_type_cast(arg.c_str(), res, err_code);
}


} /* __private */
/** @endcond */


}


#endif


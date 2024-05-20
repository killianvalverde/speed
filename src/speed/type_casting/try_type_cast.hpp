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
 * @file        speed/type_casting/type_cast.hpp
 * @brief       type_cast main header.
 * @author      Killian Valverde
 * @date        2016/08/19
 */

#ifndef SPEED_TYPE_CASTING_TRY_TYPE_CAST_HPP
#define SPEED_TYPE_CASTING_TRY_TYPE_CAST_HPP

#include <system_error>

#include "error_category.hpp"
#include "try_cast_basic_string_to_type.hpp"
#include "try_cast_c_string_to_basic_regex.hpp"
#include "try_cast_c_string_to_basic_string.hpp"
#include "try_cast_c_string_to_floating_point.hpp"
#include "try_cast_c_string_to_integral_signed.hpp"
#include "try_cast_c_string_to_integral_unsigned.hpp"
#include "try_cast_c_string_to_path.hpp"
#include "try_cast_c_string_to_secure_path.hpp"


namespace speed::type_casting {


/**
 * @brief       Try to convert the source to the target.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
inline bool try_type_cast(
    const TpSource& arg, 
    TpTarget* res, 
    std::error_code* err_code = nullptr
) noexcept
{
    return __private::__try_type_cast(arg, res, err_code);
}


}


#endif

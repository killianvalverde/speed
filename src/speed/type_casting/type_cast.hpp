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
 * @date        2016/08/19 - 15:29
 */

#ifndef SPEED_TYPE_CASTING_TYPE_CAST_HPP
#define SPEED_TYPE_CASTING_TYPE_CAST_HPP

#include "basic_string_to_type.hpp"
#include "c_string_to_basic_string.hpp"
#include "c_string_to_floating_point.hpp"
#include "c_string_to_integral_signed.hpp"
#include "c_string_to_integral_unsigned.hpp"
#include "c_string_to_path.hpp"
#include "type_casting_exception.hpp"


namespace speed::type_casting {


/**
 * @brief       Try to convert the source to the target.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
bool try_type_cast(const TpSource& arg, TpTarget* res) noexcept
{
    try
    {
        *res = type_cast<TpTarget>(arg);
        return true;
    }
    catch (...)
    {
        return false;
    }
}


/**
 * @brief       Convert the source to the target.
 * @param       arg : The value to convert.
 * @param       default_val : The value returned if the conversion fails.
 * @return      If function was successful the new value is returned, otherwise the default value
 *              is returned.
 */
template<typename TpTarget, typename TpSource, typename TpDefaultValue>
TpTarget type_cast(const TpSource& arg, TpDefaultValue&& default_val) noexcept
{
    try
    {
        return type_cast<TpTarget>(arg);
    }
    catch (...)
    {
        return std::forward<TpDefaultValue>(default_val);
    }
}


}


#endif

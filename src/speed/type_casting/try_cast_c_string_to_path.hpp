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
 * @file        speed/type_casting/try_cast_c_string_to_path.hpp
 * @brief       try_cast_c_string_to_path main header.
 * @author      Killian Valverde
 * @date        2017/03/04
 */

#ifndef SPEED_TYPE_CASTING_TRY_CAST_C_STRING_TO_PATH_HPP
#define SPEED_TYPE_CASTING_TRY_CAST_C_STRING_TO_PATH_HPP

#include <filesystem>
#include <type_traits>

#include "../type_traits/type_traits.hpp"
#include "error_category.hpp"
#include "error_codes.hpp"


namespace speed::type_casting {


/** @cond */
namespace __private {


template<typename TpTarget, typename TpSource>
std::enable_if_t<
        speed::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                speed::type_traits::is_path<TpTarget>::value,
        bool
>
__try_type_cast(const TpSource& arg, TpTarget* res, std::error_code* err_code) noexcept
{
    try
    {
        res->assign(arg);
        return true;
    }
    catch (...)
    {
        assign_type_casting_error_code(
                static_cast<int>(error_codes::FILESYSTEM_INVALID_PATH),
                err_code);

        return false;
    }
}


} /* __private */
/** @endcond */


}


#endif

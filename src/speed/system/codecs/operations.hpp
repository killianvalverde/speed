/* speed - Generic C++ library.
 * Copyright (C) 2015-2025 Killian Valverde.
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
 * @file        operations.hpp
 * @brief       codecs operations functions header.
 * @author      Killian Valverde
 * @date        2024/10/16
 */

#ifndef SPEED_SYSTEM_CODECS_OPERATIONS_HPP
#define SPEED_SYSTEM_CODECS_OPERATIONS_HPP

#include <string>

#include "../platform/platform.hpp"
#include "../detail/detail.hpp"

namespace speed::system::codecs {

/**
 * @brief       Converts a specified c_string into a wstring.
 * @param       cstr : The c_string to convert.
 * @param       wstr : The wstring that will hold the result of the conversion.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpCharTraits, typename TpCharAlloc>
bool convert_cstr_to_wstring(
        const char* cstr,
        std::basic_string<wchar_t, TpCharTraits, TpCharAlloc>& wstr,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(codecs::convert_cstr_to_wstring, false, cstr, wstr, err_code);
}

/**
 * @brief       Converts a specified w_string into a string.
 * @param       wcstr : The w_string to convert.
 * @param       str : The string that will hold the result of the conversion.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpCharTraits, typename TpCharAlloc>
bool convert_wcstr_to_string(
        const wchar_t* wcstr,
        std::basic_string<char, TpCharTraits, TpCharAlloc>& str,
        std::error_code* err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(codecs::convert_wcstr_to_string, false, wcstr, str, err_code);
}

}

#endif

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
 * @file        speed/system/api/winapi/codecs/operations.hpp
 * @brief       codecs operations functions header.
 * @author      Killian Valverde
 * @date        2024/10/16
 */

#ifndef SPEED_SYSTEM_API_WINAPI_CODECS_OPERATIONS_HPP
#define SPEED_SYSTEM_API_WINAPI_CODECS_OPERATIONS_HPP

#include "../../../compatibility/compatibility.hpp"
#ifdef SPEED_WINAPI

#include "../../../errors/errors.hpp"

#include <string>


namespace speed::system::api::winapi::codecs {


/**
 * @brief       Converts a specified c_string into a wstring.
 * @param       c_str : The c_string to convert.
 * @param       wstr : The wstring that will hold the result of the conversion.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpCharTraits, typename TpCharAlloc>
bool convert_c_str_to_wstring(
        const char* c_str,
        std::basic_string<wchar_t, TpCharTraits, TpCharAlloc>* wstr,
        std::error_code* err_code = nullptr
) noexcept
{
    try
    {
        int wstr_sz = ::MultiByteToWideChar(CP_UTF8, 0, c_str, -1, nullptr, 0);

        if (wstr_sz == 0)
        {
            return true;
        }

        wstr->resize(wstr_sz);
        wstr->at(wstr_sz - 1) = L'\0';

        if (::MultiByteToWideChar(CP_UTF8, 0, c_str, -1, &(*wstr)[0], wstr_sz) == 0)
        {
            wstr->clear();
            system::errors::assign_system_error_code((int)GetLastError(), err_code);
            return false;
        }
        wstr->resize(wstr_sz - 1);
        return true;
    }
    catch (const std::bad_alloc& ba)
    {
        system::errors::assign_system_error_code(ERROR_NOT_ENOUGH_MEMORY, err_code);
    }
    catch (...)
    {
        system::errors::assign_system_error_code(ERROR_BAD_ARGUMENTS, err_code);
    }

    return false;
}


/**
 * @brief       Converts a specified w_string into a string.
 * @param       w_str : The w_string to convert.
 * @param       str : The string that will hold the result of the conversion.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpCharTraits, typename TpCharAlloc>
bool convert_w_str_to_string(
        const wchar_t* w_str,
        std::basic_string<char, TpCharTraits, TpCharAlloc>* str,
        std::error_code* err_code = nullptr
) noexcept
{
    try
    {
        int str_sz = ::WideCharToMultiByte(CP_UTF8, 0, w_str, -1, nullptr, 0, nullptr, nullptr);

        if (str_sz == 0)
        {
            return true;
        }

        str->resize(str_sz);
        str->at(str_sz - 1) = '\0';

        if (::WideCharToMultiByte(CP_UTF8, 0, w_str, -1, &(*str)[0], str_sz, nullptr,
                                  nullptr) == 0)
        {
            str->clear();
            system::errors::assign_system_error_code((int)GetLastError(), err_code);
            return false;
        }

        str->resize(str_sz - 1);
        return true;
    }
    catch (const std::bad_alloc& ba)
    {
        system::errors::assign_system_error_code(ERROR_NOT_ENOUGH_MEMORY, err_code);
    }
    catch (...)
    {
        system::errors::assign_system_error_code(ERROR_BAD_ARGUMENTS, err_code);
    }

    return false;
}


}


#endif
#endif

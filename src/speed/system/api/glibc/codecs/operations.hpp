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
 * @file        speed/system/api/glibc/codecs/operations.hpp
 * @brief       codecs operations functions header.
 * @author      Killian Valverde
 * @date        2024/10/16
 */

#ifndef SPEED_SYSTEM_API_GLIBC_CODECS_OPERATIONS_HPP
#define SPEED_SYSTEM_API_GLIBC_CODECS_OPERATIONS_HPP

#include "../../../compatibility/compatibility.hpp"
#ifdef SPEED_GLIBC

#include "../../../errors/errors.hpp"

#include <cstring>
#include <cwchar>
#include <string>

#include <iconv.h>


namespace speed::system::api::glibc::codecs {


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
        constexpr std::size_t wchar_t_sz = sizeof(wchar_t);
        
        const char* encodng = sizeof(wchar_t) == 4 ? "UTF-32LE" : "UTF-16LE";
        iconv_t conv_desc;
        std::size_t c_str_len;
        std::size_t wstr_sz;
        std::size_t in_bytes_left;
        std::size_t out_bytes_left;
        char* in_buf;
        char* out_buf;

        conv_desc = iconv_open(encodng, "UTF-8");
        if (conv_desc == (iconv_t)-1)
        {
            system::errors::assign_system_error_code(EINVAL, err_code);
            return false;
        }

        c_str_len = strlen(c_str);
        wstr_sz = (c_str_len + 1) * wchar_t_sz;
        wstr->resize(wstr_sz);

        in_buf = (char*)c_str;
        out_buf = (char*)&(*wstr)[0];

        in_bytes_left = c_str_len;
        out_bytes_left = wstr_sz;

        if (iconv(conv_desc, &in_buf, &in_bytes_left, &out_buf, &out_bytes_left) == (size_t)-1)
        {
            iconv_close(conv_desc);
            system::errors::assign_system_error_code(errno, err_code);
            return false;
        }

        wstr->resize((wstr_sz - out_bytes_left) / wchar_t_sz);

        iconv_close(conv_desc);
        return true;
    }
    catch (const std::bad_alloc& ba)
    {
        system::errors::assign_system_error_code(ENOMEM, err_code);
    }
    catch (...)
    {
        system::errors::assign_system_error_code(EINVAL, err_code);
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
        constexpr std::size_t wchar_t_sz = sizeof(wchar_t);
        
        const char* encodng = sizeof(wchar_t) == 4 ? "UTF-32LE" : "UTF-16LE";
        iconv_t conv_desc;
        std::size_t w_str_len;
        std::size_t str_sz;
        std::size_t in_bytes_left;
        std::size_t out_bytes_left;
        char* in_buf;
        char* out_buf;

        conv_desc = iconv_open("UTF-8", encodng);
        if (conv_desc == (iconv_t)-1)
        {
            system::errors::assign_system_error_code(EINVAL, err_code);
            return false;
        }

        w_str_len = wcslen(w_str);
        str_sz = w_str_len * 4 + 1;
        str->resize(str_sz);

        in_buf = (char*)w_str;
        out_buf = (char*)&(*str)[0];

        in_bytes_left = w_str_len * wchar_t_sz;
        out_bytes_left = str_sz;

        if (iconv(conv_desc, &in_buf, &in_bytes_left, &out_buf, &out_bytes_left) == (size_t)-1)
        {
            iconv_close(conv_desc);
            system::errors::assign_system_error_code(errno, err_code);
            return false;
        }

        str->resize(str_sz - out_bytes_left);

        iconv_close(conv_desc);
        return true;
    }
    catch (const std::bad_alloc& ba)
    {
        system::errors::assign_system_error_code(ENOMEM, err_code);
    }
    catch (...)
    {
        system::errors::assign_system_error_code(EINVAL, err_code);
    }

    return false;
}


}


#endif
#endif

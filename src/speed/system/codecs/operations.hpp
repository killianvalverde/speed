/* speed - Generic C++ library.
 * Copyright (C) 2015-2026 Killian Valverde.
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
 * @file operations.hpp
 * @brief Core operations for the system::codecs submodule.
 * @author Killian Valverde
 * @date 2024-10-16
 */

#pragma once

#include "../platform/platform.hpp"

#include <cwchar>
#include <string>
#include <system_error>

#ifdef SPEED_POSIX
#include <cstring>
#include <iconv.h>
#endif

#include "../errors/errors.hpp"

namespace speed::system::codecs {

/**
 * @brief Converts a UTF-8 encoded null-terminated string to a wide string.
 *
 * On Windows, the resulting wide string typically contains UTF-16 code units.
 * On POSIX systems, it typically contains UTF-32 code units, depending on the native
 * representation of wchar_t.
 *
 * @tparam TraitsT Character traits type used by the destination string.
 * @tparam AllocatorT Allocator type used by the destination string.
 *
 * @param source Pointer to a UTF-8 encoded null-terminated string.
 * @param destination Destination wide string receiving the converted text.
 * @param err_code Optional error code receiving error information on failure.
 * @return true on success, false otherwise.
 */
template<typename TraitsT, typename AllocatorT>
bool convert_utf8_to_wstring(
    const char* source,
    std::basic_string<wchar_t, TraitsT, AllocatorT>& destination,
    std::error_code* err_code = nullptr
) noexcept
{
    if (source == nullptr)
    {
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }
    if (*source == '\0')
    {
        destination.clear();
        return true;
    }

#ifdef SPEED_WINAPI
    try
    {
        int destination_length = MultiByteToWideChar(CP_UTF8, 0, source, -1, nullptr, 0);
        if (destination_length == 0)
        {
            errors::assign_system_error_code(GetLastError(), err_code);
            return false;
        }

        destination.resize(destination_length);

        if (MultiByteToWideChar(CP_UTF8, 0, source, -1, destination.data(), destination_length) ==
            0)
        {
            destination.clear();
            errors::assign_system_error_code(GetLastError(), err_code);
            return false;
        }

        destination.resize(destination_length - 1);
        return true;
    }
    catch (const std::bad_alloc&)
    {
        errors::assign_system_error_code(ERROR_NOT_ENOUGH_MEMORY, err_code);
    }
    catch (...)
    {
        errors::assign_system_error_code(ERROR_BAD_ARGUMENTS, err_code);
    }

    return false;
#else
    constexpr std::size_t wchar_t_sz = sizeof(wchar_t);
    constexpr const char* encoding = sizeof(wchar_t) == 4 ? "UTF-32LE" : "UTF-16LE";

    iconv_t conv_desc = (iconv_t)-1;
    std::size_t source_length;
    std::size_t destination_capacity_bytes;
    std::size_t in_bytes_left;
    std::size_t out_bytes_left;
    char* in_buf;
    char* out_buf;

    try
    {
        conv_desc = iconv_open(encoding, "UTF-8");
        if (conv_desc == (iconv_t)-1)
        {
            errors::assign_errno_error_code(errno, err_code);
            return false;
        }

        source_length = strlen(source);
        destination_capacity_bytes = source_length * wchar_t_sz;
        destination.resize(source_length);

        in_buf = const_cast<char*>(source);
        out_buf = reinterpret_cast<char*>(destination.data());

        in_bytes_left = source_length;
        out_bytes_left = destination_capacity_bytes;

        if (iconv(conv_desc, &in_buf, &in_bytes_left, &out_buf, &out_bytes_left) ==
            static_cast<std::size_t>(-1))
        {
            iconv_close(conv_desc);
            errors::assign_errno_error_code(errno, err_code);
            return false;
        }

        destination.resize((destination_capacity_bytes - out_bytes_left) / wchar_t_sz);

        iconv_close(conv_desc);
        return true;
    }
    catch (const std::bad_alloc&)
    {
        errors::assign_errno_error_code(ENOMEM, err_code);
    }
    catch (...)
    {
        errors::assign_errno_error_code(EINVAL, err_code);
    }

    if (conv_desc != (iconv_t)-1)
    {
        iconv_close(conv_desc);
    }

    return false;
#endif
}

/**
 * @brief Converts a wide string to a UTF-8 encoded string.
 *
 * Converts the null-terminated wide string pointed to by source and stores the resulting
 * UTF-8 encoded text in destination.
 *
 * On Windows, the source string is typically interpreted as UTF-16.
 * On POSIX systems, it is typically interpreted as UTF-32, depending on the native
 * representation of wchar_t.
 *
 * @tparam TraitsT Character traits type used by the destination string.
 * @tparam AllocatorT Allocator type used by the destination string.
 *
 * @param source Pointer to a null-terminated wide string.
 * @param destination Destination string receiving the UTF-8 encoded text.
 * @param err_code Optional error code receiving error information on failure.
 * @return true on success, false otherwise.
 */
template<typename TraitsT, typename AllocatorT>
bool convert_wstring_to_utf8(
    const wchar_t* source,
    std::basic_string<char, TraitsT, AllocatorT>& destination,
    std::error_code* err_code = nullptr
) noexcept
{
    if (source == nullptr)
    {
        errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }
    if (*source == L'\0')
    {
        destination.clear();
        return true;
    }

#ifdef SPEED_WINAPI
    try
    {
        int destination_length = WideCharToMultiByte(CP_UTF8, 0, source, -1, nullptr, 0, nullptr,
            nullptr);
        if (destination_length == 0)
        {
            errors::assign_system_error_code(GetLastError(), err_code);
            return false;
        }

        destination.resize(destination_length);

        if (WideCharToMultiByte(CP_UTF8, 0, source, -1, destination.data(), destination_length,
            nullptr, nullptr) == 0)
        {
            destination.clear();
            errors::assign_system_error_code(GetLastError(), err_code);
            return false;
        }

        destination.resize(destination_length - 1);
        return true;
    }
    catch (const std::bad_alloc&)
    {
        errors::assign_system_error_code(ERROR_NOT_ENOUGH_MEMORY, err_code);
    }
    catch (...)
    {
        errors::assign_system_error_code(ERROR_BAD_ARGUMENTS, err_code);
    }

    return false;
#else
    constexpr const char* encoding = sizeof(wchar_t) == 4 ? "UTF-32LE" : "UTF-16LE";
    constexpr std::size_t wchar_t_sz = sizeof(wchar_t);
    constexpr std::size_t max_utf8_bytes_per_codepoint = 4;

    iconv_t conv_desc = (iconv_t)-1;
    std::size_t source_length;
    std::size_t destination_capacity_bytes;
    std::size_t in_bytes_left;
    std::size_t out_bytes_left;
    char* in_buf;
    char* out_buf;

    try
    {
        conv_desc = iconv_open("UTF-8", encoding);
        if (conv_desc == (iconv_t)-1)
        {
            errors::assign_errno_error_code(errno, err_code);
            return false;
        }

        source_length = wcslen(source);
        destination_capacity_bytes = source_length * max_utf8_bytes_per_codepoint;
        destination.resize(destination_capacity_bytes);

        in_buf = reinterpret_cast<char*>(const_cast<wchar_t*>(source));
        out_buf = reinterpret_cast<char*>(destination.data());

        in_bytes_left = source_length * wchar_t_sz;
        out_bytes_left = destination_capacity_bytes;

        if (iconv(conv_desc, &in_buf, &in_bytes_left, &out_buf, &out_bytes_left) ==
            static_cast<std::size_t>(-1))
        {
            iconv_close(conv_desc);
            errors::assign_errno_error_code(errno, err_code);
            return false;
        }

        destination.resize(destination_capacity_bytes - out_bytes_left);

        iconv_close(conv_desc);
        return true;
    }
    catch (const std::bad_alloc&)
    {
        errors::assign_errno_error_code(ENOMEM, err_code);
    }
    catch (...)
    {
        errors::assign_errno_error_code(EINVAL, err_code);
    }

    if (conv_desc != (iconv_t)-1)
    {
        iconv_close(conv_desc);
    }

    return false;
#endif
}

}

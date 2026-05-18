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
 * @brief Internal helper operations for the type_casting module.
 * @author Killian Valverde
 * @date 2016-08-19
 */

#pragma once

#include <charconv>
#include <chrono>
#include <concepts>
#include <limits>
#include <regex>
#include <system_error>
#include <type_traits>

#include "forward_declarations.hpp"
#include "../../filesystem/filesystem.hpp"
#include "../../system/system.hpp"
#include "../../type_traits/type_traits.hpp"

/** @cond */
namespace speed::type_casting::detail {

// C-string --> unsigned_integral
template<std::unsigned_integral TargetT>
bool try_type_cast(const char* arg, TargetT& res, std::error_code* err_code) noexcept
{
    const char* str = arg;
    const char* end;
    int base = 10;
    TargetT res_buildr{};

    if (str == nullptr)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    if (*str == '+')
    {
        ++str;
    }

    if (*str == '\0')
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    if (str[0] == '0')
    {
        if (str[1] == 'x' || str[1] == 'X')
        {
            base = 16;
            str += 2;
        }
        else if (str[1] == 'b' || str[1] == 'B')
        {
            base = 2;
            str += 2;
        }
        else if (str[1] == 'o' || str[1] == 'O')
        {
            base = 8;
            str += 2;
        }
    }

    for (end = str; *end != '\0'; ++end)
    {
    }

    auto [ptr, ec] = std::from_chars(str, end, res_buildr, base);

    if (ec != std::errc{})
    {
        system::errors::assign_errc(ec, err_code);
        return false;
    }

    if (ptr != end)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    res = res_buildr;
    return true;
}

// C-string --> signed_integral
template<std::signed_integral TargetT>
bool try_type_cast(const char* arg, TargetT& res, std::error_code* err_code) noexcept
{
    using unsigned_type = std::make_unsigned_t<TargetT>;

    const char* str = arg;
    const char* end;
    int base = 10;
    bool negative = false;
    unsigned_type res_builder{};

    if (str == nullptr)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    if (*str == '+' || *str == '-')
    {
        negative = *str == '-';
        ++str;
    }

    if (*str == '\0')
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    if (str[0] == '0')
    {
        if (str[1] == 'x' || str[1] == 'X')
        {
            base = 16;
            str += 2;
        }
        else if (str[1] == 'b' || str[1] == 'B')
        {
            base = 2;
            str += 2;
        }
        else if (str[1] == 'o' || str[1] == 'O')
        {
            base = 8;
            str += 2;
        }
    }

    for (end = str; *end != '\0'; ++end)
    {
    }

    auto [ptr, ec] = std::from_chars(str, end, res_builder, base);

    if (ec != std::errc{})
    {
        system::errors::assign_errc(ec, err_code);
        return false;
    }

    if (ptr != end)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    if (negative)
    {
        constexpr unsigned_type max_plus_one =
            static_cast<unsigned_type>(std::numeric_limits<TargetT>::max()) + 1;

        if (res_builder > max_plus_one)
        {
            system::errors::assign_errc(std::errc::result_out_of_range, err_code);
            return false;
        }

        res = static_cast<TargetT>(-res_builder);
    }
    else
    {
        if (res_builder > static_cast<unsigned_type>(std::numeric_limits<TargetT>::max()))
        {
            system::errors::assign_errc(std::errc::result_out_of_range, err_code);
            return false;
        }

        res = static_cast<TargetT>(res_builder);
    }

    return true;
}

// C-string --> floating_point
template<std::floating_point TargetT>
bool try_type_cast(const char* arg, TargetT& res, std::error_code* err_code) noexcept
{
    const char* str = arg;
    const char* end;

    if (str == nullptr)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    if (*str == '\0')
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    if (*str == '+')
    {
        ++str;
    }

    for (end = str; *end != '\0'; ++end)
    {
    }

    if constexpr (std::same_as<TargetT, long double>)
    {
        errno = 0;
        char* ptr = nullptr;

        long double value = std::strtold(str, &ptr);

        if (ptr == str || ptr != end)
        {
            system::errors::assign_errc(std::errc::invalid_argument, err_code);
            return false;
        }

        if (errno == ERANGE)
        {
            system::errors::assign_errc(std::errc::result_out_of_range, err_code);
            return false;
        }

        res = value;
        return true;
    }
    else
    {
        TargetT value{};
        auto [ptr, ec] = std::from_chars(str, end, value);

        if (ec != std::errc{})
        {
            system::errors::assign_errc(ec, err_code);
            return false;
        }

        if (ptr != end)
        {
            system::errors::assign_errc(std::errc::invalid_argument, err_code);
            return false;
        }

        res = value;
        return true;
    }
}

// Non-narrow C-string --> numeric_value
template<typename TargetT, typename CharT>
    requires (type_traits::is_character_v<CharT> &&
        !std::same_as<CharT, char> &&
        (std::integral<TargetT> || std::floating_point<TargetT>))
bool try_type_cast(
    const CharT* arg,
    TargetT& res,
    std::error_code* err_code
) noexcept
{
    using string_type = std::basic_string<char>;

    if (arg == nullptr)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    string_type tmp;

    if (!try_type_cast(arg, tmp, err_code))
    {
        return false;
    }

    return try_type_cast(tmp.c_str(), res, err_code);
}

// Generic C-string --> std::chrono::duration
template<typename RepresentationT, typename PeriodT, typename CharT>
    requires std::is_integral_v<RepresentationT>
bool try_type_cast(
    const CharT* arg,
    std::chrono::duration<RepresentationT, PeriodT>& res,
    std::error_code* err_code
) noexcept
{
    RepresentationT rep;

    if (!try_type_cast(arg, rep, err_code))
    {
        return false;
    }

    res = std::chrono::duration<RepresentationT, PeriodT>(rep);
    return true;
}

// Generic C-string --> std::basic_string
template<typename CharT, typename CharTraitsT, typename AllocatorT>
bool try_type_cast(
    const CharT* arg,
    std::basic_string<CharT, CharTraitsT, AllocatorT>& res,
    std::error_code* err_code
) noexcept
{
    if (arg == nullptr)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    try
    {
        res.assign(arg);
        return true;
    }
    catch (const std::bad_alloc&)
    {
        system::errors::assign_errc(std::errc::not_enough_memory, err_code);
    }
    catch (...)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
    }

    return false;
}

// C-string --> std::wstring
template<typename CharTraitsT, typename AllocatorT>
bool try_type_cast(
    const char* arg,
    std::basic_string<wchar_t, CharTraitsT, AllocatorT>& res,
    std::error_code* err_code
) noexcept
{
    if (arg == nullptr)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    if (!system::codecs::convert_cstr_to_wstring(arg, res, err_code))
    {
        return false;
    }

    return true;
}

// Wide C-string --> std::string
template<typename CharTraitsT, typename AllocatorT>
bool try_type_cast(
    const wchar_t* arg,
    std::basic_string<char, CharTraitsT, AllocatorT>& res,
    std::error_code* err_code
) noexcept
{
    if (arg == nullptr)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    if (!system::codecs::convert_wcstr_to_string(arg, res, err_code))
    {
        return false;
    }

    return true;
}

// Generic C-string --> std::basic_regex (compatible)
template<typename CharT, typename RegexTratisT>
bool try_type_cast(
    const CharT* arg,
    std::basic_regex<CharT, RegexTratisT>& res,
    std::error_code* err_code
) noexcept
{
    if (arg == nullptr)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    try
    {
        res.assign(arg);
        return true;
    }
    catch (const std::regex_error&)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
    }
    catch (const std::bad_alloc&)
    {
        system::errors::assign_errc(std::errc::not_enough_memory, err_code);
    }
    catch (...)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
    }

    return false;
}

// Generic C-string --> std::basic_regex (!compatible)
template<typename CharT, typename RegexCharT, typename RegexTraitsT>
    requires (!std::same_as<CharT, RegexCharT>)
bool try_type_cast(
    const CharT* arg,
    std::basic_regex<RegexCharT, RegexTraitsT>& res,
    std::error_code* err_code
) noexcept
{
    using string_type = std::basic_string<RegexCharT>;

    if (arg == nullptr)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    string_type tmp;

    if (!try_type_cast(arg, tmp, err_code))
    {
        return false;
    }

    try
    {
        res.assign(tmp);
        return true;
    }
    catch (const std::regex_error&)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
    }
    catch (const std::bad_alloc&)
    {
        system::errors::assign_errc(std::errc::not_enough_memory, err_code);
    }
    catch (...)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
    }

    return false;
}

// Generic C-string --> std::filesystem::path (compatible)
template<typename CharT>
    requires std::same_as<CharT, std::filesystem::path::value_type>
bool try_type_cast(
    const CharT* arg,
    std::filesystem::path& res,
    std::error_code* err_code
) noexcept
{
    if (arg == nullptr)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    try
    {
        res.assign(arg);
        return true;
    }
    catch (const std::bad_alloc&)
    {
        system::errors::assign_errc(std::errc::not_enough_memory, err_code);
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        system::errors::assign_error_code(e.code(), err_code);
    }
    catch (...)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
    }

    return false;
}

// Generic C-string --> std::filesystem::path (!compatible)
template<typename CharT>
    requires (!std::same_as<CharT, std::filesystem::path::value_type>)
bool try_type_cast(
    const CharT* arg,
    std::filesystem::path& res,
    std::error_code* err_code
) noexcept
{
    using path_char_type = std::filesystem::path::value_type;
    using string_type = std::basic_string<path_char_type>;

    if (arg == nullptr)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    string_type tmp;

    if (!try_type_cast(arg, tmp, err_code))
    {
        return false;
    }

    try
    {
        res.assign(tmp);
        return true;
    }
    catch (const std::bad_alloc&)
    {
        system::errors::assign_errc(std::errc::not_enough_memory, err_code);
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        system::errors::assign_error_code(e.code(), err_code);
    }
    catch (...)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
    }

    return false;
}

// Generic C-string --> valid_path
template<typename TargetT, typename CharT>
    requires type_traits::is_character_v<CharT> &&
        std::derived_from<TargetT, filesystem::valid_path>
bool try_type_cast(
    const CharT* arg,
    TargetT& res,
    std::error_code* err_code
) noexcept
{
    if (arg == nullptr)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
        return false;
    }

    if (!try_type_cast(arg, static_cast<std::filesystem::path&>(res), err_code))
    {
        return false;
    }

    return res.is_valid(err_code);
}

// std::basic_string_view --> TargetT
template<typename TargetT, typename CharT, typename CharTraitsT>
    requires type_traits::is_character_v<CharT>
bool try_type_cast(
    std::basic_string_view<CharT, CharTraitsT> arg,
    TargetT& res,
    std::error_code* err_code
) noexcept
{
    if constexpr (std::same_as<CharTraitsT, std::char_traits<CharT>>)
    {
        if (!arg.empty() && arg.data()[arg.size()] == CharT('\0'))
        {
            return try_type_cast(arg.data(), res, err_code);
        }
    }

    try
    {
        std::basic_string<CharT> tmp(arg);
        return try_type_cast(tmp.c_str(), res, err_code);
    }
    catch (const std::bad_alloc&)
    {
        system::errors::assign_errc(std::errc::not_enough_memory, err_code);
    }
    catch (...)
    {
        system::errors::assign_errc(std::errc::invalid_argument, err_code);
    }

    return false;
}

// std::filesystem::path --> TargetT
template<typename TargetT>
bool try_type_cast(
    const std::filesystem::path& arg,
    TargetT& res,
    std::error_code* err_code
) noexcept
{
    return try_type_cast(arg.c_str(), res, err_code);
}

}
/** @endcond */

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
 * @file        type_cast.hpp
 * @brief       type_cast main header.
 * @author      Killian Valverde
 * @date        2016/08/19
 */

#ifndef SPEED_TYPE_CASTING_TRY_TYPE_CAST_HPP
#define SPEED_TYPE_CASTING_TRY_TYPE_CAST_HPP

#include <cctype>
#include <cmath>
#include <limits>
#include <system_error>
#include <type_traits>

#include "detail/forward_declarations.hpp"
#include "../filesystem/filesystem.hpp"
#include "../stringutils/stringutils.hpp"
#include "../system/system.hpp"
#include "../type_traits/type_traits.hpp"
#include "error_category.hpp"

namespace speed::type_casting {

/** @cond */
namespace detail {

/**
 * @brief       Tries to convert a c_string into an unsigned inegral.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TargetT, typename CharT>
std::enable_if_t<
        type_traits::is_character_v<CharT> &&
                std::is_unsigned_v<TargetT>,
        bool
>
try_type_cast(const CharT* arg, TargetT& res, std::error_code* err_code) noexcept
{
    using unsigned_type = TargetT;
    using char_type = std::remove_const_t<std::remove_pointer_t<std::decay_t<CharT>>>;
    using string_pointer_type = const char_type*;

    constexpr unsigned_type max_unsigned = std::numeric_limits<unsigned_type>::max();

    string_pointer_type str = arg;
    unsigned_type res_bldr = 0;
    char_type sgn;
    char_type digt;

    if (str == nullptr)
    {
        detail::assign_error_code(error_code_enum::ARITHMETIC_CONVERSION_FAILS, err_code);
        return false;
    }
    
    while (isspace(*str))
    {
        ++str;
    }
    
    sgn = *str;
    if (sgn == '+')
    {
        ++str;
    }

    if (*str == '\0')
    {
        detail::assign_error_code(error_code_enum::ARITHMETIC_INVALID_SYNTAX, err_code);
        return false;
    }

    while ((digt = *str) != '\0')
    {
        if (!isdigit(digt))
        {
            detail::assign_error_code(error_code_enum::ARITHMETIC_INVALID_SYNTAX, err_code);
            return false;
        }

        digt -= '0';

        if ((res_bldr > max_unsigned / 10) ||
            (res_bldr == max_unsigned / 10 && digt > (max_unsigned % 10)))
        {
            detail::assign_error_code(error_code_enum::ARITHMETIC_OVERFLOW_RANGE, err_code);
            return false;
        }

        res_bldr = res_bldr * 10 + digt;
        ++str;
    }

    res = res_bldr;
    return true;
}

/**
 * @brief       Tries to convert a c_string into a signed inegral.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TargetT, typename CharT>
std::enable_if_t<
        type_traits::is_character_v<CharT> &&
                std::is_signed_v<TargetT>,
        bool
>
try_type_cast(const CharT* arg, TargetT& res, std::error_code* err_code) noexcept
{
    using signed_type = TargetT;
    using char_type = std::remove_const_t<std::remove_pointer_t<std::decay_t<CharT>>>;
    using string_pointer_type = const char_type*;

    constexpr signed_type min_signed = std::numeric_limits<signed_type>::min();
    constexpr signed_type max_signed = std::numeric_limits<signed_type>::max();

    string_pointer_type str = arg;
    signed_type res_bldr = 0;
    char_type sgn;
    char_type digt;

    if (str == nullptr)
    {
        detail::assign_error_code(error_code_enum::ARITHMETIC_CONVERSION_FAILS, err_code);
        return false;
    }

    while (isspace(*str))
    {
        ++str;
    }

    sgn = *str;
    if (sgn == '-' || sgn == '+')
    {
        ++str;
    }

    if (*str == '\0')
    {
        detail::assign_error_code(error_code_enum::ARITHMETIC_INVALID_SYNTAX, err_code);
        return false;
    }

    while ((digt = *str) != '\0')
    {
        if (!isdigit(digt))
        {
            detail::assign_error_code(error_code_enum::ARITHMETIC_INVALID_SYNTAX, err_code);
            return false;
        }

        digt -= '0';

        if ((res_bldr < min_signed / 10) ||
            (res_bldr == min_signed / 10 && digt > -(min_signed % 10)))
        {
            detail::assign_error_code(error_code_enum::ARITHMETIC_OVERFLOW_RANGE, err_code);
            return false;
        }

        res_bldr = res_bldr * 10 - digt;
        ++str;
    }

    if (sgn != '-')
    {
        if (res_bldr < -max_signed)
        {
            detail::assign_error_code(error_code_enum::ARITHMETIC_OVERFLOW_RANGE, err_code);
            return false;
        }

        res_bldr *= -1;
    }

    res = res_bldr;
    return true;
}

/**
 * @brief       Tries to convert a c_string into a float.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename CharT>
std::enable_if_t<
        type_traits::is_character_v<CharT>,
        bool
>
try_type_cast(const CharT* arg, float& res, std::error_code* err_code) noexcept
{
    errno = 0;
    char* endptr;
    float res_bldr;

    res_bldr = ::strtof(arg, &endptr);

    if (endptr == arg || *endptr != '\0')
    {
        detail::assign_error_code(error_code_enum::ARITHMETIC_CONVERSION_FAILS, err_code);
        return false;
    }

    if (res_bldr == HUGE_VALF && errno == ERANGE)
    {
        detail::assign_error_code(error_code_enum::ARITHMETIC_OVERFLOW_RANGE, err_code);
        return false;
    }

    if (errno == ERANGE)
    {
        detail::assign_error_code(error_code_enum::ARITHMETIC_UNDERFLOW_RANGE, err_code);
        return false;
    }

    res = res_bldr;
    return true;
}

/**
 * @brief       Tries to convert a c_string into a double.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename CharT>
std::enable_if_t<
        type_traits::is_character_v<CharT>,
        bool
>
try_type_cast(const CharT* arg, double& res, std::error_code* err_code) noexcept
{
    errno = 0;
    char* endptr;
    double res_bldr;

    res_bldr = ::strtod(arg, &endptr);

    if (endptr == arg || *endptr != '\0')
    {
        detail::assign_error_code(error_code_enum::ARITHMETIC_CONVERSION_FAILS, err_code);
        return false;
    }

    if (res_bldr == HUGE_VAL && errno == ERANGE)
    {
        detail::assign_error_code(error_code_enum::ARITHMETIC_OVERFLOW_RANGE, err_code);
        return false;
    }

    if (errno == ERANGE)
    {
        detail::assign_error_code(error_code_enum::ARITHMETIC_UNDERFLOW_RANGE, err_code);
        return false;
    }

    res = res_bldr;
    return true;
}

/**
 * @brief       Tries to convert a c_string into a long double.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename CharT>
std::enable_if_t<
        type_traits::is_character_v<CharT>,
        bool
>
try_type_cast(const CharT* arg, long double& res, std::error_code* err_code) noexcept
{
    errno = 0;
    char* endptr;
    long double res_bldr;

    res_bldr = ::strtold(arg, &endptr);

    if (endptr == arg || *endptr != '\0')
    {
        detail::assign_error_code(error_code_enum::ARITHMETIC_CONVERSION_FAILS, err_code);
        return false;
    }

    if (res_bldr == HUGE_VALL && errno == ERANGE)
    {
        detail::assign_error_code(error_code_enum::ARITHMETIC_OVERFLOW_RANGE, err_code);
        return false;
    }

    if (errno == ERANGE)
    {
        detail::assign_error_code(error_code_enum::ARITHMETIC_UNDERFLOW_RANGE, err_code);
        return false;
    }

    res = res_bldr;
    return true;
}

/**
 * @brief       Tries to convert a cstr into a basic_string.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename CharT, typename CharTraitsT, typename AllocatorT>
bool try_type_cast(
        const CharT* arg,
        std::basic_string<CharT, CharTraitsT, AllocatorT>& res,
        std::error_code* err_code
) noexcept
{
    try
    {
        res.assign(arg);
        return true;
    }
    catch (...)
    {
        detail::assign_error_code(error_code_enum::OTHER, err_code);
    }

    return false;
}

/**
 * @brief       Tries to convert a c_string into a wstring.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename CharTraitsT, typename AllocatorT>
bool try_type_cast(
        const char* arg,
        std::basic_string<wchar_t, CharTraitsT, AllocatorT>& res,
        std::error_code* err_code
) noexcept
{
    if (!system::codecs::convert_cstr_to_wstring(arg, res))
    {
        detail::assign_error_code(error_code_enum::RANGE_ERROR, err_code);
        return false;
    }

    return true;
}

/**
 * @brief       Tries to convert a w_string into a string.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename CharTraitsT, typename AllocatorT>
bool try_type_cast(
        const wchar_t* arg,
        std::basic_string<char, CharTraitsT, AllocatorT>& res,
        std::error_code* err_code
) noexcept
{
    if (!system::codecs::convert_wcstr_to_string(arg, res))
    {
        detail::assign_error_code(error_code_enum::RANGE_ERROR, err_code);
        return false;
    }

    return true;
}

/**
 * @brief       Tries to convert a c_string into a basic_regex when types are compatible.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename CharT, typename RegexTratisT>
std::enable_if_t<
        type_traits::is_character_v<CharT>,
        bool
>
try_type_cast(
        const CharT* arg,
        std::basic_regex<CharT, RegexTratisT>& res,
        std::error_code* err_code
) noexcept
{
    try
    {
        res.assign(arg);
        return true;
    }
    catch (...)
    {
        detail::assign_error_code(error_code_enum::OTHER, err_code);
    }

    return false;
}

/**
 * @brief       Tries to convert a c_string into a basic_regex when types are not compatible.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename RegexCharT, typename RegexTratisT, typename CharT>
std::enable_if_t<
        type_traits::is_character_v<CharT> &&
                !std::is_same_v<CharT, RegexCharT>,
        bool
>
try_type_cast(
        const CharT* arg,
        std::basic_regex<RegexCharT, RegexTratisT>& res,
        std::error_code* err_code
) noexcept
{
    try
    {
        using string_type = std::basic_string<RegexCharT>;
        string_type string_helpr;
        
        if (!try_type_cast(arg, string_helpr, err_code))
        {
            return false;
        }
        
        res.assign(std::move(string_helpr));
        return true;
    }
    catch (...)
    {
        detail::assign_error_code(error_code_enum::OTHER, err_code);
    }

    return false;
}

/**
 * @brief       Tries to convert a c_string into a path when types are not compatible.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename CharT>
std::enable_if_t<
        type_traits::is_character_v<CharT> &&
                !std::is_same_v<CharT, std::filesystem::path::value_type>,
        bool
>
try_type_cast(const CharT* arg, std::filesystem::path& res, std::error_code* err_code) noexcept
{
    try
    {
        using string_type = std::basic_string<std::filesystem::path::value_type>;
        string_type string_helpr;
        
        if (!try_type_cast(arg, string_helpr, err_code))
        {
            return false;
        }
        
        res.assign(std::move(string_helpr));
        return true;
    }
    catch (...)
    {
        detail::assign_error_code(error_code_enum::FILESYSTEM_INVALID_PATH, err_code);
        return false;
    }
}

/**
 * @brief       Tries to convert a c_string into a path when types are compatible.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename CharT>
std::enable_if_t<
        type_traits::is_character_v<CharT> &&
                std::is_same_v<CharT, std::filesystem::path::value_type>,
        bool
>
try_type_cast(const CharT* arg, std::filesystem::path& res, std::error_code* err_code) noexcept
{
    try
    {
        res.assign(arg);
        return true;
    }
    catch (...)
    {
        detail::assign_error_code(error_code_enum::FILESYSTEM_INVALID_PATH, err_code);
        return false;
    }
}

/**
 * @brief       Tries to convert a c_string into a valid_path.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TargetT, typename CharT>
std::enable_if_t<
        type_traits::is_character_v<CharT> &&
                std::is_base_of_v<filesystem::valid_path, TargetT>,
        bool
>
try_type_cast(const CharT* arg, TargetT& res, std::error_code* err_code) noexcept
{
    try
    {
        if (!try_type_cast(arg, (dynamic_cast<std::filesystem::path&>(res)), err_code))
        {
            return false;
        }
        
        return res.is_valid(err_code);
    }
    catch (...)
    {
        detail::assign_error_code(error_code_enum::FILESYSTEM_INVALID_PATH, err_code);
        return false;
    }
}

/**
 * @brief       Tries to convert a basic_string into a type.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TargetT, typename CharT, typename CharTraitsT, typename AllocatorT>
inline bool try_type_cast(
        const std::basic_string<CharT, CharTraitsT, AllocatorT>& arg,
        TargetT& res,
        std::error_code* err_code
) noexcept
{
    return try_type_cast(arg.c_str(), res, err_code);
}

/**
 * @brief       Tries to convert a basic_string into a type.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TargetT, typename CharT, typename CharTraitsT>
inline bool try_type_cast(
        const std::basic_string_view<CharT, CharTraitsT>& arg,
        TargetT& res,
        std::error_code* err_code
) noexcept
{
    return try_type_cast(arg.data(), res, err_code);
}

/**
 * @brief       Tries to convert a path into a type.
 * @param       arg : The value to convert.
 * @param       res : The result of the operation if it was successful.
 * @param       err_code : If function fails it holds the error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TargetT>
inline bool try_type_cast(
        const std::filesystem::path& arg,
        TargetT& res,
        std::error_code* err_code
) noexcept
{
    return try_type_cast(arg.c_str(), res, err_code);
}

}
/** @endcond */

/**
 * @brief       Attempts to cast a value from one type to another.
 * @param       arg : The input value to be cast.
 * @param       res : Reference to where the converted value will be stored if successful.
 * @param       err_code : Optional pointer to an error code object to store failure details.
 * @return      `true` if the cast succeeds, otherwise `false`.
 */
template<typename TargetT, typename SourceT>
inline bool try_type_cast(
    const SourceT& arg, 
    TargetT& res,
    std::error_code* err_code
) noexcept
{
    return detail::try_type_cast(arg, res, err_code);
}

}

#endif

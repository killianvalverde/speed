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
 * @file        operations.hpp
 * @brief       stringutils operations functions header.
 * @author      Killian Valverde
 * @date        2016/01/08
 */

#ifndef SPEED_STRINGUTILS_OPERATIONS_HPP
#define SPEED_STRINGUTILS_OPERATIONS_HPP

#include <algorithm>
#include <array>
#include <cwctype>
#include <string>
#include <type_traits>
#include <vector>

#include "forward_declarations.hpp"
#include "../type_traits/type_traits.hpp"
#include "concepts.hpp"

namespace speed::stringutils {

/**
 * @brief       Copies a null-terminated C-style string from `src` to `dest`, possibly with
 *              different character types.
 * @param       dest : Pointer to the destination buffer where the content is to be copied.
 *              Must be large enough to hold the contents of `src`, including the null terminator.
 * @param       src : Pointer to the null-terminated source string to copy from.
 * @return      Pointer to the beginning of the destination buffer (`dest`).
 */
template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar* cstr_copy(TpDestinationChar* dest, const TpSourceChar* src) noexcept
{
    if (dest == nullptr || src == nullptr)
    {
        return dest;
    }

    TpDestinationChar* orig_dest = dest;

    while (*src != '\0')
    {
        *dest++ = *src++;
    }

    *dest = '\0';
    return orig_dest;
}

/**
 * @brief       Copies up to `cnt` characters from a null-terminated C-style string `src` to `dest`,
 *              ensuring null-termination and preventing buffer overflows.
 * @param       dest : Pointer to the destination buffer where the content is to be copied. Must be
 *              large enough to hold up to `cnt + 1` characters to account for the null terminator.
 * @param       src : Pointer to the null-terminated source string to copy from.
 * @param       cnt : Maximum number of characters to copy (not counting the null terminator).
 * @return      Pointer to the beginning of the destination buffer (`dest`).
 */
template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar* cstr_copy(
        TpDestinationChar* dest,
        const TpSourceChar* src,
        std::size_t cnt
) noexcept
{
    if (dest == nullptr || src == nullptr || cnt == 0)
    {
        return dest;
    }
    
    TpDestinationChar* orig_dest = dest;
    const TpDestinationChar* const end_dest = dest + cnt;
    
    while (*src != '\0' && dest < end_dest)
    {
        *dest++ = *src++;
    }

    *dest = '\0';
    return orig_dest;
}

/**
 * @brief       Appends a null-terminated C-style string `src` to the end of `dest`.
 * @param       dest : Pointer to the destination buffer containing a null-terminated string and
 *              enough space to hold the additional characters from `src`.
 * @param       src : Pointer to the null-terminated source string to append.
 * @return      Pointer to the beginning of the destination buffer (`dest`).
 */
template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar* cstr_concat(TpDestinationChar* dest, const TpSourceChar* src) noexcept
{
    if (dest == nullptr || src == nullptr)
    {
        return dest;
    }
    
    TpDestinationChar* orig_dest = dest;
    
    while (*dest != '\0')
    {
        ++dest;
    }
    while (*src != '\0')
    {
        *dest++ = *src++;
    }
    
    *dest = '\0';
    return orig_dest;
}

/**
 * @brief       Appends up to `cnt` characters from a null-terminated C-style string `src` to the
 *              end of `dest`, ensuring the result is null-terminated.
 * @param       dest : Pointer to the destination buffer containing a null-terminated string and
 *              enough space to hold the additional characters from `src`.
 * @param       src : Pointer to the null-terminated source string to append.
 * @param       cnt : Maximum number of characters to append from `src`.
 * @return      Pointer to the beginning of the destination buffer (`dest`).
 */
template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar* cstr_concat(
        TpDestinationChar* dest,
        const TpSourceChar* src,
        std::size_t cnt
) noexcept
{
    if (dest == nullptr || src == nullptr || cnt == 0)
    {
        return dest;
    }
    
    TpDestinationChar* orig_dest = dest;

    while (*dest != '\0')
    {
        ++dest;
    }
    for (std::size_t i = 0; i < cnt && *src != '\0'; ++i)
    {
        *dest++ = *src++;
    }

    *dest = '\0';
    return orig_dest;
}

/**
 * @brief       Computes the length of a null-terminated C-style string.
 * @param       str : Pointer to a null-terminated string.
 * @return      The number of characters in the string (excluding the null terminator).
 */
template<typename TpChar>
[[nodiscard]] std::size_t cstr_length(const TpChar* str) noexcept
{
    if (str == nullptr)
    {
        return 0;
    }
    
    const TpChar* it = str;
    
    while (*it != '\0')
    {
        ++it;
    }
    
    return static_cast<std::size_t>(it - str);
}

/**
 * @brief       Compares two null-terminated C-style strings lexicographically.
 * @param       src : Pointer to the first null-terminated string.
 * @param       trg : Pointer to the second null-terminated string.
 * @return      An integer representing the lexicographical relationship between `src` and `trg`:
 *              - `0` if equal,
 *              - `-1` if `src` < `trg`,
 *              - `1` if `src` > `trg`.
 */
template<typename TpSourceChar, typename TpTargetChar>
[[nodiscard]] int cstr_compare(const TpSourceChar* src, const TpTargetChar* trg) noexcept
{
    if (src == (const TpSourceChar*)trg)
    {
        return 0;
    }
    if (src == nullptr)
    {
        return -1;
    }
    if (trg == nullptr)
    {
        return 1;
    }
    
    while (*src != '\0' && *trg != '\0')
    {
        if (*src != *trg)
        {
            return (*src < *trg) ? -1 : 1;
        }
        ++src;
        ++trg;
    }
    
    if (*src == *trg)
    {
        return 0;
    }
    return (*src == '\0') ? -1 : 1;
}

/**
 * @brief       Compares up to `cnt` characters of two null-terminated C-style strings
 *              lexicographically.
 * @param       src : Pointer to the first null-terminated string.
 * @param       trg : Pointer to the second null-terminated string.
 * @param       cnt : Maximum number of characters to compare.
 * @return      An integer representing the lexicographical relationship between `src` and `trg`:
 *              - `0` if equal (up to `cnt`),
 *              - `-1` if `src` < `trg`,
 *              - `1` if `src` > `trg`.
 */
template<typename TpSourceChar, typename TpTargetChar>
[[nodiscard]] int cstr_compare(
        const TpSourceChar* src,
        const TpTargetChar* trg,
        std::size_t cnt
) noexcept
{
    if (src == (const TpSourceChar*)trg || cnt == 0)
    {
        return 0;
    }
    if (src == nullptr)
    {
        return (trg == nullptr) ? 0 : -1;
    }
    if (trg == nullptr)
    {
        return 1;
    }
    
    const TpSourceChar* const src_end = src + cnt;
    
    while (src < src_end && *src != '\0' && *trg != '\0')
    {
        if (*src != *trg)
        {
            return (*src < *trg) ? -1 : 1;
        }
        ++src;
        ++trg;
    }
    
    if (src == src_end || (*src == '\0' && *trg == '\0'))
    {
        return 0;
    }
    if (*src == '\0')
    {
        return -1;
    }
    if (*trg == '\0')
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief       Performs a case-insensitive lexicographical comparison of two null-terminated
 *              C-style strings.
 * @param       src Pointer to the first null-terminated string.
 * @param       trg Pointer to the second null-terminated string.
 * @return      An integer representing the case-insensitive lexicographical relationship between
 *              `src` and `trg`:
 *              - `0` if equal,
 *              - `-1` if `src` < `trg`,
 *              - `1` if `src` > `trg`.
 */
template<typename TpSourceChar, typename TpTargetChar>
[[nodiscard]] int cstr_compare_icase(
        const TpSourceChar* src,
        const TpTargetChar* trg
) noexcept
{
    if (src == (const TpSourceChar*)trg)
    {
        return 0;
    }
    if (src == nullptr)
    {
        return -1;
    }
    if (trg == nullptr)
    {
        return 1;
    }

    while (*src != '\0' && *trg != '\0')
    {
        auto ch1 = speed::stringutils::to_lower(*src);
        auto ch2 = speed::stringutils::to_lower(*trg);

        if (ch1 != ch2)
        {
            return (ch1 < ch2) ? -1 : 1;
        }
        ++src;
        ++trg;
    }

    if (*src == *trg)
    {
        return 0;
    }

    return (*src == '\0') ? -1 : 1;
}

/**
 * @brief       Performs a case-insensitive lexicographical comparison of two null-terminated
 *              C-style strings, up to a maximum number of characters.
 * @param       src : Pointer to the first null-terminated string.
 * @param       trg : Pointer to the second null-terminated string.
 * @param       cnt : Maximum number of characters to compare.
 * @return      An integer representing the case-insensitive lexicographical relationship between
 *              `src` and `trg`:
 *              - `0` if equal (up to `cnt`),
 *              - `-1` if `src` < `trg`,
 *              - `1` if `src` > `trg`.
 */
template<typename TpSourceChar, typename TpTargetChar>
[[nodiscard]] int cstr_compare_icase(
        const TpSourceChar* src,
        const TpTargetChar* trg,
        std::size_t cnt
) noexcept
{
    if (src == (const TpSourceChar*)trg || cnt == 0)
    {
        return 0;
    }
    if (src == nullptr)
    {
        return (trg == nullptr) ? 0 : -1;
    }
    if (trg == nullptr)
    {
        return 1;
    }

    const TpSourceChar* const src_end = src + cnt;

    while (src < src_end && *src != '\0' && *trg != '\0')
    {
        auto ch1 = speed::stringutils::to_lower(*src);
        auto ch2 = speed::stringutils::to_lower(*trg);

        if (ch1 != ch2)
        {
            return (ch1 < ch2) ? -1 : 1;
        }
        ++src;
        ++trg;
    }

    if (src == src_end || (*src == '\0' && *trg == '\0'))
    {
        return 0;
    }
    if (*src == '\0')
    {
        return -1;
    }
    if (*trg == '\0')
    {
        return 1;
    }

    return 0;
}

/**
 * @brief       Finds the first occurrence of a given character in a null-terminated C-style string.
 * @param       str : Pointer to the null-terminated string to search.
 * @param       val : Character value to search for.
 * @return      A pointer to the first occurrence of `val` in `str`, or `nullptr` if `val` is not
 *              found or if `str` is `nullptr`.
 */
template<typename TpChar, typename TpIntegral>
[[nodiscard]] TpChar* cstr_find_first_char(TpChar* str, TpIntegral val) noexcept
{
    if (str == nullptr)
    {
        return nullptr;
    }

    for (; *str != '\0'; ++str)
    {
        if (*str == val)
        {
            return str;
        }
    }

    return nullptr;
}

/**
 * @brief       Finds the last occurrence of a given character in a null-terminated C-style string.
 * @param       str : Pointer to the null-terminated string to search.
 * @param       val : Character value to search for.
 * @return      A pointer to the last occurrence of `val` in `str`, or `nullptr` if `val` is not
 *              found or if `str` is `nullptr`.
 */
template<typename TpChar, typename TpIntegral>
[[nodiscard]] TpChar* cstr_find_last_char(TpChar* str, TpIntegral val) noexcept
{
    if (str == nullptr)
    {
        return nullptr;
    }

    TpChar* last = nullptr;

    for (; *str != '\0'; ++str)
    {
        if (*str == val)
        {
            last = str;
        }
    }

    return last;
}

/**
 * @brief       Counts the number of consecutive characters at the beginning of a string that
 *              match any character in a given set of prefix characters.
 * @param       str : Pointer to the null-terminated input string to scan.
 * @param       prefxs : Pointer to a null-terminated string containing the set of characters to
 *              match as prefixes.
 * @return      The number of leading characters in `str` that are present in `prefxs`.
 *              Returns `0` if `str` or `prefxs` is `nullptr`.
 */
template<typename TpSourceChar, typename TpPrefixChar>
[[nodiscard]] std::size_t cstr_count_prefix_in(
        const TpSourceChar* str,
        const TpPrefixChar* prefxs
) noexcept
{
    if (str == nullptr || prefxs == nullptr)
    {
        return 0;
    }

    std::size_t pos = 0;
    for (; *str != '\0'; ++str)
    {
        const TpPrefixChar* cur_prefx = prefxs;
        bool fnd = false;
        
        while (*cur_prefx != '\0')
        {
            if (*str == *cur_prefx)
            {
                fnd = true;
                break;
            }
            ++cur_prefx;
        }
        if (!fnd)
        {
            break;
        }
        
        ++pos;
    }
    
    return pos;
}

/**
 * @brief       Counts the number of consecutive characters at the beginning of a string that are
 *              not present in a given set of prefix characters.
 * @param       str : Pointer to the null-terminated input string to scan.
 * @param       prefxs : Pointer to a null-terminated string containing the set of characters to
 *              exclude.
 * @return      The number of leading characters in `str` that are **not** present in `prefxs`.
 *              Returns `0` if `str` or `prefxs` is `nullptr`.
 */
template<typename TpSourceChar, typename TpPrefixChar>
[[nodiscard]] std::size_t cstr_count_prefix_not_in(
        const TpSourceChar* str,
        const TpPrefixChar* prefxs
) noexcept
{
    if (str == nullptr || prefxs == nullptr)
    {
        return 0;
    }

    std::size_t pos = 0;
    for (; *str != '\0'; ++str)
    {
        for (const TpPrefixChar* cur_prefx = prefxs; *cur_prefx != '\0'; ++cur_prefx)
        {
            if (*str == *cur_prefx)
            {
                return pos;
            }
        }
        ++pos;
    }
    
    return pos;
}

/**
 * @brief       Finds the first occurrence in a string of any character from a given set.
 * @param       str : Pointer to the null-terminated string to search.
 * @param       accpt : Pointer to a null-terminated string containing the set of accepted
 *              characters.
 * @return      Pointer to the first occurrence in `str` of any character in `accpt`, or `nullptr`
 *              if none of the characters are found or if `str` or `accpt` is `nullptr`.
 */
template<typename TpSourceChar, typename TpAcceptedChar>
[[nodiscard]] TpSourceChar* cstr_find_any_of(
        TpSourceChar* str,
        const TpAcceptedChar* accpt
) noexcept
{
    if (str == nullptr || accpt == nullptr)
    {
        return nullptr;
    }

    for (; *str != '\0'; ++str)
    {
        for (const TpAcceptedChar* cur_accpt = accpt; *cur_accpt != '\0'; ++cur_accpt)
        {
            if (*str == *cur_accpt)
            {
                return str;
            }
        }
    }

    return nullptr;
}

/**
 * @brief       Finds the first occurrence of a substring within a string.
 * @param       str : Pointer to the null-terminated string to search within.
 * @param       substr : Pointer to the null-terminated substring to find.
 * @return      Pointer to the first occurrence of `substr` in `str`, or `nullptr` if `substr`
 *              is not found or if either pointer is `nullptr`.
 */
template<typename TpSourceChar, typename TpSubstringChar>
[[nodiscard]] TpSourceChar* cstr_find_substr(
        TpSourceChar* str,
        const TpSubstringChar* substr
) noexcept
{
    if (str == nullptr || substr == nullptr)
    {
        return nullptr;
    }

    const std::size_t substr_len = speed::stringutils::cstr_length(substr);
    if (substr_len == 0)
    {
        return str;
    }

    const std::size_t str_len = speed::stringutils::cstr_length(str);
    const std::size_t max_start = (str_len < substr_len) ? 0 : (str_len - substr_len + 1);

    for (std::size_t i = 0; i < max_start; ++i, ++str)
    {
        if (speed::stringutils::cstr_compare(str, substr, substr_len) == 0)
        {
            return str;
        }
    }

    return nullptr;
}

/**
 * @brief       Finds the first occurrence of a substring within a string, case-insensitively.
 * @param       str : Pointer to the null-terminated string to search within.
 * @param       substr : Pointer to the null-terminated substring to search for.
 * @return      Pointer to the first occurrence of `substr` in `str`, ignoring case; `nullptr`
 *              if `substr` is not found or if either pointer is `nullptr`.
 */
template<typename TpSourceChar, typename TpSubstringChar>
[[nodiscard]] TpSourceChar* cstr_find_substr_icase(
        TpSourceChar* str,
        const TpSubstringChar* substr
) noexcept
{
    if (str == nullptr || substr == nullptr)
    {
        return nullptr;
    }

    const std::size_t substr_len = speed::stringutils::cstr_length(substr);
    if (substr_len == 0)
    {
        return str;
    }

    const std::size_t str_len = speed::stringutils::cstr_length(str);
    const std::size_t max_start = (str_len < substr_len) ? 0 : (str_len - substr_len + 1);

    for (std::size_t i = 0; i < max_start; ++i, ++str)
    {
        if (speed::stringutils::cstr_compare_icase(str, substr, substr_len) == 0)
        {
            return str;
        }
    }

    return nullptr;
}

/**
 * @brief       Tokenizes a string by splitting it at any of the specified delimiter characters.
 * @param       str : Reference to a pointer to the null-terminated string to tokenize.
 *              On output, it will be updated to point just after the current token. If no more
 *              tokens are found, it is set to `nullptr`.
 * @param       delimitrs : Pointer to a null-terminated string containing delimiter characters.
 * @return      Pointer to the start of the next token, or `nullptr` if there are no more tokens.
 */
template<typename TpSourceChar, typename TpDelimiterChar>
[[nodiscard]] TpSourceChar* cstr_tokenize_any_of(
        TpSourceChar*& str,
        const TpDelimiterChar* delimitrs
) noexcept
{
    if (str == nullptr || delimitrs == nullptr)
    {
        return nullptr;
    }

    while (*str != '\0' && speed::stringutils::cstr_find_any_of(delimitrs, *str) != nullptr)
    {
        ++str;
    }

    if (*str == '\0')
    {
        str = nullptr;
        return nullptr;
    }

    TpSourceChar* token_strt = str;

    while (*str != '\0')
    {
        if (speed::stringutils::cstr_find_any_of(delimitrs, *str) != nullptr)
        {
            *str = '\0';
            ++str;
            return token_strt;
        }
        ++str;
    }

    str = nullptr;
    return token_strt;
}

/**
 * @brief       Truncates a C-style string at the last occurrence of a specified character.
 * @param       str : Pointer to the null-terminated input string to modify.
 * @param       val : The character value to search for.
 * @param       erase_val : If `true`, the character `val` is removed (replaced by `'\0'`);
 *              if `false`, truncation occurs after the character.
 * @return      Pointer to the modified string, or `nullptr` if `str` is `nullptr` or `val` is not
 *              found.
 */
template<typename TpChar, typename TpIntegral>
TpChar* cstr_cut(TpChar* str, TpIntegral val, bool erase_val) noexcept
{
    if (str == nullptr)
    {
        return nullptr;
    }

    TpChar* lst = speed::stringutils::cstr_find_last_char(str, val);
    if (lst == nullptr)
    {
        return nullptr;
    }

    *(lst + (erase_val ? 0 : 1)) = '\0';
    return str;
}

/**
 * @brief       Truncates a C-style string at the last occurrence of a specified character,
 *              searching only within a given number of characters.
 * @param       str : Pointer to the null-terminated string to be modified.
 * @param       val : Character value to search for.
 * @param       erase_val : If `true`, the character `val` is erased (replaced by `'\0'`);
 *              if `false`, truncation occurs just after the character.
 * @param       cnt : The number of characters from the beginning of `str` to consider for the
 *              search.
 * @return      Pointer to the modified string, or `nullptr` if `str` is `nullptr`, `cnt` is 0,
 *              or the character is not found.
 */
template<typename TpChar, typename TpIntegral>
TpChar* cstr_cut(TpChar* str, TpIntegral val, bool erase_val, std::size_t cnt) noexcept
{
    if (str == nullptr || cnt == 0)
    {
        return nullptr;
    }

    TpChar* lst = speed::stringutils::cstr_find_last_char(str, val, cnt);
    if (lst == nullptr)
    {
        return nullptr;
    }

    *(lst + (erase_val ? 0 : 1)) = '\0';
    return str;
}

/**
 * @brief       Removes all characters from a C-style string that satisfy a given predicate.
 * @param       str : Pointer to the null-terminated string to be modified.
 * @param       pred : Predicate function used to test each character in the string.
 * @return      Pointer to the modified string if `str` is not `nullptr`, or `nullptr` if `str` is
 *              `nullptr`.
 */
template<typename TpChar, typename TpPredicate>
TpChar* cstr_remove_if(TpChar* str, TpPredicate pred) noexcept
{
    if (str == nullptr)
    {
        return nullptr;
    }

    TpChar* write_it = str;

    for (TpChar* read_it = str; *read_it != '\0'; ++read_it)
    {
        if (!pred(*read_it))
        {
            *write_it++ = *read_it;
        }
    }

    *write_it = '\0';
    return str;
}

/**
 * @brief       Removes characters from the first `cnt` elements of a C-style string that satisfy
 *              a predicate.
 * @param       str : Pointer to the null-terminated string to modify.
 * @param       pred : Predicate function used to test each character in the string.
 * @param       cnt : Maximum number of characters to examine and filter from the beginning of
 *              the string.
 * @return      Pointer to the modified string if `str` is not `nullptr`, or `nullptr` if `str`
 *              is `nullptr` or `cnt == 0`.
 */
template<typename TpChar, typename TpPredicate>
TpChar* cstr_remove_if(TpChar* str, TpPredicate pred, std::size_t cnt) noexcept
{
    if (str == nullptr || cnt == 0)
    {
        return nullptr;
    }

    TpChar* write_it = str;
    std::size_t i = 0;

    for (TpChar* read_it = str; *read_it != '\0' && i < cnt; ++read_it, ++i)
    {
        if (!pred(*read_it))
        {
            *write_it++ = *read_it;
        }
    }

    while (str[i] != '\0')
    {
        *write_it++ = str[i++];
    }

    *write_it = '\0';
    return str;
}

/**
 * @brief       Removes the last character of a C-style string if it satisfies the given predicate.
 * @param       str : Pointer to the null-terminated string to modify.
 * @param       pred : Predicate function used to test the last character of the string.
 * @return      Pointer to the modified string if `str` is not `nullptr`, or `nullptr` if `str`
 *              is `nullptr`.
 */
template<typename TpChar, typename TpPredicate>
TpChar* cstr_remove_trailing_if(TpChar* str, TpPredicate pred) noexcept
{
    if (str == nullptr)
    {
        return nullptr;
    }

    std::size_t str_sz = speed::stringutils::cstr_length(str);

    if (str_sz > 0 && pred(str[str_sz - 1]))
    {
        str[str_sz - 1] = '\0';
    }

    return str;
}

/**
 * @brief       Removes the last character within a limited range of a C-style string if it
 *              satisfies the given predicate.
 * @param       str : Pointer to a null-terminated C-style string.
 * @param       pred : Predicate function used to test the last character within the limit.
 * @param       cnt : Maximum number of characters from the start of the string to consider.
 * @return      Pointer to the modified string if `str` is not `nullptr`, or `nullptr` if `str`
 *              is `nullptr` or `cnt` is 0.
 */
template<typename TpChar, typename TpPredicate>
TpChar* cstr_remove_trailing_if(TpChar* str, TpPredicate pred, std::size_t cnt) noexcept
{
    if (str == nullptr || cnt == 0)
    {
        return nullptr;
    }

    std::size_t str_len = speed::stringutils::cstr_length(str);
    if (str_len > cnt)
    {
        str_len = cnt;
    }

    if (str_len > 0 && pred(str[str_len - 1]))
    {
        str[str_len - 1] = '\0';
    }

    return str;
}

/**
 * @brief       Escapes all regex-special characters in a null-terminated C-style string.
 * @param       str : Null-terminated input string to escape. If `nullptr`, returns an empty string.
 * @return      A `std::basic_string<TpChar, TpCharTraits, TpAllocator>` with all regex-special
 *              characters escaped.
 */
template<typename TpChar, typename TpCharTraits, typename TpAllocator>
[[nodiscard]] auto escape_regex(const TpChar* str) noexcept
{
    using string_type = std::basic_string<TpChar, TpCharTraits, TpAllocator>;
    
    const std::array<TpChar, 14> special_chars =
        {'[', '\\', '^', '$', '.', '|', '?', '*', '+', '(', ')', '{', '}', ']'};

    if (str == nullptr)
    {
        return string_type{};
    }
    
    string_type res;
    const TpChar* str_it;
    std::size_t original_len = 0;
    std::size_t extr = 0;

    for (str_it = str; *str_it != '\0'; ++str_it)
    {
        ++original_len;
        if (std::find(special_chars.begin(), special_chars.end(), *str_it) != special_chars.end())
        {
            ++extr;
        }
    }

    res.reserve(original_len + extr);
    for (str_it = str; *str_it != '\0'; ++str_it)
    {
        if (std::find(special_chars.begin(), special_chars.end(), *str_it) != special_chars.end())
        {
            res += '\\';
        }
        res += *str_it;
    }

    return res;
}

/**
 * @brief       Escapes all regex-special characters in a `std::basic_string`.
 * @param       str : The input string to escape.
 * @return      A `std::basic_string<TpChar, TpCharTraits, TpAllocator>` with all regex-special
 *              characters escaped.
 */
template<typename TpChar, typename TpCharTraits, typename TpAllocator>
[[nodiscard]] auto escape_regex(
        const std::basic_string<TpChar, TpCharTraits, TpAllocator>& str
) noexcept
{
    return escape_regex<TpChar, TpCharTraits, TpAllocator>(str.c_str());
}

/**
 * @brief       Escapes all regex-special characters in-place in a `std::basic_string`.
 * @param       str : The string to be modified in place.
 * @return      `true` if the string was modified (i.e., any special characters were escaped),
 *              `false` otherwise.
 */
template<typename TpChar, typename TpCharTraits, typename TpCharAlloc>
bool escape_regex_implace(std::basic_string<TpChar, TpCharTraits, TpCharAlloc>& str) noexcept
{
    using string_type = std::basic_string<TpChar, TpCharTraits, TpCharAlloc>;
    using size_type = typename string_type::size_type;

    const std::array<TpChar, 14> special_chars =
        {'[', '\\', '^', '$', '.', '|', '?', '*', '+', '(', ')', '{', '}', ']'};
    
    auto is_special = [&](TpChar ch)
    {
        return std::find(special_chars.begin(), special_chars.end(), ch) != special_chars.end();
    };

    size_type extra_chars = 0;
    for (TpChar ch : str)
    {
        if (is_special(ch))
        {
            ++extra_chars;
        }
    }

    if (extra_chars == 0)
    {
        return false;
    }

    const size_type original_size = str.size();
    str.resize(original_size + extra_chars);

    for (size_type i = original_size, j = str.size(); i > 0;)
    {
        --i;
        --j;
        str[j] = str[i];
        if (is_special(str[i]))
        {
            --j;
            str[j] = '\\';
        }
    }

    return true;
}

/**
 * @brief       Returns the length of a null-terminated string.
 * @param       str : Pointer to the null-terminated string.
 * @return      The number of characters in the string, excluding the null terminator.
 */
template<typename TpChar>
[[nodiscard]] inline std::size_t get_length(const TpChar* str) noexcept
{
    return speed::stringutils::cstr_length(str);
}

/**
 * @brief       Returns the length of a std::basic_string.
 * @param       str : Reference to the string whose length is to be computed.
 * @return      The number of characters in the string.
 */
template<StringLike TpString>
[[nodiscard]] inline auto get_length(const TpString& str) noexcept
{
    return str.size();
}

/**
 * @brief       Matches a string against a wildcard pattern.
 *
 * Supports wildcard characters:
 * - `*` matches any sequence of characters (including an empty sequence),
 * - `?` matches exactly one character.
 *
 * @param       str : Pointer to the null-terminated string to be matched.
 * @param       pattrn : Pointer to the null-terminated wildcard pattern string.
 * @return      `true` if the string matches the pattern; otherwise, `false`.
 */
template<typename TpSourceChar, typename TpPatternChar>
[[nodiscard]] bool match_wildcard(
        const TpSourceChar* str,
        const TpPatternChar* pattrn
) noexcept
{
    if (str == nullptr || pattrn == nullptr)
    {
        return false;
    }

    const TpSourceChar* str_backup = nullptr;
    const TpPatternChar* pattrn_backup = nullptr;

    while (*str)
    {
        if (*pattrn == '*')
        {
            while (*pattrn == '*')
            {
                ++pattrn;
            }

            if (*pattrn == '\0')
            {
                return true;
            }

            pattrn_backup = pattrn;
            str_backup = str;
        }
        else if (*pattrn == '?' || *pattrn == *str)
        {
            ++pattrn;
            ++str;
        }
        else if (pattrn_backup)
        {
            pattrn = pattrn_backup;
            str = ++str_backup;
        }
        else
        {
            return false;
        }
    }

    while (*pattrn == '*')
    {
        ++pattrn;
    }

    return *pattrn == '\0';
}

/**
 * @brief       Matches a std::basic_string against a wildcard pattern.
 *
 * Supports wildcard characters:
 * - `*` matches any sequence of characters (including empty),
 * - `?` matches exactly one character.
 *
 * @param       str : The string to match.
 * @param       pattrn : Null-terminated wildcard pattern string.
 * @return      `true` if the string matches the pattern; otherwise, `false`.
 */
template<StringLike TpSourceString, typename TpPatternChar>
[[nodiscard]] bool match_wildcard(
        const TpSourceString& str,
        const TpPatternChar* pattrn
) noexcept
{
    return speed::stringutils::match_wildcard(str.data(), pattrn);
}

/**
 * @brief       Matches a string against a wildcard pattern, both provided as std::basic_string.
 *
 * Supports wildcard characters:
 * - `*` matches any sequence of characters (including empty),
 * - `?` matches exactly one character.
 *
 * @param       str : The string to be matched.
 * @param       pattrn : The wildcard pattern string.
 * @return      `true` if `str` matches `pattrn`; otherwise, `false`.
 */
template<StringLike TpSourceString, StringLike TpPatternString>
[[nodiscard]] bool match_wildcard(
        const TpSourceString& str,
        const TpPatternString& pattrn
) noexcept
{
    return speed::stringutils::match_wildcard(str.data(), pattrn.data());
}

/**
 * @brief       Case-insensitive wildcard pattern matching between C-style strings.
 *
 * Supports wildcard characters:
 * - `*` matches any sequence of characters (including empty),
 * - `?` matches exactly one character,
 *
 * @param       str : Pointer to the null-terminated input string to match.
 * @param       pattrn : Pointer to the null-terminated wildcard pattern string.
 * @return      `true` if the input string matches the wildcard pattern ignoring case;
 *              otherwise, `false`.
 */
template<typename TpSourceChar, typename TpPatternChar>
[[nodiscard]] bool match_wildcard_icase(
        const TpSourceChar* str,
        const TpPatternChar* pattrn
) noexcept
{
    if (str == nullptr || pattrn == nullptr)
    {
        return false;
    }

    const TpSourceChar* str_backup = nullptr;
    const TpPatternChar* pattrn_backup = nullptr;

    while (*str)
    {
        if (*pattrn == '*')
        {
            while (*pattrn == '*')
            {
                ++pattrn;
            }

            if (*pattrn == '\0')
            {
                return true;
            }

            pattrn_backup = pattrn;
            str_backup = str;
        }
        else if (*pattrn == '?' ||
                 speed::stringutils::to_lower(*pattrn) == speed::stringutils::to_lower(*str))
        {
            ++pattrn;
            ++str;
        }
        else if (pattrn_backup)
        {
            pattrn = pattrn_backup;
            str = ++str_backup;
        }
        else
        {
            return false;
        }
    }

    while (*pattrn == '*')
    {
        ++pattrn;
    }

    return *pattrn == '\0';
}

/**
 * @brief       Case-insensitive wildcard pattern matching between a std::basic_string and a
 *              C-style pattern string.
 *
 * It supports wildcard characters:
 * - `*` matches any sequence of characters (including empty),
 * - `?` matches exactly one character.
 *
 * @param       str : The string to be matched.
 * @param       pattrn : The wildcard pattern as a null-terminated C-style string.
 * @return      `true` if `str` matches the wildcard pattern ignoring case; otherwise, `false`.
 */
template<StringLike TpSourceString, typename TpPatternChar>
[[nodiscard]] bool match_wildcard_icase(
        const TpSourceString& str,
        const TpPatternChar* pattrn
) noexcept
{
    return speed::stringutils::match_wildcard_icase(str.data(), pattrn);
}

/**
 * @brief       Case-insensitive wildcard pattern matching between two std::basic_strings.
 *
 * It supports wildcard characters:
 * - `*` matches any sequence of characters (including empty),
 * - `?` matches exactly one character.
 *
 * @param       str : The string to be matched.
 * @param       pattrn : The wildcard pattern string.
 * @return      `true` if `str` matches the wildcard pattern ignoring case; otherwise, `false`.
 */
template<StringLike TpSourceString, StringLike TpPatternString>
[[nodiscard]] bool match_wildcard_icase(
        const TpSourceString& str,
        const TpPatternString& pattrn
) noexcept
{
    return speed::stringutils::match_wildcard_icase(str.data(), pattrn.data());
}

/**
 * @brief       Splits a string into a vector of substrings based on separator characters,
 *              supporting escape characters.
 * @param       str : Null-terminated input string to split.
 * @param       seps : Null-terminated string of separator characters.
 * @param       escape_ch : Character used to escape separators.
 * @return      A vector of substrings of type
 *              `std::basic_string<TpSourceChar, TpCharTraits, TpAllocator>`. Empty input or
 *              separators result in an empty vector.
 */
template<
        typename TpSourceChar,
        typename TpSeparatorChar,
        typename TpCharTraits,
        typename TpAllocator,
        typename TpIntegral
>
[[nodiscard]] auto split(
        const TpSourceChar* str,
        const TpSeparatorChar* seps,
        TpIntegral escape_ch
)
{
    using char_allocator_type =
        typename std::allocator_traits<TpAllocator>::template rebind_alloc<TpSourceChar>;
    using string_type =
        std::basic_string<TpSourceChar, TpCharTraits, char_allocator_type>;
    using string_allocator_type =
        typename std::allocator_traits<TpAllocator>::template rebind_alloc<string_type>;

    std::vector<string_type, string_allocator_type> vals;
    const TpSourceChar* cur_substr = str;
    std::size_t vals_sz = 0;
    std::size_t cnt;

    if (str == nullptr || seps == nullptr || *str == '\0' || *seps == '\0')
    {
        return vals;
    }

    auto is_separator = [seps](TpSourceChar ch)
    {
        for (const TpSeparatorChar* sep_it = seps; *sep_it != '\0'; ++sep_it)
        {
            if (ch == *sep_it)
            {
                return true;
            }
        }
        return false;
    };
    
    auto get_current_substring_size = [seps, escape_ch](const TpSourceChar* cur_substr)
    {
        while (*cur_substr != '\0')
        {
            std::size_t cnt = cstr_count_prefix_not_in(cur_substr, seps);
    
            if (cnt == 0)
            {
                return (std::size_t)0;
            }
            else if (cur_substr[cnt - 1] != escape_ch)
            {
                return cnt;
            }

            cur_substr += cnt + 1;
        }
    
        return (std::size_t)0;
    };
    
    auto get_substring = [](const TpSourceChar* str, std::size_t cnt)
    {
        string_type res;
        res.reserve(cnt);
        res.append(str, cnt);
        return res;
    };
    
    while (*cur_substr != '\0')
    {
        cnt = get_current_substring_size(cur_substr);
        
        if (cnt != 0)
        {
            cur_substr += cnt;
            ++vals_sz;
        }
        
        while (is_separator(*cur_substr))
        {
            ++cur_substr;
        }
    }
    vals.reserve(vals_sz);

    cur_substr = str;
    while (*cur_substr != '\0')
    {
        cnt = get_current_substring_size(cur_substr);
        
        if (cnt != 0)
        {
            vals.emplace_back(get_substring(cur_substr, cnt));
            cur_substr += cnt;
        }
        
        if (is_separator(*cur_substr))
        {
            ++cur_substr;
        }
    }

    return vals;
}

/**
 * @brief       Splits a std::basic_string into substrings based on separator characters with
 *              escape support.
 * @param       str : Input string to split.
 * @param       seps : Null-terminated string of separator characters.
 * @param       escape_ch : Character used to escape separators.
 * @return      A vector of substrings split from `str`.
 */
template<
        StringLike TpSourceString,
        typename TpSeparatorChar,
        typename TpCharTraits,
        typename TpAllocator,
        typename TpIntegral
>
[[nodiscard]] auto split(
        const TpSourceString& str,
        const TpSeparatorChar* seps,
        TpIntegral escape_ch
)
{
    return speed::stringutils::split<typename TpSourceString::value_type, TpSeparatorChar,
            TpCharTraits, TpAllocator, TpIntegral>(str.data(), seps, escape_ch);
}

/**
 * @brief       Splits a string into substrings based on separator characters with escape support.
 * @param       str : The input string to be split.
 * @param       seps : The separator characters as a string.
 * @param       escape_ch : Character used to escape separator characters.
 * @return      A vector of substrings resulting from splitting the input string.
 */
template<
        StringLike TpSourceString,
        StringLike TpSeparatorString,
        typename TpCharTraits,
        typename TpAllocator,
        typename TpIntegral
>
[[nodiscard]] auto split(
        const TpSourceString& str,
        const TpSeparatorString& seps,
        TpIntegral escape_ch
)
{
    return speed::stringutils::split<typename TpSourceString::value_type,
            typename TpSeparatorString::value_type, TpCharTraits, TpAllocator, TpIntegral>(
                    str.data(), seps.data(), escape_ch);
}

/**
 * @brief       Converts a character to lowercase.
 * @param       ch : The character to convert.
 * @return      The lowercase equivalent of `ch` if supported; otherwise, returns `ch` unchanged.
 */
template<typename TpChar>
[[nodiscard]] TpChar to_lower(TpChar ch)
{
    if constexpr (std::is_same_v<TpChar, char>)
    {
        const std::locale loc;
        return std::tolower(ch, loc);
    }
    else if constexpr (std::is_same_v<TpChar, wchar_t>)
    {
        return std::towlower(ch);
    }
    
    return ch;
}

/**
 * @brief       Converts a null-terminated string to lowercase.
 * @param       str : Pointer to the null-terminated input string.
 * @return      A `std::basic_string` containing the lowercase version of the input string.
 */
template<typename TpChar, typename TpCharTraits, typename TpAllocator>
[[nodiscard]] auto to_lower(const TpChar* str)
{
    using string_type = std::basic_string<TpChar, TpCharTraits, TpAllocator>;
    string_type res;

    if (str == nullptr)
    {
        return res;
    }

    for (const TpChar* str_it = str; *str_it != '\0'; ++str_it)
    {
        res += speed::stringutils::to_lower(*str_it);
    }

    return res;
}

/**
 * @brief       Converts a string to lowercase.
 * @param       str : The input string to convert to lowercase.
 * @return      A new string containing the lowercase version of the input.
 */
template<StringLike TpString, typename TpCharTraits, typename TpAllocator>
[[nodiscard]] auto to_lower(const TpString& str)
{
    return speed::stringutils::to_lower<
            typename TpString::value_type, TpCharTraits, TpAllocator>(str.data());
}

/**
 * @brief       Converts all characters in the string to lowercase in place.
 * @param       str : The string to convert to lowercase in place.
 * @return      true if the string was modified (at least one character changed), false otherwise.
 */
template<typename TpChar, typename TpCharTraits, typename TpAllocator>
bool to_lower_inplace(std::basic_string<TpChar, TpCharTraits, TpAllocator>& str)
{
    bool modified = false;

    for (auto& ch : str)
    {
        TpChar lower_ch = speed::stringutils::to_lower(ch);
        if (lower_ch != ch)
        {
            ch = lower_ch;
            modified = true;
        }
    }

    return modified;
}

/**
 * @brief       Converts a character to its uppercase equivalent.
 * @param       ch : The character to convert.
 * @return      The uppercase equivalent of the character.
 */
template<typename TpChar>
[[nodiscard]] TpChar to_upper(TpChar ch)
{
    if constexpr (std::is_same_v<TpChar, char>)
    {
        const std::locale loc;
        return std::toupper(ch, loc);
    }
    else if constexpr (std::is_same_v<TpChar, wchar_t>)
    {
        return std::towupper(ch);
    }

    return ch;
}

/**
 * @brief       Converts a null-terminated string to uppercase.
 * @param       str : Pointer to a null-terminated input string.
 * @return      A new `std::basic_string` with uppercase characters.
 */
template<typename TpChar, typename TpCharTraits, typename TpAllocator>
[[nodiscard]] auto to_upper(const TpChar* str)
{
    using string_type = std::basic_string<TpChar, TpCharTraits, TpAllocator>;
    string_type res;

    if (str == nullptr)
    {
        return res;
    }

    for (const TpChar* str_it = str; *str_it != '\0'; ++str_it)
    {
        res += speed::stringutils::to_upper(*str_it);
    }

    return res;
}

/**
 * @brief       Converts a string to uppercase.
 * @param       str : Input string to convert.
 * @return      A new `std::basic_string` with uppercase characters.
 */
template<StringLike TpString, typename TpCharTraits, typename TpAllocator>
[[nodiscard]] auto to_upper(const TpString& str)
{
    return speed::stringutils::to_upper<
            typename TpString::value_type, TpCharTraits, TpAllocator>(str.data());
}

/**
 * @brief       Converts all characters of the string to uppercase in-place.
 * @param       str : The string to modify in-place.
 * @return      true if the string was modified; false if no characters changed.
 */
template<typename TpChar, typename TpCharTraits, typename TpAllocator>
bool to_upper_inplace(std::basic_string<TpChar, TpCharTraits, TpAllocator>& str)
{
    bool modifid = false;

    for (auto& ch : str)
    {
        TpChar upper_ch = speed::stringutils::to_upper(ch);
        if (upper_ch != ch)
        {
            ch = upper_ch;
            modifid = true;
        }
    }

    return modifid;
}

}

#endif

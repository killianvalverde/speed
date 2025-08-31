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
 * @brief       stringutils operations functions header.
 * @author      Killian Valverde
 * @date        2016/01/08
 */

#ifndef SPEED_STRINGUTILS_OPERATIONS_HPP
#define SPEED_STRINGUTILS_OPERATIONS_HPP

#include <algorithm>
#include <array>
#include <cwctype>
#include <locale>
#include <string>
#include <type_traits>
#include <vector>

#include "detail/forward_declarations.hpp"
#include "../type_traits/type_traits.hpp"

namespace speed::stringutils {

/**
 * @brief       Copies a null-terminated C-style string from `src` to `dest`, possibly with
 *              different character types.
 * @param       dest : Pointer to the destination buffer where the content is to be copied.
 *              Must be large enough to hold the contents of `src`, including the null terminator.
 * @param       src : Pointer to the null-terminated source string to copy from.
 * @return      Pointer to the beginning of the destination buffer (`dest`).
 */
template<typename DestinationCharT, typename SourceCharT>
DestinationCharT* cstr_copy(DestinationCharT* dest, const SourceCharT* src) noexcept
{
    if (dest == nullptr || src == nullptr)
    {
        return dest;
    }

    DestinationCharT* orig_dest = dest;

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
template<typename DestinationCharT, typename SourceCharT>
DestinationCharT* cstr_copy(
        DestinationCharT* dest,
        const SourceCharT* src,
        std::size_t cnt
) noexcept
{
    if (dest == nullptr || src == nullptr || cnt == 0)
    {
        return dest;
    }
    
    DestinationCharT* orig_dest = dest;
    const DestinationCharT* const end_dest = dest + cnt;
    
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
template<typename DestinationCharT, typename SourceCharT>
DestinationCharT* cstr_concat(DestinationCharT* dest, const SourceCharT* src) noexcept
{
    if (dest == nullptr || src == nullptr)
    {
        return dest;
    }
    
    DestinationCharT* orig_dest = dest;
    
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
template<typename DestinationCharT, typename SourceCharT>
DestinationCharT* cstr_concat(
        DestinationCharT* dest,
        const SourceCharT* src,
        std::size_t cnt
) noexcept
{
    if (dest == nullptr || src == nullptr || cnt == 0)
    {
        return dest;
    }
    
    DestinationCharT* orig_dest = dest;

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
template<typename CharT>
[[nodiscard]] std::size_t cstr_length(const CharT* str) noexcept
{
    if (str == nullptr)
    {
        return 0;
    }
    
    const CharT* it = str;
    
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
template<typename SourceCharT, typename TargetCharT>
[[nodiscard]] int cstr_compare(const SourceCharT* src, const TargetCharT* trg) noexcept
{
    if (src == (const SourceCharT*)trg)
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
template<typename SourceCharT, typename TargetCharT>
[[nodiscard]] int cstr_compare(
        const SourceCharT* src,
        const TargetCharT* trg,
        std::size_t cnt
) noexcept
{
    if (src == (const SourceCharT*)trg || cnt == 0)
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
    
    const SourceCharT* const src_end = src + cnt;
    
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
template<typename SourceCharT, typename TargetCharT>
[[nodiscard]] int cstr_compare_icase(
        const SourceCharT* src,
        const TargetCharT* trg
) noexcept
{
    if (src == (const SourceCharT*)trg)
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
        auto ch1 = stringutils::to_lower(*src);
        auto ch2 = stringutils::to_lower(*trg);

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
template<typename SourceCharT, typename TargetCharT>
[[nodiscard]] int cstr_compare_icase(
        const SourceCharT* src,
        const TargetCharT* trg,
        std::size_t cnt
) noexcept
{
    if (src == (const SourceCharT*)trg || cnt == 0)
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

    const SourceCharT* const src_end = src + cnt;

    while (src < src_end && *src != '\0' && *trg != '\0')
    {
        auto ch1 = stringutils::to_lower(*src);
        auto ch2 = stringutils::to_lower(*trg);

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
template<typename CharT, typename IntegralT>
[[nodiscard]] CharT* cstr_find_first_char(CharT* str, IntegralT val) noexcept
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
template<typename CharT, typename IntegralT>
[[nodiscard]] CharT* cstr_find_last_char(CharT* str, IntegralT val) noexcept
{
    if (str == nullptr)
    {
        return nullptr;
    }

    CharT* last = nullptr;

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
template<typename SourceCharT, typename PrefixCharT>
[[nodiscard]] std::size_t cstr_count_prefix_in(
        const SourceCharT* str,
        const PrefixCharT* prefxs
) noexcept
{
    if (str == nullptr || prefxs == nullptr)
    {
        return 0;
    }

    std::size_t pos = 0;
    for (; *str != '\0'; ++str)
    {
        const PrefixCharT* cur_prefx = prefxs;
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
template<typename SourceCharT, typename PrefixCharT>
[[nodiscard]] std::size_t cstr_count_prefix_not_in(
        const SourceCharT* str,
        const PrefixCharT* prefxs
) noexcept
{
    if (str == nullptr || prefxs == nullptr)
    {
        return 0;
    }

    std::size_t pos = 0;
    for (; *str != '\0'; ++str)
    {
        for (const PrefixCharT* cur_prefx = prefxs; *cur_prefx != '\0'; ++cur_prefx)
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
template<typename SourceCharT, typename AcceptedCharT>
[[nodiscard]] SourceCharT* cstr_find_any_of(
        SourceCharT* str,
        const AcceptedCharT* accpt
) noexcept
{
    if (str == nullptr || accpt == nullptr)
    {
        return nullptr;
    }

    for (; *str != '\0'; ++str)
    {
        for (const AcceptedCharT* cur_accpt = accpt; *cur_accpt != '\0'; ++cur_accpt)
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
template<typename SourceCharT, typename SubstringCharT>
[[nodiscard]] SourceCharT* cstr_find_substr(
        SourceCharT* str,
        const SubstringCharT* substr
) noexcept
{
    if (str == nullptr || substr == nullptr)
    {
        return nullptr;
    }

    const std::size_t substr_len = cstr_length(substr);
    if (substr_len == 0)
    {
        return str;
    }

    const std::size_t str_len = cstr_length(str);
    const std::size_t max_start = (str_len < substr_len) ? 0 : (str_len - substr_len + 1);

    for (std::size_t i = 0; i < max_start; ++i, ++str)
    {
        if (cstr_compare(str, substr, substr_len) == 0)
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
template<typename SourceCharT, typename SubstringCharT>
[[nodiscard]] SourceCharT* cstr_find_substr_icase(
        SourceCharT* str,
        const SubstringCharT* substr
) noexcept
{
    if (str == nullptr || substr == nullptr)
    {
        return nullptr;
    }

    const std::size_t substr_len = cstr_length(substr);
    if (substr_len == 0)
    {
        return str;
    }

    const std::size_t str_len = cstr_length(str);
    const std::size_t max_start = (str_len < substr_len) ? 0 : (str_len - substr_len + 1);

    for (std::size_t i = 0; i < max_start; ++i, ++str)
    {
        if (cstr_compare_icase(str, substr, substr_len) == 0)
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
template<typename SourceCharT, typename DelimiterCharT>
[[nodiscard]] SourceCharT* cstr_tokenize_any_of(
        SourceCharT*& str,
        const DelimiterCharT* delimitrs
) noexcept
{
    if (str == nullptr || delimitrs == nullptr)
    {
        return nullptr;
    }

    while (*str != '\0' && cstr_find_any_of(delimitrs, *str) != nullptr)
    {
        ++str;
    }

    if (*str == '\0')
    {
        str = nullptr;
        return nullptr;
    }

    SourceCharT* token_strt = str;

    while (*str != '\0')
    {
        if (cstr_find_any_of(delimitrs, *str) != nullptr)
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
template<typename CharT, typename IntegralT>
CharT* cstr_cut(CharT* str, IntegralT val, bool erase_val) noexcept
{
    if (str == nullptr)
    {
        return nullptr;
    }

    CharT* lst = cstr_find_last_char(str, val);
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
template<typename CharT, typename IntegralT>
CharT* cstr_cut(CharT* str, IntegralT val, bool erase_val, std::size_t cnt) noexcept
{
    if (str == nullptr || cnt == 0)
    {
        return nullptr;
    }

    CharT* lst = cstr_find_last_char(str, val, cnt);
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
template<typename CharT, typename PredicateT>
CharT* cstr_remove_if(CharT* str, PredicateT pred) noexcept
{
    if (str == nullptr)
    {
        return nullptr;
    }

    CharT* write_it = str;

    for (CharT* read_it = str; *read_it != '\0'; ++read_it)
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
template<typename CharT, typename PredicateT>
CharT* cstr_remove_if(CharT* str, PredicateT pred, std::size_t cnt) noexcept
{
    if (str == nullptr || cnt == 0)
    {
        return nullptr;
    }

    CharT* write_it = str;
    std::size_t i = 0;

    for (CharT* read_it = str; *read_it != '\0' && i < cnt; ++read_it, ++i)
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
template<typename CharT, typename PredicateT>
CharT* cstr_remove_trailing_if(CharT* str, PredicateT pred) noexcept
{
    if (str == nullptr)
    {
        return nullptr;
    }

    std::size_t str_sz = cstr_length(str);

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
template<typename CharT, typename PredicateT>
CharT* cstr_remove_trailing_if(CharT* str, PredicateT pred, std::size_t cnt) noexcept
{
    if (str == nullptr || cnt == 0)
    {
        return nullptr;
    }

    std::size_t str_len = cstr_length(str);
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
 * @brief       Escapes all regular expression special characters in a given string.
 * @param       str : The input string to escape. Can be null or empty.
 * @return      A new string of the same type's allocator and traits, with all regex
 *              special characters escaped.
 *              - If `str` is null, returns an empty string.
 *              - If `str` contains no special characters, returns a copy of the input string.
 */
template<typename StringT>
[[nodiscard]] auto escape_regex(const StringT& str)
{
    using character_type = type_traits::character_type_of_t<StringT>;
    using character_traits_type = type_traits::character_traits_of_t<StringT>;
    using allocator_type = type_traits::allocator_of_t<StringT>;
    using string_view_type = type_traits::string_view_of_t<StringT>;
    using string_type = std::basic_string<character_type, character_traits_type, allocator_type>;
    using size_type = typename string_type::size_type;
    
    if (is_null(str))
    {
        return string_type();
    }

    const std::array<character_type, 14> special_chars =
        {'[', '\\', '^', '$', '.', '|', '?', '*', '+', '(', ')', '{', '}', ']'};
    
    string_view_type strv = str;
    size_type original_str_sz = strv.size();
    string_type res_str;
    size_type extra_chars = 0;
    
    auto is_special = [&](character_type ch)
    {
        return std::find(special_chars.begin(), special_chars.end(), ch) != special_chars.end();
    };

    for (auto ch : strv)
    {
        if (is_special(ch))
        {
            ++extra_chars;
        }
    }
    if (extra_chars == 0)
    {
        res_str = strv;
        return res_str;
    }
    
    res_str.resize(original_str_sz + extra_chars);

    for (size_type i = original_str_sz, j = res_str.size(); i > 0;)
    {
        --i;
        --j;
        res_str[j] = strv[i];
        if (is_special(strv[i]))
        {
            --j;
            res_str[j] = '\\';
        }
    }

    return res_str;
}

/**
 * @brief       Escapes all regex-special characters in-place in a `std::basic_string`.
 * @param       str : The string to be modified in place.
 * @return      `true` if the string was modified (i.e., any special characters were escaped),
 *              `false` otherwise.
 */
template<typename CharT, typename CharTraitsT, typename AllocatorT>
bool escape_regex_inplace(std::basic_string<CharT, CharTraitsT, AllocatorT>& str)
{
    using string_type = std::basic_string<CharT, CharTraitsT, AllocatorT>;
    using size_type = typename string_type::size_type;

    const std::array<CharT, 14> special_chars =
        {'[', '\\', '^', '$', '.', '|', '?', '*', '+', '(', ')', '{', '}', ']'};
    
    auto is_special = [&](CharT ch)
    {
        return std::find(special_chars.begin(), special_chars.end(), ch) != special_chars.end();
    };

    size_type extra_chars = 0;
    for (CharT ch : str)
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
 * @brief       Returns the length of a string-like object.
 * @param       str : The string-like object to measure.
 * @return      The number of characters in the string.
 */
template<typename StringT>
[[nodiscard]] std::size_t get_length(const StringT& str) noexcept
{
    if (is_null(str))
    {
        return 0;
    }
    
    type_traits::string_view_of_t<StringT> strv = str;
    return strv.size();
}

/**
 * @brief       Checks whether a given string is empty.
 * @param       str : The string to check.
 * @return      `true` if the string is empty, otherwise `false`.
 */
template<typename StringT>
[[nodiscard]] constexpr bool is_empty(const StringT& str) noexcept
{
    return get_length(str) == 0;
}

/**
 * @brief       Checks whether a given string pointer is null.
 * @param       str : The string object or pointer to check.
 * @return      `true` if `str` is `nullptr` (only for character pointers), otherwise `false`.
 */
template<typename StringT>
[[nodiscard]] constexpr bool is_null(const StringT& str) noexcept
{
    if constexpr (type_traits::is_character_pointer_v<StringT>)
    {
        return str == nullptr;
    }
    
    return false;
}

/**
 * @brief       Matches a string against a wildcard pattern.
 *
 * This function checks whether the given source string matches the provided
 * pattern string. The pattern supports the following wildcards:
 * - `*` matches zero or more characters.
 * - `?` matches exactly one character.
 *
 * @param       str : The input string to be matched.
 * @param       pattrn : The pattern string that may include `*` and `?` wildcards.
 * @return      `true` if the string matches the pattern, `false` otherwise.
 */
template<typename SourceStringT, typename PatternStringT>
[[nodiscard]] bool match_wildcard(
        const SourceStringT& str,
        const PatternStringT& pattrn
) noexcept
{
    using source_string_type = type_traits::string_view_of_t<SourceStringT>;
    using pattern_string_type = type_traits::string_view_of_t<PatternStringT>;
    using source_string_it_type = source_string_type::const_iterator;
    using pattern_string_it_type = pattern_string_type::const_iterator;
    
    if (is_null(str) || is_null(pattrn))
    {
        return false;
    }
    
    source_string_type strv = str;
    pattern_string_type pattrnv = pattrn;
    source_string_it_type strv_it = strv.begin();
    pattern_string_it_type pattrnv_it = pattrnv.begin();
    source_string_it_type strv_backup;
    pattern_string_it_type pattrnv_backup;
    bool has_backp = false;

    while (strv_it != strv.end())
    {
        if (pattrnv_it != pattrnv.end() && *pattrnv_it == '*')
        {
            while (pattrnv_it != pattrnv.end() && *pattrnv_it == '*')
            {
                ++pattrnv_it;
            }

            if (pattrnv_it == pattrnv.end())
            {
                return true;
            }

            pattrnv_backup = pattrnv_it;
            strv_backup = strv_it;
            has_backp = true;
        }
        else if (pattrnv_it != pattrnv.end() && (*pattrnv_it == '?' || *pattrnv_it == *strv_it))
        {
            ++pattrnv_it;
            ++strv_it;
        }
        else if (has_backp)
        {
            ++strv_backup;
            pattrnv_it = pattrnv_backup;
            strv_it = strv_backup;
        }
        else
        {
            return false;
        }
    }

    while (pattrnv_it != pattrnv.end() && *pattrnv_it == '*')
    {
        ++pattrnv_it;
    }

    return pattrnv_it == pattrnv.end();
}

/**
 * @brief       Matches a string against a wildcard pattern, case-insensitively.
 *
 * This function checks whether the given source string matches the provided
 * pattern string, ignoring case differences. The pattern supports the following wildcards:
 * - `*` matches zero or more characters.
 * - `?` matches exactly one character.
 *
 * @param       str : The input string to be matched.
 * @param       pattrn : The pattern string that may include `*` and `?` wildcards.
 * @return      `true` if the string matches the pattern (case-insensitively), `false` otherwise.
 */
template<typename SourceStringT, typename PatternStringT>
[[nodiscard]] bool match_wildcard_icase(
        const SourceStringT& str,
        const PatternStringT& pattrn
) noexcept
{
    using source_string_type = type_traits::string_view_of_t<SourceStringT>;
    using pattern_string_type = type_traits::string_view_of_t<PatternStringT>;
    using source_string_it_type = source_string_type::const_iterator;
    using pattern_string_it_type = pattern_string_type::const_iterator;
    
    if (is_null(str) || is_null(pattrn))
    {
        return false;
    }
    
    source_string_type strv = str;
    pattern_string_type pattrnv = pattrn;
    source_string_it_type strv_it = strv.begin();
    pattern_string_it_type pattrnv_it = pattrnv.begin();
    source_string_it_type strv_backup;
    pattern_string_it_type pattrnv_backup;
    bool has_backp = false;

    while (strv_it != strv.end())
    {
        if (pattrnv_it != pattrnv.end() && *pattrnv_it == '*')
        {
            while (pattrnv_it != pattrnv.end() && *pattrnv_it == '*')
            {
                ++pattrnv_it;
            }

            if (pattrnv_it == pattrnv.end())
            {
                return true;
            }

            pattrnv_backup = pattrnv_it;
            strv_backup = strv_it;
            has_backp = true;
        }
        else if (pattrnv_it != pattrnv.end() &&
                 (*pattrnv_it == '?' ||
                  stringutils::to_lower(*pattrnv_it) == stringutils::to_lower(*strv_it)))
        {
            ++pattrnv_it;
            ++strv_it;
        }
        else if (has_backp)
        {
            ++strv_backup;
            pattrnv_it = pattrnv_backup;
            strv_it = strv_backup;
        }
        else
        {
            return false;
        }
    }

    while (pattrnv_it != pattrnv.end() && *pattrnv_it == '*')
    {
        ++pattrnv_it;
    }

    return pattrnv_it == pattrnv.end();
}

/**
 * @brief       Selects the first string when its character type matches a specified type.
 * @param       str1 : The first string (returned if it matches the specified character type).
 * @param       str2 : The second string (ignored in this overload).
 * @return      A reference to `str1`.
 */
template<typename TpChar, typename TpString1, typename TpString2>
requires std::is_same_v<TpChar, type_traits::character_type_of_t<TpString1>>
constexpr TpString1& pick_by_char(TpString1& str1, TpString2& str2)
{
    return str1;
}

/**
 * @brief       Selects the second string when the first string's character type
 *              does not match a specified type.
 * @param       str1 : The first string (ignored if character type doesn't match).
 * @param       str2 : The second string (returned if `str1` doesn't match).
 * @return      A reference to `str2`.
 */
template<typename TpChar, typename TpString1, typename TpString2>
constexpr TpString2& pick_by_char(TpString1& str1, TpString2& str2)
{
    return str2;
}

/**
 * @brief       Splits a string into substrings based on a set of separator characters.
 * @param       str : The string to be split.
 * @param       seps : The string containing one or more separator characters.
 * @return      A vector of substrings resulting from the split operation.
 */
template<typename StringT1, typename StringT2>
[[nodiscard]] auto split(const StringT1& str, const StringT2& seps)
{
    using character_type = type_traits::character_type_of_t<StringT1>;
    using character_traits_type = type_traits::character_traits_of_t<StringT1>;
    using allocator_type = type_traits::allocator_of_t<StringT1>;
    using string_type = std::basic_string<character_type, character_traits_type, allocator_type>;
    using source_string_view_type = type_traits::string_view_of_t<StringT1>;
    using separators_string_view_type = type_traits::string_view_of_t<StringT2>;
    using string_allocator_type =
        typename std::allocator_traits<allocator_type>::template rebind_alloc<string_type>;
    
    std::vector<string_type, string_allocator_type> vals;
    std::size_t start = 0;
    
    if (is_empty(str) || is_empty(seps))
    {
        return vals;
    }
    
    source_string_view_type strv = str;
    separators_string_view_type sepsv = seps;
    
    vals.reserve(4);
    
    for (std::size_t i = 0; i < strv.size(); ++i)
    {
        if (sepsv.find(strv[i]) != separators_string_view_type::npos)
        {
            vals.emplace_back(strv.substr(start, i - start));
            start = i + 1;
        }
    }

    if (start < strv.size())
    {
        vals.emplace_back(strv.substr(start));
    }
    else
    {
        vals.emplace_back(string_type());
    }

    return vals;
}

/**
 * @brief       Converts a character to lowercase.
 * @param       ch : The character to convert.
 * @return      The lowercase equivalent of `ch` if supported; otherwise, returns `ch` unchanged.
 */
template<typename CharT>
/** @cond */requires std::is_integral_v<CharT>/** @endcond */
[[nodiscard]] CharT to_lower(CharT ch)
{
    if constexpr (std::is_same_v<CharT, char>)
    {
        const std::locale loc;
        return std::tolower(ch, loc);
    }
    else if constexpr (std::is_same_v<CharT, wchar_t>)
    {
        return std::towlower(ch);
    }
    
    return ch;
}

/**
 * @brief       Converts a string to lowercase.
 * @param       str : The input string to convert to lowercase.
 * @return      A new string containing the lowercase version of the input.
 */
template<typename StringT>
/** @cond */requires (!std::is_integral_v<StringT>)/** @endcond */
[[nodiscard]] auto to_lower(const StringT& str)
{
    using character_type = type_traits::character_type_of_t<StringT>;
    using character_traits_type = type_traits::character_traits_of_t<StringT>;
    using string_view_type = type_traits::string_view_of_t<StringT>;
    using string_type = std::basic_string<character_type, character_traits_type>;
    
    if (is_null(str))
    {
        return string_type();
    }
    
    string_view_type strv = str;
    string_type res;
    
    res.reserve(strv.size());

    for (auto ch : strv)
    {
        res += stringutils::to_lower(ch);
    }

    return res;
}

/**
 * @brief       Converts all characters in the string to lowercase in place.
 * @param       str : The string to convert to lowercase in place.
 * @return      true if the string was modified (at least one character changed), false otherwise.
 */
template<typename CharT, typename CharTraitsT, typename AllocatorT>
bool to_lower_inplace(std::basic_string<CharT, CharTraitsT, AllocatorT>& str)
{
    bool modif = false;

    for (auto& ch : str)
    {
        CharT lower_ch = stringutils::to_lower(ch);
        if (lower_ch != ch)
        {
            ch = lower_ch;
            modif = true;
        }
    }

    return modif;
}

/**
 * @brief       Converts a character to its uppercase equivalent.
 * @param       ch : The character to convert.
 * @return      The uppercase equivalent of the character.
 */
template<typename CharT>
/** @cond */requires std::is_integral_v<CharT>/** @endcond */
[[nodiscard]] CharT to_upper(CharT ch)
{
    if constexpr (std::is_same_v<CharT, char>)
    {
        const std::locale loc;
        return std::toupper(ch, loc);
    }
    else if constexpr (std::is_same_v<CharT, wchar_t>)
    {
        return std::towupper(ch);
    }

    return ch;
}

/**
 * @brief       Converts a string to uppercase.
 * @param       str : Input string to convert.
 * @return      A new `std::basic_string` with uppercase characters.
 */
template<typename StringT>
/** @cond */requires (!std::is_integral_v<StringT>)/** @endcond */
[[nodiscard]] auto to_upper(const StringT& str)
{
    using character_type = type_traits::character_type_of_t<StringT>;
    using character_traits_type = type_traits::character_traits_of_t<StringT>;
    using string_view_type = type_traits::string_view_of_t<StringT>;
    using string_type = std::basic_string<character_type, character_traits_type>;
    
    if (is_null(str))
    {
        return string_type();
    }
    
    string_view_type strv = str;
    string_type res;
    
    res.reserve(strv.size());

    for (auto ch : strv)
    {
        res += stringutils::to_upper(ch);
    }

    return res;
}

/**
 * @brief       Converts all characters of the string to uppercase in-place.
 * @param       str : The string to modify in-place.
 * @return      true if the string was modified; false if no characters changed.
 */
template<typename CharT, typename CharTraitsT, typename AllocatorT>
bool to_upper_inplace(std::basic_string<CharT, CharTraitsT, AllocatorT>& str)
{
    bool modif = false;

    for (auto& ch : str)
    {
        CharT upper_ch = stringutils::to_upper(ch);
        if (upper_ch != ch)
        {
            ch = upper_ch;
            modif = true;
        }
    }

    return modif;
}

}

#endif

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
 * @brief Core operations for the strings module.
 * @author Killian Valverde
 * @date 2016-01-08
 */

#pragma once

#include <string>
#include <type_traits>
#include <vector>

#include "detail/forward_declarations.hpp"
#include "../type_traits/type_traits.hpp"

namespace speed::strings {

/**
 * @brief Returns the length of a null-terminated character string.
 *
 * @param str Pointer to the null-terminated character string.
 * @return The number of characters in the string, excluding the null terminator.
 *         Returns `0` if `str` is `nullptr`.
 */
template<typename CharT>
[[nodiscard]] constexpr std::size_t cstr_length(const CharT* str) noexcept
{
    if (str == nullptr)
    {
        return 0;
    }

    const CharT* it = str;

    while (*it != 0)
    {
        ++it;
    }

    return static_cast<std::size_t>(it - str);
}

/**
 * @brief Compares two null-terminated character strings.
 *
 * Performs a lexicographical comparison between the character sequences
 * pointed to by `src` and `trg`.
 *
 * @param src Pointer to the first null-terminated character string.
 * @param trg Pointer to the second null-terminated character string.
 * @return `0` if the strings are equal, a value less than `0` if `src` is
 *         lexicographically less than `trg`, or a value greater than `0`
 *         if `src` is lexicographically greater than `trg`.
 */
template<typename SourceCharT, typename TargetCharT>
[[nodiscard]] int cstr_compare(const SourceCharT* src, const TargetCharT* trg) noexcept
{
    using src_unsigned_type = std::make_unsigned_t<SourceCharT>;
    using trg_unsigned_type = std::make_unsigned_t<TargetCharT>;

    if (src == nullptr)
    {
        return trg == nullptr ? 0 : -1;
    }
    if (trg == nullptr)
    {
        return 1;
    }

    while (*src != 0 && *trg != 0)
    {
        if (*src != *trg)
        {
            return static_cast<src_unsigned_type>(*src)
                < static_cast<trg_unsigned_type>(*trg)
                ? -1
                : 1;
        }

        ++src;
        ++trg;
    }

    if (*src == *trg)
    {
        return 0;
    }

    return *src == 0 ? -1 : 1;
}

/**
 * @brief Compares up to a specified number of characters from two
 *        null-terminated character strings.
 *
 * Performs a lexicographical comparison between the character sequences
 * pointed to by `src` and `trg`, examining at most `cnt` characters.
 *
 * @param src Pointer to the first null-terminated character string.
 * @param trg Pointer to the second null-terminated character string.
 * @param cnt Maximum number of characters to compare.
 * @return `0` if the compared character sequences are equal, `-1` if `src`
 *         is lexicographically less than `trg`, or `1` if `src` is
 *         lexicographically greater than `trg`.
 */
template<typename SourceCharT, typename TargetCharT>
[[nodiscard]] int cstr_compare(
    const SourceCharT* src,
    const TargetCharT* trg,
    std::size_t cnt
) noexcept
{
    using src_unsigned_type = std::make_unsigned_t<SourceCharT>;
    using trg_unsigned_type = std::make_unsigned_t<TargetCharT>;

    if (cnt == 0)
    {
        return 0;
    }
    if (src == nullptr)
    {
        return trg == nullptr ? 0 : -1;
    }
    if (trg == nullptr)
    {
        return 1;
    }

    const SourceCharT* const src_end = src + cnt;

    while (src < src_end && *src != 0 && *trg != 0)
    {
        if (*src != *trg)
        {
            return static_cast<src_unsigned_type>(*src)
                < static_cast<trg_unsigned_type>(*trg)
                ? -1
                : 1;
        }

        ++src;
        ++trg;
    }

    if (src == src_end)
    {
        return 0;
    }
    if (*src == *trg)
    {
        return 0;
    }

    return *src == 0 ? -1 : 1;
}

/**
 * @brief Compares two null-terminated character strings without regard
 *        to character case.
 *
 * Performs a case-insensitive lexicographical comparison between the
 * character sequences pointed to by `src` and `trg`.
 *
 * @param src Pointer to the first null-terminated character string.
 * @param trg Pointer to the second null-terminated character string.
 * @return `0` if the strings are equal, `-1` if `src` is lexicographically
 *         less than `trg`, or `1` if `src` is lexicographically greater
 *         than `trg`.
 */
template<typename SourceCharT, typename TargetCharT>
[[nodiscard]] int cstr_compare_icase(
    const SourceCharT* src,
    const TargetCharT* trg
) noexcept
{
    using src_unsigned_type = std::make_unsigned_t<SourceCharT>;
    using trg_unsigned_type = std::make_unsigned_t<TargetCharT>;

    if (src == nullptr)
    {
        return trg == nullptr ? 0 : -1;
    }
    if (trg == nullptr)
    {
        return 1;
    }

    while (*src != 0 && *trg != 0)
    {
        const auto ch1 = static_cast<src_unsigned_type>(strings::to_lower(*src));
        const auto ch2 = static_cast<trg_unsigned_type>(strings::to_lower(*trg));

        if (ch1 != ch2)
        {
            return ch1 < ch2 ? -1 : 1;
        }

        ++src;
        ++trg;
    }

    if (*src == *trg)
    {
        return 0;
    }

    return *src == 0 ? -1 : 1;
}

/**
 * @brief Compares up to a specified number of characters from two
 *        null-terminated character strings without regard to character case.
 *
 * Performs a case-insensitive lexicographical comparison between the
 * character sequences pointed to by `src` and `trg`, examining at most
 * `cnt` characters.
 *
 * @param src Pointer to the first null-terminated character string.
 * @param trg Pointer to the second null-terminated character string.
 * @param cnt Maximum number of characters to compare.
 * @return `0` if the compared character sequences are equal, `-1` if `src`
 *         is lexicographically less than `trg`, or `1` if `src` is
 *         lexicographically greater than `trg`.
 */
template<typename SourceCharT, typename TargetCharT>
[[nodiscard]] int cstr_compare_icase(
    const SourceCharT* src,
    const TargetCharT* trg,
    std::size_t cnt
) noexcept
{
    using src_unsigned_type = std::make_unsigned_t<SourceCharT>;
    using trg_unsigned_type = std::make_unsigned_t<TargetCharT>;

    if (cnt == 0)
    {
        return 0;
    }
    if (src == nullptr)
    {
        return trg == nullptr ? 0 : -1;
    }
    if (trg == nullptr)
    {
        return 1;
    }

    const SourceCharT* const src_end = src + cnt;

    while (src < src_end && *src != 0 && *trg != 0)
    {
        const auto ch1 = static_cast<src_unsigned_type>(strings::to_lower(*src));
        const auto ch2 = static_cast<trg_unsigned_type>(strings::to_lower(*trg));

        if (ch1 != ch2)
        {
            return ch1 < ch2 ? -1 : 1;
        }

        ++src;
        ++trg;
    }

    if (src == src_end)
    {
        return 0;
    }
    if (*src == *trg)
    {
        return 0;
    }

    return *src == 0 ? -1 : 1;
}

/**
 * @brief Finds the first occurrence of a substring within a null-terminated
 *        character string.
 *
 * Searches the character string pointed to by `str` for the first occurrence
 * of the character sequence pointed to by `substr`.
 *
 * @param str Pointer to the null-terminated character string to search in.
 * @param substr Pointer to the null-terminated character string to search for.
 * @return Pointer to the first occurrence of `substr` within `str`, or
 *         `nullptr` if the substring could not be found. Returns `str` if
 *         `substr` is empty.
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

    const auto first_sub_char = *substr;
    for (; *str != 0; ++str)
    {
        if (*str != first_sub_char)
        {
            continue;
        }
        if (cstr_compare(str + 1, substr + 1, substr_len - 1) == 0)
        {
            return str;
        }
    }

    return nullptr;
}

/**
 * @brief Finds the first occurrence of a substring within a null-terminated
 *        character string without regard to character case.
 *
 * Searches the character string pointed to by `str` for the first occurrence
 * of the character sequence pointed to by `substr` using a case-insensitive
 * comparison.
 *
 * @param str Pointer to the null-terminated character string to search in.
 * @param substr Pointer to the null-terminated character string to search for.
 * @return Pointer to the first occurrence of `substr` within `str`, or
 *         `nullptr` if the substring could not be found. Returns `str` if
 *         `substr` is empty.
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

    const auto first_sub_char = strings::to_lower(*substr);
    for (; *str != 0; ++str)
    {
        if (strings::to_lower(*str) != first_sub_char)
        {
            continue;
        }
        if (cstr_compare_icase(str + 1, substr + 1, substr_len - 1) == 0)
        {
            return str;
        }
    }

    return nullptr;
}

/**
 * @brief Escapes regular expression metacharacters in a string.
 *
 * Returns a copy of the specified string in which all regular expression
 * metacharacters are prefixed with a backslash.
 *
 * @param str The string to escape.
 * @return A string containing the escaped character sequence. Returns an
 *         empty string if `str` is null.
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

    string_view_type strv = str;
    const size_type original_str_sz = strv.size();
    string_type res_str;
    size_type extra_chars = 0;

    constexpr auto is_special = [](character_type ch) noexcept
    {
        switch (ch)
        {
        case '[':
        case '\\':
        case '^':
        case '$':
        case '.':
        case '|':
        case '?':
        case '*':
        case '+':
        case '(':
        case ')':
        case '{':
        case '}':
        case ']':
            return true;

        default:
            return false;
        }
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
        return string_type(strv);
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
 * @brief Escapes regular expression metacharacters in a string in place.
 *
 * Modifies the specified string by prefixing all regular expression
 * metacharacters with a backslash.
 *
 * @param str The string to escape.
 * @return `true` if the string was modified, or `false` if no escaping
 *         was required.
 */
template<typename CharT, typename CharTraitsT, typename AllocatorT>
bool escape_regex_inplace(std::basic_string<CharT, CharTraitsT, AllocatorT>& str)
{
    using string_type = std::basic_string<CharT, CharTraitsT, AllocatorT>;
    using size_type = typename string_type::size_type;

    constexpr auto is_special = [](CharT ch) noexcept
    {
        switch (ch)
        {
        case '[':
        case '\\':
        case '^':
        case '$':
        case '.':
        case '|':
        case '?':
        case '*':
        case '+':
        case '(':
        case ')':
        case '{':
        case '}':
        case ']':
            return true;

        default:
            return false;
        }
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
        const CharT ch = str[i];
        str[j] = ch;
        if (is_special(ch))
        {
            --j;
            str[j] = CharT('\\');
        }
    }

    return true;
}

/**
 * @brief Returns the length of a string.
 *
 * Obtains the number of characters contained in the specified string.
 *
 * @param str The string whose length is to be returned.
 * @return The number of characters in the string. Returns `0` if `str`
 *         is null.
 */
template<typename StringT>
[[nodiscard]] constexpr std::size_t get_length(const StringT& str) noexcept
{
    if (is_null(str))
    {
        return 0;
    }

    type_traits::string_view_of_t<StringT> strv = str;
    return strv.size();
}

/**
 * @brief Checks whether a string is empty.
 *
 * Determines whether the specified string contains any characters.
 *
 * @param str The string to examine.
 * @return `true` if the string is empty; otherwise, `false`.
 */
template<typename StringT>
[[nodiscard]] constexpr bool is_empty(const StringT& str) noexcept
{
    return get_length(str) == 0;
}

/**
 * @brief Checks whether a string is null.
 *
 * Determines whether the specified string is represented by a null character
 * pointer.
 *
 * @param str The string to examine.
 * @return `true` if `str` is a null character pointer; otherwise, `false`.
 */
template<typename StringT>
[[nodiscard]] constexpr bool is_null(const StringT& str) noexcept
{
    if constexpr (type_traits::is_character_pointer_v<std::remove_cvref_t<StringT>>)
    {
        return str == nullptr;
    }

    return false;
}

/**
 * @brief Checks whether a string matches a wildcard pattern.
 *
 * Compares the specified string against a wildcard pattern. The pattern may
 * contain the `*` wildcard character, which matches zero or more characters,
 * and the `?` wildcard character, which matches exactly one character.
 *
 * @param str The string to test.
 * @param pattern The wildcard pattern to match against.
 * @return `true` if the string matches the pattern; otherwise, `false`.
 */
template<typename SourceStringT, typename PatternStringT>
[[nodiscard]] bool match_wildcard(
    const SourceStringT& str,
    const PatternStringT& pattern
) noexcept
{
    using source_string_view_type = type_traits::string_view_of_t<SourceStringT>;
    using pattern_string_view_type = type_traits::string_view_of_t<PatternStringT>;
    using char_type = typename source_string_view_type::value_type;

    if (is_null(str) || is_null(pattern))
    {
        return false;
    }

    source_string_view_type strv = str;
    pattern_string_view_type patv = pattern;
    const char_type* s = strv.data();
    const char_type* p = patv.data();
    const char_type* const s_end = s + strv.size();
    const char_type* const p_end = p + patv.size();
    const char_type* star_pattern = nullptr;
    const char_type* star_string = nullptr;

    while (s != s_end)
    {
        if (p != p_end && (*p == '?' || *p == *s))
        {
            ++s;
            ++p;
        }
        else if (p != p_end && *p == '*')
        {
            while (p != p_end && *p == '*')
            {
                ++p;
            }

            if (p == p_end)
            {
                return true;
            }

            star_pattern = p;
            star_string = s;
        }
        else if (star_pattern != nullptr)
        {
            ++star_string;
            s = star_string;
            p = star_pattern;
        }
        else
        {
            return false;
        }
    }

    while (p != p_end && *p == '*')
    {
        ++p;
    }

    return p == p_end;
}

/**
 * @brief Checks whether a string matches a wildcard pattern without regard
 *        to character case.
 *
 * Compares the specified string against a wildcard pattern using a
 * case-insensitive comparison. The pattern may contain the `*` wildcard
 * character, which matches zero or more characters, and the `?` wildcard
 * character, which matches exactly one character.
 *
 * @param str The string to test.
 * @param pattern The wildcard pattern to match against.
 * @return `true` if the string matches the pattern; otherwise, `false`.
 */
template<typename SourceStringT, typename PatternStringT>
[[nodiscard]] bool match_wildcard_icase(
    const SourceStringT& str,
    const PatternStringT& pattern
) noexcept
{
    using source_string_view_type = type_traits::string_view_of_t<SourceStringT>;
    using pattern_string_view_type = type_traits::string_view_of_t<PatternStringT>;
    using char_type = typename source_string_view_type::value_type;

    if (is_null(str) || is_null(pattern))
    {
        return false;
    }

    source_string_view_type strv = str;
    pattern_string_view_type patv = pattern;
    const char_type* s = strv.data();
    const char_type* p = patv.data();
    const char_type* const s_end = s + strv.size();
    const char_type* const p_end = p + patv.size();
    const char_type* star_pattern = nullptr;
    const char_type* star_string = nullptr;

    while (s != s_end)
    {
        if (p != p_end)
        {
            const char_type pc = *p;

            if (pc == '?' || strings::to_lower(pc) == strings::to_lower(*s))
            {
                ++s;
                ++p;
                continue;
            }

            if (pc == '*')
            {
                while (p != p_end && *p == '*')
                {
                    ++p;
                }

                if (p == p_end)
                {
                    return true;
                }

                star_pattern = p;
                star_string = s;
                continue;
            }
        }
        if (star_pattern != nullptr)
        {
            ++star_string;
            s = star_string;
            p = star_pattern;
        }
        else
        {
            return false;
        }
    }

    while (p != p_end && *p == '*')
    {
        ++p;
    }

    return p == p_end;
}

/**
 * @brief Splits a string into substrings using a set of separator characters.
 *
 * Separates the specified string into a sequence of substrings delimited by
 * any character contained in `seps`.
 *
 * @param str The string to split.
 * @param seps The set of separator characters.
 * @return A vector containing the resulting substrings. Returns an empty
 *         vector if `str` or `seps` is empty.
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
    using string_allocator_type = typename std::allocator_traits<allocator_type>
        ::template rebind_alloc<string_type>;

    std::vector<string_type, string_allocator_type> vals;

    if (is_empty(str) || is_empty(seps))
    {
        return vals;
    }

    source_string_view_type strv = str;
    separators_string_view_type sepsv = seps;

    vals.reserve(4);
    std::size_t start = 0;

    if (sepsv.size() == 1)
    {
        const character_type sep = sepsv[0];

        for (std::size_t i = 0; i < strv.size(); ++i)
        {
            if (strv[i] == sep)
            {
                vals.emplace_back(strv.data() + start, i - start);
                start = i + 1;
            }
        }
    }
    else if (sepsv.size() == 2)
    {
        const character_type sep0 = sepsv[0];
        const character_type sep1 = sepsv[1];

        for (std::size_t i = 0; i < strv.size(); ++i)
        {
            const character_type ch = strv[i];

            if (ch == sep0 || ch == sep1)
            {
                vals.emplace_back(strv.data() + start, i - start);
                start = i + 1;
            }
        }
    }
    else
    {
        for (std::size_t i = 0; i < strv.size(); ++i)
        {
            if (sepsv.find(strv[i]) != separators_string_view_type::npos)
            {
                vals.emplace_back(strv.data() + start, i - start);
                start = i + 1;
            }
        }
    }

    vals.emplace_back(strv.data() + start, strv.size() - start);
    return vals;
}

/**
 * @brief Converts a character to lowercase.
 *
 * Converts an uppercase ASCII letter to its lowercase equivalent. Characters
 * that do not have an uppercase ASCII representation are returned unchanged.
 *
 * @param ch The character to convert.
 * @return The lowercase equivalent of `ch` if applicable; otherwise, `ch`.
 */
template<typename CharT>
    requires type_traits::is_character_v<CharT>
[[nodiscard]] constexpr CharT to_lower(CharT ch) noexcept
{
    if (ch >= CharT('A') && ch <= CharT('Z'))
    {
        return static_cast<CharT>(ch + (CharT('a') - CharT('A')));
    }

    return ch;
}

/**
 * @brief Converts a string to lowercase.
 *
 * Returns a copy of the specified string with all uppercase ASCII letters
 * converted to their lowercase equivalents.
 *
 * @param str The string to convert.
 * @return A lowercase copy of the string. Returns an empty string if `str` is null.
 */
template<typename StringT>
    requires (!type_traits::is_character_v<StringT>)
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

    res.resize(strv.size());
    auto* dst = res.data();

    for (std::size_t i = 0; i < strv.size(); ++i)
    {
        dst[i] = strings::to_lower(strv[i]);
    }

    return res;
}

/**
 * @brief Converts a string to lowercase in place.
 *
 * Converts all uppercase ASCII letters in the specified string to their
 * lowercase equivalents.
 *
 * @param str The string to convert.
 * @return `true` if the string was modified; otherwise, `false`.
 */
template<typename CharT, typename CharTraitsT, typename AllocatorT>
bool to_lower_inplace(std::basic_string<CharT, CharTraitsT, AllocatorT>& str)
{
    bool modified = false;

    auto* p = str.data();
    auto* end = p + str.size();

    while (p != end)
    {
        CharT lower = strings::to_lower(*p);
        if (lower != *p)
        {
            *p = lower;
            modified = true;
        }

        ++p;
    }

    return modified;
}

/**
 * @brief Converts a character to uppercase.
 *
 * Converts a lowercase ASCII letter to its uppercase equivalent. Characters
 * that do not have a lowercase ASCII representation are returned unchanged.
 *
 * @param ch The character to convert.
 * @return The uppercase equivalent of `ch` if applicable; otherwise, `ch`.
 */
template<typename CharT>
    requires type_traits::is_character_v<CharT>
[[nodiscard]] constexpr CharT to_upper(CharT ch) noexcept
{
    if (ch >= CharT('a') && ch <= CharT('z'))
    {
        return static_cast<CharT>(ch - (CharT('a') - CharT('A')));
    }

    return ch;
}

/**
 * @brief Converts a string to uppercase.
 *
 * Returns a copy of the specified string with all lowercase ASCII letters
 * converted to their uppercase equivalents.
 *
 * @param str The string to convert.
 * @return An uppercase copy of the string. Returns an empty string if `str` is null.
 */
template<typename StringT>
    requires (!type_traits::is_character_v<StringT>)
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

    res.resize(strv.size());
    auto* dst = res.data();

    for (std::size_t i = 0; i < strv.size(); ++i)
    {
        dst[i] = strings::to_upper(strv[i]);
    }

    return res;
}

/**
 * @brief Converts a string to uppercase in place.
 *
 * Converts all lowercase ASCII letters in the specified string to their
 * uppercase equivalents.
 *
 * @param str The string to convert.
 * @return `true` if the string was modified; otherwise, `false`.
 */
template<typename CharT, typename CharTraitsT, typename AllocatorT>
bool to_upper_inplace(std::basic_string<CharT, CharTraitsT, AllocatorT>& str)
{
    bool modified = false;

    auto* p = str.data();
    auto* end = p + str.size();

    while (p != end)
    {
        CharT upper = strings::to_upper(*p);

        if (upper != *p)
        {
            *p = upper;
            modified = true;
        }

        ++p;
    }

    return modified;
}

}

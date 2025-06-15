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
 * @file        forward_declarations.hpp
 * @brief       forward_declarations header.
 * @author      Killian Valverde
 * @date        2024/11/16
 */

#ifndef SPEED_STRINGUTILS_FORWARD_DECLARATIONS_HPP
#define SPEED_STRINGUTILS_FORWARD_DECLARATIONS_HPP

#include <cwctype>
#include <system_error>
#include <type_traits>

#include "concepts.hpp"

namespace speed::stringutils {

template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar* cstr_copy(TpDestinationChar* dest, const TpSourceChar* src) noexcept;

template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar* cstr_copy(
        TpDestinationChar* dest,
        const TpSourceChar* src,
        std::size_t cnt
) noexcept;

template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar* cstr_concat(TpDestinationChar* dest, const TpSourceChar* src) noexcept;

template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar* cstr_concat(
        TpDestinationChar* dest,
        const TpSourceChar* src,
        std::size_t cnt
) noexcept;

template<typename TpChar>
[[nodiscard]] std::size_t cstr_length(const TpChar* str) noexcept;

template<typename TpSourceChar, typename TpTargetChar>
[[nodiscard]] int cstr_compare(const TpSourceChar* src, const TpTargetChar* trg) noexcept;

template<typename TpSourceChar, typename TpTargetChar>
[[nodiscard]] int cstr_compare(
        const TpSourceChar* src,
        const TpTargetChar* trg,
        std::size_t cnt
) noexcept;

template<typename TpSourceChar, typename TpTargetChar>
[[nodiscard]] int cstr_compare_icase(
        const TpSourceChar* src,
        const TpTargetChar* trg
) noexcept;

template<typename TpSourceChar, typename TpTargetChar>
[[nodiscard]] int cstr_compare_icase(
        const TpSourceChar* src,
        const TpTargetChar* trg,
        std::size_t cnt
) noexcept;

template<typename TpChar, typename TpIntegral>
[[nodiscard]] TpChar* cstr_find_first_char(TpChar* str, TpIntegral val) noexcept;

template<typename TpChar, typename TpIntegral>
[[nodiscard]] TpChar* cstr_find_last_char(TpChar* str, TpIntegral val) noexcept;

template<typename TpSourceChar, typename TpPrefixChar>
[[nodiscard]] std::size_t cstr_count_prefix_in(
        const TpSourceChar* str,
        const TpPrefixChar* prefxs
) noexcept;

template<typename TpSourceChar, typename TpPrefixChar>
[[nodiscard]] std::size_t cstr_count_prefix_not_in(
        const TpSourceChar* str,
        const TpPrefixChar* prefxs
) noexcept;

template<typename TpSourceChar, typename TpAcceptedChar>
[[nodiscard]] TpSourceChar* cstr_find_any_of(
        TpSourceChar* str,
        const TpAcceptedChar* accpt
) noexcept;

template<typename TpSourceChar, typename TpSubstringChar>
[[nodiscard]] TpSourceChar* cstr_find_substr(
        TpSourceChar* str,
        const TpSubstringChar* substr
) noexcept;

template<typename TpSourceChar, typename TpSubstringChar>
[[nodiscard]] TpSourceChar* cstr_find_substr_icase(
        TpSourceChar* str,
        const TpSubstringChar* substr
) noexcept;

template<typename TpSourceChar, typename TpDelimiterChar>
[[nodiscard]] TpSourceChar* cstr_tokenize_any_of(
        TpSourceChar*& str,
        const TpDelimiterChar* delimitrs
) noexcept;

template<typename TpChar, typename TpIntegral>
TpChar* cstr_cut(TpChar* str, TpIntegral val, bool erase_val = false) noexcept;

template<typename TpChar, typename TpIntegral>
TpChar* cstr_cut(TpChar* str, TpIntegral val, bool erase_val, std::size_t cnt) noexcept;

template<typename TpChar, typename TpPredicate>
TpChar* cstr_remove_if(TpChar* str, TpPredicate pred) noexcept;

template<typename TpChar, typename TpPredicate>
TpChar* cstr_remove_if(TpChar* str, TpPredicate pred, std::size_t cnt) noexcept;

template<typename TpChar, typename TpPredicate>
TpChar* cstr_remove_trailing_if(TpChar* str, TpPredicate pred) noexcept;

template<typename TpChar, typename TpPredicate>
TpChar* cstr_remove_trailing_if(TpChar* str, TpPredicate pred, std::size_t cnt) noexcept;

template<
        typename TpChar,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAllocator = std::allocator<TpChar>
>
[[nodiscard]] auto escape_regex(const TpChar* str) noexcept;

template<typename TpChar, typename TpCharTraits, typename TpAllocator>
[[nodiscard]] auto escape_regex(
        const std::basic_string<TpChar, TpCharTraits, TpAllocator>& str
) noexcept;

template<typename TpChar, typename TpCharTraits, typename TpCharAlloc>
bool escape_regex_implace(std::basic_string<TpChar, TpCharTraits, TpCharAlloc>& str) noexcept;

template<typename TpChar>
[[nodiscard]] inline std::size_t get_length(const TpChar* str) noexcept;

template<StringLike TpString>
[[nodiscard]] inline auto get_length(const TpString& str) noexcept;

template<typename TpSourceChar, typename TpPatternChar>
[[nodiscard]] bool match_wildcard(
        const TpSourceChar* str,
        const TpPatternChar* pattrn
) noexcept;

template<StringLike TpSourceString, typename TpPatternChar>
[[nodiscard]] bool match_wildcard(
        const TpSourceString& str,
        const TpPatternChar* pattrn
) noexcept;

template<StringLike TpSourceString, StringLike TpPatternString>
[[nodiscard]] bool match_wildcard(
        const TpSourceString& str,
        const TpPatternString& pattrn
) noexcept;

template<typename TpSourceChar, typename TpPatternChar>
[[nodiscard]] bool match_wildcard_icase(
        const TpSourceChar* str,
        const TpPatternChar* pattrn
) noexcept;

template<StringLike TpSourceString, typename TpPatternChar>
[[nodiscard]] bool match_wildcard_icase(
        const TpSourceString& str,
        const TpPatternChar* pattrn
) noexcept;

template<StringLike TpSourceString, StringLike TpPatternString>
[[nodiscard]] bool match_wildcard_icase(
        const TpSourceString& str,
        const TpPatternString& pattrn
) noexcept;

template<
        typename TpSourceChar,
        typename TpSeparatorChar,
        typename TpCharTraits = std::char_traits<TpSourceChar>,
        typename TpAllocator = std::allocator<TpSourceChar>,
        typename TpIntegral = char
>
[[nodiscard]] auto split(
        const TpSourceChar* str,
        const TpSeparatorChar* seps,
        TpIntegral escape_ch = '\\'
);

template<
        StringLike TpSourceString,
        typename TpSeparatorChar,
        typename TpCharTraits = std::char_traits<typename TpSourceString::value_type>,
        typename TpAllocator = std::allocator<typename TpSourceString::value_type>,
        typename TpIntegral = char
>
[[nodiscard]] auto split(
        const TpSourceString& str,
        const TpSeparatorChar* seps,
        TpIntegral escape_ch = '\\'
);

template<
        StringLike TpSourceString,
        StringLike TpSeparatorString,
        typename TpCharTraits = std::char_traits<typename TpSourceString::value_type>,
        typename TpAllocator = std::allocator<typename TpSourceString::value_type>,
        typename TpIntegral = char
>
[[nodiscard]] auto split(
        const TpSourceString& str,
        const TpSeparatorString& seps,
        TpIntegral escape_ch = '\\'
);

template<typename TpChar>
[[nodiscard]] TpChar to_lower(TpChar ch);

template<
        typename TpChar,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAllocator = std::allocator<TpChar>
>
[[nodiscard]] auto to_lower(const TpChar* str);

template<
        StringLike TpString,
        typename TpCharTraits = std::char_traits<typename TpString::value_type>,
        typename TpAllocator = std::allocator<typename TpString::value_type>
>
[[nodiscard]] auto to_lower(const TpString& str);

template<typename TpChar, typename TpCharTraits, typename TpAllocator>
bool to_lower_inplace(std::basic_string<TpChar, TpCharTraits, TpAllocator>& str);

template<typename TpChar>
[[nodiscard]] TpChar to_upper(TpChar ch);

template<
        typename TpChar,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAllocator = std::allocator<TpChar>
>
[[nodiscard]] auto to_upper(const TpChar* str);

template<
        StringLike TpString,
        typename TpCharTraits = std::char_traits<typename TpString::value_type>,
        typename TpAllocator = std::allocator<typename TpString::value_type>
>
[[nodiscard]] auto to_upper(const TpString& str);

template<typename TpChar, typename TpCharTraits, typename TpAllocator>
bool to_upper_inplace(std::basic_string<TpChar, TpCharTraits, TpAllocator>& str);

}

#endif

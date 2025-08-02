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
 * @file        forward_declarations.hpp
 * @brief       forward_declarations header.
 * @author      Killian Valverde
 * @date        2024/11/16
 */

#ifndef SPEED_STRINGUTILS_DETAIL_FORWARD_DECLARATIONS_HPP
#define SPEED_STRINGUTILS_DETAIL_FORWARD_DECLARATIONS_HPP

#include <cwctype>
#include <system_error>
#include <type_traits>

namespace speed::stringutils {

/** @cond */
template<typename DestinationCharT, typename SourceCharT>
DestinationCharT* cstr_copy(DestinationCharT* dest, const SourceCharT* src) noexcept;

template<typename DestinationCharT, typename SourceCharT>
DestinationCharT* cstr_copy(
        DestinationCharT* dest,
        const SourceCharT* src,
        std::size_t cnt
) noexcept;

template<typename DestinationCharT, typename SourceCharT>
DestinationCharT* cstr_concat(DestinationCharT* dest, const SourceCharT* src) noexcept;

template<typename DestinationCharT, typename SourceCharT>
DestinationCharT* cstr_concat(
        DestinationCharT* dest,
        const SourceCharT* src,
        std::size_t cnt
) noexcept;

template<typename CharT>
[[nodiscard]] std::size_t cstr_length(const CharT* str) noexcept;

template<typename SourceCharT, typename TargetCharT>
[[nodiscard]] int cstr_compare(const SourceCharT* src, const TargetCharT* trg) noexcept;

template<typename SourceCharT, typename TargetCharT>
[[nodiscard]] int cstr_compare(
        const SourceCharT* src,
        const TargetCharT* trg,
        std::size_t cnt
) noexcept;

template<typename SourceCharT, typename TargetCharT>
[[nodiscard]] int cstr_compare_icase(
        const SourceCharT* src,
        const TargetCharT* trg
) noexcept;

template<typename SourceCharT, typename TargetCharT>
[[nodiscard]] int cstr_compare_icase(
        const SourceCharT* src,
        const TargetCharT* trg,
        std::size_t cnt
) noexcept;

template<typename CharT, typename IntegralT>
[[nodiscard]] CharT* cstr_find_first_char(CharT* str, IntegralT val) noexcept;

template<typename CharT, typename IntegralT>
[[nodiscard]] CharT* cstr_find_last_char(CharT* str, IntegralT val) noexcept;

template<typename SourceCharT, typename PrefixCharT>
[[nodiscard]] std::size_t cstr_count_prefix_in(
        const SourceCharT* str,
        const PrefixCharT* prefxs
) noexcept;

template<typename SourceCharT, typename PrefixCharT>
[[nodiscard]] std::size_t cstr_count_prefix_not_in(
        const SourceCharT* str,
        const PrefixCharT* prefxs
) noexcept;

template<typename SourceCharT, typename AcceptedCharT>
[[nodiscard]] SourceCharT* cstr_find_any_of(
        SourceCharT* str,
        const AcceptedCharT* accpt
) noexcept;

template<typename SourceCharT, typename SubstringCharT>
[[nodiscard]] SourceCharT* cstr_find_substr(
        SourceCharT* str,
        const SubstringCharT* substr
) noexcept;

template<typename SourceCharT, typename SubstringCharT>
[[nodiscard]] SourceCharT* cstr_find_substr_icase(
        SourceCharT* str,
        const SubstringCharT* substr
) noexcept;

template<typename SourceCharT, typename DelimiterCharT>
[[nodiscard]] SourceCharT* cstr_tokenize_any_of(
        SourceCharT*& str,
        const DelimiterCharT* delimitrs
) noexcept;

template<typename CharT, typename IntegralT>
CharT* cstr_cut(CharT* str, IntegralT val, bool erase_val = false) noexcept;

template<typename CharT, typename IntegralT>
CharT* cstr_cut(CharT* str, IntegralT val, bool erase_val, std::size_t cnt) noexcept;

template<typename CharT, typename PredicateT>
CharT* cstr_remove_if(CharT* str, PredicateT pred) noexcept;

template<typename CharT, typename PredicateT>
CharT* cstr_remove_if(CharT* str, PredicateT pred, std::size_t cnt) noexcept;

template<typename CharT, typename PredicateT>
CharT* cstr_remove_trailing_if(CharT* str, PredicateT pred) noexcept;

template<typename CharT, typename PredicateT>
CharT* cstr_remove_trailing_if(CharT* str, PredicateT pred, std::size_t cnt) noexcept;

template<typename StringT>
[[nodiscard]] auto escape_regex(const StringT& str);

template<typename CharT, typename CharTraitsT, typename AllocatorT>
bool escape_regex_inplace(std::basic_string<CharT, CharTraitsT, AllocatorT>& str);

template<typename StringT>
[[nodiscard]] inline std::size_t get_length(const StringT& str) noexcept;

template<typename StringT>
[[nodiscard]] constexpr bool is_empty(const StringT& str) noexcept;

template<typename StringT>
[[nodiscard]] constexpr bool is_null(const StringT& str) noexcept;

template<typename SourceStringT, typename PatternStringT>
[[nodiscard]] bool match_wildcard(
        const SourceStringT& str,
        const PatternStringT& pattrn
) noexcept;

template<typename SourceStringT, typename PatternStringT>
[[nodiscard]] bool match_wildcard_icase(
        const SourceStringT& str,
        const PatternStringT& pattrn
) noexcept;

template<typename StringT1, typename StringT2>
[[nodiscard]] auto split(const StringT1& str, const StringT2& seps);

template<typename CharT>
requires std::is_integral_v<CharT>
[[nodiscard]] CharT to_lower(CharT ch);

template<typename StringT>
requires (!std::is_integral_v<StringT>)
[[nodiscard]] auto to_lower(const StringT& str);

template<typename CharT, typename CharTraitsT, typename AllocatorT>
bool to_lower_inplace(std::basic_string<CharT, CharTraitsT, AllocatorT>& str);

template<typename CharT>
requires std::is_integral_v<CharT>
[[nodiscard]] CharT to_upper(CharT ch);

template<typename StringT>
requires (!std::is_integral_v<StringT>)
[[nodiscard]] auto to_upper(const StringT& str);

template<typename CharT, typename CharTraitsT, typename AllocatorT>
bool to_upper_inplace(std::basic_string<CharT, CharTraitsT, AllocatorT>& str);
/** @endcond */

}

#endif

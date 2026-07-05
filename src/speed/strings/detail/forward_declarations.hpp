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
 * @file forward_declarations.hpp
 * @brief Forward declarations for the strings module.
 * @author Killian Valverde
 * @date 2024-11-16
 */

#pragma once

#include <string>

#include "../../type_traits/type_traits.hpp"

namespace speed::strings {

/** @cond */
template<typename CharT>
[[nodiscard]] constexpr std::size_t cstr_length(const CharT* str) noexcept;

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

template<typename StringT>
[[nodiscard]] auto escape_regex(const StringT& str);

template<typename CharT, typename CharTraitsT, typename AllocatorT>
bool escape_regex_inplace(std::basic_string<CharT, CharTraitsT, AllocatorT>& str);

template<typename StringT>
[[nodiscard]] constexpr std::size_t get_length(const StringT& str) noexcept;

template<typename StringT>
[[nodiscard]] constexpr bool is_empty(const StringT& str) noexcept;

template<typename StringT>
[[nodiscard]] constexpr bool is_null(const StringT& str) noexcept;

template<typename SourceStringT, typename PatternStringT>
[[nodiscard]] bool match_wildcard(
    const SourceStringT& str,
    const PatternStringT& pattern
) noexcept;

template<typename SourceStringT, typename PatternStringT>
[[nodiscard]] bool match_wildcard_icase(
    const SourceStringT& str,
    const PatternStringT& pattern
) noexcept;

template<typename StringT1, typename StringT2>
[[nodiscard]] auto split(const StringT1& str, const StringT2& seps);

template<typename CharT>
    requires type_traits::is_character_v<CharT>
[[nodiscard]] constexpr CharT to_lower(CharT ch) noexcept;

template<typename StringT>
    requires (!type_traits::is_character_v<StringT>)
[[nodiscard]] auto to_lower(const StringT& str);

template<typename CharT, typename CharTraitsT, typename AllocatorT>
bool to_lower_inplace(std::basic_string<CharT, CharTraitsT, AllocatorT>& str);

template<typename CharT>
    requires type_traits::is_character_v<CharT>
[[nodiscard]] constexpr CharT to_upper(CharT ch) noexcept;

template<typename StringT>
    requires (!type_traits::is_character_v<StringT>)
[[nodiscard]] auto to_upper(const StringT& str);

template<typename CharT, typename CharTraitsT, typename AllocatorT>
bool to_upper_inplace(std::basic_string<CharT, CharTraitsT, AllocatorT>& str);
/** @endcond */

}

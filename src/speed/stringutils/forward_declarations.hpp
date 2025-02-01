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
 * @file        speed/stringutils/forward_declarations.hpp
 * @brief       forward_declarations header.
 * @author      Killian Valverde
 * @date        2024/11/16
 */

#ifndef SPEED_STRINGUTILS_FORWARD_DECLARATIONS_HPP
#define SPEED_STRINGUTILS_FORWARD_DECLARATIONS_HPP

#include <cwctype>
#include <system_error>
#include <type_traits>


namespace speed::stringutils {


template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar* strcpy(TpDestinationChar* dest, const TpSourceChar* src) noexcept;

template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar* strncpy(
        TpDestinationChar* dest,
        const TpSourceChar* src,
        std::size_t nbr
) noexcept;

template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar* strcat(TpDestinationChar* dest, const TpSourceChar* src) noexcept;

template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar* strncat(
        TpDestinationChar* dest,
        const TpSourceChar* src,
        std::size_t nbr
) noexcept;

template<typename TpChar>
[[nodiscard]] std::size_t strlen(const TpChar* str) noexcept;

template<typename TpChar, typename TpCharTraits, typename TpCharAlloc>
[[nodiscard]] auto strlen(
        const std::basic_string<TpChar, TpCharTraits, TpCharAlloc>& str
) noexcept;

template<typename TpSourceChar, typename TpTargetChar>
[[nodiscard]] int strcmp(const TpSourceChar* src, const TpTargetChar* trg) noexcept;

template<typename TpSourceChar, typename TpTargetChar>
[[nodiscard]] int strncmp(
        const TpSourceChar* src,
        const TpTargetChar* trg,
        std::size_t nbr
) noexcept;

template<typename TpChar, typename TpIntegral>
[[nodiscard]] TpChar* strchr(TpChar* str, TpIntegral val) noexcept;

template<typename TpChar, typename TpIntegral>
[[nodiscard]] TpChar* strnchr(TpChar* str, TpIntegral val, std::size_t nbr) noexcept;

template<typename TpChar, typename TpIntegral>
[[nodiscard]] TpChar* strrchr(TpChar* str, TpIntegral val) noexcept;

template<typename TpChar, typename TpIntegral>
[[nodiscard]] TpChar* strrnchr(TpChar* str, TpIntegral val, std::size_t nbr) noexcept;

template<typename TpChar>
const TpChar* strstr(const TpChar* str, const TpChar* substr);

template<typename TpChar, typename TpIntegral>
TpChar* strcut(TpChar* str, TpIntegral val, bool erase_val = false) noexcept;

template<typename TpChar>
[[nodiscard]] std::basic_string<TpChar> strescregexchars(const TpChar* str) noexcept;

template<typename TpChar, typename TpCharTraits, typename TpCharAlloc>
[[nodiscard]] auto strescregexchars(
        const std::basic_string<TpChar, TpCharTraits, TpCharAlloc>& str
) noexcept;

template<typename TpChar, typename TpIntegral>
TpChar* strrmbelow(TpChar* str, TpIntegral val) noexcept;

template<typename TpChar, typename TpIntegral>
bool strrmlast(TpChar* str, TpIntegral val) noexcept;

template<
        typename TpChar,
        typename TpIntegral,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAllocator = std::allocator<TpChar>
>
[[nodiscard]] auto strsplit(const TpChar* str, TpIntegral sep);

template<
        typename TpChar,
        typename TpIntegral,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAllocator = std::allocator<TpChar>
>
[[nodiscard]] auto strsplit(
        const std::basic_string<TpChar, TpCharTraits, TpAllocator>&  str,
        TpIntegral sep
);

template<typename TpChar>
TpChar strtolower(TpChar ch);

template<typename TpChar, typename TpCharTraits, typename TpCharAlloc>
std::basic_string<TpChar, TpCharTraits, TpCharAlloc> strtolower(
        const std::basic_string<TpChar, TpCharTraits, TpCharAlloc>& str
);

template<typename TpChar1, typename TpChar2>
[[nodiscard]] bool strwildmatch(
        const TpChar1* str,
        const TpChar2* pattrn
) noexcept;


}


#endif

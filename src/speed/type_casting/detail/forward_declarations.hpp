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
 * @brief Internal forward declarations for the type_casting module.
 * @author Killian Valverde
 * @date 2024-10-15
 */

#pragma once

#include <system_error>

namespace speed::type_casting {

namespace detail {

template<typename CharT, typename CharTraitsT, typename AllocatorT>
bool try_type_cast(
    const CharT* arg,
    std::basic_string<CharT, CharTraitsT, AllocatorT>& res,
    std::error_code* err_code
) noexcept;

template<typename CharTraitsT, typename AllocatorT>
bool try_type_cast(
    const char* arg,
    std::basic_string<wchar_t, CharTraitsT, AllocatorT>& res,
    std::error_code* err_code
) noexcept;

template<typename CharTraitsT, typename AllocatorT>
bool try_type_cast(
    const wchar_t* arg,
    std::basic_string<char, CharTraitsT, AllocatorT>& res,
    std::error_code* err_code
) noexcept;

}

template<typename TargetT, typename SourceT>
bool try_type_cast(const SourceT&, TargetT&, std::error_code* = nullptr) noexcept;

template<typename TargetT, typename SourceT>
TargetT type_cast(const SourceT&);

template<typename TargetT, typename SourceT, typename InvocableT>
    requires std::invocable<InvocableT> &&
        std::convertible_to<std::invoke_result_t<InvocableT>, TargetT>
TargetT type_cast_or(const SourceT& arg, InvocableT&& callabl);

template<typename SourceT>
std::string to_utf8(const SourceT& arg);

}

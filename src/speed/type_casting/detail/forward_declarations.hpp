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
 * @date        2024/10/15
 */

#ifndef SPEED_TYPE_CASTING_DETAIL_FORWARD_DECLARATIONS_HPP
#define SPEED_TYPE_CASTING_DETAIL_FORWARD_DECLARATIONS_HPP

#include <system_error>

namespace speed::type_casting {

enum class errc : std::uint8_t;

class error_category;

template<typename TargetT, typename SourceT>
bool try_type_cast(const SourceT&, TargetT&, std::error_code* = nullptr) noexcept;

template<typename TargetT, typename SourceT>
TargetT type_cast(const SourceT&);

template<typename TargetT, typename SourceT, typename DefaultValueT>
TargetT type_cast(const SourceT&, DefaultValueT&&) noexcept;

}

#endif

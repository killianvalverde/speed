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
 * @brief       filesystem functions header.
 * @author      Killian Valverde
 * @date        2024/11/09
 */

#ifndef SPEED_FILESYSTEM_OPERATIONS_HPP
#define SPEED_FILESYSTEM_OPERATIONS_HPP

namespace speed::filesystem {

/**
 * @brief       Normalizes a filesystem path by replacing alternative slash characters.
 * @param       pth : The input filesystem path.
 * @return      A normalized filesystem path with consistent slash separators.
 */
inline std::filesystem::path get_normalized_path(const std::filesystem::path& pth)
{
    auto path_str = pth.native();
    
    std::replace(path_str.begin(), path_str.end(), SPEED_SYSTEM_FILESYSTEM_ALT_SLASH_CHAR,
                 SPEED_SYSTEM_FILESYSTEM_SLASH_CHAR);
    
    return {std::move(path_str)};
}

/**
 * @brief       Selects the appropriate path string based on the filesystem's character type.
 * @param       lhs : First string candidate.
 * @param       rhs : Second string candidate.
 * @return      Reference to the selected string.
 */
template<typename StringT1, typename StringT2>
constexpr auto& pick_path(const StringT1& lhs, const StringT2& rhs)
{
    return stringutils::pick_by_char<system::filesystem::path_char_t>(lhs, rhs);
}

}

#endif

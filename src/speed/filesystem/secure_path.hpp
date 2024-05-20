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
 * @file        speed/filesystem/secure_path.hpp
 * @brief       secure_path main header.
 * @author      Killian Valverde
 * @date        2024/05/20
 */

#ifndef SPEED_FILESYSTEM_SECURE_PATH_HPP
#define SPEED_FILESYSTEM_SECURE_PATH_HPP

#include <filesystem>
#include <system_error>


namespace speed::filesystem {


/**
 * @brief       Path class that allows performing validity checks throught decorators.
 */
class secure_path : public std::filesystem::path
{
public:
    /** Inherits all the path constructors. */
    using std::filesystem::path::path;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] virtual inline bool is_valid(std::error_code* err_code = nullptr)
    {
        return true;
    }
};


}


#endif

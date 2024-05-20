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
 * @file        speed/filesystem/regular_file_path_decorator.hpp
 * @brief       regular_file_path_decorator main header.
 * @author      Killian Valverde
 * @date        2024/05/20
 */

#ifndef SPEED_FILESYSTEM_REGULAR_FILE_PATH_DECORATOR_HPP
#define SPEED_FILESYSTEM_REGULAR_FILE_PATH_DECORATOR_HPP

#include <filesystem>
#include <system_error>

#include "../system/system.hpp"


namespace speed::filesystem {


/**
 * @brief       Path class decorator that checks whether the path is a regular file.
 */
template<typename TpComponent>
class regular_file_path_decorator : public TpComponent
{
public:
    /** Component type. */
    using TpComponent::TpComponent;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        if (speed::system::filesystem::is_regular_file(TpComponent::c_str(), err_code))
        {
            return TpComponent::is_valid(err_code);
        }

        return false;
    }
};


}


#endif

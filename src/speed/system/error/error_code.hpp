/* speed - Generic C++ library.
 * Copyright (C) 2015-2023 Killian Valverde.
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
 * @file        speed/system/error/error_code.hpp
 * @brief       system_error_code functions header.
 * @author      Killian Green
 * @date        2017/11/02 - 18:39
 */

#ifndef SPEED_SYSTEM_ERROR_ERROR_CODE_HPP
#define SPEED_SYSTEM_ERROR_ERROR_CODE_HPP

#include <system_error>


namespace speed::system::error {


/**
 * @brief       Assign the value to the error_code if it is not null using a system category.
 * @param       value : The value to assign.
 * @param       err_code : The object that will contain the error.
 */
inline void assign_system_error_code(int value, std::error_code* err_code)
{
    if (err_code != nullptr)
    {
        err_code->assign(value, std::system_category());
    }
}


/**
 * @brief       Assign the value to the error_code if it is not null using a generic category.
 * @param       value : The value to assign.
 * @param       err_code : The object that will contain the error.
 */
inline void assign_generic_error_code(int value, std::error_code* err_code)
{
    if (err_code != nullptr)
    {
        err_code->assign(value, std::generic_category());
    }
}


}


#endif

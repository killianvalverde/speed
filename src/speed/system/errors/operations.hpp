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
 * @file        operations.hpp
 * @brief       errors operations functions header.
 * @author      Killian Valverde
 * @date        2017/11/02
 */

#ifndef SPEED_SYSTEM_ERRORS_OPERATIONS_HPP
#define SPEED_SYSTEM_ERRORS_OPERATIONS_HPP

#include <concepts>
#include <system_error>

namespace speed::system::errors {

/**
 * @brief       Assign the value to the error_code if it is not null using a system category.
 * @param       val : The value to assign.
 * @param       err_code : The object that will contain the error.
 */
template <std::integral IntegralT>
void assign_system_error_code(IntegralT val, std::error_code* err_code)
{
    if (err_code != nullptr)
    {
        err_code->assign(static_cast<int>(val), std::system_category());
    }
}

/**
 * @brief       Assign the value to the error_code if it is not null using a generic category.
 * @param       val : The value to assign.
 * @param       err_code : The object that will contain the error.
 */
template <std::integral IntegralT>
void assign_generic_error_code(IntegralT val, std::error_code* err_code)
{
    if (err_code != nullptr)
    {
        err_code->assign(static_cast<int>(val), std::generic_category());
    }
}

inline void assign_errc(std::errc ec, std::error_code* err_code)
{
    if (err_code != nullptr)
    {
        *err_code = std::make_error_code(ec);
    }
}

inline void assign_error_code(const std::error_code& ec_input, std::error_code* ec_output)
{
    if (ec_output != nullptr)
    {
        *ec_output = ec_input;
    }
}

}

#endif

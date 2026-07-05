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
 * @file operations.hpp
 * @brief Core operations for the system::errors submodule.
 * @author Killian Valverde
 * @date 2017-11-02
 */

#pragma once

#include <concepts>
#include <system_error>

namespace speed::system::errors {

/**
 * @brief Assigns a system-category error code.
 *
 * Assigns the specified integral error value to the output error code using
 * `std::system_category()`. If `err_code` is `nullptr`, no action is performed.
 *
 * @tparam IntegralT Integral type containing the error value.
 *
 * @param val Error value to assign.
 * @param err_code Pointer to the destination error code, or `nullptr`.
 */
template<std::integral IntegralT>
void assign_system_error_code(IntegralT val, std::error_code* err_code) noexcept
{
    if (err_code != nullptr)
    {
        err_code->assign(static_cast<int>(val), std::system_category());
    }
}

/**
 * @brief Assigns a generic-category error code.
 *
 * Assigns the specified integral error value to the output error code using
 * `std::generic_category()`. If `err_code` is `nullptr`, no action is performed.
 *
 * @tparam IntegralT Integral type containing the error value.
 *
 * @param val Error value to assign.
 * @param err_code Pointer to the destination error code, or `nullptr`.
 */
template<std::integral IntegralT>
void assign_generic_error_code(IntegralT val, std::error_code* err_code) noexcept
{
    if (err_code != nullptr)
    {
        err_code->assign(static_cast<int>(val), std::generic_category());
    }
}

/**
 * @brief Assigns an errno-based error code.
 *
 * Assigns the specified errno value to the output error code using `std::generic_category()`.
 * If `err_code` is `nullptr`, no action is performed.
 *
 * @tparam IntegralT Integral type containing the errno value.
 *
 * @param val errno value to assign.
 * @param err_code Pointer to the destination error code, or `nullptr`.
 */
template<std::integral IntegralT>
void assign_errno_error_code(IntegralT val, std::error_code* err_code) noexcept
{
    assign_generic_error_code(val, err_code);
}

/**
 * @brief Assigns an error code from a standard error condition.
 *
 * Creates an error code corresponding to the specified `std::errc` value and assigns it to
 * the output error code. If `err_code` is `nullptr`, no action is performed.
 *
 * @param ec Standard error condition.
 * @param err_code Pointer to the destination error code, or `nullptr`.
 */
inline void assign_errc(std::errc ec, std::error_code* err_code) noexcept
{
    if (err_code != nullptr)
    {
        *err_code = std::make_error_code(ec);
    }
}

/**
 * @brief Copies an error code.
 *
 * Copies the specified error code into the output error code. If `ec_output` is `nullptr`,
 * no action is performed.
 *
 * @param ec_input Source error code.
 * @param ec_output Pointer to the destination error code, or `nullptr`.
 */
inline void assign_error_code(const std::error_code& ec_input, std::error_code* ec_output) noexcept
{
    if (ec_output != nullptr)
    {
        *ec_output = ec_input;
    }
}

}

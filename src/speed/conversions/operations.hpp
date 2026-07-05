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
 * @brief Core operations for the conversions module.
 * @author Killian Valverde
 * @date 2016-08-19
 */

#pragma once

#include "detail/forward_declarations.hpp"
#include "detail/operations.hpp"
#include "exception.hpp"

namespace speed::conversions {

/**
 * @brief Converts a value to the specified target type.
 *
 * @tparam TargetT Target type of the conversion.
 * @tparam SourceT Source type of the value.
 *
 * @param arg Value to convert.
 * @return The converted value.
 * @throws exception if the conversion fails.
 */
template<typename TargetT, typename SourceT>
TargetT convert(const SourceT& arg)
{
    std::error_code ec;
    TargetT res{};

    if (try_convert(arg, res, &ec))
    {
        return res;
    }

    throw exception(ec);
}

/**
 * @brief Converts a value to the specified target type or returns a fallback.
 *
 * @tparam TargetT Target type of the conversion.
 * @tparam SourceT Source type of the value.
 * @tparam CallableT Callable type returning a fallback value.
 *
 * @param arg Value to convert.
 * @param callabl Callable object invoked to produce a fallback value if the conversion fails.
 * @return The converted value on success, or the result of the fallback callable otherwise.
 */
template<typename TargetT, typename SourceT, typename CallableT>
    requires std::invocable<CallableT> &&
    std::convertible_to<std::invoke_result_t<CallableT>, TargetT>
TargetT convert_or(const SourceT& arg, CallableT&& callabl)
{
    std::error_code ec;
    TargetT res{};

    if (try_convert(arg, res, &ec))
    {
        return res;
    }

    return static_cast<TargetT>(std::invoke(std::forward<CallableT>(callabl)));
}

/**
 * @brief Converts a value to a UTF-8 encoded string.
 *
 * @tparam SourceT Type of the source value.
 *
 * @param arg Value to convert.
 * @return A UTF-8 encoded std::string representation of the input.
 */
template<typename SourceT>
std::string to_utf8(const SourceT& arg)
{
    return convert<std::string>(arg);
}

/**
 * @brief Attempts to convert a value to the specified target type.
 *
 * @tparam TargetT Target type of the conversion.
 * @tparam SourceT Source type of the value.
 *
 * @param arg Value to convert.
 * @param res Output parameter receiving the converted value on success.
 * @param err_code Optional pointer to an error code receiving failure details.
 * @return true if the conversion succeeded, false otherwise.
 */
template<typename TargetT, typename SourceT>
bool try_convert(
    const SourceT& arg,
    TargetT& res,
    std::error_code* err_code
) noexcept
{
    return detail::try_convert(arg, res, err_code);
}

}

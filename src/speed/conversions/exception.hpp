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
 * @file exception.hpp
 * @brief Defines the exception type used by the conversions module.
 * @author Killian Valverde
 * @date 2016-08-19
 */

#pragma once

#include "detail/forward_declarations.hpp"
#include "../exceptions/exception.hpp"

namespace speed::conversions {

/**
 * @brief Exception thrown on type conversions failure.
 *
 * @details This exception is raised when a type conversion operation fails.
 * It extends speed::exceptions::source_location_exception to provide additional context such as
 * source location and error code.
 */
class exception : public exceptions::source_location_exception
{
public:
    /**
     * @brief Constructs a conversions_exception.
     *
     * @param ec Error code describing the failure.
     */
    explicit exception(std::error_code ec)
        : source_location_exception("conversion failed", ec)
    {
    }
};

}

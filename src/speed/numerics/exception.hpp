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
 * @brief xxxxx
 * @author Killian Valverde
 * @date 2017-10-20
 */

#pragma once

#include "../exceptions/exception.hpp"

namespace speed::numerics {

/**
 * @brief Base exception type for the numerics module.
 *
 * This exception serves as the root of the numerics exception hierarchy.
 * It is thrown to report runtime errors related to numerical operations and can be used to
 * catch any exception originating from the numerics module.
 */
class exception : public exceptions::runtime_exception_base
{
public:
    /**
     * @brief Inherit constructors from the base exception type.
     */
    using runtime_exception_base::runtime_exception_base;
};

/**
 * @brief Exception thrown when an arithmetic overflow occurs.
 *
 * This exception is used to indicate that the result of a numerical operation cannot be
 * represented by the destination type because it exceeds its maximum representable value.
 */
class overflow_exception : public exception
{
public:
    overflow_exception()
        : exception("overflow exception")
    {
    }
};

}

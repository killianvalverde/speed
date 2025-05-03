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
 * @file        error_category.hpp
 * @brief       error_category main header.
 * @author      Killian Valverde
 * @date        2024/04/16
 */

#ifndef SPEED_TYPE_CASTING_ERROR_CATEGORY_HPP
#define SPEED_TYPE_CASTING_ERROR_CATEGORY_HPP

#include <system_error>

#include "../errors/errors.hpp"
#include "error_conditions.hpp"

namespace speed::type_casting {

/**
 * @brief       Error category class for the type_casting module.
 */
class error_category : public std::error_category
{
public:
    /**
     * @brief       Allows gettin the name of the category.
     * @return      The name of the category.
     */
    [[nodiscard]] inline const char* name() const noexcept override
    {
        return "type casting error category";
    }

    /**
     * @brief       Allows getting the error condition asigned to the value.
     * @param       val : Value used to get the error condition.
     * @return      The error condition.
     */
    [[nodiscard]] std::error_condition default_error_condition(int val) const noexcept override;

    /**
     * @brief       Allows knowing if the error code specified is equivalent to a specified
     *              condition.
     * @param       err_code : The error code to check.
     * @param       cond : The condition.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool equivalent(
            const std::error_code& err_code, int cond
    ) const noexcept override
    {
        return *this == err_code.category() && 
                static_cast<int>(default_error_condition(err_code.value()).value()) == cond;
    }

    /**
     * @brief       Allows getting the a string describing the specified error code value.
     * @param       val : The error code value.
     * @return      The string describing the error code value.
     */
    [[nodiscard]] std::string message(int val) const override;
};

/** @cond */
namespace __private {
extern error_category error_category_obj;
} /* __private */
/** @endcond */

/**
 * @brief       Assign the value to the error_code if it is not null using a system category.
 * @param       value : The value to assign.
 * @param       err_code : The object that will contain the error.
 */
inline void assign_type_casting_error_code(int value, std::error_code* err_code)
{
    if (err_code != nullptr)
    {
        err_code->assign(value, __private::error_category_obj);
    }
}

/**
 * @brief       Allows getting an error condition from the error condition enum.
 * @param       errc : Error condition enul object.
 * @return      The error condition.
 */
inline std::error_condition make_error_condition(speed::type_casting::error_conditions errc)
{
    return {static_cast<int>(errc), __private::error_category_obj};
}

}

#endif

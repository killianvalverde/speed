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
 * @file        error_category.hpp
 * @brief       error_category main header.
 * @author      Killian Valverde
 * @date        2024/04/16
 */

#ifndef SPEED_TYPE_CASTING_ERROR_CATEGORY_HPP
#define SPEED_TYPE_CASTING_ERROR_CATEGORY_HPP

#include <cstdint>
#include <system_error>

namespace speed::type_casting {

/** @cond */
namespace detail {

enum class error_code_value : std::uint8_t
{
    NIL = 0x0,
    ARITHMETIC_CODES_BEGINS = 0x1,
    ARITHMETIC_CONVERSION_FAILS = 0x1,
    ARITHMETIC_INVALID_SYNTAX = 0x2,
    ARITHMETIC_OVERFLOW_RANGE = 0x3,
    ARITHMETIC_UNDERFLOW_RANGE = 0x4,
    ARITHMETIC_CODES_ENDS = 0x4,
    SYSTEM_CODES_BEGINS = 0x5,
    FILESYSTEM_CODES_BEGINS = 0x5,
    FILESYSTEM_INVALID_PATH = 0x6,
    FILESYSTEM_CODES_ENDS = 0x6,
    SYSTEM_CODES_ENDS = 0x6,
    OTHERS_BEGINS = 0x7,
    RANGE_ERROR = 0x7,
    OTHER = 0x8,
    OTHERS_ENDS = 0x8
};

}
/** @endcond */

/**
 * @brief       The type casting error condition enum.
 */
enum class errc : std::uint8_t
{
    /** Null condition. */
    NIL = 0x0,
    
    /** Arithmetic error. */
    ARITHMETIC_ERROR = 0x1,
    
    /** System error. */
    SYSTEM_ERROR = 0x2,
    
    /** Other error. */
    OTHER = 0x3
};

}

namespace std {
template<> struct is_error_condition_enum<speed::type_casting::errc>
        : public std::true_type
{
};
}

namespace speed::type_casting {

/**
 * @brief       Error category class for the type_casting module.
 */
class error_category : public std::error_category
{
public:
    /**
     * @brief Deleted copy constructor.
     */
    error_category(const error_category&) = delete;

    /**
     * @brief Deleted move constructor.
     */
    error_category(error_category&&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    error_category& operator=(const error_category&) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    error_category& operator=(error_category&&) = delete;

    /**
     * @brief Provides access to the singleton instance of error_category.
     * @return Reference to the single error_category instance.
     */
    static error_category& instance()
    {
        static error_category obj;
        return obj;
    }
    
    /**
     * @brief       Allows gettin the name of the category.
     * @return      The name of the category.
     */
    [[nodiscard]] constexpr const char* name() const noexcept override
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
    [[nodiscard]] bool equivalent(
            const std::error_code& err_code,
            int cond
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
    
private:
    /**
     * @brief Default constructor.
     */
    error_category() = default;

    /**
     * @brief Default destructor.
     */
    ~error_category() override = default;
};

/** @cond */
namespace detail {

inline void assign_error_code(error_code_value ec, std::error_code* err_code)
{
    if (err_code != nullptr)
    {
        err_code->assign(static_cast<int>(ec), error_category::instance());
    }
}

}
/** @endcond */

/**
 * @brief       Allows getting an error condition from the error condition enum.
 * @param       errc : Error condition enul object.
 * @return      The error condition.
 */
inline std::error_condition make_error_condition(type_casting::errc errc)
{
    return {static_cast<int>(errc), error_category::instance()};
}

}

#endif

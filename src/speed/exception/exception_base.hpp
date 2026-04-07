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
 * @file        exception_base.hpp
 * @brief       exception_base class header.
 * @author      Killian Valverde
 * @date        2016/08/19
 */

#ifndef SPEED_EXCEPTIONS_EXCEPTION_BASE_HPP
#define SPEED_EXCEPTIONS_EXCEPTION_BASE_HPP

#include <source_location>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>

namespace speed::exception {

/**
 * @brief       Base class for all custom exceptions in the library.
 */
class exception_base
{
public:
    /**
     * @brief       Virtual destructor.
     */
    virtual ~exception_base() = default;
};

/**
 * @brief       Base class for logic-related exceptions. Represents errors in the program logic
 *              such as invalid arguments, violated preconditions, or inconsistent internal state.
 *              These exceptions typically indicate bugs in the code and are not meant to be
 *              recovered from at runtime.
 */
class logic_exception_base : public std::logic_error, public exception_base
{
public:
    /**
     * @brief       Inherit constructors from std::logic_error.
     */
    using std::logic_error::logic_error;
};

/**
 * @brief       Base class for runtime-related exceptions. Represents errors that occur due to
 *              external conditions such as I/O failures, resource unavailability, or system errors.
 *              These exceptions are typically recoverable and should be handled appropriately by
 *              the caller.
 */
class runtime_exception_base : public std::runtime_error, public exception_base
{
public:
    /**
     * @brief       Inherit constructors from std::runtime_error.
     */
    using std::runtime_error::runtime_error;

    /**
     * @brief       Constructs the exception with an empty message.
     */
    runtime_exception_base()
            : runtime_error({})
    {
    }

    /**
     * @brief       Constructs the exception with a message.
     * @param       msg : The error message associated with the exception.
     */
    explicit runtime_exception_base(std::string_view msg)
            : runtime_error({})
            , message_(msg)
    {
    }

    /**
     * @brief       Returns the error message.
     * @return      A C-string describing the error.
     */
    [[nodiscard]] const char* what() const noexcept override
    {
        return message_.c_str();
    }

protected:
    /**
     * @brief       Updates the error message.
     * @param       msg : The new error message.
     */
    void reset_message(std::string msg) noexcept
    {
        message_ = std::move(msg);
    }

private:
    /** Internal storage for the error message. */
    std::string message_;
};

/**
 * @brief       Exception class that encapsulates a std::error_code with context. This class
 *              extends runtime_exception_base by associating a std::error_code with the exception.
 *              It provides helper methods to inspect and compare the stored error code.
 */
class error_code_exception : public runtime_exception_base
{
public:
    /**
     * @brief       Constructs the exception with context and an error code.
     * @param       context : A textual description of where or why the error occurred.
     * @param       ec : The associated error code.
     */
    error_code_exception(
            std::string_view context,
            std::error_code ec
    )
            : runtime_exception_base(build_message(context, ec))
            , ec_(ec)
    {
    }

    /**
     * @brief       Returns the stored error code.
     * @return      A reference to the internal std::error_code.
     */
    [[nodiscard]] const std::error_code& code() const noexcept
    {
        return ec_;
    }

    /**
     * @brief       Checks whether an error is present.
     * @return      true if the error code is non-zero, false otherwise.
     */
    [[nodiscard]] bool has_error() const noexcept
    {
        return static_cast<bool>(ec_);
    }

    /**
     * @brief       Compares the stored error code with a std::errc value.
     * @param       e : The std::errc value to compare against.
     * @return      true if the error codes match, false otherwise.
     */
    [[nodiscard]] bool is(std::errc e) const noexcept
    {
        return ec_ == e;
    }

    /**
     * @brief       Compares the stored error code with another std::error_code.
     * @param       e : The error code to compare against.
     * @return      true if the error codes are equal, false otherwise.
     */
    [[nodiscard]] bool is(const std::error_code& e) const noexcept
    {
        return ec_ == e;
    }

protected:
    /**
     * @brief       Builds a descriptive error message from context and error code.
     * @param       contxt : A textual description of the error context.
     * @param       ec : The error code used to generate the message.
     * @return      A formatted error message string.
     */
    static std::string build_message(
            std::string_view contxt,
            const std::error_code& ec
    )
    {
        std::string msg(contxt);

        if (ec)
        {
            msg += ": ";
            msg += ec.message();
        }

        return msg;
    }

private:
    /** The associated error code. */
    std::error_code ec_;
};

/**
 * @brief       Exception that enriches error_code_exception with source location information.
 *              It allows retrieving detailed information about where the exception was created
 *              (file, line, function).
 */
class source_location_exception : public error_code_exception
{
public:
    /**
     * @brief       Constructs the exception with context, error code, and source location.
     * @param       context : A textual description of the error context.
     * @param       ec : The associated error code.
     * @param       loc : The source location where the exception is created (defaults to
     *              std::source_location::current()).
     */
    source_location_exception(
            std::string_view context,
            std::error_code ec,
            std::source_location loc = std::source_location::current()
    )
            : error_code_exception("", ec)
            , loc_(loc)
    {
        this->reset_message(build_message(context, ec, loc));
    }

    /**
     * @brief       Returns the stored source location.
     * @return      A reference to the internal std::source_location.
     */
    [[nodiscard]] const std::source_location& location() const noexcept
    {
        return loc_;
    }

    /**
     * @brief       Returns the file name where the exception was created.
     * @return      A C-string containing the file name.
     */
    [[nodiscard]] const char* file_name() const noexcept
    {
        return loc_.file_name();
    }

    /**
     * @brief       Returns the line number where the exception was created.
     * @return      The line number.
     */
    [[nodiscard]] std::uint_least32_t line() const noexcept
    {
        return loc_.line();
    }

    /**
     * @brief       Returns the function name where the exception was created.
     * @return      A C-string containing the function name.
     */
    [[nodiscard]] const char* function_name() const noexcept
    {
        return loc_.function_name();
    }

protected:
    /**
     * @brief       Builds a descriptive error message including source location.
     * @param       context : A textual description of the error context.
     * @param       ec : The error code.
     * @param       loc : The source location.
     * @return      A formatted error message string.
     */
    static std::string build_message(
            std::string_view context,
            const std::error_code& ec,
            const std::source_location& loc
    )
    {
        std::string msg = error_code_exception::build_message(context, ec);

        msg += " [";
        msg += loc.file_name();
        msg += ":";
        msg += std::to_string(loc.line());
        msg += "]";

        return msg;
    }

private:
    /** The source location where the exception was created. */
    std::source_location loc_;
};

}

#endif

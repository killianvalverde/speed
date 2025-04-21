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
 * @file        exception.hpp
 * @brief       argparse exception class header.
 * @author      Killian Valverde
 * @date        2016/01/01
 */

#ifndef SPEED_ARGPARSE_EXCEPTION_HPP
#define SPEED_ARGPARSE_EXCEPTION_HPP

#include "../exceptions/exceptions.hpp"

namespace speed::argparse {

/**
 * @brief       The base class that the argparse classes will use to throw exceptions.
 */
class exception : public speed::exceptions::exception_base
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "argparse exception";
    }
};

/**
 * @brief       Class used to thrown exceptions when keys vectors and keys to add are empty.
 */
class no_key_specified_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "no key specified exception";
    }
};

/**
 * @brief       Class used to thrown exceptions when an argument value is not found.
 */
class no_value_id_specified_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "no value id specified exception";
    }
};

/**
 * @brief       Class used to thrown exceptions when keys vectors to add are empty.
 */
class no_help_menu_id_specified_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "no help menu id specified exception";
    }
};

/**
 * @brief       Class used to thrown exceptions when a new key already exists.
 */
class key_already_exists_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "key already exists exception";
    }
};

/**
 * @brief       Class used to thrown exceptions when a version argument exists and the user try to
 *              add another.
 */
class version_already_exists_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "version already exists exception";
    }
};

/**
 * @brief       Class used to thrown exceptions when a help menu exists and the user try to
 *              add another.
 */
class help_menu_already_exists_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "help menu already exists exception";
    }
};

/**
 * @brief       Class used to thrown exceptions when an argument spedified doesn't exists.
 */
class key_not_found_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "key not found exception";
    }
};

/**
 * @brief       Class used to thrown exceptions when a help menu is not found.
 */
class version_not_found_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "version not found exception";
    }
};

/**
 * @brief       Class used to thrown exceptions when an argument value is not found.
 */
class value_not_found_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "value not found exception";
    }
};

/**
 * @brief       Class used to thrown exceptions when a help menu is not found.
 */
class help_menu_not_found_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "help menu not found exception";
    }
};

/**
 * @brief       Exception class for handling wrong constraint errors.
 */
class wrong_constraint_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "wrong dependency exception";
    }
};

/**
 * @brief       Exception class for handling invalid or incorrect descriptions.

 */
class wrong_description_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "wrong description exception";
    }
};

/**
 * @brief       Class used to thrown exceptions when the minimum and maximum number of values is 
 *              incoherent.
 */
class wrong_min_max_interval_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "wrong min max interval exception";
    }
};

/**
 * @brief       Class used to thrown exceptions when the minimum and maximum number of values is
 *              incoherent.
 */
class arguments_not_parsed_exception : public exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    [[nodiscard]] char const* what() const noexcept override
    {
        return "arguments not parsed exception";
    }
};

}

#endif

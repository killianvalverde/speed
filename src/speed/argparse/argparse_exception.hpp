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
 * @file        speed/argparse/argparse_exception.hpp
 * @brief       argparse_exception class header.
 * @author      Killian Green
 * @date        2016/01/01 - 02:24
 */

#ifndef SPEED_ARGPARSE_ARGPARSE_EXCEPTION_HPP
#define SPEED_ARGPARSE_ARGPARSE_EXCEPTION_HPP

#include "../exception.hpp"


namespace speed::argparse {


/**
 * @brief       argparse_exception is the base class that the argparse classes will use to throw
 *              exceptions.
 */
class argparse_exception : public speed::exception::exception_base
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "argparse exception";
    }
};


/**
 * @brief       Class used to thrown exceptions when keys vectors to add are empty.
 */
class default_help_menu_id_empty_exception : public argparse_exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "default help menu id empty exception";
    }
};


/**
 * @brief       Class used to thrown exceptions when keys vectors to add are empty.
 */
class no_key_specified_exception : public argparse_exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "no key specified exception";
    }
};


/**
 * @brief       Class used to thrown exceptions when a new key already exists.
 */
class key_already_exists_exception : public argparse_exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "key already exists exception";
    }
};


/**
 * @brief       Class used to thrown exceptions when a version argument exists and the user try to
 *              add another.
 */
class version_arg_already_exists_exception : public argparse_exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "version arg already exists exception";
    }
};


/**
 * @brief       Class used to thrown exceptions when an argument spedified doesn't exists.
 */
class arg_doesnt_exists_exception : public argparse_exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "arg not found exception";
    }
};


/**
 * @brief       Class used to thrown exceptions when an argument value is not found.
 */
class value_not_found_exception : public argparse_exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "value not found exception";
    }
};


/**
 * @brief       Class used to thrown exceptions when a help menu is not found.
 */
class help_menu_not_found_exception : public argparse_exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "help menu not found exception";
    }
};


/**
 * @brief       Class used to thrown exceptions when an argument is not of the requested type.
 */
class bad_arg_type_exception : public argparse_exception
{
public:
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    char const* what() const noexcept override
    {
        return "bad arg type exception";
    }
};


}


#endif

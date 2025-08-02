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
 * @file        operations.hpp
 * @brief       terminal operations header.
 * @author      Killian Valverde
 * @date        2017/10/18
 */

#ifndef SPEED_SYSTEM_TERMINAL_OPERATIONS_HPP
#define SPEED_SYSTEM_TERMINAL_OPERATIONS_HPP

#include <cstdio>
#include <iostream>

#include "../platform/platform.hpp"
#include "text_attribute.hpp"

namespace speed::system::terminal {

/**
 * @brief       Flush the input terminal buffer.
 * @param       input_strm : Terminal input stream.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool flush_input_terminal(::FILE* input_strm, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(terminal::flush_input_terminal, false, input_strm, err_code);
}

/**
 * @brief       Flush the output terminal buffer.
 * @param       output_strm : Terminal output stream.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
inline bool flush_output_terminal(::FILE* output_strm, std::error_code* err_code = nullptr) noexcept
{
    return SPEED_SELECT_API(terminal::flush_output_terminal, false, output_strm, err_code);
}

/**
 * @brief       Print a message and pause de program execution until a key is pressed
 * @param       mess : The message to print before the pause process. If a null pointer is used
 *              there won't be a message printed.
 * @param       flush_input_term : Is it is true the input terminal buffer will be flused before the
 *              operation.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If the function was successful true is returned, otherwise false is returned.
 */
inline bool kbhit(
        const char *mess = nullptr,
        bool flush_input_term = true,
        std::error_code *err_code = nullptr
) noexcept
{
    return SPEED_SELECT_API(terminal::kbhit, false, mess, flush_input_term, err_code);
}

/**
 * @brief       Sets the foreground text attribute for a narrow-character output stream.
 * @param       os : The output stream (e.g., `std::cout`) to modify.
 * @param       text_attr : The text attribute to set.
 * @return      `true` if the attribute was successfully set, otherwise `false`.
 */
inline bool set_foreground_text_attribute(
        std::ostream& os,
        text_attribute text_attr
) noexcept
{
    return SPEED_SELECT_API(terminal::set_foreground_text_attribute, false, os, text_attr);
}

/**
 * @brief       Sets the foreground text attribute for a wide-character output stream.
 * @param       wos The wide-character output stream to modify.
 * @param       text_attr The text attribute to set.
 * @return      `true` if the attribute was successfully set, otherwise `false`.
 */
inline bool set_foreground_text_attribute(
        std::wostream& wos,
        text_attribute text_attr
) noexcept
{
    return SPEED_SELECT_API(terminal::set_foreground_text_attribute, false, wos, text_attr);
}

}

#endif

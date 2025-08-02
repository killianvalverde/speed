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

#ifndef SPEED_SYSTEM_DETAIL_WINAPI_TERMINAL_OPERATIONS_HPP
#define SPEED_SYSTEM_DETAIL_WINAPI_TERMINAL_OPERATIONS_HPP

#include "../../../platform/platform.hpp"
#ifdef SPEED_WINAPI

#include <cstdio>

#include "../../../errors/errors.hpp"
#include "../../../terminal/text_attribute.hpp"

namespace speed::system::detail::winapi::terminal {

/**
 * @brief       Flush the input terminal buffer.
 * @param       input_strm : Terminal input stream.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool flush_input_terminal(::FILE* input_strm, std::error_code* err_code = nullptr) noexcept;

/**
 * @brief       Flush the output terminal buffer.
 * @param       output_strm : Terminal output stream.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
bool flush_output_terminal(::FILE* output_strm, std::error_code* err_code = nullptr) noexcept;

/**
 * @brief       Print a message and pause de program execution until a key is pressed
 * @param       mess : The message to print before the pause process. If a null pointer is used
 *              there won't be a message printed.
 * @param       flush_input_term : Is it is true the input terminal buffer will be flused before the
 *              operation.
 * @param       err_code : If function fails it holds the platform-dependent error code.
 * @return      If the function was successful true is returned, otherwise false is returned.
 */
bool kbhit(
        const char* mess = nullptr,
        bool flush_input_term = true,
        std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief       Set a terminal text attribute.
 * @param       terminal_strm : Stream in which set the attribute.
 * @param       text_attr : Attribute to set.
 * @return      If function was successful 0 is returned, otherwise -1 is returned.
 */
bool set_foreground_text_attribute(
        std::ostream& os,
        system::terminal::text_attribute text_attr
) noexcept;

/**
 * @brief       Set a terminal text attribute.
 * @param       terminal_strm : Stream in which set the attribute.
 * @param       text_attr : Attribute to set.
 * @return      If function was successful 0 is returned, otherwise -1 is returned.
 */
bool set_foreground_text_attribute(
        std::wostream& wos,
        system::terminal::text_attribute text_attr
) noexcept;

/**
 * @brief       Get the current texte attibute set in the console.
 * @param       console_handl : Console handle in which get the information.
 * @param       text_attr : Holder in which set the current text attibute.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
static bool get_current_text_attribute(HANDLE console_handl, WORD* text_attr) noexcept;

/**
 * @brief       Set a terminal text attribute.
 * @param       terminal_strm : Stream in which set the attribute.
 * @param       text_attr : Attribute to set.
 * @return      If function was successful 0 is returned, otherwise -1 is returned.
 */
static bool set_foreground_text_attribute(
        void* os,
        system::terminal::text_attribute text_attr
) noexcept;

}

#endif
#endif

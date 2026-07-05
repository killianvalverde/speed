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
 * @brief Core operations for the system::terminal submodule.
 * @author Killian Valverde
 * @date 2017-10-18
 */

#pragma once

#include <ostream>

#include "color.hpp"

namespace speed::system::terminal {

/**
 * @brief Flushes the input buffer of a terminal stream.
 *
 * Removes all pending input events from the specified terminal input stream.
 *
 * @param input_stream Pointer to the input stream to flush.
 * @param err_code Optional pointer to an error code object that receives error information
 *                 if the operation fails.
 * @return `true` if the input buffer was successfully flushed, otherwise `false`.
 */
bool flush_input_terminal(FILE* input_stream, std::error_code* err_code = nullptr) noexcept;

/**
 * @brief Flushes the output buffer of a terminal stream.
 *
 * Forces any buffered output data to be written to the terminal.
 *
 * @param output_stream Pointer to the output stream to flush.
 * @param err_code Optional pointer to an error code object that receives error information
 *                 if the operation fails.
 * @return `true` if the output buffer was successfully flushed, otherwise `false`.
 */
bool flush_output_terminal(FILE* output_stream, std::error_code* err_code = nullptr) noexcept;

/**
 * @brief Sets the background color of a narrow character output stream.
 *
 * Applies the specified background color to the console associated with the given output stream.
 *
 * @param os Output stream whose console background color will be modified.
 * @param new_color Background color to apply.
 * @param err_code Optional pointer to an error code object that receives error information
 *                 if the operation fails.
 * @return `true` if the background color was successfully applied, otherwise `false`.
 */
bool set_background_color(
    std::ostream& os,
    color new_color,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Sets the background color of a wide character output stream.
 *
 * Applies the specified background color to the console associated with the given wide
 * output stream.
 *
 * @param wos Wide output stream whose console background color will be modified.
 * @param new_color Background color to apply.
 * @param err_code Optional pointer to an error code object that receives error information
 *                 if the operation fails.
 * @return `true` if the background color was successfully applied, otherwise `false`.
 */
bool set_background_color(
    std::wostream& wos,
    color new_color,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Sets the foreground color of a narrow character output stream.
 *
 * Applies the specified foreground color to the console associated with the given output stream.
 *
 * @param os Output stream whose console foreground color will be modified.
 * @param new_color Foreground color to apply.
 * @param err_code Optional pointer to an error code object that receives error information
 *                 if the operation fails.
 * @return `true` if the foreground color was successfully applied, otherwise `false`.
 */
bool set_foreground_color(
    std::ostream& os,
    color new_color,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Sets the foreground color of a wide character output stream.
 *
 * Applies the specified foreground color to the console associated with the given wide
 * output stream.
 *
 * @param wos Wide output stream whose console foreground color will be modified.
 * @param new_color Foreground color to apply.
 * @param err_code Optional pointer to an error code object that receives error information
 *                 if the operation fails.
 * @return `true` if the foreground color was successfully applied, otherwise `false`.
 */
bool set_foreground_color(
    std::wostream& wos,
    color new_color,
    std::error_code* err_code = nullptr
) noexcept;

/**
 * @brief Waits for a keyboard key press event.
 *
 * Blocks execution until a key press event is detected in the console input buffer.
 *
 * @param flush_input If `true`, the console input buffer is flushed before waiting for input.
 * @param err_code Optional pointer to an error code object that receives error information if
 *                 the operation fails.
 * @return `true` if a key press event was detected, otherwise `false`.
 */
bool wait_for_key(bool flush_input = true, std::error_code* err_code = nullptr) noexcept;

}

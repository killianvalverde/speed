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
 * @brief Provides utilities for standard stream operations.
 * @author Killian Valverde
 * @date 2016-08-24
 */

#pragma once

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string_view>
#include <type_traits>

#include "detail/forward_declarations.hpp"
#include "../system/system.hpp"

namespace speed::iostream {

/**
 * @brief Declares access to the standard error stream by character type.
 * 
 * @tparam CharT Character type of the requested stream.
 * 
 * @note The primary template is deleted. Only `char` and `wchar_t`
 *       specializations are supported.
 */
template<typename CharT>
std::basic_ostream<CharT>& get_cerr() noexcept = delete;

/**
 * @brief Returns the narrow-character standard error stream.
 * 
 * @return Reference to `std::cerr`.
 */
template<>
constexpr std::ostream& get_cerr<char>() noexcept
{
    return std::cerr;
}

/**
 * @brief Returns the wide-character standard error stream.
 * 
 * @return Reference to `std::wcerr`.
 */
template<>
constexpr std::wostream& get_cerr<wchar_t>() noexcept
{
    return std::wcerr;
}

/**
 * @brief Declares access to the standard input stream by character type.
 * 
 * @tparam CharT Character type of the requested stream.
 * 
 * @note The primary template is deleted. Only `char` and `wchar_t`
 *       specializations are supported.
 */
template<typename CharT>
std::basic_istream<CharT>& get_cin() noexcept = delete;

/**
 * @brief Returns the narrow-character standard input stream.
 * 
 * @return Reference to `std::cin`.
 */
template<>
constexpr std::istream& get_cin<char>() noexcept
{
    return std::cin;
}

/**
 * @brief Returns the wide-character standard input stream.
 * 
 * @return Reference to `std::wcin`.
 */
template<>
constexpr std::wistream& get_cin<wchar_t>() noexcept
{
    return std::wcin;
}

/**
 * @brief Declares access to the standard logging stream by character type.
 * 
 * @tparam CharT Character type of the requested stream.
 * 
 * @note The primary template is deleted. Only `char` and `wchar_t`
 *       specializations are supported.
 */
template<typename CharT>
std::basic_ostream<CharT>& get_clog() noexcept = delete;

/**
 * @brief Returns the narrow-character standard logging stream.
 * 
 * @return Reference to `std::clog`.
 */
template<>
constexpr std::ostream& get_clog<char>() noexcept
{
    return std::clog;
}

/**
 * @brief Returns the wide-character standard logging stream.
 * 
 * @return Reference to `std::wclog`.
 */
template<>
constexpr std::wostream& get_clog<wchar_t>() noexcept
{
    return std::wclog;
}

/**
 * @brief Declares access to the standard output stream by character type.
 * 
 * @tparam CharT Character type of the requested stream.
 * 
 * @note The primary template is deleted. Only `char` and `wchar_t`
 *       specializations are supported.
 */
template<typename CharT>
std::basic_ostream<CharT>& get_cout() noexcept = delete;

/**
 * @brief Returns the narrow-character standard output stream.
 * 
 * @return Reference to `std::cout`.
 */
template<>
constexpr std::ostream& get_cout<char>() noexcept
{
    return std::cout;
}

/**
 * @brief Returns the wide-character standard output stream.
 * 
 * @return Reference to `std::wcout`.
 */
template<>
constexpr std::wostream& get_cout<wchar_t>() noexcept
{
    return std::wcout;
}

/**
 * @brief Writes a newline to an output stream without flushing it.
 * 
 * @tparam CharT Character type of the stream.
 * @tparam TraitsT Character traits type of the stream.
 * 
 * @param os Output stream to which the widened newline is written.
 * @return Reference to @p os.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& newl(std::basic_ostream<CharT, TraitsT>& os)
{
    return os.put(os.widen('\n'));
}

/**
 * @brief Prints a color-highlighted error and terminates the process.
 * 
 * @tparam CharT Character type of the stream and message strings.
 * @tparam TraitsT Character traits type of the stream and message strings.
 * 
 * @param os Output stream that receives the error message.
 * @param name Name or label identifying the error source.
 * @param message Text describing the error.
 * @param exit_code Process exit status passed to `std::exit`.
 */
template<typename CharT, typename TraitsT>
[[noreturn]] void print_error_and_exit(
    std::basic_ostream<CharT, TraitsT>& os,
    std::type_identity_t<std::basic_string_view<CharT, TraitsT>> name,
    std::type_identity_t<std::basic_string_view<CharT, TraitsT>> message,
    int exit_code
) noexcept
{
    try
    {
        os << set_light_red_text << name << ':' << ' '
           << set_default_text << message
           << std::endl;
    }
    catch (...)
    {
    }

    std::exit(exit_code);
}

/**
 * @brief Writes text with line wrapping at spaces.
 * 
 * @tparam CharT Character type of the output stream and string view.
 * @tparam TraitsT Character traits type of the stream and string view.
 * 
 * @param os Output stream to which text is written.
 * @param strv Text to be written.
 * @param max_line_length Preferred maximum line length in characters.
 * @param new_line_indentation Number of spaces inserted after newlines; defaults to four.
 * @param current_line_length Existing length of the current output line; defaults to zero.
 * @return Reference to os.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& print_wrapped(
    std::basic_ostream<CharT, TraitsT>& os,
    std::type_identity_t<std::basic_string_view<CharT, TraitsT>> strv,
    std::size_t max_line_length,
    std::size_t new_line_indentation = 4,
    std::size_t current_line_length = 0
)
{
    const CharT newline = os.widen('\n');
    const CharT space = os.widen(' ');

    const auto write_new_line = [&]
    {
        os.put(newline);

        for (std::size_t i = 0; i < new_line_indentation && os; ++i)
        {
            os.put(space);
        }

        current_line_length = new_line_indentation;
    };

    for (auto it = strv.cbegin(); it != strv.cend() && os; ++it)
    {
        if (*it == newline)
        {
            write_new_line();
            continue;
        }

        if (*it == space)
        {
            auto next = it;
            std::size_t len_to_next = 0;

            do
            {
                ++next;
                ++len_to_next;
            } while (next != strv.cend() && *next != space && *next != newline);

            if (current_line_length >= max_line_length ||
                len_to_next > max_line_length - current_line_length)
            {
                write_new_line();
                continue;
            }
        }

        os.put(*it);

        if (current_line_length < max_line_length)
        {
            ++current_line_length;
        }
    }

    return os;
}

/**
 * @brief Sets the terminal background color to black.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_black(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::BLACK);
    return os;
}

/**
 * @brief Sets the terminal background color to blue.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_blue(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::BLUE);
    return os;
}

/**
 * @brief Sets the terminal background color to bright black.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_bright_black(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::BRIGHT_BLACK);
    return os;
}

/**
 * @brief Sets the terminal background color to bright blue.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_bright_blue(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::BRIGHT_BLUE);
    return os;
}

/**
 * @brief Sets the terminal background color to bright cyan.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_bright_cyan(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::BRIGHT_CYAN);
    return os;
}

/**
 * @brief Sets the terminal background color to bright green.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_bright_green(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::BRIGHT_GREEN);
    return os;
}

/**
 * @brief Sets the terminal background color to bright magenta.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_bright_magenta(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::BRIGHT_MAGENTA);
    return os;
}

/**
 * @brief Sets the terminal background color to bright red.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_bright_red(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::BRIGHT_RED);
    return os;
}

/**
 * @brief Sets the terminal background color to bright white.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_bright_white(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::BRIGHT_WHITE);
    return os;
}

/**
 * @brief Sets the terminal background color to bright yellow.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_bright_yellow(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::BRIGHT_YELLOW);
    return os;
}

/**
 * @brief Sets the terminal background color to cyan.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_cyan(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::CYAN);
    return os;
}

/**
 * @brief Resets the terminal background color to its default.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_default(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::DEFAULT);
    return os;
}

/**
 * @brief Sets the terminal background color to green.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_green(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::GREEN);
    return os;
}

/**
 * @brief Sets the terminal background color to magenta.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_magenta(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::MAGENTA);
    return os;
}

/**
 * @brief Sets the terminal background color to red.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_red(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::RED);
    return os;
}

/**
 * @brief Sets the terminal background color to white.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_white(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::WHITE);
    return os;
}

/**
 * @brief Sets the terminal background color to yellow.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal background color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_background_yellow(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_background_text_attribute(os, system::terminal::color::YELLOW);
    return os;
}

/**
 * @brief Sets the terminal foreground color to black.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal foreground color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_black(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::BLACK);
    return os;
}

/**
 * @brief Sets the terminal foreground color to blue.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal foreground color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_blue(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::BLUE);
    return os;
}

/**
 * @brief Sets the terminal foreground color to bright black.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal foreground color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_bright_black(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::BRIGHT_BLACK);
    return os;
}

/**
 * @brief Sets the terminal foreground color to bright blue.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal foreground color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_bright_blue(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::BRIGHT_BLUE);
    return os;
}

/**
 * @brief Sets the terminal foreground color to bright cyan.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal foreground color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_bright_cyan(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::BRIGHT_CYAN);
    return os;
}

/**
 * @brief Sets the terminal foreground color to bright green.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * @param os Output stream whose terminal foreground color is updated.
 * 
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_bright_green(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::BRIGHT_GREEN);
    return os;
}

/**
 * @brief Sets the terminal foreground color to bright magenta.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal foreground color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_bright_magenta(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::BRIGHT_MAGENTA);
    return os;
}

/**
 * @brief Sets the terminal foreground color to bright red.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal foreground color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_bright_red(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::BRIGHT_RED);
    return os;
}

/**
 * @brief Sets the terminal foreground color to bright white.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal foreground color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_bright_white(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::BRIGHT_WHITE);
    return os;
}

/**
 * @brief Sets the terminal foreground color to bright yellow.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal foreground color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_bright_yellow(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::BRIGHT_YELLOW);
    return os;
}

/**
 * @brief Sets the terminal foreground color to cyan.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal foreground color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_cyan(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::CYAN);
    return os;
}

/**
 * @brief Resets the terminal foreground color to its default.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal foreground color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_default(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::DEFAULT);
    return os;
}

/**
 * @brief Sets the terminal foreground color to green.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal foreground color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_green(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::GREEN);
    return os;
}

/**
 * @brief Sets the terminal foreground color to magenta.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal foreground color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_magenta(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::MAGENTA);
    return os;
}

/**
 * @brief Sets the terminal foreground color to red.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal foreground color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_red(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::RED);
    return os;
}

/**
 * @brief Sets the terminal foreground color to white.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal foreground color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_white(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::WHITE);
    return os;
}

/**
 * @brief Sets the terminal foreground color to yellow.
 * 
 * @tparam CharT Character type of the output stream.
 * @tparam TraitsT Character traits type of the output stream.
 * 
 * @param os Output stream whose terminal foreground color is updated.
 * @return Reference to os, for use as a stream manipulator.
 */
template<typename CharT, typename TraitsT>
std::basic_ostream<CharT, TraitsT>& set_foreground_yellow(
    std::basic_ostream<CharT, TraitsT>& os
)
{
    system::terminal::set_foreground_text_attribute(os, system::terminal::color::YELLOW);
    return os;
}

}

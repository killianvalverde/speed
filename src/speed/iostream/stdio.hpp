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
 * @file        speed/iostream/stdio.hpp
 * @brief       stdio functions header.
 * @author      Killian Valverde
 * @date        2016/08/24
 */

#ifndef SPEED_IOSTREAM_STDIO_HPP
#define SPEED_IOSTREAM_STDIO_HPP

#include <cstdio>


namespace speed::iostream {


/**
 * @brief       Clears the buffers of the given stream. For output streams this discards any
 *              unwritten output. For input streams this discards any input read from the
 *              underlying object but not yet obtained via getc(3); this includes any text pushed
 *              back via ungetc(3).
 * @param       fp : Pointer to the FILE structure to purge.
 */
void fpurge(::FILE* fp) noexcept;


/**
 * @brief       Writes the C string pointed by format to the standard output (stdout). If format
 *              includes format specifiers (subsequences beginning with %), the additional arguments
 *              following format are formatted and inserted in the resulting string replacing their
 *              respective specifiers.
 * @param       formt : C string that contains the text to be written to stdout. It can optionally
 *              contain embedded format specifiers that are replaced by the values specified in
 *              subsequent additional arguments and formatted as requested.
 * @return      On success, the total number of characters written is returned. If a writing error
 *              occurs, the error indicator (ferror) is set and a negative number is returned. If a
 *              multibyte character encoding error occurs while writing wide characters, errno is
 *              set to EILSEQ and a negative number is returned.
 */
int printf(const char* formt, ...) noexcept;


/**
 * @brief       Writes the C string pointed by format to the standard output (stdout). If format
 *              includes format specifiers (subsequences beginning with %), the additional arguments
 *              following format are formatted and inserted in the resulting string replacing their
 *              respective specifiers.
 * @param       formt : C string that contains the text to be written to stdout. It can optionally
 *              contain embedded format specifiers that are replaced by the values specified in
 *              subsequent additional arguments and formatted as requested.
 * @return      On success, the total number of characters written is returned. If a writing error
 *              occurs, the error indicator (ferror) is set and a negative number is returned. If a
 *              multibyte character encoding error occurs while writing wide characters, errno is
 *              set to EILSEQ and a negative number is returned.
 */
int printf(const wchar_t* formt, ...) noexcept;


}


#endif

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
 * @file        tiostream.hpp
 * @brief       tiostream header.
 * @author      Killian Valverde
 * @date        2026/03/15
 */

#ifndef SPEED_IOSTREAM_TIOSTREAM_HPP
#define SPEED_IOSTREAM_TIOSTREAM_HPP

#include <iostream>

namespace speed::iostream {

/** Standard input stream using the system's character type. */
#ifdef _WIN32
inline std::wistream& tcin = std::wcin;
#else
inline std::istream& tcin = std::cin;
#endif

/** Standard output stream using the system's character type. */
#ifdef _WIN32
inline std::wostream& tcout = std::wcout;
#else
inline std::ostream& tcout = std::cout;
#endif

/** Standard error unbuffered stream using the system's character type. */
#ifdef _WIN32
inline std::wostream& tcerr = std::wcerr;
#else
inline std::ostream& tcerr = std::cerr;
#endif

/** Standard error buffered stream using the system's character type. */
#ifdef _WIN32
inline std::wostream& tclog = std::wclog;
#else
inline std::ostream& tclog = std::clog;
#endif

}

#endif

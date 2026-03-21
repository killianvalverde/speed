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
 * @file        crossplatform_unicode_main.hpp
 * @brief       crossplatform_unicode_main header.
 * @author      Killian Valverde
 * @date        2024/10/26
 */

#ifndef SPEED_SYSTEM_COMPATIBILITY_CROSSPLATFORM_UNICODE_MAIN_HPP
#define SPEED_SYSTEM_COMPATIBILITY_CROSSPLATFORM_UNICODE_MAIN_HPP

#include "compatibility.hpp"

/** @cond */
#ifdef SPEED_CROSSPLATFORM_UNICODE_MAIN
#if defined(SPEED_WINAPI)

#include <string>
#include <vector>
#include <windows.h>
#include "../detail/winapi/codecs/operations.hpp"

#define main crossplatform_unicode_main

int extern crossplatform_unicode_main(int argc, char* argv[]);

inline int wmain(int argc, wchar_t* wargv[])
{
    std::vector<std::string> storage;
    std::vector<char*> argv_utf8;

    storage.reserve(argc);
    argv_utf8.reserve(argc);

    for (int i = 0; i < argc; ++i)
    {
        std::string tmp;
        speed::system::detail::winapi::codecs::convert_wcstr_to_string(wargv[i], tmp);
        storage.push_back(std::move(tmp));
    }

    for (auto& s : storage)
    {
        argv_utf8.push_back(s.data());
    }

    ::SetConsoleCP(CP_UTF8);
    ::SetConsoleOutputCP(CP_UTF8);

    return crossplatform_unicode_main(argc, argv_utf8.data());
}

#endif
#endif
/** @endcond */

#endif

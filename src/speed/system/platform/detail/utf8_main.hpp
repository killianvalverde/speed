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
 * @file utf8_main.hpp
 * @brief Compatibility header that enables a UTF-8 argv entry point on Windows.
 * @author Killian Valverde
 * @date 2024-10-26
 */

#pragma once

#include "../compatibility.hpp"

/** @cond */
#if defined(SPEED_ENABLE_UTF8_MAIN) && defined(SPEED_WINAPI)

#include <cstdlib>
#include <string>
#include <vector>

#include "../../codecs/operations.hpp"

#define main utf8_main

extern int utf8_main(int argc, char* argv[]);

inline int wmain(int argc, wchar_t* wargv[])
{
    std::vector<std::string> storage;
    std::vector<char*> argv_utf8;

    storage.resize(argc);
    argv_utf8.reserve(argc + 1);

    for (int i = 0; i < argc; ++i)
    {
        if (!speed::system::codecs::convert_wstring_to_utf8(wargv[i], storage[i]))
        {
            return EXIT_FAILURE;
        }
    }

    for (auto& str : storage)
    {
        argv_utf8.emplace_back(str.data());
    }

    argv_utf8.emplace_back(nullptr);

    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    return utf8_main(argc, argv_utf8.data());
}

#endif
/** @endcond */

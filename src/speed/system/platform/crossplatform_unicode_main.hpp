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
#define main(c, v) \
    __secondary(c, v); \
    int wmain(int argc, wchar_t* wargv[]) \
    { \
        std::string str; \
        char** argv = (char**)calloc(argc, sizeof(char*)); \
        int res = -1; \
        if (argv == nullptr) \
        { \
            return -1; \
        } \
        for (int i = 0; i < argc; ++i) \
        { \
            if (!speed::system::codecs::convert_wcstr_to_string(wargv[i], str)) \
            { \
                goto cleanup; \
            } \
            argv[i] = (char*)malloc((str.length() + 1) * sizeof(char)); \
            if (argv[i] == nullptr) \
            { \
                goto cleanup; \
            } \
            strcpy_s(argv[i], str.length() + 1, str.c_str()); \
        } \
        ::SetConsoleOutputCP(CP_UTF8); \
        res = __secondary(argc, argv); \
cleanup: \
        for (int i = 0; i < argc; ++i) \
        { \
            if (argv[i] != nullptr) \
            { \
                free(argv[i]); \
            } \
        } \
        free(argv); \
        return res; \
    } \
    int __secondary(c, v)
#endif
#endif
/** @endcond */

#endif

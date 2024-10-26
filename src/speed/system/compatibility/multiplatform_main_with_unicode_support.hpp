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
 * @file        speed/system/compatibility/multiplatform_main_with_unicode_support.hpp
 * @brief       multiplatform_main_with_unicode_support header.
 * @author      Killian Valverde
 * @date        2024/10/26
 */

#ifndef SPEED_SYSTEM_COMPATIBILITY_MULTIPLATFORM_MAIN_WITH_UNICODE_SUPPORT_HPP
#define SPEED_SYSTEM_COMPATIBILITY_MULTIPLATFORM_MAIN_WITH_UNICODE_SUPPORT_HPP

#include "platform.hpp"


/** @cond */
#ifdef SPEED_MULTIPLATFORM_MAIN_WITH_UNICODE_SUPPORT
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
            if (!speed::system::codecs::convert_w_string_to_string(wargv[i], &str)) \
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

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
 * @file        platform.hpp
 * @brief       platform header.
 * @author      Killian Valverde
 * @date        2024/10/11
 */

#ifndef SPEED_SYSTEM_COMPATIBILITY_PLATFORM_HPP
#define SPEED_SYSTEM_COMPATIBILITY_PLATFORM_HPP

#ifdef __unix__
#undef _XOPEN_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <unistd.h>
#ifdef __linux__
#include <linux/version.h>
#endif
#define SPEED_GLIBC 1
#define SPEED_SYSTEM_FILESYSTEM_SLASH_CHAR '/'
#define SPEED_SYSTEM_FILESYSTEM_SLASH_WCHAR L'/'
#define SPEED_SYSTEM_FILESYSTEM_SLASH_CSTR "/"
#define SPEED_SYSTEM_FILESYSTEM_SLASH_WSTR L"/"
#define SPEED_SYSTEM_FILESYSTEM_ALT_SLASH_CHAR '\\'
#define SPEED_SYSTEM_FILESYSTEM_ALT_SLASH_WCHAR L'\\'
#define SPEED_SYSTEM_FILESYSTEM_ALT_SLASH_CSTR "\\"
#define SPEED_SYSTEM_FILESYSTEM_ALT_SLASH_WSTR L"\\"
#define SPEED_SYSTEM_FILESYSTEM_SHORTCUT_EXTENSION_CSTR ""
#define SPEED_SYSTEM_FILESYSTEM_SHORTCUT_EXTENSION_WSTR L""
#elif defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
//#define _WINSOCKAPI_
#include <windows.h>
#if !defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL
#error "Traditional preprocessor not supported, use at least Visual Studio 2019" \
       " version 16.5 with /Zc:preprocessor compiler switch"
#endif
#define SPEED_WINAPI 1
#define SPEED_SYSTEM_FILESYSTEM_SLASH_CHAR '\\'
#define SPEED_SYSTEM_FILESYSTEM_SLASH_WCHAR L'\\'
#define SPEED_SYSTEM_FILESYSTEM_SLASH_CSTR "\\"
#define SPEED_SYSTEM_FILESYSTEM_SLASH_WSTR L"\\"
#define SPEED_SYSTEM_FILESYSTEM_ALT_SLASH_CHAR '/'
#define SPEED_SYSTEM_FILESYSTEM_ALT_SLASH_WCHAR L'/'
#define SPEED_SYSTEM_FILESYSTEM_ALT_SLASH_CSTR "/"
#define SPEED_SYSTEM_FILESYSTEM_ALT_SLASH_WSTR L"/"
#define SPEED_SYSTEM_FILESYSTEM_SHORTCUT_EXTENSION_CSTR ".lnk"
#define SPEED_SYSTEM_FILESYSTEM_SHORTCUT_EXTENSION_WSTR L".lnk"
#endif

#endif

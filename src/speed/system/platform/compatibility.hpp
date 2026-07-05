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
 * @file compatibility.hpp
 * @brief Platform detection and compatibility macros.
 * @author Killian Valverde
 * @date 2024-10-11
 */

#pragma once

#if defined(__unix__) || defined(__APPLE__)

#if defined(__linux__) || defined(__CYGWIN__)
#define _GNU_SOURCE
#else
#define _XOPEN_SOURCE 700
#endif

#include <unistd.h>

#define SPEED_POSIX 1
#define SPEED_TMAIN main
#define SPEED_EXIT_CODE_USAGE_ERROR 2
#define SPEED_PATH_SEPARATOR_CHAR '/'
#define SPEED_PATH_SEPARATOR_WCHAR L'/'
#define SPEED_PATH_SEPARATOR_CSTR "/"
#define SPEED_PATH_SEPARATOR_WCSTR L"/"
#define SPEED_SHORTCUT_EXTENSION_CSTR ""
#define SPEED_SHORTCUT_EXTENSION_WCSTR L""

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

#include <windows.h>

#define SPEED_WINAPI 1
#define SPEED_TMAIN wmain
#define SPEED_EXIT_CODE_USAGE_ERROR 1
#define SPEED_PATH_SEPARATOR_CHAR '\\'
#define SPEED_PATH_SEPARATOR_WCHAR L'\\'
#define SPEED_PATH_SEPARATOR_CSTR "\\"
#define SPEED_PATH_SEPARATOR_WCSTR L"\\"
#define SPEED_SHORTCUT_EXTENSION_CSTR ".lnk"
#define SPEED_SHORTCUT_EXTENSION_WCSTR L".lnk"

#else
#error Unsupported platform

#endif

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
 * @file        select_api.hpp
 * @brief       select_api header.
 * @author      Killian Valverde
 * @date        2018/05/27
 */

#ifndef SPEED_SYSTEM_COMPATIBILITY_SELECT_API_HPP
#define SPEED_SYSTEM_COMPATIBILITY_SELECT_API_HPP

#include "platform.hpp"

/** @cond */
#ifdef SPEED_GLIBC
#define SPEED_API_PREFIX 1, ::speed::system::detail::glibc
#elif defined(SPEED_WINAPI)
#define SPEED_API_PREFIX 1, ::speed::system::detail::winapi
#else
#define SPEED_API_PREFIX 0
#endif

#define SPEED_GET_PARAM2_HELPER(a, b, ...) b
#define SPEED_GET_PARAM2(...) SPEED_GET_PARAM2_HELPER(__VA_ARGS__)
#define SPEED_API_DEFINED_YES(a, b, ...) SPEED_GET_PARAM2(SPEED_API_PREFIX)::a(__VA_ARGS__)
#define SPEED_API_DEFINED_NO(a, b, ...) b
#define SPEED_IS_API_DEFINED_HELPER(a, b, c, ...) c
#define SPEED_IS_API_DEFINED(...) SPEED_IS_API_DEFINED_HELPER(__VA_ARGS__, YES, NO)
#define SPEED_CONCAT_HELPER(a, b) a ## b
#define SPEED_CONCAT(a, b) SPEED_CONCAT_HELPER(a, b)
#define SPEED_SELECT_API(a, b, ...) SPEED_CONCAT(SPEED_API_DEFINED_,\
        SPEED_IS_API_DEFINED(SPEED_API_PREFIX))(a, b, __VA_ARGS__)
/** @endcond */

#endif

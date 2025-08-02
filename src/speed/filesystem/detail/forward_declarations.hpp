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
 * @file        forward_declarations.hpp
 * @brief       forward_declarations header.
 * @author      Killian Valverde
 * @date        2024/10/15
 */

#ifndef SPEED_FILESYSTEM_DETAIL_FORWARD_DECLARATIONS_HPP
#define SPEED_FILESYSTEM_DETAIL_FORWARD_DECLARATIONS_HPP

namespace speed::filesystem {

class directory_iteration;

class valid_path;

template<typename BaseT>
class read_path_decorator;

template<typename BaseT>
class write_path_decorator;

template<typename BaseT>
class execute_path_decorator;

template<typename BaseT>
class regular_file_path_decorator;

template<typename BaseT>
class directory_path_decorator;

template<typename BaseT>
class output_regular_file_path_decorator;

template<typename BaseT>
class output_directory_path_decorator;

}

#endif

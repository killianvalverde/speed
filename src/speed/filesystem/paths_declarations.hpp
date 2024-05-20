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
 * @file        speed/filesystem/paths_declarations.hpp
 * @brief       paths_declarations main header.
 * @author      Killian Valverde
 * @date        2024/05/20
 */

#ifndef SPEED_FILESYSTEM_PATHS_DECLARATIONS_HPP
#define SPEED_FILESYSTEM_PATHS_DECLARATIONS_HPP

#include "directory_path_decorator.hpp"
#include "execute_path_decorator.hpp"
#include "read_path_decorator.hpp"
#include "regular_file_path_decorator.hpp"
#include "secure_path.hpp"
#include "touch_path_decorator.hpp"
#include "write_path_decorator.hpp"


namespace speed::filesystem {


/** Regular file path type. */
using regular_file_path = regular_file_path_decorator<secure_path>;

/** Directory path type. */
using directory_path = directory_path_decorator<secure_path>;

/** Regular file path type that can be executed. */
using x_regular_file_path = execute_path_decorator<regular_file_path>;

/** Regular file path type that can be written. */
using w_regular_file_path = write_path_decorator<regular_file_path>;

/** Regular file path type that can be written and executed. */
using wx_regular_file_path = write_path_decorator<execute_path_decorator<regular_file_path>>;

/** Regular file path type that can be read. */
using r_regular_file_path = read_path_decorator<regular_file_path>;

/** Regular file path type that can be read and executed. */
using rx_regular_file_path = read_path_decorator<execute_path_decorator<regular_file_path>>;

/** Regular file path type that can be read and written. */
using rw_regular_file_path = read_path_decorator<write_path_decorator<regular_file_path>>;

/** Regular file path type that can be read, written and executed. */
using rwx_regular_file_path = read_path_decorator<
        write_path_decorator<execute_path_decorator<regular_file_path>>>;

/** Directory path type that can be executed. */
using x_directory_path = execute_path_decorator<directory_path>;

/** Directory path type that can be written. */
using w_directory_path = write_path_decorator<directory_path>;

/** Directory path type that can be written and executed. */
using wx_directory_path = write_path_decorator<execute_path_decorator<directory_path>>;

/** Directory path type that can be read. */
using r_directory_path = read_path_decorator<directory_path>;

/** Directory path type that can be read and executed. */
using rx_directory_path = read_path_decorator<execute_path_decorator<directory_path>>;

/** Directory path type that can be read and written. */
using rw_directory_path = read_path_decorator<write_path_decorator<directory_path>>;

/** Directory path type that can be read, written and executed. */
using rwx_directory_path = read_path_decorator<
        write_path_decorator<execute_path_decorator<directory_path>>>;

/** Regular file path type that can be touched. */
using touch_path = touch_path_decorator<secure_path>;


}


#endif

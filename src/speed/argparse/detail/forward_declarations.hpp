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
 * @date        2018/10/27
 */

#ifndef SPEED_ARGPARSE_DETAIL_FORWARD_DECLARATIONS_HPP
#define SPEED_ARGPARSE_DETAIL_FORWARD_DECLARATIONS_HPP

namespace speed::argparse {

namespace detail {

template<typename AllocatorT>
class basic_arg_key;

template<typename AllocatorT>
class basic_arg_value;

template<typename AllocatorT>
class basic_base_arg;

template<typename AllocatorT>
class basic_key_arg;

template<typename AllocatorT>
class basic_value_arg;

template<typename AllocatorT>
class basic_key_value_arg;

template<typename AllocatorT>
class basic_positional_arg;

template<typename AllocatorT>
class basic_help_arg;

template<typename AllocatorT>
class basic_version_arg;

template<typename AllocatorT>
class basic_arg_constraint;

template<typename AllocatorT>
class basic_help_menu;

template<typename AllocatorT, typename ActualT>
class basic_base_arg_setter;

template<typename AllocatorT, typename ActualT>
class basic_key_arg_setter;

template<typename AllocatorT, typename ActualT>
class basic_value_arg_setter;

template<typename AllocatorT, typename ActualT>
class basic_key_value_arg_setter;

template<typename AllocatorT, typename ActualT>
class basic_positional_arg_setter;

template<typename AllocatorT, typename ActualT>
class basic_help_arg_setter;

template<typename AllocatorT, typename ActualT>
class basic_version_arg_setter;

template<typename AllocatorT>
class basic_help_menu_setter;

template<typename AllocatorT>
class basic_arg_parser_setter;

}

template<typename AllocatorT>
class basic_arg_parser;

}

#endif

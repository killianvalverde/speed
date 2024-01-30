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
 * @file        speed/argparse/argparse_type_casters.cpp
 * @brief       argparse_type_casters implementation.
 * @author      Killian Valverde
 * @date        2024/01/30
 */

#include "argparse_type_casters.hpp"


namespace speed::argparse {


speed::type_casting::string_caster<bool> str_to_bool;

speed::type_casting::string_caster<short> str_to_short;

speed::type_casting::string_caster<int> str_to_int;

speed::type_casting::string_caster<long> str_to_long;

speed::type_casting::string_caster<long long> str_to_llong;

speed::type_casting::string_caster<unsigned short> str_to_ushort;

speed::type_casting::string_caster<unsigned int> str_to_uint;

speed::type_casting::string_caster<unsigned long> str_to_ulong;

speed::type_casting::string_caster<unsigned long long> str_to_ullong;

speed::type_casting::string_caster<float> str_to_float;

speed::type_casting::string_caster<double> str_to_double;

speed::type_casting::string_caster<long double> str_to_ldouble;

speed::type_casting::string_caster<std::uint8_t> str_to_uint8;

speed::type_casting::string_caster<std::uint16_t> str_to_uint16;

speed::type_casting::string_caster<std::uint32_t> str_to_uint32;

speed::type_casting::string_caster<std::uint64_t> str_to_uint64;

speed::type_casting::string_caster<std::int8_t> str_to_int8;

speed::type_casting::string_caster<std::int16_t> str_to_int16;

speed::type_casting::string_caster<std::int32_t> str_to_int32;

speed::type_casting::string_caster<std::int64_t> str_to_int64;


}

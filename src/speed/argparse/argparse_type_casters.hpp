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
 * @file        speed/argparse/argparse_type_casters.hpp
 * @brief       argparse_type_casters header.
 * @author      Killian Valverde
 * @date        2024/01/30
 */

#ifndef SPEED_ARGPARSE_ARGPARSE_TYPE_CASTERS_HPP
#define SPEED_ARGPARSE_ARGPARSE_TYPE_CASTERS_HPP

#include "../type_casting.hpp"


namespace speed::argparse {


/** Classs used as a string to bool casting strategy holder. */
extern speed::type_casting::string_caster<bool> str_to_bool;

/** Classs used as a string to short casting strategy holder. */
extern speed::type_casting::string_caster<short> str_to_short;

/** Classs used as a string to int casting strategy holder. */
extern speed::type_casting::string_caster<int> str_to_int;

/** Classs used as a string to long casting strategy holder. */
extern speed::type_casting::string_caster<long> str_to_long;

/** Classs used as a string to long long casting strategy holder. */
extern speed::type_casting::string_caster<long long> str_to_llong;

/** Classs used as a string to unsigned short casting strategy holder. */
extern speed::type_casting::string_caster<unsigned short> str_to_ushort;

/** Classs used as a string to unsigned int casting strategy holder. */
extern speed::type_casting::string_caster<unsigned int> str_to_uint;

/** Classs used as a string to unsigned long casting strategy holder. */
extern speed::type_casting::string_caster<unsigned long> str_to_ulong;

/** Classs used as a string to unsigned long long casting strategy holder. */
extern speed::type_casting::string_caster<unsigned long long> str_to_ullong;

/** Classs used as a string to float casting strategy holder. */
extern speed::type_casting::string_caster<float> str_to_float;

/** Classs used as a string to double casting strategy holder. */
extern speed::type_casting::string_caster<double> str_to_double;

/** Classs used as a string to long double casting strategy holder. */
extern speed::type_casting::string_caster<long double> str_to_ldouble;

/** Classs used as a string to uint8 casting strategy holder. */
extern speed::type_casting::string_caster<std::uint8_t> str_to_uint8;

/** Classs used as a string to uint16 casting strategy holder. */
extern speed::type_casting::string_caster<std::uint16_t> str_to_uint16;

/** Classs used as a string to uint32 casting strategy holder. */
extern speed::type_casting::string_caster<std::uint32_t> str_to_uint32;

/** Classs used as a string to uint64 casting strategy holder. */
extern speed::type_casting::string_caster<std::uint64_t> str_to_uint64;

/** Classs used as a string to int8 casting strategy holder. */
extern speed::type_casting::string_caster<std::int8_t> str_to_int8;

/** Classs used as a string to int16 casting strategy holder. */
extern speed::type_casting::string_caster<std::int16_t> str_to_int16;

/** Classs used as a string to int32 casting strategy holder. */
extern speed::type_casting::string_caster<std::int32_t> str_to_int32;

/** Classs used as a string to int64 casting strategy holder. */
extern speed::type_casting::string_caster<std::int64_t> str_to_int64;


}


#endif

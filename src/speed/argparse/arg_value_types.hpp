/* speed - Generic C++ library.
 * Copyright (C) 2015-2023 Killian Valverde.
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
 * @file        speed/argparse/arg_value_types.hpp
 * @brief       arg_value_types enum class header.
 * @author      Killian Green
 * @date        2016/08/06 - 05:30
 */

#ifndef SPEED_ARGPARSE_ARG_VALUE_TYPES_HPP
#define SPEED_ARGPARSE_ARG_VALUE_TYPES_HPP

#include <ostream>

#include "../lowlevel.hpp"


namespace speed::argparse {


/**
 * @brief       Contains all the posible values of the 'basic_arg_value' type.
 */
enum class arg_value_types : std::uint32_t
{
    /** Null flag. */
    NIL = 0x0,
    
    /** Argument value bool type. */
    BOOL = 0x1,
    
    /** Argument value double type. */
    DOUBLE = 0x2,
    
    /** Argument value float type. */
    FLOAT = 0x4,
    
    /** Argument value int type. */
    INT = 0x8,
    
    /** Argument value 8 bits int type. */
    INT8 = 0x10,
    
    /** Argument value 16 bits int type. */
    INT16 = 0x20,
    
    /** Argument value 32 bits int type. */
    INT32 = 0x40,
    
    /** Argument value 64 bits int type. */
    INT64 = 0x80,
    
    /** Argument value long type. */
    LONG = 0x100,
    
    /** Argument value long double type. */
    LONG_DOUBLE = 0x200,
    
    /** Argument value long long type. */
    LONG_LONG = 0x400,
    
    /** Argument value short type. */
    SHORT = 0x800,
    
    /** Argument value string type. */
    STRING = 0x1000,
    
    /** Argument value 8 bits unsigned int type. */
    UINT8 = 0x2000,
    
    /** Argument value 16 bits unsigned int type. */
    UINT16 = 0x4000,
    
    /** Argument value 32 bits unsigned int type. */
    UINT32 = 0x8000,
    
    /** Argument value 64 bits unsigned int type. */
    UINT64 = 0x1'0000,
    
    /** Argument value unsigned int type. */
    UNSIGNED_INT = 0x2'0000,
    
    /** Argument value unsigned long type. */
    UNSIGNED_LONG = 0x4'0000,
    
    /** Argument value unsigned long long type. */
    UNSIGNED_LONG_LONG = 0x8'0000,
    
    /** Argument value unsigned short type. */
    UNSIGNED_SHORT = 0x10'0000,
    
    /** Argument value readable file type. */
    R_FILE = 0x20'0000,
    
    /** Argument value writable file type. */
    W_FILE = 0x40'0000,
    
    /** Argument value executable file type. */
    X_FILE = 0x80'0000,
    
    /** Argument value readable and writable file type. */
    RW_FILE = R_FILE | W_FILE,
    
    /** Argument value readable and executable file type. */
    RX_FILE = R_FILE | X_FILE,
    
    /** Argument value writable and executable file type. */
    WX_FILE = W_FILE | X_FILE,
    
    /** Argument value readable, writable and executable file type. */
    RWX_FILE = R_FILE | W_FILE | X_FILE,
    
    /** Argument value readable regular file type. */
    R_REG_FILE = 0x100'0000,
    
    /** Argument value writable regular file type. */
    W_REG_FILE = 0x200'0000,
    
    /** Argument value executable regular file type. */
    X_REG_FILE = 0x400'0000,
    
    /** Argument value creatable regular file type. */
    C_REG_FILE = 0x800'0000,
    
    /** Argument value readable directory type. */
    R_DIR = 0x1000'0000,
    
    /** Argument value writable directory type. */
    W_DIR = 0x2000'0000,
    
    /** Argument value executable directory type. */
    X_DIR = 0x4000'0000,
    
    /** Argument value creatable directory type. */
    C_DIR = 0x8000'0000,
    
    /** Argument value readable and writable directory type. */
    RW_DIR = R_DIR | W_DIR,
    
    /** Argument value readable and executable directory type. */
    RX_DIR = R_DIR | X_DIR,
    
    /** Argument value writable and executable directory type. */
    WX_DIR = W_DIR | X_DIR,
    
    /** Argument value readable, writable and executable directory type. */
    RWX_DIR = R_DIR | W_DIR | X_DIR,
    
    /** Argument value readable, writable, executable and creatable directory type. */
    RWXC_DIR = R_DIR | W_DIR | X_DIR | C_DIR,
    
    /** All argument value types. */
    ALL = 0xFFFF'FFFF
};


/** Contains all the posible values of the 'basic_arg_value' type. Alias of 'arg_value_types'
 *  enumeration. */
using avt_t = arg_value_types;


}


/** @cond */
namespace speed::lowlevel {
template<>
struct enable_bitwise_operators<speed::argparse::arg_value_types>
{
    static constexpr bool enable_with_same_type = true;
};
}
/** @endcond */


#endif

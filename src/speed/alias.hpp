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
 * @file        speed/alias.hpp
 * @brief       File that include the librarie namespace alias.
 * @author      Killian Valverde
 * @date        2018/10/09
 */

#ifndef SPEED_ALIAS_HPP
#define SPEED_ALIAS_HPP

#ifndef SPEED_DISABLE_ALIAS


namespace speed {


namespace algorithm {}


namespace argparse {}


namespace containers {}


namespace errors {}


namespace exceptions {}


namespace filesystem {}


namespace iostream {}


namespace lowlevel {}


namespace math {}


namespace scalars {}


namespace stringutils {}


namespace system {


namespace errors {}


namespace exceptions {}


namespace filesystem {}


namespace process {}


namespace terminal {}


namespace time {}


namespace type_traits {}


/**
 * @brief       Constains ressources to assist handling with function errors.
 */
namespace err = errors;


/**
 * @brief       Contains resources for all system exceptions thrown by the elements of speed along
 *              with several types and utilities to assist handling exceptions.
 */
namespace except = exceptions;


/**
 * @brief       Contains filesystem interaction resources.
 */
namespace fsys = filesystem;


/**
 * @brief       Contains resources for process interactions.
 */
namespace proc = process;


/**
 * @brief       Contains resources for interacting and customizing the terminal.
 */
namespace term = terminal;


/**
 * @brief       Contains resources and facilities related to time manipulation.
 */
namespace tm = time;


/**
 * @brief       Contains utilities for compile-time introspection and modification.
 */
namespace traits = type_traits;


}


namespace time {}


namespace type_casting {}


namespace type_traits {}


/**
 * @brief       Contians a set of algorithms.
 */
namespace algo = algorithm;


/**
 * @brief       Contians all argument parser resources.
 */
namespace ap = argparse;


/**
 * @brief       Contians definitions of containers.
 */
namespace contain = containers;


/**
 * @brief       Defines a series of standardized elements to report errors originating from the 
 *              speed library.
 */
namespace err = errors;


/**
 * @brief       Contains resources for all exceptions thrown by the elements of speed along with
 *              several types and utilities to assist handling exceptions.
 */
namespace except = exceptions;


/**
 * @brief       Contains filesystem interaction resources.
 */
namespace fsys = filesystem;


/**
 * @brief       Contains resources for input and output streams.
 */
namespace ios = iostream;


/**
 * @brief       Constains lowlevel resources.
 */
namespace low = lowlevel;


/**
 * @brief       Contains a set of resources to compute common mathematical operations and
 *              transformations.
 */
namespace mth = math;


/**
 * @brief       Contains a set of resources to compute common scalars operations.
 */
namespace scals = scalars;


/**
 * @brief       Contains resources for common strings operations.
 */
namespace str = stringutils;


/**
 * @brief       Contains resources for interact with the system.
 */
namespace sys = system;


/**
 * @brief       Contains resources and facilities related to time manipulation.
 */
namespace tm = time;


/**
 * @brief       Contains utilities for type casting.
 */
namespace cast = type_casting;


/**
 * @brief       Contains utilities for compile-time introspection and modification, including type
 *              classification traits, type property inspection traits and type transformation
 *              traits.
 */
namespace traits = type_traits;


}


/**
 * @brief       Contians all speed librarie.
 */
namespace spd = speed;


#endif

#endif

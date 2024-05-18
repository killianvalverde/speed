# Speed library #

Hello everyone using C++,

Speed is my personal, free, and generic C++ library. Its goal is to enhance the speed and 
smoothness of working with the C++ language.

I began working on Speed in 2015, initially as an argument parser. As I developed this argument 
parser, I found that it required several additional tools to meet my requirements. Consequently, I 
created various additional resources organized into their own namespaces. Over time, this small 
project evolved into a library with a wide range of useful resources.

Speed is a static library that currently relies on the STL and a system API for proper 
functionality. Currently, it exclusively supports the glibc, although adding support for the Win32 
API would not be particularly complex due to the library's organization.

Feel free to continue reading to learn about the library's main features and how to build it for 
use in your own projects. 

## Features ##

Speed is composed of a set of modules, each with a specific purpose:

- algorithm : This module defines a collection of functions designed for use on ranges of elements.
- argparse : The argparse module makes it easy to write user-friendly command-line interfaces. The 
program defines what arguments it requires, and argparse will figure out how to parse those out of 
argc and argv. The argparse module also automatically generates help, usage messages and issues 
errors when users give the program invalid arguments.
- containers : The containers module implements holder objects that store collections of other 
objects (elements). These holders are class templates, offering flexibility in supported element 
types.
- errors : Currently this module is empty.
- exceptions : The exception module defines the base class for all exceptions thrown by speed's 
elements. It also includes various types and utilities to aid in exception handling.
- filesystem : Currently this module is empty.
- iostream : This module provides resources for working with standard input/output stream objects.
- lowlevel : The lowlevel module offers resources for common low-level operations.
- math : Currently, this module is empty.
- scalars : The scalars module provides resources for working with scalar objects.
- stringutils : This module defines several functions for manipulating C strings, arrays, and 
strings in a generic context.
- system : The system module provides resources for interacting with the system.
- time : This module defines resources for common time-related operations.
- type_casting : The type_casting module defines functions that facilitate type casting.
- type_traits : The type_traits module defines various type traits.

## Build ##

Speed includes a CMake build script that can be employed on a variety of platforms. Currently, the
library is exclusively compatible with Linux systems using the glibc as the API.

If you are using APT as your package management system, you can install CMake with the following 
command:

    $ sudo apt-get install cmake

You can also get the backage by visiting the official website: <http://www.cmake.org/>.

CMake works by generating native makefiles or build projects that can be used in the compiler 
environment of your choice. You can either build Speed as a standalone project or it can be 
incorporated into an existing CMake build for another project.

#### Dependencies ####

In order to compile this software you have to use a C++ revision equal or highter to C++17 
(ISO/IEC 14882:2017). Additionally, you must have the following tools and libraries, each with a 
version that is equal to or higher than the specified ones:
- GCC 8.3.0
- CMake 3.15

#### Standalone CMake Project ####

When building Speed as a standalone project, the typical workflow starts with:

1. Create a directory to hold the build output and generate the native build scripts:

        $ cmake -H. -Bbuild

2. Compile the project directly from CMake using the native build scripts:

        $ cmake --build build

3. Install the library in your environment:

        $ sudo cmake --install build

#### Incorporating Into An Existing CMake Project ####

If you want to use Speed in a project which already uses CMake, then a more robust and flexible 
approach is to build Speed as part of that project directly. 
This is done by making the Speed source code available to the main build and adding it using 
CMake's `add_subdirectory()` command. 
This has the significant advantage that the same compiler and linker settings are used between 
Speed and the rest of your project, so issues associated with using incompatible libraries 
(eg debug/release), etc. are avoided.

## Documentation ##

If you want to generate Doxygen documentation, you can use the `doxygen Doxyfile` command, and in 
the `./doc` directory, the results will be placed. In the `./test` directory, you will find several 
unit tests for each module that you can use as an example to understand how to use the resources

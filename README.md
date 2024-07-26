<p align="center">
  <img height="90" src="docs/images/speed.png" alt="speed"/>
</p>


## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Build & Install](#build--install)
- [Documentation](#documentation)

## Introduction

Speed is my personal, free, and generic C++ library. Its goal is to complement the standard library 
with useful and flexible resources to enhance the smoothness of working with the C++ language.

Speed currently relies on the STL and a system API for proper functionality. At present, it 
exclusively supports glibc and Cygwin, although adding support for the WinAPI would not be 
particularly complex due to the library's organization.

Feel free to continue reading to learn about the library's main features and how to build it for 
use in your own projects.

## Features

Speed is composed of a set of modules, each serving a specific purpose. Currently, the most 
accomplished module is `argparse`, which offers a flexible and customizable argument parser. Feel 
free to check the [documentation](docs/index.md) of the library to learn how to utilize all the 
resources.

## Build & Install

Speed includes a CMake build script that can be employed on a variety of platforms. Currently, the
library is exclusively compatible with Linux systems using the glibc as the API.

If you are using APT as your package management system, you can install CMake with the following 
command:

    sudo apt-get install cmake

You can also get the package by visiting the official website: <http://www.cmake.org/>.

CMake works by generating native makefiles or build projects that can be used in the compiler 
environment of your choice. You can either build Speed as a standalone project or it can be 
incorporated into an existing CMake build for another project.

#### Dependencies

In order to compile this software you have to use a C++ revision equal or highter to C++17 
(ISO/IEC 14882:2017). Additionally, you must have the following tools and libraries, each with a 
version that is equal to or higher than the specified ones:
- GCC 8.3.0
- CMake 3.15

#### Standalone CMake Project

When building Speed as a standalone project, the typical workflow starts with:

1. Create a directory to hold the build output and generate the native build scripts:

        cmake -H. -Bbuild

2. Compile the project directly from CMake using the native build scripts:

        cmake --build build

3. Install the library in your environment:

        sudo cmake --install build

#### Incorporating Into An Existing CMake Project

If you want to use Speed in a project which already uses CMake, then a more robust and flexible 
approach is to build Speed as part of that project directly. 
This is done by making the Speed source code available to the main build and adding it using 
CMake's `add_subdirectory()` command. 
This has the significant advantage that the same compiler and linker settings are used between 
Speed and the rest of your project, so issues associated with using incompatible libraries 
(eg debug/release), etc. are avoided.

## Documentation

In order to learn how to use the library resources, feel free to check the 
[documentation](docs/index.md).

If you want to generate Doxygen documentation, you can use the `doxygen Doxyfile` command, and 
the results will be placed in the `./docs` directory. 

In addition, in the `./test` directory, you will find several unit tests for each module that you 
can use as an example to understand how to use the resources.

<div align="center">
  <img height="90" src="docs/images/speed.png" alt="speed"/>

  [![license](https://img.shields.io/badge/license-GPLv3-red.svg)](https://github.com/killianvalverde/speed/blob/master/COPYING)
  [![release](https://img.shields.io/badge/release-v0.4.1-blue.svg?cacheSeconds=2592000)](https://github.com/killianvalverde/speed/tree/v0.4.1)
</div>

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Documentation](#documentation)
- [Supported Compilers](#supported-compilers)
- [Requirements](#requirements)
- [Build](#build)
- [Install](#install)
- [Integration](#integration)

## Introduction

__Speed__ is my personal, free, and generic C++ library. Its goal is to complement the standard 
library with useful and flexible resources to enhance the smoothness of working with the C++ 
language.

## Features

The library is composed of a set of modules, each serving a specific purpose. Currently, the most 
accomplished module is `argparse`, which offers a flexible and customizable argument parser.

## Documentation

In order to learn how to use the library resources, feel free to check the 
[documentation](docs/index.md) subsection for an in-depth explanation of the currently documented 
modules. 

If you want to generate __Doxygen__ documentation, you can use the `doxygen Doxyfile` command, and 
the results will be placed in the `./docs` directory.

Additionally, the `./tests` directory contains several unit tests for the currently tested modules, 
which you can use as examples to better understand how to use the provided resources

## Supported Compilers

The following compilers are currently known to be compatible:

| Compiler                                                   | Standard Library | Test Environment   |
|:-----------------------------------------------------------| :--------------- | :----------------- |
| [GCC](https://gcc.gnu.org/) >= 11.4.0                      | libstdc++        | Ubuntu 22.04       |
| [MSVC](https://visualstudio.microsoft.com/) >= 19.44.35213 | Microsoft STL    | Visual Studio 2022 |

## Requirements

To compile this software, you must have the following tools and libraries installed:

| Software                                | Type                   | Platform Dependency | GNU/Linux Installation Command |
|:----------------------------------------|:-----------------------|:--------------------|:-------------------------------|
| [CMake](https://www.cmake.org/) >= 3.22 | Build system generator | All                 | `sudo apt install cmake`       |

## Build

Use the following commands to build the __CMake__ project:

1. Create a directory to hold the build output and generate the native build scripts:

       cmake -S . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
       cmake -S . -B cmake-build-release -DCMAKE_BUILD_TYPE=Release

2. Compile the project directly from __CMake__ using the native build scripts:

       cmake --build cmake-build-debug --config Debug
       cmake --build cmake-build-release --config Release

## Install

By default, __CMake__ installs the software in a standard location. If you wish to customize the 
installation directory, use the `--prefix <path>` option at the end of the __CMake__ command to 
specify the desired path. Note that depending on your system, elevated privileges may be required 
to complete the installation. The provided commands take this into account.

### Linux

Run the following __Bash__ command to install the software with root privileges:

    sudo cmake --install cmake-build-debug --config Debug
    sudo cmake --install cmake-build-release --config Release

### Windows

Run the following __PowerShell__ command to install the software with administrator privileges:

    $script = @"
    cd '$PWD'
    cmake --install cmake-build-debug --config Debug
    cmake --install cmake-build-release --config Release
    pause
    "@

    Start-Process powershell -Verb RunAs -ArgumentList "-Command", $script

## Integration

Once the library is installed, follow these steps to integrate it into your project:

1. Ensure that your project uses at least __C++20__, as the library is not compatible with earlier 
standards:

       set(CMAKE_CXX_STANDARD 20)

2. If your project targets __Windows (MSVC)__, you need to configure runtime linking and enable 
recommended compiler flags:

       if(MSVC)
           set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
           add_compile_options(/utf-8 /EHsc /Zc:preprocessor /Zc:__cplusplus)
       endif()

3. Add the library's package to the `CMakeLists.txt` that builds your project:

       find_package(speed REQUIRED)

4. Link the library to the target that uses it:

       target_link_libraries(<target-name> speed::speed)

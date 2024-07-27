<p align="center">
  <img height="90" src="docs/images/speed.png" alt="speed"/>
</p>


## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Requirements](#requirements)
- [Build & Install](#build--install)
- [Documentation](#documentation)

## Introduction

Speed is my personal, free, and generic C++ library. Its goal is to complement the standard library 
with useful and flexible resources to enhance the smoothness of working with the C++ language.

## Features

Speed is composed of a set of modules, each serving a specific purpose. Currently, the most 
accomplished module is `argparse`, which offers a flexible and customizable argument parser. Feel 
free to check the [documentation](docs/index.md) of the library to learn how to utilize all the 
resources.

## Requirements

In order to compile this software you have to use a C++ revision equal or highter to C++17 
(ISO/IEC 14882:2017). Additionally, you must have the following tools and libraries, each with a 
version that is equal to or higher than the specified ones:
- GCC 8.3.0
- CMake 3.15

## Build & Install

Use the folowing commands to buil and install the CMake project:

1. Create a directory to hold the build output and generate the native build scripts:

        cmake -H. -Bbuild

2. Compile the project directly from CMake using the native build scripts:

        cmake --build build

3. Install the library in your environment:

        sudo cmake --install build

## Documentation

In order to learn how to use the library resources, feel free to check the 
[documentation](docs/index.md).

If you want to generate Doxygen documentation, you can use the `doxygen Doxyfile` command, and 
the results will be placed in the `./docs` directory. 

In addition, in the `./tests` directory, you will find several unit tests for each module that you 
can use as an example to understand how to use the resources.

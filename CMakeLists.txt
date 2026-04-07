cmake_minimum_required(VERSION 3.22)

project(speed VERSION 0.6.0 LANGUAGES CXX)

option(BUILD_TESTS "Build tests" OFF)

add_subdirectory(src)

if(BUILD_TESTS AND PROJECT_IS_TOP_LEVEL)
    enable_testing()
    add_subdirectory(tests)
endif()

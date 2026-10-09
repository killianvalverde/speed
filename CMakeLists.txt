cmake_minimum_required(VERSION 3.28)

project(speed VERSION 0.6.0 LANGUAGES CXX)

option(SPEED_BUILD_TESTS "Build speed tests" OFF)

add_subdirectory(src)

if(SPEED_BUILD_TESTS AND PROJECT_IS_TOP_LEVEL)
    enable_testing()
    add_subdirectory(tests)
endif()

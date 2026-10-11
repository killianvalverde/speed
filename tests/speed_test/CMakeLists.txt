# =========================
# Helpers
# =========================
include(GoogleTest)

function(add_speed_test target sources)
    if("${target}" STREQUAL "speed_test")
        set(library speed)
    else()
        string(REPLACE "speed_" "" library "${target}")
        string(REPLACE "_test" "" library "${library}")
    endif()

    add_executable(${target})

    target_compile_features(${target} PRIVATE cxx_std_20)

    if(MSVC)
        target_compile_options(${target} PRIVATE
            /utf-8 /EHsc /Zc:preprocessor /Zc:__cplusplus
        )
    endif()

    target_sources(${target} PRIVATE main.cpp ${sources} )

    target_link_libraries(${target} PRIVATE speed::${library} GTest::gtest)

    set_target_properties(${target} PROPERTIES DEBUG_POSTFIX d)

    gtest_discover_tests(${target} DISCOVERY_TIMEOUT 30)
endfunction()

# =========================
# Target Sources
# =========================
set(SPEED_ALGORITHM_TEST_SOURCE_FILES
    algorithm_test/operations_test.cpp
)

set(SPEED_ARGPARSE_TEST_SOURCE_FILES
    argparse_test/arg_parser_test.cpp
)

set(SPEED_CONTAINERS_TEST_SOURCE_FILES
    containers_test/flags_test.cpp
    containers_test/static_cache_test.cpp
)

set(SPEED_FILESYSTEM_TEST_SOURCE_FILES
    filesystem_test/directory_iteration_test.cpp
    filesystem_test/valid_path_test.cpp
)

set(SPEED_IOSTREAM_TEST_SOURCE_FILES
    iostream_test/indentation_test.cpp
)

set(SPEED_MATH_TEST_SOURCE_FILES
    math_test/operations_test.cpp
)

set(SPEED_LOWLEVEL_TEST_SOURCE_FILES
    lowlevel_test/enum_bitwise_operators_test.cpp
    lowlevel_test/operations_test.cpp
)

set(SPEED_SAFETY_TEST_SOURCE_FILES
    safety_test/operations_test.cpp
)

set(SPEED_SCALARS_TEST_SOURCE_FILES
    scalars_test/operations_test.cpp
)

set(SPEED_STRINGS_TEST_SOURCE_FILES
    strings_test/operations_test.cpp
    strings_test/tstring_test.cpp
)

set(SPEED_SYSTEM_TEST_SOURCE_FILES
    system_test/codecs_test/operations_test.cpp
    system_test/errors_test/operations_test.cpp
    system_test/filesystem_test/operations_test.cpp
    system_test/process_test/operations_test.cpp
    system_test/process_test/user_id_test.cpp
    system_test/terminal_test/operations_test.cpp
    system_test/time_test/calendar_time_test.cpp
    system_test/time_test/operations_test.cpp
    system_test/time_test/time_value_test.cpp
)

set(SPEED_TIME_TEST_SOURCE_FILES
    time_test/cpu_chrono_test.cpp
    time_test/monotonic_chrono_test.cpp
)

set(SPEED_TYPE_CASTING_TEST_SOURCE_FILES
    type_casting_test/operations.cpp
    type_casting_test/type_cast_test.cpp
)

set(SPEED_TYPE_TRAITS_TEST_SOURCE_FILES
    type_traits_test/operations_test.cpp
)

set(SPEED_TEST_SOURCE_FILES
#    ${SPEED_ALGORITHM_TEST_SOURCE_FILES}
#    ${SPEED_ARGPARSE_TEST_SOURCE_FILES}
#    ${SPEED_CONTAINERS_TEST_SOURCE_FILES}
#    ${SPEED_FILESYSTEM_TEST_SOURCE_FILES}
#    ${SPEED_IOSTREAM_TEST_SOURCE_FILES}
    ${SPEED_MATH_TEST_SOURCE_FILES}
#    ${SPEED_LOWLEVEL_TEST_SOURCE_FILES}
#    ${SPEED_SAFETY_TEST_SOURCE_FILES}
#    ${SPEED_SCALARS_TEST_SOURCE_FILES}
    ${SPEED_STRINGS_TEST_SOURCE_FILES}
    ${SPEED_SYSTEM_TEST_SOURCE_FILES}
    ${SPEED_TIME_TEST_SOURCE_FILES}
#    ${SPEED_TYPE_CASTING_TEST_SOURCE_FILES}
    ${SPEED_TYPE_TRAITS_TEST_SOURCE_FILES}
)

# =========================
# Target Definitions
# =========================
#add_speed_test(speed_algorithm_test algorithm "${SPEED_ALGORITHM_TEST_SOURCE_FILES}")
#add_speed_test(speed_argparse_test argparse "${SPEED_ARGPARSE_TEST_SOURCE_FILES}")
#add_speed_test(speed_containers_test containers "${SPEED_CONTAINERS_TEST_SOURCE_FILES}")
#add_speed_test(speed_filesystem_test filesystem "${SPEED_FILESYSTEM_TEST_SOURCE_FILES}")
#add_speed_test(speed_iostream_test iostream "${SPEED_IOSTREAM_TEST_SOURCE_FILES}")
#add_speed_test(speed_lowlevel_test lowlevel "${SPEED_LOWLEVEL_TEST_SOURCE_FILES}")
#add_speed_test(speed_safety_test safety "${SPEED_SAFETY_TEST_SOURCE_FILES}")
#add_speed_test(speed_scalars_test scalars "${SPEED_SCALARS_TEST_SOURCE_FILES}")
#add_speed_test(speed_type_casting_test type_casting "${SPEED_TYPE_CASTING_TEST_SOURCE_FILES}")

add_speed_test(speed_math_test "${SPEED_MATH_TEST_SOURCE_FILES}")
add_speed_test(speed_strings_test "${SPEED_STRINGS_TEST_SOURCE_FILES}")
add_speed_test(speed_system_test "${SPEED_SYSTEM_TEST_SOURCE_FILES}")
add_speed_test(speed_time_test "${SPEED_TIME_TEST_SOURCE_FILES}")
add_speed_test(speed_type_traits_test "${SPEED_TYPE_TRAITS_TEST_SOURCE_FILES}")
add_speed_test(speed_test "${SPEED_TEST_SOURCE_FILES}")

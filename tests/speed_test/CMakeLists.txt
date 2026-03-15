# =========================
# Dependencies
# =========================
find_package(GTest REQUIRED)

# =========================
# Helpers
# =========================
function(add_speed_test target_name library sources)
    add_executable(${target_name} main.cpp ${sources})
    target_link_libraries(${target_name} PRIVATE ${library} GTest::gtest GTest::gtest_main)
endfunction()

# =========================
# Source files
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

set(SPEED_STRINGUTILS_TEST_SOURCE_FILES
    stringutils_test/operations_test.cpp
)

set(SPEED_SYSTEM_TEST_SOURCE_FILES
    system_test/filesystem_test.cpp
    system_test/process_test.cpp
    system_test/terminal_test.cpp
    system_test/time_test.cpp
)

set(SPEED_TIME_TEST_SOURCE_FILES
    time_test/cpu_chono_test.cpp
    time_test/monotonic_chrono_test.cpp
)

set(SPEED_TYPE_CASTING_TEST_SOURCE_FILES
    type_casting_test/try_type_cast_test.cpp
    type_casting_test/type_cast_test.cpp
)

set(SPEED_TYPE_TRAITS_TEST_SOURCE_FILES
    type_traits_test/operations_test.cpp
)

set(SPEED_TEST_SOURCE_FILES
    ${SPEED_ALGORITHM_TEST_SOURCE_FILES}
    ${SPEED_ARGPARSE_TEST_SOURCE_FILES}
    ${SPEED_CONTAINERS_TEST_SOURCE_FILES}
    ${SPEED_FILESYSTEM_TEST_SOURCE_FILES}
    ${SPEED_IOSTREAM_TEST_SOURCE_FILES}
    ${SPEED_LOWLEVEL_TEST_SOURCE_FILES}
    ${SPEED_SAFETY_TEST_SOURCE_FILES}
    ${SPEED_SCALARS_TEST_SOURCE_FILES}
    ${SPEED_STRINGUTILS_TEST_SOURCE_FILES}
    ${SPEED_SYSTEM_TEST_SOURCE_FILES}
    ${SPEED_TIME_TEST_SOURCE_FILES}
    ${SPEED_TYPE_CASTING_TEST_SOURCE_FILES}
    ${SPEED_TYPE_TRAITS_TEST_SOURCE_FILES}
)

# =========================
# Targets
# =========================
add_speed_test(speed_algorithm_test speed_algorithm "${SPEED_ALGORITHM_TEST_SOURCE_FILES}")
add_speed_test(speed_argparse_test speed_argparse "${SPEED_ARGPARSE_TEST_SOURCE_FILES}")
add_speed_test(speed_containers_test speed_containers "${SPEED_CONTAINERS_TEST_SOURCE_FILES}")
add_speed_test(speed_filesystem_test speed_filesystem "${SPEED_FILESYSTEM_TEST_SOURCE_FILES}")
add_speed_test(speed_iostream_test speed_iostream "${SPEED_IOSTREAM_TEST_SOURCE_FILES}")
add_speed_test(speed_lowlevel_test speed_lowlevel "${SPEED_LOWLEVEL_TEST_SOURCE_FILES}")
add_speed_test(speed_safety_test speed_safety "${SPEED_SAFETY_TEST_SOURCE_FILES}")
add_speed_test(speed_scalars_test speed_scalars "${SPEED_SCALARS_TEST_SOURCE_FILES}")
add_speed_test(speed_stringutils_test speed_stringutils "${SPEED_STRINGUTILS_TEST_SOURCE_FILES}")
add_speed_test(speed_system_test speed_system "${SPEED_SYSTEM_TEST_SOURCE_FILES}")
add_speed_test(speed_time_test speed_time "${SPEED_TIME_TEST_SOURCE_FILES}")
add_speed_test(speed_type_casting_test speed_type_casting "${SPEED_TYPE_CASTING_TEST_SOURCE_FILES}")
add_speed_test(speed_type_traits_test speed_type_traits "${SPEED_TYPE_TRAITS_TEST_SOURCE_FILES}")
add_speed_test(speed_test speed "${SPEED_TEST_SOURCE_FILES}")

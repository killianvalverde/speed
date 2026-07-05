# =========================
# Setup
# =========================
include(GNUInstallDirs)

# =========================
# Target Definitions
# =========================
set(SPEED_HEADER_ONLY_TARGETS
    speed_algorithm
    speed_argparse
    speed_concepts
    speed_containers
    speed_conversions
    speed_cryptography
    speed_enums
    speed_exception
    speed_filesystem
    speed_iostream
    speed_math
    speed_memory
    speed_numerics
    speed_strings
    speed_time
    speed_type_traits
    speed
)

set(SPEED_COMPILED_TARGETS
    speed_system
)

set(SPEED_TARGETS
    ${SPEED_HEADER_ONLY_TARGETS}
    ${SPEED_COMPILED_TARGETS}
)

foreach (target IN LISTS SPEED_HEADER_ONLY_TARGETS)
    add_library(${target} INTERFACE)
endforeach ()

foreach (target IN LISTS SPEED_COMPILED_TARGETS)
    add_library(${target} STATIC)
endforeach ()

# =========================
# Namespaced Aliases
# =========================
foreach (target IN LISTS SPEED_TARGETS)
    string(REPLACE "speed_" "" short_name ${target})

    set_target_properties(${target} PROPERTIES EXPORT_NAME ${short_name})

    if (NOT TARGET speed::${short_name})
        add_library(speed::${short_name} ALIAS ${target})
    endif ()
endforeach ()

# =========================
# Target Compile Settings
# =========================
foreach (target IN LISTS SPEED_HEADER_ONLY_TARGETS)
    target_compile_features(${target} INTERFACE cxx_std_20)
endforeach ()

foreach (target IN LISTS SPEED_COMPILED_TARGETS)
    target_compile_features(${target} PUBLIC cxx_std_20)
endforeach ()

if (MSVC)
    foreach (target IN LISTS SPEED_HEADER_ONLY_TARGETS)
        target_compile_options(${target} INTERFACE
            /utf-8 /EHsc /Zc:preprocessor /Zc:__cplusplus
        )
    endforeach ()

    foreach (target IN LISTS SPEED_COMPILED_TARGETS)
        target_compile_options(${target} PUBLIC
            /utf-8 /EHsc /Zc:preprocessor /Zc:__cplusplus
        )
    endforeach ()
endif ()

# =========================
# Target Sources
# =========================
set(SPEED_ALGORITHM_HEADERS
    algorithm/algorithm.hpp
    algorithm/operations.hpp
    algorithm/simple_compare.hpp
)

set(SPEED_ARGPARSE_HEADERS
    argparse/detail/arg_constraint_flags.hpp
    argparse/detail/arg_error_flags.hpp
    argparse/detail/arg_flags.hpp
    argparse/detail/arg_parser_error_flags.hpp
    argparse/detail/arg_parser_flags.hpp
    argparse/detail/arg_value_error_flags.hpp
    argparse/detail/basic_arg_constraint.hpp
    argparse/detail/basic_arg_constraint_setter.hpp
    argparse/detail/basic_arg_key.hpp
    argparse/detail/basic_arg_parser_setter.hpp
    argparse/detail/basic_arg_value.hpp
    argparse/detail/basic_base_arg.hpp
    argparse/detail/basic_base_arg_setter.hpp
    argparse/detail/basic_help_arg.hpp
    argparse/detail/basic_help_arg_setter.hpp
    argparse/detail/basic_help_menu.hpp
    argparse/detail/basic_help_menu_setter.hpp
    argparse/detail/basic_key_arg.hpp
    argparse/detail/basic_key_arg_setter.hpp
    argparse/detail/basic_key_value_arg.hpp
    argparse/detail/basic_key_value_arg_setter.hpp
    argparse/detail/basic_positional_arg.hpp
    argparse/detail/basic_positional_arg_setter.hpp
    argparse/detail/basic_type_caster.hpp
    argparse/detail/basic_value_arg.hpp
    argparse/detail/basic_value_arg_setter.hpp
    argparse/detail/basic_version_arg.hpp
    argparse/detail/basic_version_arg_setter.hpp
    argparse/detail/forward_declarations.hpp
    argparse/detail/help_menu_flags.hpp
    argparse/detail/type_traits.hpp
    argparse/argparse.hpp
    argparse/basic_arg_parser.hpp
    argparse/exceptions.hpp
)

set(SPEED_CONCEPTS_HEADERS
    concepts/concepts.hpp
    concepts/operations.hpp
)

set(SPEED_CONTAINERS_HEADERS
    containers/containers.hpp
    containers/exceptions.hpp
    containers/flags.hpp
    containers/iterator_base.hpp
    containers/static_cache.hpp
)

set(SPEED_CRYPTOGRAPHY_SOURCES
    cryptography/city_hash.cpp
    cryptography/city_hash.hpp
    cryptography/cryptography.hpp
)

set(SPEED_CRYPTOGRAPHY_HEADERS
    cryptography/cryptography.hpp
    cryptography/city_hash.hpp
)

set(SPEED_ENUMS_HEADERS
    enums/enums.hpp
    enums/is_flag_enum.hpp
    enums/operations.hpp
)

set(SPEED_EXCEPTION_HEADERS
    exceptions/exception_base.hpp
    exceptions/exception.hpp
)

set(SPEED_FILESYSTEM_SOURCES
    filesystem/directory_iteration.cpp
)

set(SPEED_FILESYSTEM_HEADERS
    filesystem/detail/forward_declarations.hpp
    filesystem/directory_iteration.hpp
    filesystem/filesystem.hpp
    filesystem/operations.hpp
    filesystem/valid_path.hpp
)

set(SPEED_IOSTREAM_HEADERS
    iostream/detail/forward_declarations.hpp
    iostream/basic_ios_redirect.hpp
    iostream/indentation.hpp
    iostream/iostream.hpp
    iostream/operations.hpp
    iostream/tiostream.hpp
)

set(SPEED_MATH_HEADERS
    math/math.hpp
    math/operations.hpp
)

set(SPEED_MEMORY_HEADERS
    memory/memory.hpp
    memory/operations.hpp
)

set(SPEED_NUMERICS_HEADERS
    numerics/detail/operations.hpp
    numerics/exception.hpp
    numerics/numerics.hpp
    numerics/operations.hpp
)

set(SPEED_STRINGS_HEADERS
    strings/detail/forward_declarations.hpp
    strings/operations.hpp
    strings/strings.hpp
    strings/tstring.hpp
)

if (WIN32)
    set(SPEED_SYSTEM_SOURCES
        system/filesystem/impl/winapi/operations.cpp
        system/process/impl/winapi/operations.cpp
        system/terminal/impl/winapi/operations.cpp
        system/time/impl/winapi/operations.cpp
    )
else ()
    set(SPEED_SYSTEM_SOURCES
        system/filesystem/impl/posix/operations.cpp
        system/process/impl/posix/operations.cpp
        system/terminal/impl/posix/operations.cpp
        system/time/impl/posix/operations.cpp
    )
endif ()

set(SPEED_SYSTEM_HEADERS
    system/codecs/codecs.hpp
    system/codecs/operations.hpp
    system/errors/errors.hpp
    system/errors/operations.hpp
    system/filesystem/access_modes.hpp
    system/filesystem/directory_stream.hpp
    system/filesystem/file_types.hpp
    system/filesystem/filesystem.hpp
    system/filesystem/operations.hpp
    system/filesystem/symlink_mode.hpp
    system/filesystem/symlink_target_type.hpp
    system/filesystem/types.hpp
    system/platform/detail/ntapi.hpp
    system/platform/detail/utf8_main.hpp
    system/platform/compatibility.hpp
    system/platform/platform.hpp
    system/process/execution_result.hpp
    system/process/operations.hpp
    system/process/process.hpp
    system/process/types.hpp
    system/process/user_id.hpp
    system/terminal/color.hpp
    system/terminal/operations.hpp
    system/terminal/terminal.hpp
    system/time/calendar_time.hpp
    system/time/operations.hpp
    system/time/time.hpp
    system/time/time_value.hpp
    system/system.hpp
)

set(SPEED_TIME_HEADERS
    time/basic_chrono.hpp
    time/clock_base.hpp
    time/cpu_clock.hpp
    time/monotonic_clock.hpp
    time/time.hpp
)

set(SPEED_CONVERSIONS_HEADERS
    conversions/detail/forward_declarations.hpp
    conversions/detail/operations.hpp
    conversions/exception.hpp
    conversions/operations.hpp
    conversions/conversions.hpp
)

set(SPEED_TYPE_TRAITS_HEADERS
    type_traits/detail/forward_declarations.hpp
    type_traits/detail/operations.hpp
    type_traits/operations.hpp
    type_traits/type_traits.hpp
)

set(SPEED_HEADERS
    ${SPEED_ALGORITHM_HEADERS}
    ${SPEED_ARGPARSE_HEADERS}
    ${SPEED_CONTAINERS_HEADERS}
    ${SPEED_CRYPTOGRAPHY_HEADERS}
    ${SPEED_ENUMS_HEADERS}
    ${SPEED_EXCEPTION_HEADERS}
    ${SPEED_FILESYSTEM_HEADERS}
    ${SPEED_IOSTREAM_HEADERS}
    ${SPEED_MATH_HEADERS}
    ${SPEED_MEMORY_HEADERS}
    ${SPEED_NUMERICS_HEADERS}
    ${SPEED_STRINGS_HEADERS}
    ${SPEED_SYSTEM_HEADERS}
    ${SPEED_TIME_HEADERS}
    ${SPEED_CONVERSIONS_HEADERS}
    ${SPEED_TYPE_TRAITS_HEADERS}
    speed.hpp
    version.hpp
)

foreach (target IN LISTS SPEED_TARGETS)
    string(TOUPPER ${target} target_upper)

    set(src_var "${target_upper}_SOURCES")
    set(hdr_var "${target_upper}_HEADERS")

    set(all_files "")

    if (DEFINED ${src_var})
        list(APPEND all_files ${${src_var}})
    endif ()

    if (DEFINED ${hdr_var})
        list(APPEND all_files ${${hdr_var}})
    endif ()

    if (all_files)
        target_sources(${target} PRIVATE ${all_files})
    else ()
        message(WARNING "No sources or headers found for ${target}")
    endif ()
endforeach ()

# =========================
# Usage Requirements
# =========================
foreach (target IN LISTS SPEED_HEADER_ONLY_TARGETS)
    target_include_directories(${target} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
endforeach ()

foreach (target IN LISTS SPEED_COMPILED_TARGETS)
    target_include_directories(${target} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
endforeach ()

# =========================
# Target Dependencies
# =========================
target_link_libraries(speed_argparse INTERFACE
    speed_containers
    speed_exception
    speed_numerics
    speed_strings
    speed_system
    speed_conversions
)

target_link_libraries(speed_concepts INTERFACE
    speed_type_traits
)

target_link_libraries(speed_containers INTERFACE
    speed_exception
    speed_iostream
    speed_type_traits
)

target_link_libraries(speed_enums INTERFACE
    speed_type_traits
)

target_link_libraries(speed_cryptography INTERFACE
    speed_strings
)

target_link_libraries(speed_filesystem INTERFACE
    speed_containers
    speed_strings
    speed_system
    speed_conversions
)

target_link_libraries(speed_iostream INTERFACE
    speed_system
)

target_link_libraries(speed_strings INTERFACE
    speed_type_traits
)

target_link_libraries(speed_system PUBLIC
    speed_cryptography
    speed_exception
    speed_numerics
)

if (MSVC)
    target_link_libraries(speed_system PRIVATE ntdll)
endif ()

if (CYGWIN)
    target_link_libraries(speed_system PRIVATE iconv)
endif ()

target_link_libraries(speed_time INTERFACE
    speed_system
)

target_link_libraries(speed_conversions INTERFACE
    speed_exception
    speed_filesystem
    speed_system
    speed_type_traits
)

target_link_libraries(speed INTERFACE
    speed_algorithm
    speed_argparse
    speed_concepts
    speed_containers
    speed_cryptography
    speed_enums
    speed_exception
    speed_filesystem
    speed_iostream
    speed_math
    speed_memory
    speed_numerics
    speed_strings
    speed_system
    speed_time
    speed_conversions
    speed_type_traits
)

# =========================
# Target Properties
# =========================
foreach (target IN LISTS SPEED_COMPILED_TARGETS)
    set_target_properties(${target} PROPERTIES DEBUG_POSTFIX d)
endforeach ()

# =========================
# Installation
# =========================
if (PROJECT_IS_TOP_LEVEL)
    include(CMakePackageConfigHelpers)

    install(TARGETS
        ${SPEED_TARGETS}
        EXPORT speedTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../speed
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp"
    )

    install(EXPORT speedTargets
        FILE speedTargets.cmake
        NAMESPACE speed::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/speed
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/speedConfigVersion.cmake"
        VERSION 0.6.0
        COMPATIBILITY AnyNewerVersion
    )

    configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/speedConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/speedConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/speed
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/speedConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/speedConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/speed
    )
endif ()

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
    speed_exception
    speed_iostream
    speed_lowlevel
    speed_math
    speed_memory
    speed_safety
    speed_scalars
    speed_stringutils
    speed_time
    speed_type_casting
    speed_type_traits
    speed
)

set(SPEED_COMPILED_TARGETS
    speed_cryptography
    speed_filesystem
    speed_system
)

set(SPEED_TARGETS
    ${SPEED_HEADER_ONLY_TARGETS}
    ${SPEED_COMPILED_TARGETS}
)

foreach(target IN LISTS SPEED_HEADER_ONLY_TARGETS)
    add_library(${target} INTERFACE)
endforeach()

foreach(target IN LISTS SPEED_COMPILED_TARGETS)
    add_library(${target} STATIC)
endforeach()

# =========================
# Namespaced Aliases
# =========================
foreach(target IN LISTS SPEED_TARGETS)
    string(REPLACE "speed_" "" short_name ${target})

    set_target_properties(${target} PROPERTIES EXPORT_NAME ${short_name})

    if(NOT TARGET speed::${short_name})
        add_library(speed::${short_name} ALIAS ${target})
    endif()
endforeach()

# =========================
# Target Compile Settings
# =========================
foreach(target IN LISTS SPEED_HEADER_ONLY_TARGETS)
    target_compile_features(${target} INTERFACE cxx_std_20)
endforeach()

foreach(target IN LISTS SPEED_COMPILED_TARGETS)
    target_compile_features(${target} PUBLIC cxx_std_20)
endforeach()

if(MSVC)
    foreach(target IN LISTS SPEED_HEADER_ONLY_TARGETS)
        target_compile_options(${target} INTERFACE
            /utf-8 /EHsc /Zc:preprocessor /Zc:__cplusplus
        )
    endforeach()

    foreach(target IN LISTS SPEED_COMPILED_TARGETS)
        target_compile_options(${target} PUBLIC
            /utf-8 /EHsc /Zc:preprocessor /Zc:__cplusplus
        )
    endforeach()
endif()

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
    argparse/exception.hpp
)

set(SPEED_CONCEPTS_HEADERS
    concepts/concepts.hpp
    concepts/operations.hpp
)

set(SPEED_CONTAINERS_HEADERS
    containers/containers.hpp
    containers/exception.hpp
    containers/flags.hpp
    containers/iterator_base.hpp
    containers/static_cache.hpp
)

set(SPEED_CRYPTOGRAPHY_SOURCES
    cryptography/city_hash.cpp
)

set(SPEED_CRYPTOGRAPHY_HEADERS
    cryptography/cryptography.hpp
    cryptography/city_hash.hpp
)

set(SPEED_EXCEPTION_HEADERS
    exception/exception_base.hpp
    exception/exception.hpp
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

set(SPEED_LOWLEVEL_HEADERS
    lowlevel/lowlevel.hpp
    lowlevel/operations.hpp
)

set(SPEED_MATH_HEADERS
    math/math.hpp
    math/operations.hpp
)

set(SPEED_MEMORY_HEADERS
    memory/memory.hpp
    memory/operations.hpp
)

set(SPEED_SAFETY_HEADERS
    safety/exception.hpp
    safety/operations.hpp
    safety/safety.hpp
)

set(SPEED_SCALARS_HEADERS
    scalars/is_flag_enum.hpp
    scalars/operations.hpp
    scalars/scalars.hpp
)

set(SPEED_STRINGUTILS_HEADERS
    stringutils/detail/forward_declarations.hpp
    stringutils/operations.hpp
    stringutils/stringutils.hpp
    stringutils/tstring.hpp
)

if(WIN32)
    set(SPEED_SYSTEM_SOURCES
        system/detail/winapi/codecs/operations.cpp
        system/detail/winapi/filesystem/operations.cpp
        system/detail/winapi/process/operations.cpp
        system/detail/winapi/terminal/operations.cpp
        system/detail/winapi/time/operations.cpp
        system/time/impl/winapi/operations.cpp
    )
else()
    set(SPEED_SYSTEM_SOURCES
        system/detail/glibc/codecs/operations.cpp
        system/detail/glibc/filesystem/operations.cpp
        system/detail/glibc/process/operations.cpp
        system/detail/glibc/terminal/operations.cpp
        system/detail/glibc/time/operations.cpp
        system/time/impl/posix/operations.cpp
    )
endif()

set(SPEED_SYSTEM_HEADERS
    system/codecs/codecs.hpp
    system/codecs/operations.hpp
    system/detail/detail.hpp
    system/detail/glibc/codecs/codecs.hpp
    system/detail/glibc/codecs/operations.hpp
    system/detail/glibc/filesystem/filesystem.hpp
    system/detail/glibc/filesystem/operations.hpp
    system/detail/glibc/filesystem/types.hpp
    system/detail/glibc/glibc.hpp
    system/detail/glibc/process/operations.hpp
    system/detail/glibc/process/process.hpp
    system/detail/glibc/terminal/operations.hpp
    system/detail/glibc/terminal/terminal.hpp
    system/detail/glibc/time/operations.hpp
    system/detail/glibc/time/time.hpp
    system/detail/winapi/codecs/codecs.hpp
    system/detail/winapi/codecs/operations.hpp
    system/detail/winapi/filesystem/filesystem.hpp
    system/detail/winapi/filesystem/operations.hpp
    system/detail/winapi/filesystem/types.hpp
    system/detail/winapi/process/operations.hpp
    system/detail/winapi/process/process.hpp
    system/detail/winapi/terminal/operations.hpp
    system/detail/winapi/terminal/terminal.hpp
    system/detail/winapi/time/operations.hpp
    system/detail/winapi/time/time.hpp
    system/detail/winapi/winapi.hpp
    system/errors/errors.hpp
    system/errors/operations.hpp
    system/exception/exception.hpp
    system/exception/exceptions.hpp
    system/filesystem/access_modes.hpp
    system/filesystem/directory_entity.hpp
    system/filesystem/file_types.hpp
    system/filesystem/filesystem.hpp
    system/filesystem/operations.hpp
    system/filesystem/types.hpp
    system/platform/compatibility.hpp
    system/platform/crossplatform_unicode_main.hpp
    system/platform/platform.hpp
    system/platform/select_api.hpp
    system/process/operations.hpp
    system/process/process.hpp
    system/process/types.hpp
    system/terminal/operations.hpp
    system/terminal/terminal.hpp
    system/terminal/text_attribute.hpp
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

set(SPEED_TYPE_CASTING_HEADERS
    type_casting/detail/forward_declarations.hpp
    type_casting/detail/operations.hpp
    type_casting/exception.hpp
    type_casting/operations.hpp
    type_casting/type_casting.hpp
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
    ${SPEED_EXCEPTION_HEADERS}
    ${SPEED_FILESYSTEM_HEADERS}
    ${SPEED_IOSTREAM_HEADERS}
    ${SPEED_LOWLEVEL_HEADERS}
    ${SPEED_MATH_HEADERS}
    ${SPEED_MEMORY_HEADERS}
    ${SPEED_SAFETY_HEADERS}
    ${SPEED_SCALARS_HEADERS}
    ${SPEED_STRINGUTILS_HEADERS}
    ${SPEED_SYSTEM_HEADERS}
    ${SPEED_TIME_HEADERS}
    ${SPEED_TYPE_CASTING_HEADERS}
    ${SPEED_TYPE_TRAITS_HEADERS}
    speed.hpp
    version.hpp
)

foreach(target IN LISTS SPEED_TARGETS)
    string(TOUPPER ${target} target_upper)

    set(src_var "${target_upper}_SOURCES")
    set(hdr_var "${target_upper}_HEADERS")

    set(all_files "")

    if(DEFINED ${src_var})
        list(APPEND all_files ${${src_var}})
    endif()

    if(DEFINED ${hdr_var})
        list(APPEND all_files ${${hdr_var}})
    endif()

    if(all_files)
        target_sources(${target} PRIVATE ${all_files})
    else()
        message(WARNING "No sources or headers found for ${target}")
    endif()
endforeach()

# =========================
# Usage Requirements
# =========================
foreach(target IN LISTS SPEED_HEADER_ONLY_TARGETS)
    target_include_directories(${target} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
endforeach()

foreach(target IN LISTS SPEED_COMPILED_TARGETS)
    target_include_directories(${target} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
endforeach()

# =========================
# Target Dependencies
# =========================
target_link_libraries(speed_argparse INTERFACE
    speed_containers
    speed_exception
    speed_lowlevel
    speed_stringutils
    speed_system
    speed_type_casting
)

target_link_libraries(speed_concepts INTERFACE
    speed_type_traits
)

target_link_libraries(speed_containers INTERFACE
    speed_exception
    speed_iostream
    speed_type_traits
)

target_link_libraries(speed_cryptography PUBLIC
    speed_stringutils
)

target_link_libraries(speed_filesystem PUBLIC
    speed_containers
    speed_stringutils
    speed_system
    speed_type_casting
)

target_link_libraries(speed_iostream INTERFACE
    speed_system
)

target_link_libraries(speed_lowlevel INTERFACE
    speed_exception
    speed_type_traits
)

target_link_libraries(speed_stringutils INTERFACE
    speed_type_traits
)

target_link_libraries(speed_system PUBLIC
    speed_cryptography
    speed_exception
    speed_lowlevel
)

if(CYGWIN)
    target_link_libraries(speed_system PRIVATE iconv)
endif()

target_link_libraries(speed_time INTERFACE
    speed_system
)

target_link_libraries(speed_type_casting INTERFACE
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
    speed_exception
    speed_filesystem
    speed_iostream
    speed_lowlevel
    speed_math
    speed_memory
    speed_safety
    speed_scalars
    speed_stringutils
    speed_system
    speed_time
    speed_type_casting
    speed_type_traits
)

# =========================
# Target Properties
# =========================
foreach(target IN LISTS SPEED_COMPILED_TARGETS)
    set_target_properties(${target} PROPERTIES DEBUG_POSTFIX d)
endforeach()

# =========================
# Installation
# =========================
if(PROJECT_IS_TOP_LEVEL)
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
endif()

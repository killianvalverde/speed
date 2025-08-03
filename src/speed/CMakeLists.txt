set(debug_suffix "$<IF:$<CONFIG:Debug>,d,>")

set(SPEED_ALGORITHM_SOURCE_FILES
        algorithm/algorithm.cpp
        algorithm/algorithm.hpp
        algorithm/operations.hpp
        algorithm/simple_compare.hpp
)

set(SPEED_ARGPARSE_SOURCE_FILES
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
        argparse/argparse.cpp
        argparse/argparse.hpp
        argparse/basic_arg_parser.hpp
        argparse/exception.hpp
)

set(SPEED_CONTAINERS_SOURCE_FILES
        containers/containers.cpp
        containers/containers.hpp
        containers/exception.hpp
        containers/flags.hpp
        containers/iterator_base.hpp
        containers/static_cache.hpp
)

set(SPEED_CRYPTOGRAPHY_SOURCE_FILES
        cryptography/cryptography.cpp
        cryptography/cryptography.hpp
        cryptography/city_hash.cpp
        cryptography/city_hash.hpp
)

set(SPEED_ERRORS_SOURCE_FILES
        errors/errors.cpp
        errors/errors.hpp
)

set(SPEED_EXCEPTIONS_SOURCE_FILES
        exceptions/basic_message_exception.hpp
        exceptions/exception_base.hpp
        exceptions/exceptions.cpp
        exceptions/exceptions.hpp
)

set(SPEED_FILESYSTEM_SOURCE_FILES
        filesystem/detail/forward_declarations.hpp
        filesystem/directory_iteration.cpp
        filesystem/directory_iteration.hpp
        filesystem/filesystem.cpp
        filesystem/filesystem.hpp
        filesystem/operations.hpp
        filesystem/valid_path.hpp
)

set(SPEED_IOSTREAM_SOURCE_FILES
        iostream/detail/forward_declarations.hpp
        iostream/basic_ios_redirect.hpp
        iostream/indentation.cpp
        iostream/indentation.hpp
        iostream/iostream.cpp
        iostream/iostream.hpp
        iostream/operations.cpp
        iostream/operations.hpp
)

set(SPEED_LOWLEVEL_SOURCE_FILES
        lowlevel/lowlevel.cpp
        lowlevel/lowlevel.hpp
        lowlevel/operations.hpp
)

set(SPEED_MATH_SOURCE_FILES
        math/math.cpp
        math/math.hpp
        math/operations.hpp
)

set(SPEED_MEMORY_SOURCE_FILES
        memory/memory.cpp
        memory/memory.hpp
        memory/operations.hpp
)

set(SPEED_SAFETY_SOURCE_FILES
        safety/exception.hpp
        safety/operations.hpp
        safety/safety.cpp
        safety/safety.hpp
)

set(SPEED_SCALARS_SOURCE_FILES
        scalars/is_flag_enum.hpp
        scalars/operations.hpp
        scalars/scalars.cpp
        scalars/scalars.hpp
)

set(SPEED_STRINGUTILS_SOURCE_FILES
        stringutils/detail/forward_declarations.hpp
        stringutils/operations.hpp
        stringutils/stringutils.cpp
        stringutils/stringutils.hpp
)

set(SPEED_SYSTEM_SOURCE_FILES
        system/codecs/codecs.hpp
        system/codecs/operations.hpp
        system/detail/detail.hpp
        system/detail/glibc/codecs/codecs.hpp
        system/detail/glibc/codecs/operations.cpp
        system/detail/glibc/codecs/operations.hpp
        system/detail/glibc/filesystem/filesystem.hpp
        system/detail/glibc/filesystem/operations.cpp
        system/detail/glibc/filesystem/operations.hpp
        system/detail/glibc/filesystem/types.hpp
        system/detail/glibc/glibc.hpp
        system/detail/glibc/process/operations.cpp
        system/detail/glibc/process/operations.hpp
        system/detail/glibc/process/process.hpp
        system/detail/glibc/terminal/operations.cpp
        system/detail/glibc/terminal/operations.hpp
        system/detail/glibc/terminal/terminal.hpp
        system/detail/glibc/time/operations.cpp
        system/detail/glibc/time/operations.hpp
        system/detail/glibc/time/time.hpp
        system/detail/winapi/codecs/codecs.hpp
        system/detail/winapi/codecs/operations.cpp
        system/detail/winapi/codecs/operations.hpp
        system/detail/winapi/filesystem/filesystem.hpp
        system/detail/winapi/filesystem/operations.cpp
        system/detail/winapi/filesystem/operations.hpp
        system/detail/winapi/filesystem/types.hpp
        system/detail/winapi/process/operations.cpp
        system/detail/winapi/process/operations.hpp
        system/detail/winapi/process/process.hpp
        system/detail/winapi/terminal/operations.cpp
        system/detail/winapi/terminal/operations.hpp
        system/detail/winapi/terminal/terminal.hpp
        system/detail/winapi/time/operations.cpp
        system/detail/winapi/time/operations.hpp
        system/detail/winapi/time/time.hpp
        system/detail/winapi/winapi.hpp
        system/errors/errors.hpp
        system/errors/operations.hpp
        system/exceptions/exception.hpp
        system/exceptions/exceptions.hpp
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
        system/time/operations.hpp
        system/time/system_time.hpp
        system/time/time.hpp
        system/time/time_specification.cpp
        system/time/time_specification.hpp
        system/system.cpp
        system/system.hpp
)

set(SPEED_TIME_SOURCE_FILES
        time/chrono_base.cpp
        time/chrono_base.hpp
        time/chrono_states.hpp
        time/cpu_chrono.hpp
        time/monotonic_chrono.hpp
        time/time.cpp
        time/time.hpp
)

set(SPEED_TYPE_CASTING_SOURCE_FILES
        type_casting/detail/forward_declarations.hpp
        type_casting/error_category.cpp
        type_casting/error_category.hpp
        type_casting/exception.hpp
        type_casting/try_type_cast.hpp
        type_casting/type_cast.hpp
        type_casting/type_casting.cpp
        type_casting/type_casting.hpp
)

set(SPEED_TYPE_TRAITS_SOURCE_FILES
        type_traits/detail/forward_declarations.hpp
        type_traits/detail/operations.hpp
        type_traits/operations.hpp
        type_traits/type_traits.cpp
        type_traits/type_traits.hpp
)

set(SPEED_SOURCE_FILES
        ${SPEED_ALGORITHM_SOURCE_FILES}
        ${SPEED_ARGPARSE_SOURCE_FILES}
        ${SPEED_CONTAINERS_SOURCE_FILES}
        ${SPEED_CRYPTOGRAPHY_SOURCE_FILES}
        ${SPEED_ERRORS_SOURCE_FILES}
        ${SPEED_EXCEPTIONS_SOURCE_FILES}
        ${SPEED_FILESYSTEM_SOURCE_FILES}
        ${SPEED_IOSTREAM_SOURCE_FILES}
        ${SPEED_LOWLEVEL_SOURCE_FILES}
        ${SPEED_MATH_SOURCE_FILES}
        ${SPEED_MEMORY_SOURCE_FILES}
        ${SPEED_SAFETY_SOURCE_FILES}
        ${SPEED_SCALARS_SOURCE_FILES}
        ${SPEED_STRINGUTILS_SOURCE_FILES}
        ${SPEED_SYSTEM_SOURCE_FILES}
        ${SPEED_TIME_SOURCE_FILES}
        ${SPEED_TYPE_CASTING_SOURCE_FILES}
        ${SPEED_TYPE_TRAITS_SOURCE_FILES}
        speed.hpp
        version.hpp
)

add_library(speed_algorithm STATIC ${SPEED_ALGORITHM_SOURCE_FILES})
add_library(speed_argparse STATIC ${SPEED_ARGPARSE_SOURCE_FILES})
add_library(speed_containers STATIC ${SPEED_CONTAINERS_SOURCE_FILES})
add_library(speed_cryptography STATIC ${SPEED_CRYPTOGRAPHY_SOURCE_FILES})
add_library(speed_errors STATIC ${SPEED_ERRORS_SOURCE_FILES})
add_library(speed_exception STATIC ${SPEED_EXCEPTIONS_SOURCE_FILES})
add_library(speed_filesystem STATIC ${SPEED_FILESYSTEM_SOURCE_FILES})
add_library(speed_iostream STATIC ${SPEED_IOSTREAM_SOURCE_FILES})
add_library(speed_lowlevel STATIC ${SPEED_LOWLEVEL_SOURCE_FILES})
add_library(speed_math STATIC ${SPEED_MATH_SOURCE_FILES})
add_library(speed_memory STATIC ${SPEED_MEMORY_SOURCE_FILES})
add_library(speed_safety STATIC ${SPEED_SAFETY_SOURCE_FILES})
add_library(speed_scalars STATIC ${SPEED_SCALARS_SOURCE_FILES})
add_library(speed_stringutils STATIC ${SPEED_STRINGUTILS_SOURCE_FILES})
add_library(speed_system STATIC ${SPEED_SYSTEM_SOURCE_FILES})
add_library(speed_time STATIC ${SPEED_TIME_SOURCE_FILES})
add_library(speed_type_casting STATIC ${SPEED_TYPE_CASTING_SOURCE_FILES})
add_library(speed_type_traits STATIC ${SPEED_TYPE_TRAITS_SOURCE_FILES})
add_library(speed STATIC ${SPEED_SOURCE_FILES})

set_target_properties(speed_algorithm PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_argparse PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_containers PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_cryptography PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_errors PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_exception PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_filesystem PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_iostream PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_lowlevel PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_math PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_memory PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_safety PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_scalars PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_stringutils PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_system PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_time PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_type_casting PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_type_traits PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(speed_algorithm PROPERTIES OUTPUT_NAME speed_algorithm${debug_suffix})
set_target_properties(speed_argparse PROPERTIES OUTPUT_NAME speed_argparse${debug_suffix})
set_target_properties(speed_containers PROPERTIES OUTPUT_NAME speed_containers${debug_suffix})
set_target_properties(speed_cryptography PROPERTIES OUTPUT_NAME speed_cryptography${debug_suffix})
set_target_properties(speed_errors PROPERTIES OUTPUT_NAME speed_errors${debug_suffix})
set_target_properties(speed_exception PROPERTIES OUTPUT_NAME speed_exception${debug_suffix})
set_target_properties(speed_filesystem PROPERTIES OUTPUT_NAME speed_filesystem${debug_suffix})
set_target_properties(speed_iostream PROPERTIES OUTPUT_NAME speed_iostream${debug_suffix})
set_target_properties(speed_lowlevel PROPERTIES OUTPUT_NAME speed_lowlevel${debug_suffix})
set_target_properties(speed_math PROPERTIES OUTPUT_NAME speed_math${debug_suffix})
set_target_properties(speed_memory PROPERTIES OUTPUT_NAME speed_memory${debug_suffix})
set_target_properties(speed_safety PROPERTIES OUTPUT_NAME speed_safety${debug_suffix})
set_target_properties(speed_scalars PROPERTIES OUTPUT_NAME speed_scalars${debug_suffix})
set_target_properties(speed_stringutils PROPERTIES OUTPUT_NAME speed_stringutils${debug_suffix})
set_target_properties(speed_system PROPERTIES OUTPUT_NAME speed_system${debug_suffix})
set_target_properties(speed_time PROPERTIES OUTPUT_NAME speed_time${debug_suffix})
set_target_properties(speed_type_casting PROPERTIES OUTPUT_NAME speed_type_casting${debug_suffix})
set_target_properties(speed_type_traits PROPERTIES OUTPUT_NAME speed_type_traits${debug_suffix})
set_target_properties(speed PROPERTIES OUTPUT_NAME speed${debug_suffix})

target_link_libraries(speed_argparse 
        speed_containers 
        speed_exception 
        speed_lowlevel 
        speed_stringutils 
        speed_system 
        speed_type_casting
)

target_link_libraries(speed_containers 
        speed_exception 
        speed_iostream
        speed_type_traits
)

target_link_libraries(speed_cryptography
        speed_stringutils
)

target_link_libraries(speed_filesystem 
        speed_containers
        speed_stringutils
        speed_system
        speed_type_casting
)

target_link_libraries(speed_iostream 
        speed_system
)

target_link_libraries(speed_lowlevel 
        speed_exception 
        speed_type_traits
)

target_link_libraries(speed_stringutils 
        speed_type_traits
)

target_link_libraries(speed_system
        speed_cryptography
        speed_exception 
        speed_lowlevel
)

if(CYGWIN)
    target_link_libraries(speed_system iconv)
endif()

target_link_libraries(speed_time 
        speed_system
)

target_link_libraries(speed_type_casting
        speed_errors
        speed_exception
        speed_filesystem
        speed_stringutils
        speed_system
        speed_type_traits
)

target_link_libraries(speed_type_traits
)

target_link_libraries(speed
        speed_algorithm
        speed_argparse
        speed_containers
        speed_cryptography
        speed_errors
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

include(CMakePackageConfigHelpers)

install(TARGETS speed
        speed_algorithm
        speed_argparse
        speed_containers
        speed_cryptography
        speed_errors
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
    EXPORT speedTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    INCLUDES DESTINATION include
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../speed
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp"
)

install(EXPORT speedTargets
    FILE speedTargets.cmake
    NAMESPACE speed::
    DESTINATION lib/cmake/speed
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/speedConfigVersion.cmake"
    VERSION 0.5.0
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/speedConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/speedConfig.cmake"
    INSTALL_DESTINATION lib/cmake/speed
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/speedConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/speedConfigVersion.cmake"
    DESTINATION lib/cmake/speed
)

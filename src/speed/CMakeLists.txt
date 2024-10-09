set(SPEED_ALGORITHM_SOURCE_FILES
        algorithm/operations.hpp
        algorithm/simple_compare.hpp
        algorithm/algorithm.hpp
)

set(SPEED_ARGPARSE_SOURCE_FILES
        argparse/arg_error_flags.hpp
        argparse/arg_flags.hpp
        argparse/arg_parser_error_flags.hpp
        argparse/arg_parser_flags.hpp
        argparse/arg_value_error_flags.hpp
        argparse/basic_arg_constraint.hpp
        argparse/basic_arg_key.hpp
        argparse/basic_arg_parser.hpp
        argparse/basic_arg_parser_setter.hpp
        argparse/basic_arg_value.hpp
        argparse/basic_at_least_one_found.hpp
        argparse/basic_base_arg.hpp
        argparse/basic_base_arg_setter.hpp
        argparse/basic_help_arg.hpp
        argparse/basic_help_arg_setter.hpp
        argparse/basic_help_menu.hpp
        argparse/basic_help_menu_setter.hpp
        argparse/basic_key_arg.hpp
        argparse/basic_key_arg_setter.hpp
        argparse/basic_key_value_arg.hpp
        argparse/basic_key_value_arg_setter.hpp
        argparse/basic_keyless_arg.hpp
        argparse/basic_keyless_arg_setter.hpp
        argparse/basic_mutually_exclusive.hpp
        argparse/basic_type_caster.hpp
        argparse/basic_value_arg.hpp
        argparse/basic_value_arg_setter.hpp
        argparse/basic_version_arg.hpp
        argparse/basic_version_arg_setter.hpp
        argparse/exception.hpp
        argparse/forward_declarations.hpp
        argparse/argparse.hpp
)

set(SPEED_CONTAINERS_SOURCE_FILES
        containers/circular_doubly_linked_list.hpp
        containers/doubly_linked_node.hpp
        containers/exception.hpp
        containers/flags.hpp
        containers/iterator_base.hpp
        containers/static_cache.hpp
        containers/containers.hpp
)

set(SPEED_ERRORS_SOURCE_FILES
        errors/error_category_base.hpp
        errors/errors.hpp
)

set(SPEED_EXCEPTIONS_SOURCE_FILES
        exceptions/exception_base.hpp
        exceptions/basic_message_exception.hpp
        exceptions/exceptions.hpp
)

set(SPEED_FILESYSTEM_SOURCE_FILES
        filesystem/directory_iteration.cpp
        filesystem/directory_iteration.hpp
        filesystem/filesystem.hpp
        filesystem/valid_path.hpp
)

set(SPEED_IOSTREAM_SOURCE_FILES
        iostream/indentation.cpp
        iostream/indentation.hpp
        iostream/basic_ios_redirect.hpp
        iostream/operations.cpp
        iostream/operations.hpp
        iostream/iostream.hpp
)

set(SPEED_LOWLEVEL_SOURCE_FILES
        lowlevel/enable_bitwise_operators.hpp
        lowlevel/exception.hpp
        lowlevel/operations.hpp
        lowlevel/lowlevel.hpp
)

set(SPEED_MATH_SOURCE_FILES
        math/operations.hpp
        math/math.hpp
)

set(SPEED_SAFETY_SOURCE_FILES
        safety/exception.hpp
        safety/operations.hpp
        safety/safety.hpp
)

set(SPEED_SCALARS_SOURCE_FILES
        scalars/operations.hpp
        scalars/scalar.hpp
        scalars/scalars.hpp
)

set(SPEED_STRINGUTILS_SOURCE_FILES
        stringutils/stringutils.hpp
        stringutils/operations.hpp
)

set(SPEED_SYSTEM_SOURCE_FILES
        system/api/glibc/filesystem/directory_entity_extension.hpp
        system/api/glibc/filesystem/operations.cpp
        system/api/glibc/filesystem/operations.hpp
        system/api/glibc/process/operations.cpp
        system/api/glibc/process/operations.hpp
        system/api/glibc/terminal/operations.cpp
        system/api/glibc/terminal/operations.hpp
        system/api/glibc/time/operations.cpp
        system/api/glibc/time/operations.hpp
        system/api/glibc/filesystem/filesystem.hpp
        system/api/glibc/process/process.hpp
        system/api/glibc/terminal/terminal.hpp
        system/api/glibc/time/time.hpp
        system/api/glibc/glibc.hpp
        system/errors/operations.hpp
        system/exceptions/exception.hpp
        system/filesystem/access_modes.hpp
        system/filesystem/directory_entity.hpp
        system/filesystem/file_type.hpp
        system/filesystem/operations.hpp
        system/process/operations.hpp
        system/terminal/operations.hpp
        system/terminal/text_attribute.hpp
        system/time/operations.cpp
        system/time/operations.hpp
        system/time/time_specification.cpp
        system/time/time_specification.hpp
        system/type_traits/speed_select_api.hpp
        system/api/api.hpp
        system/errors/errors.hpp
        system/exceptions/exceptions.hpp
        system/filesystem/filesystem.hpp
        system/process/process.hpp
        system/terminal/terminal.hpp
        system/time/time.hpp
        system/type_traits/type_traits.hpp
        system/system.hpp
)

set(SPEED_TIME_SOURCE_FILES
        time/child_cpu_chrono.hpp
        time/chrono_base.cpp
        time/chrono_base.hpp
        time/chrono_states.hpp
        time/cpu_chrono.hpp
        time/monotonic_chrono.hpp
        time/time.hpp
)

set(SPEED_TYPE_CASTING_SOURCE_FILES
        type_casting/basic_type_caster.hpp
        type_casting/error_category.cpp
        type_casting/error_category.hpp
        type_casting/error_codes.hpp
        type_casting/error_conditions.hpp
        type_casting/exception.hpp
        type_casting/try_type_cast.hpp
        type_casting/type_cast.hpp
        type_casting/type_caster_base.hpp
        type_casting/type_casting.hpp
)

set(SPEED_TYPE_TRAITS_SOURCE_FILES
        type_traits/operations.hpp
        type_traits/type_traits.hpp
)

set(SPEED_SOURCE_FILES
        ${SPEED_ALGORITHM_SOURCE_FILES}
        ${SPEED_ARGPARSE_SOURCE_FILES}
        ${SPEED_CONTAINERS_SOURCE_FILES}
        ${SPEED_ERRORS_SOURCE_FILES}
        ${SPEED_EXCEPTIONS_SOURCE_FILES}
        ${SPEED_FILESYSTEM_SOURCE_FILES}
        ${SPEED_IOSTREAM_SOURCE_FILES}
        ${SPEED_LOWLEVEL_SOURCE_FILES}
        ${SPEED_MATH_SOURCE_FILES}
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
add_library(speed_errors STATIC ${SPEED_ERRORS_SOURCE_FILES})
add_library(speed_exception STATIC ${SPEED_EXCEPTIONS_SOURCE_FILES})
add_library(speed_filesystem STATIC ${SPEED_FILESYSTEM_SOURCE_FILES})
add_library(speed_iostream STATIC ${SPEED_IOSTREAM_SOURCE_FILES})
add_library(speed_lowlevel STATIC ${SPEED_LOWLEVEL_SOURCE_FILES})
add_library(speed_math STATIC ${SPEED_MATH_SOURCE_FILES})
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
set_target_properties(speed_errors PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_exception PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_filesystem PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_iostream PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_lowlevel PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_math PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_safety PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_scalars PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_stringutils PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_system PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_time PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_type_casting PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed_type_traits PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(speed PROPERTIES LINKER_LANGUAGE CXX)

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

target_link_libraries(speed_filesystem 
        speed_containers 
        speed_system
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
        speed_exception 
        speed_lowlevel
)

target_link_libraries(speed_time 
        speed_system
)

target_link_libraries(speed_type_casting 
        speed_exception 
        speed_stringutils 
        speed_type_traits
)

target_link_libraries(speed_type_traits
)

target_link_libraries(speed
        speed_algorithm
        speed_argparse
        speed_containers
        speed_errors
        speed_exception
        speed_filesystem
        speed_iostream
        speed_lowlevel
        speed_math
        speed_safety
        speed_scalars
        speed_stringutils
        speed_system
        speed_time
        speed_type_casting
        speed_type_traits
)

install(TARGETS speed_algorithm LIBRARY DESTINATION lib)
install(TARGETS speed_argparse LIBRARY DESTINATION lib)
install(TARGETS speed_containers LIBRARY DESTINATION lib)
install(TARGETS speed_errors LIBRARY DESTINATION lib)
install(TARGETS speed_exception LIBRARY DESTINATION lib)
install(TARGETS speed_filesystem LIBRARY DESTINATION lib)
install(TARGETS speed_iostream LIBRARY DESTINATION lib)
install(TARGETS speed_lowlevel LIBRARY DESTINATION lib)
install(TARGETS speed_math LIBRARY DESTINATION lib)
install(TARGETS speed_safety LIBRARY DESTINATION lib)
install(TARGETS speed_scalars LIBRARY DESTINATION lib)
install(TARGETS speed_stringutils LIBRARY DESTINATION lib)
install(TARGETS speed_system LIBRARY DESTINATION lib)
install(TARGETS speed_time LIBRARY DESTINATION lib)
install(TARGETS speed_type_casting LIBRARY DESTINATION lib)
install(TARGETS speed_type_traits LIBRARY DESTINATION lib)
install(TARGETS speed LIBRARY DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../speed
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp"
)
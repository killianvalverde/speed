/* speed - Generic C++ library.
 * Copyright (C) 2015-2026 Killian Valverde.
 *
 * This file is part of speed.
 *
 * speed is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * speed is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with speed. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file validatable_path.hpp
 * @brief Provides filesystem path validation, creation operations, and decorators.
 * @author Killian Valverde
 * @date 2024-05-20
 */

#pragma once

#include <concepts>
#include <filesystem>
#include <new>
#include <system_error>

#include "detail/forward_declarations.hpp"
#include "../system/system.hpp"
#include "detail/validatable_path.hpp"

namespace speed::filesystem {

/**
 * @brief Represents a filesystem path with composable validation requirements.
 *
 * Inherits the std::filesystem::path interface and checks existence, access permissions, and
 * filesystem object types. Derived decorators contribute requirements through virtual accessors.
 * Validation does not create or modify filesystem objects and is performed only when requested.
 */
class validatable_path : public std::filesystem::path
{
public:
    /** Inherits constructors from std::filesystem::path. */
    using std::filesystem::path::path;

    /** Inherits assignment operators from std::filesystem::path. */
    using std::filesystem::path::operator=;

    /**
     * @brief Constructs an empty path.
     */
    validatable_path() = default;

    /**
     * @brief Copy-constructs a path with the same value.
     */
    validatable_path(const validatable_path&) = default;

    /**
     * @brief Move-constructs a path from another instance.
     */
    validatable_path(validatable_path&&) = default;

    /**
     * @brief Destroys the path.
     */
    virtual ~validatable_path() = default;

    /**
     * @brief Copy-assigns another path.
     */
    validatable_path& operator=(const validatable_path&) = default;

    /**
     * @brief Move-assigns another path.
     */
    validatable_path& operator=(validatable_path&&) = default;

    /**
     * @brief Checks whether the path satisfies all configured validation requirements.
     *
     * Checks both access modes and accepted object types when specified; otherwise checks the
     * specified requirement alone. If neither is specified, checks path existence. Symbolic links
     * are handled according to symlink_mode(). Does not create filesystem objects.
     *
     * @param err_code Optional pointer to receive error information.
     * @return true if the path satisfies its requirements, false otherwise.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) const noexcept
    {
        const auto access_modes = required_access_modes();
        const auto file_types = accepted_file_types();

        if (access_modes != system::filesystem::access_modes::NIL &&
            file_types != system::filesystem::file_types::NIL)
        {
            return system::filesystem::file_matches(
                c_str(), symlink_mode(), access_modes, file_types, err_code);
        }

        if (access_modes != system::filesystem::access_modes::NIL)
        {
            return system::filesystem::file_has_access(
                c_str(), symlink_mode(), access_modes, err_code);
        }

        if (file_types != system::filesystem::file_types::NIL)
        {
            return system::filesystem::is_file_type(
                c_str(), symlink_mode(), file_types, err_code);
        }

        return system::filesystem::file_exists(c_str(), symlink_mode(), err_code);
    }

protected:
    /**
     * @brief Retrieves the access permissions required by the path type.
     *
     * @return Required access modes, or NIL when no access modes are required.
     */
    [[nodiscard]] virtual system::filesystem::access_modes required_access_modes() const noexcept
    {
        return system::filesystem::access_modes::NIL;
    }

    /**
     * @brief Retrieves the filesystem object types accepted by the path type.
     *
     * @return Accepted file types, or NIL when no file type is required.
     */
    [[nodiscard]] virtual system::filesystem::file_types accepted_file_types() const noexcept
    {
        return system::filesystem::file_types::NIL;
    }

    /**
     * @brief Retrieves the symbolic-link resolution policy used for validation.
     *
     * @return Symbolic-link resolution mode, resolving links by default.
     */
    [[nodiscard]] virtual system::filesystem::symlink_mode symlink_mode() const noexcept
    {
        return system::filesystem::symlink_mode::RESOLVE;
    }
};

/**
 * @brief Adds a regular-file requirement and a regular-file creation operation.
 *
 * @tparam BaseT Base path type to decorate.
 */
template<typename BaseT>
class regular_file_path_decorator : public BaseT
{
public:
    /** Inherits constructors from the base path type. */
    using BaseT::BaseT;

    /** Inherits assignment operators from the base path type. */
    using BaseT::operator=;

    /**
     * @brief Creates the regular file and missing parent directories if the path does not exist.
     *
     * Does not perform path validation or check access permissions. Currently, if any filesystem
     * object exists at the path without following symbolic links, no creation is attempted, even
     * if that object is not a regular file. The current check-then-create behavior is not atomic.
     *
     * @param err_code Optional pointer to receive error information.
     * @return true if no creation was needed or creation succeeded, false on creation failure.
     */
    bool ensure_exists(std::error_code* err_code = nullptr) const noexcept
    {
        // TODO(Killian): Avoid check-then-create pattern.
        if (!system::filesystem::file_exists(BaseT::c_str(),
            system::filesystem::symlink_mode::NO_RESOLVE))
        {
            try
            {
                auto parent_pth = BaseT::parent_path();

                if (!parent_pth.empty())
                {
                    if (!system::filesystem::create_directories(parent_pth.c_str(), err_code))
                    {
                        return false;
                    }
                }
                if (!system::filesystem::create_regular_file(BaseT::c_str(), err_code))
                {
                    return false;
                }
            }
            catch (const std::bad_alloc&)
            {
                system::errors::assign_errc(std::errc::not_enough_memory, err_code);
                return false;
            }
            catch (...)
            {
                system::errors::assign_errc(std::errc::io_error, err_code);
                return false;
            }
        }

        return true;
    }

protected:
    /**
     * @brief Adds regular-file type to the accepted types of the base path.
     *
     * @return Combined accepted filesystem object types.
     */
    [[nodiscard]] system::filesystem::file_types accepted_file_types() const noexcept override
    {
        return BaseT::accepted_file_types() | system::filesystem::file_types::REGULAR_FILE;
    }
};

/**
 * @brief Adds a directory requirement and a directory creation operation.
 *
 * @tparam BaseT Base path type to decorate.
 */
template<typename BaseT>
class directory_path_decorator : public BaseT
{
public:
    /** Inherits constructors from the base path type. */
    using BaseT::BaseT;

    /** @brief Inherits assignment operators from the base path type. */
    using BaseT::operator=;

    /**
     * @brief Creates the directory and missing parent directories if the path does not exist.
     *
     * Does not perform path validation or check access permissions. Currently, if any filesystem
     * object exists at the path without following symbolic links, no creation is attempted, even
     * if that object is not a directory. The current check-then-create behavior is not atomic.
     *
     * @param err_code Optional pointer to receive error information.
     * @return true if no creation was needed or creation succeeded, false on creation failure.
     */
    bool ensure_exists(std::error_code* err_code = nullptr) const noexcept
    {
        // TODO(Killian): Avoid check-then-create pattern.
        if (!system::filesystem::file_exists(BaseT::c_str(),
            system::filesystem::symlink_mode::NO_RESOLVE))
        {
            if (!system::filesystem::create_directories(BaseT::c_str(), err_code))
            {
                return false;
            }
        }

        return true;
    }

protected:
    /**
     * @brief Adds directory type to the accepted types of the base path.
     *
     * @return Combined accepted filesystem object types.
     */
    [[nodiscard]] system::filesystem::file_types accepted_file_types() const noexcept override
    {
        return BaseT::accepted_file_types() | system::filesystem::file_types::DIRECTORY;
    }
};

/**
 * @brief Adds a read-access requirement to a path.
 *
 * @tparam BaseT Base path type to decorate.
 */
template<typename BaseT>
class readable_path_decorator : public BaseT
{
public:
    /** Inherits constructors from the base path type. */
    using BaseT::BaseT;

    /** Inherits assignment operators from the base path type. */
    using BaseT::operator=;

protected:
    /**
     * @brief Adds read access to the requirements of the base path.
     *
     * @return Combined required access modes.
     */
    [[nodiscard]] system::filesystem::access_modes required_access_modes() const noexcept override
    {
        return BaseT::required_access_modes() | system::filesystem::access_modes::READ;
    }
};

/**
 * @brief Adds a write-access requirement to a path.
 *
 * @tparam BaseT Base path type to decorate.
 */
template<typename BaseT>
class writable_path_decorator : public BaseT
{
public:
    /** Inherits constructors from the base path type. */
    using BaseT::BaseT;

    /** Inherits assignment operators from the base path type. */
    using BaseT::operator=;

protected:
    /**
     * @brief Adds write access to the requirements of the base path.
     *
     * @return Combined required access modes.
     */
    [[nodiscard]] system::filesystem::access_modes required_access_modes() const noexcept override
    {
        return BaseT::required_access_modes() | system::filesystem::access_modes::WRITE;
    }
};

/**
 * @brief Adds an execute-access requirement to a path.
 *
 * @tparam BaseT Base path type to decorate.
 */
template<typename BaseT>
class executable_path_decorator : public BaseT
{
public:
    /** Inherits constructors from the base path type. */
    using BaseT::BaseT;

    /** Inherits assignment operators from the base path type. */
    using BaseT::operator=;

protected:
    /**
     * @brief Adds execute access to the requirements of the base path.
     *
     * @return Combined required access modes.
     */
    [[nodiscard]] system::filesystem::access_modes required_access_modes() const noexcept override
    {
        return BaseT::required_access_modes() | system::filesystem::access_modes::EXECUTE;
    }
};

/**
 * @brief Marks a creatable path for output-path handling by consumers such as argument parsers.
 *
 * Does not create filesystem objects during construction or validation. Consumers can use the
 * output-path tag to decide when to call the inherited ensure_exists() operation.
 *
 * @tparam BaseT Base path type providing a const ensure_exists() operation returning bool.
 */
template<typename BaseT>
    requires requires(const BaseT& path, std::error_code* err_code)
    {
        { path.ensure_exists(err_code) } -> std::same_as<bool>;
    }
class output_path_decorator : public BaseT, public detail::output_path_tag
{
public:
    /** Inherits constructors from the base path type. */
    using BaseT::BaseT;

    /** Inherits assignment operators from the base path type. */
    using BaseT::operator=;
};

/** Path requiring a regular file. */
using regular_file_path = regular_file_path_decorator<validatable_path>;

/** Path requiring a directory. */
using directory_path = directory_path_decorator<validatable_path>;

/** Regular file path requiring execute access. */
using x_regular_file_path = executable_path_decorator<regular_file_path>;

/** Regular file path requiring write access. */
using w_regular_file_path = writable_path_decorator<regular_file_path>;

/** Regular file path requiring write and execute access. */
using wx_regular_file_path = writable_path_decorator<executable_path_decorator<regular_file_path>>;

/** Regular file path requiring read access. */
using r_regular_file_path = readable_path_decorator<regular_file_path>;

/** Regular file path requiring read and execute access. */
using rx_regular_file_path = readable_path_decorator<executable_path_decorator<regular_file_path>>;

/** Regular file path requiring read and write access. */
using rw_regular_file_path = readable_path_decorator<writable_path_decorator<regular_file_path>>;

/** Regular file path requiring read, write, and execute access. */
using rwx_regular_file_path = readable_path_decorator<writable_path_decorator<
    executable_path_decorator<regular_file_path>>>;

/** Directory path requiring execute access. */
using x_directory_path = executable_path_decorator<directory_path>;

/** Directory path requiring write access. */
using w_directory_path = writable_path_decorator<directory_path>;

/** Directory path requiring write and execute access. */
using wx_directory_path = writable_path_decorator<executable_path_decorator<directory_path>>;

/** Directory path requiring read access. */
using r_directory_path = readable_path_decorator<directory_path>;

/** Directory path requiring read and execute access. */
using rx_directory_path = readable_path_decorator<executable_path_decorator<directory_path>>;

/** Directory path requiring read and write access. */
using rw_directory_path = readable_path_decorator<writable_path_decorator<directory_path>>;

/** Directory path requiring read, write, and execute access. */
using rwx_directory_path = readable_path_decorator<writable_path_decorator<
    executable_path_decorator<directory_path>>>;

/** Writable regular-file path marked for automatic creation by supporting consumers. */
using output_regular_file_path = output_path_decorator<writable_path_decorator<regular_file_path>>;

/** Writable, traversable directory path marked for creation by supporting consumers. */
using output_directory_path = output_path_decorator<writable_path_decorator<
    executable_path_decorator<directory_path>>>;

}

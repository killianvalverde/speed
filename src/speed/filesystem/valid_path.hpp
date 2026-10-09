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
 * @file valid_path.hpp
 * @brief Provides filesystem path validation and access decorators.
 * @author Killian Valverde
 * @date 2024-05-20
 */

#pragma once

#include <filesystem>
#include <system_error>

#include "detail/forward_declarations.hpp"
#include "../containers/containers.hpp"
#include "../system/system.hpp"

namespace speed::filesystem {

/**
 * @brief Represents a filesystem path with configurable validation rules.
 *
 * Extends std::filesystem::path with support for validating file existence, access permissions, 
 * file types, and symbolic link resolution. Validation rules can be configured by derived classes.
 */
class valid_path : public std::filesystem::path
{
public:
    /** Inherits constructors from std::filesystem::path. */
    using std::filesystem::path::path;

    /** 
     * @brief Constructs an empty valid path. 
     */
    valid_path() = default;

    /**
     * @brief Constructs a valid path by copying another instance. 
     */
    valid_path(const valid_path&) = default;

    /** 
     * @brief Constructs a valid path by moving another instance. 
     */
    valid_path(valid_path&&) = default;

    /** 
     * @brief Destroys the valid path. 
     */
    virtual ~valid_path() = default;

    /** 
     * @brief Assigns another valid path by copying it. 
     */
    valid_path& operator=(const valid_path&) = default;

    /** 
     * @brief Assigns another valid path by moving it. 
     */
    valid_path& operator=(valid_path&&) = default;

    /**
     * @brief Verifies that the filesystem path satisfies its validation rules.
     *
     * Checks the path according to the configured access modes, file types, and symbolic link 
     * resolution mode. If no access modes or file types are specified, only file existence 
     * is checked.
     *
     * @param err_code Optional pointer to receive an error code.
     * @return true if the path satisfies the validation rules, false otherwise.
     */
    [[nodiscard]] virtual bool verify(std::error_code* err_code = nullptr) noexcept
    {
        if (!access_modes_.is_empty() && !file_types_.is_empty())
        {
            return system::filesystem::file_matches(c_str(), symlink_mode_, 
                access_modes_.get_value(), file_types_.get_value(), err_code);
        }
        else if (!access_modes_.is_empty())
        {
            return system::filesystem::file_has_access(c_str(), symlink_mode_, 
                access_modes_.get_value(), err_code);
        }
        else if (file_types_.is_not_empty())
        {
            return system::filesystem::is_file_type(c_str(), symlink_mode_, 
                file_types_.get_value(), err_code);
        }
        else
        {
            return system::filesystem::file_exists(c_str(), symlink_mode_, err_code);
        }
    }
    
protected:
    /**
     * @brief Sets the access modes required for path validation.
     *
     * @param access_modes Access modes to require.
     */
    void set_access_modes(system::filesystem::access_modes access_modes) noexcept
    {
        access_modes_.set(access_modes);
    }
    
    /**
     * @brief Sets the file types accepted during path validation.
     *
     * @param file_types File types to require.
     */
    void set_file_types(system::filesystem::file_types file_types) noexcept
    {
        file_types_.set(file_types);
    }

    /**
     * @brief Sets the symbolic link resolution mode.
     *
     * @param symlink_mode Symbolic link resolution mode to use.
     */
    void set_resolve_symlink(system::filesystem::symlink_mode symlink_mode) noexcept
    {
        symlink_mode_ = symlink_mode;
    }
    
private:
    /** Access modes required for validation. */
    containers::flags<system::filesystem::access_modes> access_modes_ =
        system::filesystem::access_modes::NIL;

    /** File types accepted during validation. */
    containers::flags<system::filesystem::file_types> file_types_ =
        system::filesystem::file_types::NIL;

    /** Symbolic link resolution mode used during validation. */
    system::filesystem::symlink_mode symlink_mode_ =
        system::filesystem::symlink_mode::RESOLVE;
};

/**
 * @brief Decorates a path with a read access requirement.
 *
 * @tparam BaseT Base path type to decorate.
 */
template<typename BaseT>
class read_path_decorator : public BaseT
{
public:
    /** Inherits constructors from the base path type. */
    using BaseT::BaseT;

    /**
     * @brief Verifies the path with a read access requirement.
     *
     * @param err_code Optional pointer to receive an error code.
     * @return true if the path satisfies the validation rules, false otherwise.
     */
    [[nodiscard]] bool verify(std::error_code* err_code = nullptr) noexcept override
    {
        BaseT::set_access_modes(system::filesystem::access_modes::READ);
        return BaseT::verify(err_code);
    }
};

/**
 * @brief Decorates a path with a write access requirement.
 *
 * @tparam BaseT Base path type to decorate.
 */
template<typename BaseT>
class write_path_decorator : public BaseT
{
public:
    /** Inherits constructors from the base path type. */
    using BaseT::BaseT;

    /**
     * @brief Verifies the path with a write access requirement.
     *
     * @param err_code Optional pointer to receive an error code.
     * @return true if the path satisfies the validation rules, false otherwise.
     */
    [[nodiscard]] bool verify(std::error_code* err_code = nullptr) noexcept override
    {
        BaseT::set_access_modes(system::filesystem::access_modes::WRITE);
        return BaseT::verify(err_code);
    }
};

/**
 * @brief Decorates a path with an execute access requirement.
 *
 * @tparam BaseT Base path type to decorate.
 */
template<typename BaseT>
class execute_path_decorator : public BaseT
{
public:
    /** Inherits constructors from the base path type. */
    using BaseT::BaseT;

    /**
     * @brief Verifies the path with an execute access requirement.
     *
     * @param err_code Optional pointer to receive an error code.
     * @return true if the path satisfies the validation rules, false otherwise.
     */
    [[nodiscard]] bool verify(std::error_code* err_code = nullptr) noexcept override
    {
        BaseT::set_access_modes(system::filesystem::access_modes::EXECUTE);
        return BaseT::verify(err_code);
    }
};

/**
 * @brief Decorates a path with a regular file type requirement.
 *
 * @tparam BaseT Base path type to decorate.
 */
template<typename BaseT>
class regular_file_path_decorator : public BaseT
{
public:
    /** Inherits constructors from the base path type. */
    using BaseT::BaseT;

    /**
     * @brief Verifies that the path refers to a regular file.
     *
     * @param err_code Optional pointer to receive an error code.
     * @return true if the path satisfies the validation rules, false otherwise.
     */
    [[nodiscard]] bool verify(std::error_code* err_code = nullptr) noexcept override
    {
        BaseT::set_file_types(system::filesystem::file_types::REGULAR_FILE);
        return BaseT::verify(err_code);
    }
};

/**
 * @brief Decorates a path with a directory type requirement.
 *
 * @tparam BaseT Base path type to decorate.
 */
template<typename BaseT>
class directory_path_decorator : public BaseT
{
public:
    /** Inherits constructors from the base path type. */
    using BaseT::BaseT;

    /**
     * @brief Verifies that the path refers to a directory.
     *
     * @param err_code Optional pointer to receive an error code.
     * @return true if the path satisfies the validation rules, false otherwise.
     */
    [[nodiscard]] bool verify(std::error_code* err_code = nullptr) noexcept override
    {
        BaseT::set_file_types(system::filesystem::file_types::DIRECTORY);
        return BaseT::verify(err_code);
    }
};

/**
 * @brief Decorates a path for use as an output regular file.
 *
 * Requires a writable regular file. If the path does not exist, attempts to create its parent 
 * directories and the regular file before performing validation.
 *
 * @tparam BaseT Base path type to decorate.
 */
template<typename BaseT>
class output_regular_file_path_decorator : public BaseT
{
public:
    /** Inherits constructors from the base path type. */
    using BaseT::BaseT;

    /**
     * @brief Creates the output file if necessary and verifies its validity.
     *
     * Configures write access and regular file requirements. If the path does not exist without 
     * resolving symbolic links, creates the parent directories and the file. Memory allocation
     * failures and other exceptions during creation are converted into error codes.
     *
     * @param err_code Optional pointer to receive an error code.
     * @return true if the output file satisfies the validation rules, false otherwise.
     */
    [[nodiscard]] bool verify(std::error_code* err_code = nullptr) noexcept override
    {
        BaseT::set_access_modes(system::filesystem::access_modes::WRITE);
        BaseT::set_file_types(system::filesystem::file_types::REGULAR_FILE);

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
        
        return BaseT::verify(err_code);
    }
};

/**
 * @brief Decorates a path for use as an output directory.
 *
 * Requires a writable and executable directory. If the path does not exist, attempts to create 
 * the directory and its missing parent directories before performing validation.
 *
 * @tparam BaseT Base path type to decorate.
 */
template<typename BaseT>
class output_directory_path_decorator : public BaseT
{
public:
    /** Inherits constructors from the base path type. */
    using BaseT::BaseT;

    /**
     * @brief Creates the output directory if necessary and verifies its validity.
     *
     * Configures write and execute access requirements and a directory type requirement. If the 
     * path does not exist without resolving symbolic links, attempts to create it recursively.
     *
     * @param err_code Optional pointer to receive an error code.
     * @return true if the output directory satisfies the validation rules, false otherwise.
     */
    [[nodiscard]] bool verify(std::error_code* err_code = nullptr) noexcept override
    {
        BaseT::set_access_modes(system::filesystem::access_modes::WRITE | 
            system::filesystem::access_modes::EXECUTE);
        BaseT::set_file_types(system::filesystem::file_types::DIRECTORY);
        
        if (!system::filesystem::file_exists(BaseT::c_str(), 
            system::filesystem::symlink_mode::NO_RESOLVE))
        {
            if (!system::filesystem::create_directories(BaseT::c_str(), err_code))
            {
                return false;
            }
        }
        
        return BaseT::verify(err_code);
    }
};

/** Path requiring a regular file. */
using regular_file_path = regular_file_path_decorator<valid_path>;

/** Path requiring a directory. */
using directory_path = directory_path_decorator<valid_path>;

/** Regular file path requiring execute access. */
using x_regular_file_path = execute_path_decorator<regular_file_path>;

/** Regular file path requiring write access. */
using w_regular_file_path = write_path_decorator<regular_file_path>;

/** Regular file path requiring write and execute access. */
using wx_regular_file_path = write_path_decorator<execute_path_decorator<regular_file_path>>;

/** Regular file path requiring read access. */
using r_regular_file_path = read_path_decorator<regular_file_path>;

/** Regular file path requiring read and execute access. */
using rx_regular_file_path = read_path_decorator<execute_path_decorator<regular_file_path>>;

/** Regular file path requiring read and write access. */
using rw_regular_file_path = read_path_decorator<write_path_decorator<regular_file_path>>;

/** Regular file path requiring read, write, and execute access. */
using rwx_regular_file_path = read_path_decorator<
        write_path_decorator<execute_path_decorator<regular_file_path>>>;

/** Directory path requiring execute access. */
using x_directory_path = execute_path_decorator<directory_path>;

/** Directory path requiring write access. */
using w_directory_path = write_path_decorator<directory_path>;

/** Directory path requiring write and execute access. */
using wx_directory_path = write_path_decorator<execute_path_decorator<directory_path>>;

/** Directory path requiring read access. */
using r_directory_path = read_path_decorator<directory_path>;

/** Directory path requiring read and execute access. */
using rx_directory_path = read_path_decorator<execute_path_decorator<directory_path>>;

/** Directory path requiring read and write access. */
using rw_directory_path = read_path_decorator<write_path_decorator<directory_path>>;

/** Directory path requiring read, write, and execute access. */
using rwx_directory_path = read_path_decorator<
        write_path_decorator<execute_path_decorator<directory_path>>>;

/** Writable regular file path that creates the file if necessary. */
using output_regular_file_path = output_regular_file_path_decorator<valid_path>;

/** Writable directory path that creates the directory if necessary. */
using output_directory_path = output_directory_path_decorator<valid_path>;

}

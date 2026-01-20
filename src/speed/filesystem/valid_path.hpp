/* speed - Generic C++ library.
 * Copyright (C) 2015-2025 Killian Valverde.
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
 * @file        valid_path.hpp
 * @brief       valid_path main header.
 * @author      Killian Valverde
 * @date        2024/05/20
 */

#ifndef SPEED_FILESYSTEM_VALID_PATH_HPP
#define SPEED_FILESYSTEM_VALID_PATH_HPP

#include <filesystem>
#include <system_error>

#include "detail/forward_declarations.hpp"
#include "../containers/containers.hpp"

namespace speed::filesystem {

/**
 * @brief       Path class that allows performing validity checks throught decorators.
 */
class valid_path : public std::filesystem::path
{
public:
    /** Inherits all the path constructors. */
    using std::filesystem::path::path;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] virtual bool is_valid(std::error_code* err_code = nullptr)
    {
        if (access_mods_.is_not_empty() && file_typs_.is_not_empty())
        {
            return system::filesystem::check_file(c_str(), resolve_symlnk,
                    access_mods_.get_value(), file_typs_.get_value(), err_code);
        }
        else if (access_mods_.is_not_empty())
        {
            return system::filesystem::access(c_str(), resolve_symlnk,
                    access_mods_.get_value(), err_code);
        }
        else if (file_typs_.is_not_empty())
        {
            return system::filesystem::is_file_type(c_str(), resolve_symlnk,
                    file_typs_.get_value(), err_code);
        }
        else
        {
            return system::filesystem::access(c_str(), resolve_symlnk,
                    system::filesystem::access_modes::EXISTS, err_code);
        }
    }
    
protected:
    /**
     * @brief       Sets the allowed access modes.
     * @param       access_mods : A bitmask or enumeration value representing the desired access
     *              modes.
     */
    void set_access_modes(system::filesystem::access_modes access_mods) noexcept
    {
        access_mods_.set(access_mods);
    }
    
    /**
     * @brief       Sets the file types to filter or use.
     * @param       file_typs : A bitmask or enumeration value representing the file types to set.
     */
    void set_file_types(system::filesystem::file_types file_typs) noexcept
    {
        file_typs_.set(file_typs);
    }
    
    /**
     * @brief       Specifies is symlinks has to be resolved.
     * @param       enabl : Specifies whether or not the symlinks will be resolved.
     */
    void set_resolve_symlink(bool enabl) noexcept
    {
        resolve_symlnk = enabl;
    }
    
private:
    /** The access modes to check. */
    containers::flags<system::filesystem::access_modes> access_mods_ =
            system::filesystem::access_modes::NIL;
    
    /** The types of file to check. */
    containers::flags<system::filesystem::file_types> file_typs_ =
            system::filesystem::file_types::NIL;
    
    /** Specifies is symlinks has to be resolved. */
    bool resolve_symlnk = true;
};

/**
 * @brief       Path class decorator that checks whether the path can be read.
 */
template<typename BaseT>
class read_path_decorator : public BaseT
{
public:
    /** Component type. */
    using BaseT::BaseT;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        BaseT::set_access_modes(system::filesystem::access_modes::READ);
        return BaseT::is_valid(err_code);
    }
};

/**
 * @brief       Path class decorator that checks whether the path can be written.
 */
template<typename BaseT>
class write_path_decorator : public BaseT
{
public:
    /** Component type. */
    using BaseT::BaseT;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        BaseT::set_access_modes(system::filesystem::access_modes::WRITE);
        return BaseT::is_valid(err_code);
    }
};

/**
 * @brief       Path class decorator that checks whether the path can be executed.
 */
template<typename BaseT>
class execute_path_decorator : public BaseT
{
public:
    /** Component type. */
    using BaseT::BaseT;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        BaseT::set_access_modes(system::filesystem::access_modes::EXECUTE);
        return BaseT::is_valid(err_code);
    }
};

/**
 * @brief       Path class decorator that checks whether the path is a regular file.
 */
template<typename BaseT>
class regular_file_path_decorator : public BaseT
{
public:
    /** Component type. */
    using BaseT::BaseT;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        BaseT::set_file_types(system::filesystem::file_types::REGULAR_FILE);
        return BaseT::is_valid(err_code);
    }
};

/**
 * @brief       Path class decorator that checks whether the path is a directory.
 */
template<typename BaseT>
class directory_path_decorator : public BaseT
{
public:
    /** Component type. */
    using BaseT::BaseT;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        BaseT::set_file_types(system::filesystem::file_types::DIRECTORY);
        return BaseT::is_valid(err_code);
    }
};

/**
 * @brief       Path class decorator that checks whether the regular file path can be used as an
 *              output.
 */
template<typename BaseT>
class output_regular_file_path_decorator : public BaseT
{
public:
    /** Component type. */
    using BaseT::BaseT;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        BaseT::set_access_modes(system::filesystem::access_modes::WRITE);
        BaseT::set_file_types(system::filesystem::file_types::REGULAR_FILE);
        
        if (!system::filesystem::access(BaseT::c_str(), system::filesystem::access_modes::EXISTS))
        {
            auto parent_pth = BaseT::parent_path();
            if (!parent_pth.empty())
            {
                system::filesystem::mkdir_recursively(parent_pth.c_str());
            }
            if (!system::filesystem::touch(BaseT::c_str(), err_code))
            {
                return false;
            }
        }
        
        return BaseT::is_valid(err_code);
    }
};

/**
 * @brief       Path class decorator that checks whether the directory path can be used as an
 *              output.
 */
template<typename BaseT>
class output_directory_path_decorator : public BaseT
{
public:
    /** Component type. */
    using BaseT::BaseT;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        BaseT::set_access_modes(system::filesystem::access_modes::WRITE);
        BaseT::set_file_types(system::filesystem::file_types::DIRECTORY);
        
        if (!system::filesystem::access(BaseT::c_str(), system::filesystem::access_modes::EXISTS))
        {
            if (!system::filesystem::mkdir_recursively(BaseT::c_str(), err_code))
            {
                return false;
            }
        }
        
        return BaseT::is_valid(err_code);
    }
};

/** Regular file path type. */
using regular_file_path = regular_file_path_decorator<valid_path>;

/** Directory path type. */
using directory_path = directory_path_decorator<valid_path>;

/** Regular file path type that can be executed. */
using x_regular_file_path = execute_path_decorator<regular_file_path>;

/** Regular file path type that can be written. */
using w_regular_file_path = write_path_decorator<regular_file_path>;

/** Regular file path type that can be written and executed. */
using wx_regular_file_path = write_path_decorator<execute_path_decorator<regular_file_path>>;

/** Regular file path type that can be read. */
using r_regular_file_path = read_path_decorator<regular_file_path>;

/** Regular file path type that can be read and executed. */
using rx_regular_file_path = read_path_decorator<execute_path_decorator<regular_file_path>>;

/** Regular file path type that can be read and written. */
using rw_regular_file_path = read_path_decorator<write_path_decorator<regular_file_path>>;

/** Regular file path type that can be read, written and executed. */
using rwx_regular_file_path = read_path_decorator<
        write_path_decorator<execute_path_decorator<regular_file_path>>>;

/** Directory path type that can be executed. */
using x_directory_path = execute_path_decorator<directory_path>;

/** Directory path type that can be written. */
using w_directory_path = write_path_decorator<directory_path>;

/** Directory path type that can be written and executed. */
using wx_directory_path = write_path_decorator<execute_path_decorator<directory_path>>;

/** Directory path type that can be read. */
using r_directory_path = read_path_decorator<directory_path>;

/** Directory path type that can be read and executed. */
using rx_directory_path = read_path_decorator<execute_path_decorator<directory_path>>;

/** Directory path type that can be read and written. */
using rw_directory_path = read_path_decorator<write_path_decorator<directory_path>>;

/** Directory path type that can be read, written and executed. */
using rwx_directory_path = read_path_decorator<
        write_path_decorator<execute_path_decorator<directory_path>>>;

/** Regular file path type that can be used as an output file. */
using output_regular_file_path = output_regular_file_path_decorator<valid_path>;

/** Directory path type that can be used as an output file. */
using output_directory_path = output_directory_path_decorator<valid_path>;

}

#endif

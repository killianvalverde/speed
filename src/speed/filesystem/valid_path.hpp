/* speed - Generic C++ library.
 * Copyright (C) 2015-2024 Killian Valverde.
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
            return speed::system::filesystem::access(c_str(), access_mods_.get_value(),
                    file_typs_.get_value(), err_code);
        }
        else if (access_mods_.is_not_empty())
        {
            return speed::system::filesystem::access(c_str(), access_mods_.get_value(), err_code);
        }
        else if (file_typs_.is_not_empty())
        {
            return speed::system::filesystem::is_file_type(c_str(), file_typs_.get_value(),
                    err_code);
        }
        else
        {
            return speed::system::filesystem::access(c_str(),
                    speed::system::filesystem::access_modes::EXISTS, err_code);
        }
    }
    
protected:
    /**
     * @brief       Sets the allowed access modes.
     * @param       access_mods : A bitmask or enumeration value representing the desired access
     *              modes.
     */
    inline void set_access_modes(speed::system::filesystem::access_modes access_mods) noexcept
    {
        access_mods_.set(access_mods);
    }
    
    /**
     * @brief       Sets the file types to filter or use.
     * @param       file_typs : A bitmask or enumeration value representing the file types to set.
     */
    inline void set_file_types(speed::system::filesystem::file_types file_typs) noexcept
    {
        file_typs_.set(file_typs);
    }
    
private:
    /** The access modes to check. */
    speed::containers::flags<speed::system::filesystem::access_modes> access_mods_ =
            system::filesystem::access_modes::NIL;
    
    /** The types of file to check. */
    speed::containers::flags<speed::system::filesystem::file_types> file_typs_ =
            system::filesystem::file_types::NIL;
};

/**
 * @brief       Path class decorator that checks whether the path can be read.
 */
template<typename TpBase>
class read_path_decorator : public TpBase
{
public:
    /** Component type. */
    using TpBase::TpBase;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        TpBase::set_access_modes(speed::system::filesystem::access_modes::READ);
        return TpBase::is_valid(err_code);
    }
};

/**
 * @brief       Path class decorator that checks whether the path can be written.
 */
template<typename TpBase>
class write_path_decorator : public TpBase
{
public:
    /** Component type. */
    using TpBase::TpBase;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        TpBase::set_access_modes(speed::system::filesystem::access_modes::WRITE);
        return TpBase::is_valid(err_code);
    }
};

/**
 * @brief       Path class decorator that checks whether the path can be executed.
 */
template<typename TpBase>
class execute_path_decorator : public TpBase
{
public:
    /** Component type. */
    using TpBase::TpBase;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        TpBase::set_access_modes(speed::system::filesystem::access_modes::EXECUTE);
        return TpBase::is_valid(err_code);
    }
};

/**
 * @brief       Path class decorator that checks whether the path is a regular file.
 */
template<typename TpBase>
class regular_file_path_decorator : public TpBase
{
public:
    /** Component type. */
    using TpBase::TpBase;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        TpBase::set_file_types(speed::system::filesystem::file_types::REGULAR_FILE);
        return TpBase::is_valid(err_code);
    }
};

/**
 * @brief       Path class decorator that checks whether the path is a directory.
 */
template<typename TpBase>
class directory_path_decorator : public TpBase
{
public:
    /** Component type. */
    using TpBase::TpBase;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        TpBase::set_file_types(speed::system::filesystem::file_types::DIRECTORY);
        return TpBase::is_valid(err_code);
    }
};

/**
 * @brief       Path class decorator that checks whether the regular file path can be used as an
 *              output.
 */
template<typename TpBase>
class output_regular_file_path_decorator : public TpBase
{
public:
    /** Component type. */
    using TpBase::TpBase;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        TpBase::set_access_modes(speed::system::filesystem::access_modes::WRITE);
        TpBase::set_file_types(speed::system::filesystem::file_types::REGULAR_FILE);
        
        if (!TpBase::is_valid(err_code))
        {
            return speed::system::filesystem::touch(TpBase::c_str(), err_code);
        }
        
        return true;
    }
};

/**
 * @brief       Path class decorator that checks whether the directory path can be used as an
 *              output.
 */
template<typename TpBase>
class output_directory_path_decorator : public TpBase
{
public:
    /** Component type. */
    using TpBase::TpBase;

    /**
     * @brief       Allows knowing whether the path is valid.
     * @param       err_code : If function fails it holds the platform-dependent error code.
     * @return      If function is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(std::error_code* err_code = nullptr) override
    {
        TpBase::set_access_modes(speed::system::filesystem::access_modes::WRITE);
        TpBase::set_file_types(speed::system::filesystem::file_types::DIRECTORY);
        
        if (!TpBase::is_valid(err_code))
        {
            return speed::system::filesystem::mkdir_recursively(TpBase::c_str(), err_code);
        }
        
        return true;
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

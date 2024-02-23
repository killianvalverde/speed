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
 * @file        speed/argparse/basic_arg_value.hpp
 * @brief       basic_arg_value class header.
 * @author      Killian Valverde
 * @date        2015/12/31 - 03:15
 */

#ifndef SPEED_ARGPARSE_BASIC_ARG_VALUE_HPP
#define SPEED_ARGPARSE_BASIC_ARG_VALUE_HPP

#include <filesystem>
#include <regex>
#include <stdexcept>
#include <string>
#include <system_error>

#include "../containers.hpp"
#include "../system.hpp"
#include "../type_casting.hpp"
#include "argparse_type_casters.hpp"
#include "arg_error_flags.hpp"
#include "arg_flags.hpp"
#include "arg_value_error_flags.hpp"
#include "arg_value_types.hpp"
#include "argparse_forward_declarations.hpp"
#include "basic_value_arg.hpp"


namespace speed::argparse {


/**
 * @brief       Class that represents a value for an argument.
 */
template<typename TpAllocator>
class basic_arg_value
{
public:
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;
    
    /** Class that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpAllocator>;
    
    /** Class that represents flags container */
    template<typename T>
    using flags_type = speed::containers::flags<T>;
    
    /** Class that represent sequences of characters. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;
    
    /**
     * @brief       Default constructor.
     */
    basic_arg_value()
            : val_()
            , str_cstr_(nullptr)
            , regx_to_match_()
            , err_flgs_(arg_value_error_flags::NIL)
            , err_message_()
            , compo_(nullptr)
    {
    }
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       val : Argument value in string format.
     * @param       typ : Type of the value.
     * @param       regx_to_match : Regex that the value has to match. If the string is empty the
     *              value will always match.
     * @param       compo : The composite object.
     * @throw       speed::argparse::regex_syntax_error_exception : An exception is thrown if a 
     *              regex with a syntax error is specified.
     */
    template<
            typename TpString1_,
            typename TpString2_ = string_type,
            typename = std::enable_if_t<
                    !std::is_base_of<basic_arg_value, std::decay_t<TpString1_>>::value
            >
    >
    basic_arg_value(
            TpString1_&& val,
            speed::type_casting::i_string_caster* str_cstr = &str_to_str,
            TpString2_&& regx_to_match = string_type(),
            const value_arg_type* compo = nullptr
    )
            : val_(std::forward<TpString1_>(val))
            , str_cstr_(str_cstr)
            , regx_to_match_(std::forward<TpString2_>(regx_to_match))
            , err_flgs_(arg_value_error_flags::NIL)
            , err_message_()
            , compo_(compo)
    {
        if (str_cstr_ == nullptr)
        {
            str_cstr_ = &str_to_str;
        }
        
        check_validity();
    }

    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_arg_value(const basic_arg_value& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_arg_value(basic_arg_value&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_arg_value() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_arg_value& operator =(const basic_arg_value& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_arg_value& operator =(basic_arg_value&& rhs) noexcept = default;
    
    /**
     * @brief       Get the raw string object.
     * @return      The raw string object.
     */
    inline const string_type& get_value() const noexcept
    {
        return val_;
    }
    
    /**
     * @brief       Allows knowing whether the value can be coverted to the specified type.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool is_valid() const noexcept
    {
        if (!regx_to_match.empty())
        {
            try
            {
                if (!std::regex_match(val_, std::regex(regx_to_match)))
                {
                    return false;
                }
            }
            catch (const std::regex_error& re)
            {
                return false;
            }
        }

        return str_cstr_->is_valid(&val_);
    }
    
    /**
     * @brief       Get the value converted to tp.
     * @return      The value converted to tp.
     * @throw       speed::type_casting::type_casting_exception : If no conversion could be 
     *              performed, an exception is thrown.
     */
    template<typename TpTarget_>
    TpTarget_ as() const
    {
        return speed::type_casting::type_cast<TpTarget_>(val_);
    }
    
    /**
     * @brief       Get the value converted to tp.
     * @param       default_val : The value returned if the conversion fails.
     * @return      If function was successful the value converted to tp is returned, otherwise the
     *              default value specified is returned.
     */
    template<typename TpTarget_, typename TpDefaultValue_>
    TpTarget_ as(TpDefaultValue_&& default_val) const
    {
        return speed::type_casting::type_cast<TpTarget_>(
                val_, std::forward<TpDefaultValue_>(default_val));
    }
    
    /**
     * @brief       Try to get the value converted to tp.
     * @param       res : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TpTarget_>
    bool try_as(TpTarget_* res) const noexcept
    {
        return speed::type_casting::try_type_cast<TpTarget_>(val_, res);
    }
    
    /**
     * @brief       Allows knowing whether the argument value has errors.
     * @return      If there are errors true is returned, otherwise false is returned.
     */
    inline bool there_are_errors() const noexcept
    {
        return !err_flgs_.is_empty();
    }
    
    /**
     * @brief       Print argument value errors.
     * @param       prog_name : The name of the program.
     * @param       err_id : The argument error id.
     * @param       colrs_enable : If it is true, colors will be used during the print.
     */
    void print_errors(
            const string_type& prog_name,
            const string_type& err_id,
            bool colrs_enable
    ) const
    {
        if (err_flgs_.is_set(arg_value_error_flags::WRONG_VALUE_ERROR) ||
            err_flgs_.is_set(arg_value_error_flags::REGEX_TO_MATCH_ERROR))
        {
            std::cout << prog_name << ": ";
            
            if (!err_id.empty() && 
                (!err_flgs_.is_set(arg_value_error_flags::INVALID_PATH_ERROR) || 
                 composite_flag_is_set(arg_flags::PRINT_ERROR_ID_WHEN_PATH_ERROR)))
            {
                if (colrs_enable)
                {
                    std::cout << speed::iostream::set_light_red_text
                              << err_id << ": "
                              << speed::iostream::set_default_text;
                }
                else
                {
                    std::cout << err_id << ": ";
                }
            }
            
            if (err_flgs_.is_set(arg_value_error_flags::INVALID_PATH_ERROR))
            {
                if (colrs_enable)
                {
                    std::cout << speed::iostream::set_light_red_text
                              << val_ << ": "
                              << speed::iostream::set_default_text
                              << err_message_
                              << speed::iostream::newl;
                }
                else
                {
                    std::cout << val_ << ": " << err_message_ << speed::iostream::newl;
                }
            }
            else
            {
                std::cout << err_message_ << " '" << val_ << speed::iostream::newl;
            }
        }
    }

private:
    /**
     * @brief       Allows knowing whether the composite object has a specified flag set.
     * @param       af : The flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool composite_flag_is_set(arg_flags af) const noexcept
    {
        if (compo_ != nullptr)
        {
            return compo_->flag_is_set(af);
        }
        
        return false;
    }
    
    /**
     * @brief       Allows knowing whether the value can be coverted to the specified type, and if
     *              not error flags will be set.
     * @return      If function was successful true is returned, otherwise false is returned.
     * @throw       speed::argparse::regex_syntax_error_exception : An exception is thrown if a
     *              regex with a syntax error is specified.
     */
    bool check_validity()
    {
        bool succs;

        if (!regx_to_match.empty())
        {
            try
            {
                if (!std::regex_match(val_, std::regex(regx_to_match)))
                {
                    err_flgs_.set(arg_value_error_flags::REGEX_TO_MATCH_ERROR);
                    err_message_ = "Invalid argument";
                }
            }
            catch (const std::regex_error& re)
            {
                throw regex_syntax_error_exception();
            }
        }

        succs = str_cstr_->is_valid(&val_);
        if (!succs)
        {
            err_flgs_.set(arg_value_error_flags::WRONG_VALUE_ERROR);

            if (std::is_arithmetic<TpTarget_>::value)
            {
                err_message_ = "Invalid number";
            }
            else
            {
                err_message_ = "Invalid argument";
            }
        }
        
        return succs;
    }
    
    /**
     * @brief       Allows knowing whether the value is a valid path.
     * @return      If function was successful true is returned, otherwise false is returne.
     */
    template<typename TpTarget_>
    std::enable_if_t<
            speed::type_traits::is_path<TpTarget_>::value,
            bool
    >
    check_value(arg_value_types cur_avt)
    {
        std::error_code err_code;
        speed::system::filesystem::ft_t ft = speed::system::filesystem::ft_t::NIL;
        bool succss = true;
        
        if (!speed::system::filesystem::access(
                val_.c_str(), speed::system::filesystem::am_t::EXISTS))
        {
            if (typ_.is_set(arg_value_types::C_REG_FILE))
            {
                if (!speed::system::filesystem::touch(val_.c_str(), 0644, &err_code))
                {
                    err_message_ = err_code.message();
                    succss = false;
                }
            }
            if (typ_.is_set(arg_value_types::C_DIR))
            {
                if (!speed::system::filesystem::mkdir(val_.c_str(), 0755, &err_code))
                {
                    err_message_ = err_code.message();
                    succss = false;
                }
            }
        }
        
        if (cur_avt != arg_value_types::C_REG_FILE &&
            cur_avt != arg_value_types::C_DIR &&
            !speed::system::filesystem::access(
                val_.c_str(),
                avt_to_am[get_avt_file_index(cur_avt)],
                avt_to_ft[get_avt_file_index(cur_avt)],
                &err_code))
        {
            if (cur_avt >= arg_value_types::R_REG_FILE && cur_avt < arg_value_types::R_DIR)
            {
                ft = speed::system::filesystem::ft_t::REGULAR_FILE;
            }
            else if (cur_avt >= arg_value_types::R_DIR)
            {
                ft = speed::system::filesystem::ft_t::DIRECTORY;
            }
            
            succss = false;
        }
        
        if (!succss)
        {
            err_flgs_.set(arg_value_error_flags::INVALID_PATH_ERROR);
    
            if (err_code.value() == EINVAL)
            {
                if (ft == speed::system::filesystem::ft_t::NIL)
                {
                    err_message_ = "Not a file";
                }
                else if (ft == speed::system::filesystem::ft_t::REGULAR_FILE)
                {
                    err_message_ = "Not a regular file";
                }
                else
                {
                    err_message_ = "Not a directory";
                }
            }
            else
            {
                err_message_ = err_code.message();
            }
        }
        
        return succss;
    }

private:
    /** Argument value container. */
    string_type val_;

    /** Type caster used to validate the value syntax. */
    speed::type_casting::i_string_caster* str_cstr_;
    
    /** Regex that the value has to match. If the string is empty the value will always match. */
    string_type regx_to_match_;
    
    /** Error flags that allows knowing whether there are errors. */
    flags_type<arg_value_error_flags> err_flgs_;
    
    /** Message to be displayed when there are errors. */
    string_type err_message_;
    
    /** Composite flags. */
    const value_arg_type* compo_;
};


/** Class that represents a value for an argument. */
using arg_value = basic_arg_value<std::allocator<int>>;
    
    
}


#endif

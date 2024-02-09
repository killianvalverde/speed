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
            , invalid_pth_(false)
            , err_message_()
            , fles_created_(false)
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
            , invalid_pth_(false)
            , err_message_()
            , fles_created_(false)
            , compo_(compo)
    {
        if (str_cstr_ == nullptr)
        {
            str_cstr_ = &str_to_str;
        }

        if (!str_cstr_->is_valid(&val_))
        {
            err_flgs_.set(arg_value_error_flags::WRONG_VALUE_ERROR);
        }
        
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
                throw regex_syntax_error_exception(); /// ?
            }
        }
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
     * @brief       Allows knowing whether the value can be coverted to the specified type.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename T>
    std::enable_if_t<
            !speed::type_traits::is_path<T>::value,
            bool
    >
    is_type_valid() const
    {
        if (std::is_arithmetic<T>::value)
        {
            if (composite_flag_is_set(arg_flags::ALLOW_MIN_CONSTANT) &&
                val_ == "min")
            {
                return true;
            }
            else if (composite_flag_is_set(arg_flags::ALLOW_MAX_CONSTANT) &&
                     val_ == "max")
            {
                return true;
            }
        }
        
        T aux;
        return speed::type_casting::try_type_cast<T>(val_, &aux);
    }
    
    /**
     * @brief       Allows knowing whether the value can be coverted to the specified type.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename T>
    std::enable_if_t<
            speed::type_traits::is_path<T>::value,
            bool
    >
    is_type_valid() const noexcept
    {
        bool scs = true;
        
        for (auto& x : typ_)
        {
            if (!speed::system::filesystem::access(val_.c_str(),
                    avt_to_am[get_avt_file_index(x)],
                    avt_to_ft[get_avt_file_index(x)]))
            {
                scs = false;
            }
        }
        
        return scs;
    }
    
    /**
     * @brief       Get the raw string object.
     * @return      The raw string object.
     */
    inline const string_type& get_value() const noexcept
    {
        return val_;
    }
    
    /**
     * @brief       Get the value converted to tp.
     * @return      The value converted to tp.
     * @throw       speed::type_casting::type_casting_exception : If no conversion could be performed,
     *              an exception is thrown.
     */
    template<typename TpTarget_>
    std::enable_if_t<
            std::is_arithmetic<TpTarget_>::value,
            TpTarget_
    >
    as() const
    {
        if (composite_flag_is_set(arg_flags::ALLOW_MIN_CONSTANT) &&
            val_ == "min")
        {
            return std::numeric_limits<TpTarget_>::min();
        }
        else if (composite_flag_is_set(arg_flags::ALLOW_MAX_CONSTANT) &&
                 val_ == "max")
        {
            return std::numeric_limits<TpTarget_>::max();
        }
        
        return speed::type_casting::type_cast<TpTarget_>(val_);
    }
    
    /**
     * @brief       Get the value converted to tp.
     * @return      The value converted to tp.
     * @throw       speed::type_casting::type_casting_exception : If no conversion could be performed,
     *              an exception is thrown.
     */
    template<typename TpTarget_>
    inline std::enable_if_t<
            !std::is_arithmetic<TpTarget_>::value,
            TpTarget_
    >
    as() const
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
    std::enable_if_t<
            std::is_arithmetic<TpTarget_>::value,
            TpTarget_
    >
    as(TpDefaultValue_&& default_val) const
    {
        if (composite_flag_is_set(arg_flags::ALLOW_MIN_CONSTANT) && val_ == "min")
        {
            return std::numeric_limits<TpTarget_>::min();
        }
        else if (composite_flag_is_set(arg_flags::ALLOW_MAX_CONSTANT) && val_ == "max")
        {
            return std::numeric_limits<TpTarget_>::max();
        }
    
        return speed::type_casting::type_cast<TpTarget_>(
                val_, std::forward<TpDefaultValue_>(default_val));
    }
    
    /**
     * @brief       Get the value converted to tp.
     * @param       default_val : The value returned if the conversion fails.
     * @return      If function was successful the value converted to tp is returned, otherwise the
     *              default value specified is returned.
     */
    template<typename TpTarget_, typename TpDefaultValue_>
    std::enable_if_t<
            !std::is_arithmetic<TpTarget_>::value,
            TpTarget_
    >
    as(TpDefaultValue_&& default_val) const
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
    std::enable_if_t<
            std::is_arithmetic<TpTarget_>::value,
            bool
    >
    try_as(TpTarget_* res) const noexcept
    {
        if (composite_flag_is_set(arg_flags::ALLOW_MIN_CONSTANT) && val_ == "min")
        {
            *res = std::numeric_limits<TpTarget_>::min();
            return true;
        }
        else if (composite_flag_is_set(arg_flags::ALLOW_MAX_CONSTANT) && val_ == "max")
        {
            *res = std::numeric_limits<TpTarget_>::max();
            return true;
        }
        
        return speed::type_casting::try_type_cast<TpTarget_>(val_, res);
    }
    
    /**
     * @brief       Try to get the value converted to tp.
     * @param       res : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TpTarget_>
    inline std::enable_if_t<
            !std::is_arithmetic<TpTarget_>::value,
            bool
    >
    try_as(TpTarget_* res) const noexcept
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
                (!invalid_pth_ || composite_flag_is_set(arg_flags::PRINT_ERROR_ID_WHEN_PATH_ERROR)))
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
            
            if (invalid_pth_)
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
                std::cout << err_message_ << " '" << val_ << "'\n";
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
     * @brief       Allows knowing whether the value can be coverted to the specified type.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TpTarget_>
    std::enable_if_t<
            !speed::type_traits::is_path<TpTarget_>::value,
            bool
    >
    check_value(arg_value_types cur_avt)
    {
        if (std::is_arithmetic<TpTarget_>::value)
        {
            if (composite_flag_is_set(arg_flags::ALLOW_MIN_CONSTANT) && val_ == "min")
            {
                return true;
            }
            else if (composite_flag_is_set(arg_flags::ALLOW_MAX_CONSTANT) && val_ == "max")
            {
                return true;
            }
        }
        
        TpTarget_ aux;
        if (!speed::type_casting::try_type_cast<TpTarget_>(val_, &aux))
        {
            if (std::is_arithmetic<TpTarget_>::value)
            {
                err_message_ = "Invalid number";
            }
            else
            {
                err_message_ = "Invalid argument";
            }
            
            return false;
        }
        
        return true;
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
    
        if (!fles_created_)
        {
            fles_created_ = true;
            
            if (!speed::system::filesystem::access(val_.c_str(), 
                                                   speed::system::filesystem::am_t::EXISTS))
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
        }
        
        if (cur_avt != arg_value_types::C_REG_FILE &&
            cur_avt != arg_value_types::C_DIR &&
            !speed::system::filesystem::access(val_.c_str(),
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
            invalid_pth_ = true;
    
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
    
    /**
     * @brief       Get the index array for an argument value type.
     * @param       avt : The argument value type.
     * @return      The index array for an argument value type.
     */
    static constexpr std::size_t get_avt_index(arg_value_types avt)
    {
        return static_cast<std::size_t>(
                speed::lowlevel::onehot_to_binary(
                        static_cast<std::underlying_type_t<arg_value_types>>(avt)) - 1);
    }
    
    /**
     * @brief       Get the index array for an argument value type that is a file.
     * @param       avt : The argument value type that is a file.
     * @return      The index array for an argument value type that is a file.
     */
    static constexpr std::size_t get_avt_file_index(arg_value_types avt)
    {
        constexpr std::uint8_t bse_val =
                speed::lowlevel::onehot_to_binary(
                        static_cast<std::underlying_type_t<arg_value_types>>(
                                arg_value_types::R_FILE));
        
        return speed::lowlevel::onehot_to_binary(
                static_cast<std::underlying_type_t<arg_value_types>>(avt)) - bse_val;
    }

private:
    /** Argument value container. */
    string_type val_;

    speed::type_casting::i_string_caster* str_cstr_;
    
    /** Type of the value. */
    flags_type<arg_value_types> typ_;
    
    /** Regex that the value has to match. If the string is empty the value will always match. */
    string_type regx_to_match_;
    
    /** Error flags that allows knowing whether there are errors. */
    flags_type<arg_value_error_flags> err_flgs_;
    
    /** Indicates whether the path is invalid. */
    bool invalid_pth_;
    
    /** Message to be displayed when there are errors. */
    string_type err_message_;
    
    /** Used in the paths parsing methods. */
    bool fles_created_;
    
    /** Composite flags. */
    const value_arg_type* compo_;
    
    /** Pointer to a method. */
    typedef bool (basic_arg_value::*p_check_value)(arg_value_types);
    
    /** Array to match an argument value type with the check_value class method to execute. */
    static constexpr p_check_value avt_to_check_value[32] = {
            &basic_arg_value::check_value<bool>,
            &basic_arg_value::check_value<double>,
            &basic_arg_value::check_value<float>,
            &basic_arg_value::check_value<int>,
            &basic_arg_value::check_value<std::int8_t>,
            &basic_arg_value::check_value<std::int16_t>,
            &basic_arg_value::check_value<std::int32_t>,
            &basic_arg_value::check_value<std::int64_t>,
            &basic_arg_value::check_value<long>,
            &basic_arg_value::check_value<long double>,
            &basic_arg_value::check_value<long long>,
            &basic_arg_value::check_value<short>,
            &basic_arg_value::check_value<string_type>,
            &basic_arg_value::check_value<std::uint8_t>,
            &basic_arg_value::check_value<std::uint16_t>,
            &basic_arg_value::check_value<std::uint32_t>,
            &basic_arg_value::check_value<std::uint64_t>,
            &basic_arg_value::check_value<unsigned int>,
            &basic_arg_value::check_value<unsigned long>,
            &basic_arg_value::check_value<unsigned long long>,
            &basic_arg_value::check_value<unsigned short>,
            &basic_arg_value::check_value<std::filesystem::path>,
            &basic_arg_value::check_value<std::filesystem::path>,
            &basic_arg_value::check_value<std::filesystem::path>,
            &basic_arg_value::check_value<std::filesystem::path>,
            &basic_arg_value::check_value<std::filesystem::path>,
            &basic_arg_value::check_value<std::filesystem::path>,
            &basic_arg_value::check_value<std::filesystem::path>,
            &basic_arg_value::check_value<std::filesystem::path>,
            &basic_arg_value::check_value<std::filesystem::path>,
            &basic_arg_value::check_value<std::filesystem::path>,
            &basic_arg_value::check_value<std::filesystem::path>
    };
    
    // TODO(KillianValverde@gmail.com): It used to be a CREATE flag that aparentally has been 
    //      deleted. I had to replace it by NIL since that flag no longer exists. We need
    //      to check if this keep working and how to make this creation feature available again.
    /** Array to match an argument value type that is a file with an access mode. */
    static constexpr speed::system::filesystem::am_t avt_to_am[11] = {
            speed::system::filesystem::am_t::READ,
            speed::system::filesystem::am_t::WRITE,
            speed::system::filesystem::am_t::EXECUTE,
            speed::system::filesystem::am_t::READ,
            speed::system::filesystem::am_t::WRITE,
            speed::system::filesystem::am_t::EXECUTE,
            speed::system::filesystem::am_t::NIL,
            speed::system::filesystem::am_t::READ,
            speed::system::filesystem::am_t::WRITE,
            speed::system::filesystem::am_t::EXECUTE,
            speed::system::filesystem::am_t::NIL
    };
    
    /** Array to match an argument value type that is a file with a file type. */
    static constexpr speed::system::filesystem::ft_t avt_to_ft[11] = {
            speed::system::filesystem::ft_t::NIL,
            speed::system::filesystem::ft_t::NIL,
            speed::system::filesystem::ft_t::NIL,
            speed::system::filesystem::ft_t::REGULAR_FILE,
            speed::system::filesystem::ft_t::REGULAR_FILE,
            speed::system::filesystem::ft_t::REGULAR_FILE,
            speed::system::filesystem::ft_t::NIL,
            speed::system::filesystem::ft_t::DIRECTORY,
            speed::system::filesystem::ft_t::DIRECTORY,
            speed::system::filesystem::ft_t::DIRECTORY,
            speed::system::filesystem::ft_t::NIL
    };
};


/** Class that represents a value for an argument. */
using arg_value = basic_arg_value<std::allocator<int>>;
    
    
}


#endif

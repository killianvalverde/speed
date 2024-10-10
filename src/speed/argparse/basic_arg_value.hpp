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
 * @date        2015/12/31
 */

#ifndef SPEED_ARGPARSE_BASIC_ARG_VALUE_HPP
#define SPEED_ARGPARSE_BASIC_ARG_VALUE_HPP

#include <memory>
#include <regex>
#include <string>
#include <system_error>
#include <utility>

#include "../containers/containers.hpp"
#include "../system/system.hpp"
#include "../type_casting/type_casting.hpp"
#include "arg_value_error_flags.hpp"
#include "basic_arg_parser.hpp"
#include "basic_value_arg.hpp"
#include "forward_declarations.hpp"


namespace speed::argparse {


/**
 * @brief       Class that represents a value for an argument.
 */
template<typename TpAllocator>
class basic_arg_value
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<TpAllocator>::template rebind_alloc<T>;

    /** Pair type used in the class. */
    template<typename T1, typename T2>
    using pair_type = std::pair<T1, T2>;

    /** Shared smart pointer type used in the class. */
    template<typename T>
    using shared_ptr_type = std::shared_ptr<T>;

    /** String type used in the class. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;

    /** Regex type used in the class. */
    using regex_type = std::basic_regex<char, std::regex_traits<char>>;

    /** Flags type used in the class. */
    template<typename T>
    using flags_type = speed::containers::flags<T>;

    /** Caster base type used in the class. */
    using caster_base_type = speed::type_casting::type_caster_base<string_type>;

    /** Type that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpAllocator>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;

    /**
     * @brief       Constructor with parameters.
     * @param       regx : Regex that the value has to match.
     * @param       val : The value.
     * @param       castr : Caster object used to parse the value.
     * @param       arg_parsr : Argument parser that holds this object.
     * @param       val_arg : Value arg that holds this object.
     */
    template<
            typename TpString_,
            typename TpRegex_ = regex_type,
            typename = std::enable_if_t<
                    !std::is_base_of<basic_arg_value, std::decay_t<TpString_>>::value
            >
    >
    basic_arg_value(
            TpRegex_&& regx,
            TpString_&& val,
            shared_ptr_type<caster_base_type> castr,
            arg_parser_type* arg_parsr,
            value_arg_type* val_arg
    )
            : regx_(std::forward<TpRegex_>(regx))
            , val_(std::forward<TpString_>(val))
            , err_message_()
            , castr_(std::move(castr))
            , arg_parsr_(arg_parsr)
            , val_arg_(val_arg)
            , err_flgs_(arg_value_error_flags::NIL)
    {
        parse_value();
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
     * @brief       Parse the value using the caster.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool parse_value()
    {
        std::error_code err_code;
        bool succs;

        err_flgs_.clear();

        if (!std::regex_match(val_, regx_))
        {
            err_flgs_.set(arg_value_error_flags::REGEX_TO_MATCH_ERROR);
            err_message_ = "Invalid argument";
            return false;
        }

        succs = (!castr_) || castr_->try_type_cast(val_, &err_code);

        if (!succs)
        {
            err_flgs_.set(arg_value_error_flags::WRONG_VALUE_ERROR);

            if (err_code == speed::type_casting::error_conditions::ARITHMETIC_ERROR)
            {
                err_message_ = "Invalid number";
            }
            else
            {
                err_message_ = err_code.message();

                if (err_code.category() == std::system_category())
                {
                    err_flgs_.set(arg_value_error_flags::INVALID_PATH_ERROR);
                }
            }

            if (err_message_.empty())
            {
                err_message_ = "Unknown error";
            }

            return false;
        }

        return true;
    }
    
    /**
     * @brief       Get the value in a string type.
     * @return      The value in a string type.
     */
    [[nodiscard]] inline const string_type& get_value() const noexcept
    {
        return val_;
    }
    
    /**
     * @brief       Get the value converted to tp.
     * @return      The value converted to tp.
     */
    template<typename TpTarget_>
    [[nodiscard]] TpTarget_ as() const
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
    [[nodiscard]] TpTarget_ as(TpDefaultValue_&& default_val) const noexcept
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
    [[nodiscard]] inline bool has_errors() const noexcept
    {
        return err_flgs_.is_not_empty();
    }
    
    /**
     * @brief       Print argument value errors.
     */
    void print_errors() const
    {
        if (err_flgs_.is_not_empty())
        {
            std::cout << arg_parsr_->get_program_name() << ": ";
            
            if (!val_arg_->is_error_name_empty() &&
                !err_flgs_.is_set(arg_value_error_flags::INVALID_PATH_ERROR))
            {
                if (arg_parsr_->colors_enabled())
                {
                    std::cout << speed::iostream::set_light_red_text
                              << val_arg_->get_error_name() << ": "
                              << speed::iostream::set_default_text;
                }
                else
                {
                    std::cout << val_arg_->get_error_name() << ": ";
                }
            }
            
            if (err_flgs_.is_set(arg_value_error_flags::INVALID_PATH_ERROR))
            {
                if (arg_parsr_->colors_enabled())
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
    /** Regex that the value has to match. */
    regex_type regx_;

    /** Argument value. */
    string_type val_;
    
    /** Message to be displayed when there are errors. */
    string_type err_message_;

    /** Type caster used to validate the value syntax. */
    shared_ptr_type<caster_base_type> castr_;

    /** Holds a reference to the argument parser object. */
    arg_parser_type* arg_parsr_;

    /** Holds a reference to the value arg object. */
    value_arg_type* val_arg_;

    /** Error flags that allows knowing whether there are errors. */
    flags_type<arg_value_error_flags> err_flgs_;

    friend class basic_value_arg<TpAllocator>;
    friend class basic_arg_parser<TpAllocator>;
};
    
    
}


#endif

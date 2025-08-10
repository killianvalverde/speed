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
 * @file        basic_arg_value.hpp
 * @brief       basic_arg_value class header.
 * @author      Killian Valverde
 * @date        2015/12/31
 */

#ifndef SPEED_ARGPARSE_DETAIL_BASIC_ARG_VALUE_HPP
#define SPEED_ARGPARSE_DETAIL_BASIC_ARG_VALUE_HPP

#include <memory>
#include <regex>
#include <string>
#include <system_error>
#include <utility>

#include "forward_declarations.hpp"
#include "../../containers/containers.hpp"
#include "../../system/system.hpp"
#include "../../type_casting/type_casting.hpp"
#include "arg_value_error_flags.hpp"
#include "speed/argparse/basic_arg_parser.hpp"
#include "basic_type_caster.hpp"
#include "basic_value_arg.hpp"

namespace speed::argparse::detail {

/**
 * @brief       Class that represents a value for an argument.
 */
template<typename AllocatorT>
class basic_arg_value
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<T>;

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
    
    /** Type that represents a validator lambda. */
    using assertion_type = std::function<bool(const string_type&)>;

    /** Flags type used in the class. */
    template<typename T>
    using flags_type = containers::flags<T>;

    /** Caster base type used in the class. */
    using caster_base_type = type_caster_base<string_type>;

    /** Type that represents arguments that have values. */
    using value_arg_type = basic_value_arg<AllocatorT>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<AllocatorT>;

    /**
     * @brief       Constructor with parameters.
     * @param       val : The value.
     * @param       castr : Caster object used to parse the value.
     * @param       assrt : Assetion that needs to be valid.
     * @param       regx : Regex that the value has to match.
     * @param       arg_parsr : Argument parser that holds this object.
     * @param       val_arg : Value arg that holds this object.
     */
    template<
            typename StringT_,
            typename RegexT_ = regex_type,
            typename = std::enable_if_t<
                    !std::is_base_of<basic_arg_value, std::decay_t<StringT_>>::value
            >
    >
    basic_arg_value(
            StringT_&& val,
            caster_base_type* castr,
            assertion_type* assrt,
            regex_type* regx,
            arg_parser_type* arg_parsr,
            value_arg_type* val_arg
    )
            : val_(std::forward<StringT_>(val))
            , castr_(castr)
            , assrt_(assrt)
            , regx_(regx)
            , arg_parsr_(arg_parsr)
            , val_arg_(val_arg)
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
        
        if (assrt_ && !(*assrt_)(val_))
        {
            err_flgs_.set(arg_value_error_flags::ASSERTION_ERROR);
            err_message_ = "Invalid argument";
            return false;
        }

        if (regx_ != nullptr && !std::regex_match(val_, *regx_))
        {
            err_flgs_.set(arg_value_error_flags::REGEX_MATCH_ERROR);
            err_message_ = "Invalid argument";
            return false;
        }

        succs = !castr_ || castr_->try_type_cast(val_, &err_code);

        if (!succs)
        {
            err_flgs_.set(arg_value_error_flags::WRONG_VALUE_ERROR);

            if (err_code == type_casting::errc::ARITHMETIC_ERROR)
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
    template<typename TargetT_>
    [[nodiscard]] TargetT_ as() const
    {
        return type_casting::type_cast<TargetT_>(val_);
    }
    
    /**
     * @brief       Get the value converted to tp.
     * @param       default_val : The value returned if the conversion fails.
     * @return      If function was successful the value converted to tp is returned, otherwise the
     *              default value specified is returned.
     */
    template<typename TargetT_, typename DefaultValueT_>
    [[nodiscard]] TargetT_ as(DefaultValueT_&& default_val) const noexcept
    {
        return type_casting::type_cast<TargetT_>(val_, std::forward<DefaultValueT_>(default_val));
    }
    
    /**
     * @brief       Try to get the value converted to tp.
     * @param       res : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TargetT_>
    bool try_as(TargetT_& res) const noexcept
    {
        return type_casting::try_type_cast<TargetT_>(val_, res);
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
        if (err_flgs_.is_empty())
        {
            return;
        }
        
        auto& os = arg_parsr_->get_ostream();
        
        os << arg_parsr_->get_program_name() << ": ";
        
        if (!val_arg_->is_error_name_empty() &&
            !err_flgs_.is_set(arg_value_error_flags::INVALID_PATH_ERROR))
        {
            if (arg_parsr_->colors_enabled())
            {
                os << iostream::set_light_red_text
                   << val_arg_->get_error_name() << ": "
                   << iostream::set_default_text;
            }
            else
            {
                os << val_arg_->get_error_name() << ": ";
            }
        }
        
        if (err_flgs_.is_set(arg_value_error_flags::INVALID_PATH_ERROR))
        {
            if (arg_parsr_->colors_enabled())
            {
                os << iostream::set_light_red_text
                   << val_ << ": "
                   << iostream::set_default_text
                   << err_message_
                   << iostream::newl;
            }
            else
            {
                os << val_ << ": " << err_message_ << iostream::newl;
            }
        }
        else
        {
            os << err_message_ << " '" << val_ << "'\n";
        }
    }

private:
    /** Argument value. */
    string_type val_;
    
    /** Message to be displayed when there are errors. */
    string_type err_message_;

    /** Type caster used to validate the value syntax. */
    caster_base_type* castr_;
    
    /** Function to execute in order to know if the value is valid. */
    assertion_type* assrt_;
    
    /** Regex that the value has to match. */
    regex_type* regx_;

    /** Holds a reference to the argument parser object. */
    arg_parser_type* arg_parsr_;

    /** Holds a reference to the value arg object. */
    value_arg_type* val_arg_;

    /** Error flags that allows knowing whether there are errors. */
    flags_type<arg_value_error_flags> err_flgs_ = arg_value_error_flags::NIL;
};

}

#endif

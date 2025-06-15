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
 * @file        basic_key_value_arg.hpp
 * @brief       basic_key_value_arg class header.
 * @author      Killian Valverde
 * @date        2016/03/11
 */

#ifndef SPEED_ARGPARSE_BASIC_KEY_VALUE_ARG_HPP
#define SPEED_ARGPARSE_BASIC_KEY_VALUE_ARG_HPP

#include <string>
#include <utility>
#include <vector>

#include "arg_flags.hpp"
#include "basic_arg_key.hpp"
#include "basic_arg_parser.hpp"
#include "basic_base_arg.hpp"
#include "basic_key_arg.hpp"
#include "basic_value_arg.hpp"
#include "exception.hpp"
#include "forward_declarations.hpp"

namespace speed::argparse {

/**
 * @brief       Class that represents arguments that have keys and values.
 */
template<typename TpAllocator>
class basic_key_value_arg
        : public basic_key_arg<TpAllocator>
        , public basic_value_arg<TpAllocator>
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<TpAllocator>::template rebind_alloc<T>;

    /** String type used in the class. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;

    /** Vector type used in the class. */
    template<typename T>
    using vector_type = std::vector<T, allocator_type<T>>;

    /** Type that represents a key for an argument. */
    using arg_key_type = basic_arg_key<TpAllocator>;

    /** Type that represents a value for an argument. */
    using arg_value_type = basic_arg_value<TpAllocator>;
    
    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;
    
    /** Type that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<TpAllocator>;

    /** Type that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpAllocator>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;

    /**
     * @brief       Constructor with parameters.
     * @param       arg_parsr : Argument parser that holds this object.
     * @param       kys : Argument keys.
     */
    template<typename... Ts_>
    explicit basic_key_value_arg(arg_parser_type* arg_parsr, Ts_&&... kys)
            : base_arg_type(arg_parsr)
            , key_arg_type(arg_parsr, std::forward<Ts_>(kys)...)
            , value_arg_type(arg_parsr)
            , usage_str_needs_refresh_(true)
    {
        base_arg_type::set_flags(arg_flags::DEFAULT_KEY_VALUE_ARG_FLAGS);
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_key_value_arg(const basic_key_value_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_key_value_arg(basic_key_value_arg&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_key_value_arg() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_key_value_arg& operator =(const basic_key_value_arg& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_key_value_arg& operator =(basic_key_value_arg&& rhs) noexcept = default;

    /**
     * @brief       Build the usage string.
     */
    void build_usage_string()
    {
        std::size_t cnt = 0;

        if (!usage_str_needs_refresh_)
        {
            return;
        }

        usage_str_.clear();

        if (value_arg_type::get_max_values() == 0)
        {
            return;
        }

        if (vals_names_.empty())
        {
            build_usage_string_value("VALUE", cnt);
        }
        for (auto& val_name : vals_names_)
        {
            build_usage_string_value(val_name, cnt);
        }

        if (cnt < value_arg_type::get_max_values())
        {
            usage_str_ += "...";
        }

        usage_str_needs_refresh_ = false;
    }

    /**
     * @brief       Get the necessary length to print long arguments keys.
     * @return      The necessary length to print long arguments keys.
     */
    [[nodiscard]] inline std::size_t get_long_keys_length() noexcept override
    {
        std::size_t additional_len = 0;

        if (base_arg_type::is_help_text_empty())
        {
            return 0;
        }

        if (key_arg_type::has_long_prefix_keys())
        {
            build_usage_string();
            additional_len = usage_str_.length();
        }

        return speed::safety::addm(key_arg_type::get_long_keys_length(), additional_len);
    }

    /**
     * @brief       Get the necessary length to print short arguments keys.
     * @return      The necessary length to print short arguments keys.
     */
    [[nodiscard]] inline std::size_t get_short_keys_length() noexcept override
    {
        std::size_t additional_len = 0;

        if (base_arg_type::is_help_text_empty())
        {
            return 0;
        }

        if (!key_arg_type::has_long_prefix_keys())
        {
            build_usage_string();
            additional_len = usage_str_.length();
        }

        return speed::safety::addm(key_arg_type::get_short_keys_length(), additional_len);
    }

    /**
     * @brief       Set an argument flag.
     * @param       flg : Flag to set.
     */
    inline void set_flag(arg_flags flg) noexcept override
    {
        base_arg_type::set_flag(flg);
        if (flg == arg_flags::ASSIGNMENT_OPERATOR)
        {
            usage_str_needs_refresh_ = true;
        }
    }

    /**
     * @brief       Set the minimum and maximum number of values.
     * @param       min : Minimum number of values.
     * @param       max : Maximum number of values.
     */
    void set_minmax_values(std::size_t min, std::size_t max) override
    {
        value_arg_type::set_minmax_values(min, max);
        usage_str_needs_refresh_ = true;
    }

    /**
     * @brief       Set the values names.
     * @param       vals_names : The values names.
     */
    template<typename... Ts_>
    void set_values_names(Ts_&&... vals_names)
    {
        vals_names_.clear();
        (vals_names_.push_back(std::forward<Ts_>(vals_names)), ...);

        for (auto& val_id : vals_names_)
        {
            if (val_id.empty())
            {
                vals_names_.clear();
                throw no_value_id_specified_exception();
            }
        }
        
        value_arg_type::update_minmax_values(vals_names_.size());
        usage_str_needs_refresh_ = true;
    }

    /**
     * @brief       Unset an argument flag.
     * @param       flg : Flag to unset.
     */
    inline void unset_flag(arg_flags flg) noexcept override
    {
        base_arg_type::unset_flag(flg);
        if (flg == arg_flags::ASSIGNMENT_OPERATOR)
        {
            usage_str_needs_refresh_ = true;
        }
    }

    /**
     * @brief       Print the usage.
     */
    void print_usage() override
    {
        build_usage_string();
        key_arg_type::print_usage();
        std::cout << usage_str_;
    }

    /**
     * @brief       Print the argument help text.
     * @param       args_indent : Indentation used to print arguments help description.
     * @param       max_line_len : The maximum line length that will be printed.
     * @param       new_line_indent : The indentation used after a newline is printed.
     * @param       short_kys_len : The maximum short keys length.
     * @param       long_kys_len : The maximum long keys length.
     */
    void print_help_text(
            std::size_t args_indent,
            std::size_t max_line_len,
            std::size_t new_line_indent,
            std::size_t short_kys_len,
            std::size_t long_kys_len
    ) override
    {
        if (base_arg_type::is_help_text_empty())
        {
            return;
        }
        
        std::size_t current_id_len = 0;
        std::size_t n_args_printd = 0;
        std::size_t i;

        build_usage_string();
    
        for (i = args_indent; i > 0; --i)
        {
            std::cout << ' ';
        }

        key_arg_type::print_keys(&n_args_printd, &current_id_len, true);

        if (!key_arg_type::has_long_prefix_keys())
        {
            std::cout << usage_str_;
            speed::safety::try_addm(&current_id_len, usage_str_.length());
        }
        
        if (n_args_printd < key_arg_type::get_keys_size() && n_args_printd > 0)
        {
            std::cout << ", ";
            speed::safety::try_addm(&current_id_len, 2);
        }
        
        if (current_id_len < short_kys_len)
        {
            for (i = short_kys_len - current_id_len; i > 0; --i)
            {
                std::cout << ' ';
            }
        }
        
        n_args_printd = 0;
        current_id_len = 0;

        key_arg_type::print_keys(&n_args_printd, &current_id_len, false);

        if (key_arg_type::has_long_prefix_keys())
        {
            std::cout << usage_str_;
            speed::safety::try_addm(&current_id_len, usage_str_.length());
        }
        
        if (current_id_len < long_kys_len)
        {
            for (i = long_kys_len - current_id_len; i > 0; --i)
            {
                std::cout << ' ';
            }
        }
    
        speed::safety::try_addm(&args_indent, short_kys_len, long_kys_len);
        speed::safety::try_addm(&new_line_indent, args_indent);
        
        base_arg_type::print_help_text(args_indent, max_line_len, new_line_indent);
    }
    
    /**
     * @brief       Print argument errors.
     */
    void print_errors() const override
    {
        if (!base_arg_type::has_errors())
        {
            return;
        }
        
        value_arg_type::print_errors();
    }

private:
    /**
     * @brief       Build the usage string value.
     * @param       val_name : The value name.
     * @param       cnt : The current number of processed values.
     */
    void build_usage_string_value(const string_type& val_name, std::size_t& cnt)
    {
        if (cnt > 0 || !base_arg_type::is_flag_set(arg_flags::ASSIGNMENT_OPERATOR))
        {
            usage_str_ += ' ';
        }

        if (value_arg_type::get_min_values() <= cnt)
        {
            usage_str_ += '[';
        }

        if (cnt == 0 && base_arg_type::is_flag_set(arg_flags::ASSIGNMENT_OPERATOR))
        {
            usage_str_ += '=';
        }

        usage_str_ += val_name;

        if (value_arg_type::get_min_values() <= cnt)
        {
            usage_str_ += ']';
        }

        ++cnt;
    }

private:
    /** The values names. */
    vector_type<string_type> vals_names_;

    /** The usage string. */
    string_type usage_str_;

    /** Indicates whether the usage string has to be rebuilt. */
    bool usage_str_needs_refresh_;
};

}

#endif

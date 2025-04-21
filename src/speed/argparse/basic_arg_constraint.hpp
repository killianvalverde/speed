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
 *  @file       basic_arg_constraint.hpp
 *  @brief      basic_arg_constraint class header.
 *  @author     Killian Valverde
 *  @date       2017/10/21
 */

#ifndef SPEED_ARGPARSE_BASIC_ARG_CONSTRAINT_HPP
#define SPEED_ARGPARSE_BASIC_ARG_CONSTRAINT_HPP

#include <memory>
#include <string>
#include <unordered_set>

#include "../containers/containers.hpp"
#include "forward_declarations.hpp"
#include "arg_constraint_flags.hpp"
#include "basic_arg_parser.hpp"
#include "basic_base_arg.hpp"
#include "basic_help_arg.hpp"
#include "basic_key_arg.hpp"
#include "basic_key_value_arg.hpp"
#include "basic_keyless_arg.hpp"
#include "basic_value_arg.hpp"
#include "basic_version_arg.hpp"
#include "exception.hpp"

namespace speed::argparse {

/**
 * @brief       Class used to represent the base of an argument constraint.
 */
template<typename TpAllocator>
class basic_arg_constraint
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

    /** Class that represents a bit field */
    template<typename T>
    using flags_type = speed::containers::flags<T>;

    /** Type that represents an argument key. */
    using arg_key_type = basic_arg_key<TpAllocator>;

    /** Type that represents an argument value. */
    using arg_value_type = basic_arg_value<TpAllocator>;
    
    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;
    
    /** Type that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<TpAllocator>;
    
    /** Type that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpAllocator>;
    
    /** Type that represents the option to get the version information. */
    using version_arg_type = basic_version_arg<TpAllocator>;

    /** Type that represents arguments without keys. */
    using keyless_arg_type = basic_keyless_arg<TpAllocator>;
    
    /** Type that represents arguments that have keys and values. */
    using key_value_arg_type = basic_key_value_arg<TpAllocator>;

    /** Type that represents the option to get the help information. */
    using help_arg_type = basic_help_arg<TpAllocator>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;

    /**
     * @brief       Constructor with parameters.
     * @param       arg_parsr : Argument parser that holds this constraint.
     * @param       kys : The arguments keys in which apply the constraint.
     */
    template<typename... Ts_>
    basic_arg_constraint(arg_parser_type* arg_parsr, const Ts_&... kys)
            : arg_parsr_(arg_parsr)
    {
        base_arg_type* bse_arg;

        int foreach[sizeof...(Ts_) + 1] = { (
                (bse_arg = arg_parsr_->get_base_arg(kys)) == nullptr ?
                        throw key_not_found_exception() :
                        bse_args_.emplace_back(bse_arg),
                0)... };
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_arg_constraint(const basic_arg_constraint& rhs) = delete;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_arg_constraint(basic_arg_constraint&& rhs) noexcept = delete;
    
    /**
     * @brief       Destructor.
     */
    ~basic_arg_constraint() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_arg_constraint& operator =(const basic_arg_constraint& rhs) = delete;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_arg_constraint& operator =(basic_arg_constraint&& rhs) noexcept = delete;
    
    /**
     * @brief       Allows knowing whether the constraint is violed.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_violed() const
    {
        if (flgs_.is_empty())
        {
            throw wrong_constraint_exception();
        }
        
        if ((flgs_.is_set(arg_constraint_flags::ONE_OR_MORE_REQUIRED) &&
                is_one_or_more_required_violed()) ||
            (flgs_.is_set(arg_constraint_flags::MUTUALLY_EXCLUSIVE) &&
             is_mutually_exclusive_violed()))
        {
            return true;
        }
        
        return false;
    }
    
    /**
     * @brief       Enforces that the set of arguments must be mutually exclusive, meaning only one
     *              of the arguments in the group can be used at a time.
     */
    void set_mutually_exclusive(bool enabl)
    {
        if (!enabl)
        {
            flgs_.unset(arg_constraint_flags::MUTUALLY_EXCLUSIVE);
            return;
        }
        
        if (bse_args_.size() <= 1)
        {
            throw wrong_constraint_exception();
        }
        
        for (auto& bse_arg : bse_args_)
        {
            if (bse_arg->is_flag_set(arg_flags::MANDATORY))
            {
                throw wrong_constraint_exception();
            }
        }
        
        flgs_.set(arg_constraint_flags::MUTUALLY_EXCLUSIVE);
    }
    
    /**
     * @brief       Enforces that one or more arguments in the group must be provided.
     */
    void set_one_or_more_required(bool enabl)
    {
        if (!enabl)
        {
            flgs_.unset(arg_constraint_flags::ONE_OR_MORE_REQUIRED);
            return;
        }
        
        if (bse_args_.size() < 2)
        {
            throw wrong_constraint_exception();
        }

        for (auto& bse_arg : bse_args_)
        {
            if (bse_arg->is_flag_set(arg_flags::MANDATORY))
            {
                throw wrong_constraint_exception();
            }
        }
        
        flgs_.set(arg_constraint_flags::ONE_OR_MORE_REQUIRED);
    }

    /**
     * @brief       Print the constraint.
     */
    void print_help(
            std::size_t args_indent,
            std::size_t max_line_len,
            std::size_t new_line_indent,
            std::size_t short_kys_len,
            std::size_t long_kys_len
    ) const
    {
        string_type desc;
        std::size_t kys_len = 0;
        std::size_t total_id_len = speed::safety::addm(short_kys_len, long_kys_len);
        bool arg_printd = false;
        bool is_mutually_exclusiv = flgs_.is_set(arg_constraint_flags::MUTUALLY_EXCLUSIVE);
        std::size_t i;
        
        if (is_mutually_exclusiv)
        {
            desc += "Arguments are mutually exclusive";
        }
        if (flgs_.is_set(arg_constraint_flags::ONE_OR_MORE_REQUIRED))
        {
            if (is_mutually_exclusiv)
            {
                desc += " and at least one is required";
            }
            else
            {
                desc += "At least one argument is required";
            }
        }
        
        desc += '.';
    
        for (i = args_indent; i > 0; --i)
        {
            std::cout << ' ';
        }
        
        for (auto& bse_arg : bse_args_)
        {
            if (arg_printd)
            {
                std::cout << ", ";
                bse_arg->print_name();
                speed::safety::try_addml(&kys_len, bse_arg->get_name_length(), 2);
            }
            else
            {
                bse_arg->print_name();
                speed::safety::try_addm(&kys_len, bse_arg->get_name_length());
                arg_printd = true;
            }
        }
        
        std::cout << "  ";
        speed::safety::try_addm(&kys_len, 2);
        
        if (kys_len < total_id_len)
        {
            for (i = total_id_len - kys_len; i > 0; i--)
            {
                std::cout << ' ';
            }
            
            kys_len = total_id_len;
        }
        
        speed::safety::try_addm(&kys_len, args_indent);
        speed::safety::try_addml(&new_line_indent, args_indent, total_id_len);
        
        speed::iostream::print_wrapped(std::cout, desc, max_line_len, new_line_indent, kys_len);
        std::cout << '\n';
    }
    
    /**
     * @brief       Print the constraint errors.
     */
    virtual void print_errors()
    {
        if (flgs_.is_set(arg_constraint_flags::ONE_OR_MORE_REQUIRED))
        {
            print_one_or_more_required_errors();
        }
        if (flgs_.is_set(arg_constraint_flags::MUTUALLY_EXCLUSIVE))
        {
            print_mutually_exclusive_errors();
        }
    }

private:
    /**
     * @brief       Allows knowing whether a relational constraint is is_violed.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool is_mutually_exclusive_violed() const noexcept
    {
        bool fnd = false;

        for (auto& bse_arg : bse_args_)
        {
            if (bse_arg->was_found())
            {
                if (fnd)
                {
                    return true;
                }

                fnd = true;
            }
        }

        return false;
    }
    
    /**
     * @brief       Allows knowing whether the 'one or more' constraint is is_violed.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_one_or_more_required_violed() const noexcept
    {
        for (auto& bse_arg : bse_args_)
        {
            if (bse_arg->was_found())
            {
                return false;
            }
        }

        return true;
    }
    
    /**
     * @brief       Print the constraints errors.
     */
    void print_mutually_exclusive_errors() const
    {
        if (!is_mutually_exclusive_violed())
        {
            return;
        }

        print_arguments_during_error();
        
        std::cout << "The arguments are mutually exclusive.\n";
    }
    
    /**
     * @brief       Print the 'one or more' constraint errors.
     */
    void print_one_or_more_required_errors() const
    {
        if (!is_one_or_more_required_violed())
        {
            return;
        }

        print_arguments_during_error();
        
        std::cout << "At least one of the arguments has to be found.\n";
    }
    
    /**
     * @brief       Prints the arguments during an error with optional color formatting.
     */
    void print_arguments_during_error() const
    {
        const string_type* err_name;
        bool colors_enabld = arg_parsr_->is_flag_set(arg_parser_flags::USE_COLORS);
        
        std::cout << arg_parsr_->get_program_name() << ": ";
        
        for (auto it = bse_args_.cbegin(); it != bse_args_.cend(); )
        {
            err_name = &(*it)->get_error_name();
            
            if (!err_name->empty())
            {
                if (colors_enabld)
                {
                    std::cout << speed::iostream::set_light_red_text;
                }
                std::cout << *err_name;

                if (++it != bse_args_.cend())
                {
                    std::cout << ", ";
                }
                else
                {
                    std::cout << ": ";
                }
                
                if (colors_enabld)
                {
                    std::cout << speed::iostream::set_default_text;
                }
            }
        }
    }

private:
    /** The arguments in which apply the dependencies. */
    vector_type<base_arg_type*> bse_args_;

    /** Holds a reference to the composite object. */
    arg_parser_type* arg_parsr_;
    
    /** Flags that control the behavior and settings of the argument constraint. */
    flags_type<arg_constraint_flags> flgs_;
};

}

#endif

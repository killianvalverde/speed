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
 *  @file       speed/argparse/basic_arg_constraint.hpp
 *  @brief      basic_arg_constraint class header.
 *  @author     Killian Valverde
 *  @date       2017/10/21
 */

#ifndef SPEED_ARGPARSE_BASIC_ARG_CONSTRAINT_HPP
#define SPEED_ARGPARSE_BASIC_ARG_CONSTRAINT_HPP

#include <memory>
#include <string>
#include <unordered_set>

#include "basic_arg_parser.hpp"
#include "basic_base_arg.hpp"
#include "basic_help_arg.hpp"
#include "basic_key_arg.hpp"
#include "basic_key_value_arg.hpp"
#include "basic_keyless_arg.hpp"
#include "basic_value_arg.hpp"
#include "basic_version_arg.hpp"
#include "exception.hpp"
#include "forward_declarations.hpp"


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

    /** Unordered set type used in the class. */
    template<typename TpKey_>
    using unordered_set_type = std::unordered_set<
            TpKey_, std::hash<TpKey_>,
            std::equal_to<TpKey_>,
            allocator_type<TpKey_>>;

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
    explicit basic_arg_constraint(arg_parser_type* arg_parsr, const Ts_&... kys)
            : bse_args_()
            , arg_parsr_(arg_parsr)
    {
        base_arg_type* bse_arg;

        int foreach[sizeof...(Ts_) + 1] = { (
                (bse_arg = arg_parsr_->get_base_arg(kys)) == nullptr ?
                        throw key_not_found_exception() :
                        bse_args_.emplace(bse_arg),
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
    virtual ~basic_arg_constraint() = default;
    
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
     * @brief       Allows knowing whether a relational constraint is violed.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual bool violed()
    {
        return false;
    }

    /**
     * @brief       Print the constraint usage.
     */
    virtual void print_usage()
    {
    }
    
    /**
     * @brief       Print the constraints errors.
     */
    virtual void print_errors()
    {
    }

protected:
    /**
     * @brief       Allows knowing whether or not the colors are enabled.
     * @return      If fucntion is successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] virtual inline bool colors_enabled() const noexcept
    {
        return arg_parsr_->is_flag_set(arg_parser_flags::USE_COLORS);
    }

    /**
     * @brief       Get the base args in which the constraint is applied.
     * @return      The base args in which the constraint is applied.
     */
    [[nodiscard]] virtual inline unordered_set_type<base_arg_type*>& get_base_args() noexcept
    {
        return bse_args_;
    }

    /**
     * @brief       Get the number of arguments concerned by the constraint.
     * @return      The number of arguments concerned by the constraint.
     */
    [[nodiscard]] virtual inline std::size_t get_base_args_size() const noexcept
    {
        return bse_args_.size();
    }

    /**
     * @brief       Get the program name.
     * @return      The program name.
     */
    [[nodiscard]] virtual inline const string_type& get_program_name() const noexcept
    {
        return arg_parsr_->get_program_name();
    }

private:
    /** The arguments in which apply the dependencies. */
    unordered_set_type<base_arg_type*> bse_args_;

    /** Holds a reference to the composite object. */
    arg_parser_type* arg_parsr_;
};


}


#endif

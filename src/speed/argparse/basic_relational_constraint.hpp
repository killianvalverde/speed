/* speed - Generic C++ library.
 * Copyright (C) 2015-2023 Killian Valverde.
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
 *  @file       speed/argparse/basic_relational_constraint.hpp
 *  @brief      basic_relational_constraint class header.
 *  @author     Killian
 *  @date       2017/10/21 - 14:39
 */

#ifndef SPEED_ARGPARSE_BASIC_RELATIONAL_CONSTRAINT_HPP
#define SPEED_ARGPARSE_BASIC_RELATIONAL_CONSTRAINT_HPP

#include <memory>

#include "basic_keyless_arg.hpp"
#include "basic_help_arg.hpp"
#include "basic_key_value_arg.hpp"
#include "basic_version_arg.hpp"
#include "relational_constraint_types.hpp"


namespace speed::argparse {


/**
 * @brief       Class used to parse arguments.
 */
template<typename TpAllocator>
class basic_relational_constraint
{
public:
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;
    
    /** Class that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;
    
    /** Class that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<TpAllocator>;
    
    /** Class that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpAllocator>;
    
    /** Class that represents the option to get the help information. */
    using help_arg_type = basic_help_arg<TpAllocator>;
    
    /** Class that represents the option to get the version information. */
    using version_arg_type = basic_version_arg<TpAllocator>;
    
    /** Class that represents arguments that have keys and values. */
    using key_value_arg_type = basic_key_value_arg<TpAllocator>;
    
    /** Class that represents arguments without keys. */
    using keyless_arg_type = basic_keyless_arg<TpAllocator>;
    
    /** Class that represents a key for an argument. */
    using arg_key_type = basic_arg_key<TpAllocator>;
    
    /** Class that represents a value for an argument. */
    using arg_value_type = basic_arg_value<TpAllocator>;
    
    /** Class that represents a bit field */
    template<typename T>
    using flags_type = speed::containers::flags<T>;
    
    /** Class that represent sequences of characters. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;
    
    /** Class that represents a sequence container. */
    template<typename T>
    using vector_type = std::vector<T, allocator_type<T>>;
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       bse_args : The arguments in which appy the constraint.
     * @param       contrs : Type of constraint to apply.
     */
    template<typename TpBaseArgPointerVector_ = vector_type<base_arg_type*>>
    basic_relational_constraint(
            TpBaseArgPointerVector_&& bse_args,
            relational_constraint_types contrs
    )
            : bse_args_(std::forward<TpBaseArgPointerVector_>(bse_args))
            , contrs_(contrs)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_relational_constraint(const basic_relational_constraint& rhs) = delete;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_relational_constraint(basic_relational_constraint&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_relational_constraint() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_relational_constraint& operator =(const basic_relational_constraint& rhs) = delete;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_relational_constraint& operator =(basic_relational_constraint&& rhs) noexcept = default;
    
    // TODO(kKillianValverde@gmail.com): Delete this method?
    /**
     * @brief       Return the base arguments container in which apply the constraints.
     * @return      The base arguments container in which apply the constraints.
     */
    const vector_type<base_arg_type*>& get_base_args() const noexcept
    {
        return bse_args_;
    }
    
    // TODO(KillianValverde@gmail.com): Delete this method?
    /**
     * @brief       Get the relational constraints.
     * @return      The relational constraints.
     */
    relational_constraint_types get_contraints() const noexcept
    {
        return contrs_.get_value();
    }
    
    /**
     * @brief       Allows knowing whether a relational constraint is violed.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool is_relational_constraint_violed() const noexcept
    {
        bool fnd = false;
        
        if (contrs_.is_set(relational_constraint_types::AT_LEAST_ONE_FOUND))
        {
            for (auto& x : bse_args_)
            {
                if (x->was_found())
                {
                    fnd = true;
                    break;
                }
            }
            
            if (!fnd)
            {
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * @brief       Allows knowing whether a relational constraint is violed.
     * @param       constr : Constraint to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool is_relational_constraint_violed(relational_constraint_types constr) const noexcept
    {
        bool fnd = false;
        
        if ((constr & relational_constraint_types::AT_LEAST_ONE_FOUND) !=
                    relational_constraint_types::NIL &&
            contrs_.is_set(relational_constraint_types::AT_LEAST_ONE_FOUND))
        {
            for (auto& x : bse_args_)
            {
                if (x->was_found())
                {
                    fnd = true;
                    break;
                }
            }
    
            if (!fnd)
            {
                return true;
            }
        }
        
        return false;
    }
    
    // TODO(KillianValverde@gmail.com): This method is weird... check it !
    /**
     * @brief       Allows knowing whether at least one argument key is always required due to the
     *              constraints.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool key_arg_always_required() const noexcept
    {
        if (!contrs_.is_set(relational_constraint_types::AT_LEAST_ONE_FOUND))
        {
            return false;
        }
        
        for (auto& x : bse_args_)
        {
            if (dynamic_cast<keyless_arg_type*>(x) != nullptr ||
                dynamic_cast<key_arg_type*>(x) == nullptr)
            {
                return false;
            }
        }
        
        return true;
    }
    
    /**
     * @brief       Print the constraints errors.
     * @param       prog_name : The program name.
     * @param       colrs_enable : Allows knowing whether the colors have to be used.
     */
    void print_errors(const string_type& prog_name, bool colrs_enable) const
    {
        if (is_relational_constraint_violed(relational_constraint_types::AT_LEAST_ONE_FOUND))
        {
            std::cout << prog_name << ": ";
            
            for (auto it = bse_args_.cbegin(); it != bse_args_.cend(); ++it)
            {
                auto& error_id = (*it)->get_error_id();
                
                if (!error_id.empty())
                {
                    if (colrs_enable)
                    {
                        std::cout << speed::iostream::set_light_red_text;
                    }
    
                    std::cout << error_id;
    
                    if (it + 1 != bse_args_.cend())
                    {
                        std::cout << ", ";
                    }
                    else
                    {
                        std::cout << ": ";
                    }
                    
                    if (colrs_enable)
                    {
                        std::cout << speed::iostream::set_default_text;
                    }
                }
            }
            
            std::cout << "At least one of the arguments has to be found\n";
        }
    }

private:
    /** The arguments in which apply the constraint. */
    vector_type<base_arg_type*> bse_args_;
    
    /** Type of constraint to apply. */
    flags_type<relational_constraint_types> contrs_;
};


/** Class used to parse arguments. */
using relational_constraint = basic_relational_constraint<std::allocator<int>>;


}


#endif

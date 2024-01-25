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
 * @file        speed/argparse/basic_key_value_arg.hpp
 * @brief       basic_key_value_arg class header.
 * @author      Killian Valverde
 * @date        2016/03/11 - 08:12
 */

#ifndef SPEED_ARGPARSE_BASIC_KEY_VALUE_ARG_HPP
#define SPEED_ARGPARSE_BASIC_KEY_VALUE_ARG_HPP

#include "basic_key_arg.hpp"
#include "basic_value_arg.hpp"


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
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;
    
    /** Class that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;
    
    /** Class that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<TpAllocator>;
    
    /** Class that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpAllocator>;
    
    /** Class that represents a value for an argument. */
    using arg_value_type = basic_arg_value<TpAllocator>;
    
    /** Class that represents a key for an argument. */
    using arg_key_type = basic_arg_key<TpAllocator>;
    
    /** Class that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;
    
    /** Class that represents flags container */
    template<typename T>
    using flags_type = speed::containers::flags<T>;
    
    /** Class that represents sequences of characters. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;
    
    /** Class that represents a sequence container. */
    template<typename T>
    using vector_type = std::vector<T, allocator_type<T>>;
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       desc : Argument description content.
     * @param       err_id : The id used to reference an arguments during the error display.
     * @param       flgs : Flags that dictates the argument behavior.
     * @param       kys : Argument keys collection.
     * @param       min_vals : Minimum number of values for an option.
     * @param       max_vals : Maximum number of values for an option.
     * @param       vals_types : Collection that has the values types that must have the values.
     * @param       regx_vector : Regular expressions that the values have to match. If the vector
     *              is empty all values will match.
     * @param       compo : The composite object of this class.
     */
    template<
            typename TpString1_,
            typename TpString2_,
            typename TpArgKeyVector_ = vector_type<string_type>,
            typename TpStringVector_ = vector_type<string_type>,
            typename TpArgValueTypesVector_ = vector_type<arg_value_types>
    >
    basic_key_value_arg(
            TpString1_&& desc,
            TpString2_&& err_id,
            arg_flags flgs,
            TpArgKeyVector_&& kys,
            std::size_t min_vals,
            std::size_t max_vals,
            TpArgValueTypesVector_&& vals_types,
            TpStringVector_&& regx_vector,
            arg_parser_type* compo
    )
            : base_arg_type(std::forward<TpString1_>(desc),
                            std::forward<TpString2_>(err_id),
                            flgs,
                            compo)
            , key_arg_type(desc,
                           err_id,
                           flgs,
                           std::forward<TpArgKeyVector_>(kys),
                           compo)
            , value_arg_type(desc,
                             err_id,
                             flgs,
                             min_vals,
                             max_vals,
                             std::forward<TpArgValueTypesVector_>(vals_types),
                             std::forward<TpStringVector_>(regx_vector),
                             compo)
    {
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
    basic_key_value_arg& operator =(const basic_key_value_arg& rhs)
    {
        if (this != &rhs)
        {
            base_arg_type::operator =(rhs);
            key_arg_type::local_assignment(rhs);
            value_arg_type::local_assignment(rhs);
        }
        
        return *this;
    }
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_key_value_arg& operator =(basic_key_value_arg&& rhs) noexcept
    {
        if (this != &rhs)
        {
            base_arg_type::operator =(std::move(rhs));
            key_arg_type::local_assignment(std::move(rhs));
            value_arg_type::local_assignment(std::move(rhs));
        }
        
        return *this;
    }
    
    /**
     * @brief       Print argument errors in standard output.
     * @param       prog_name : The name of the program.
     * @param       colrs_enable : If it is true, colors will be used during the print.
     */
    void print_errors(const string_type& prog_name, bool colrs_enable) const override
    {
        value_arg_type::print_errors(prog_name, colrs_enable);
    }
    
    /**
     * @brief       Parse the argument.
     * @param       argc : Contains the number of arguments gotten through the program call.
     * @param       argv : Contains the arguments gotten through the program call.
     * @param       i : The index in argv of the argument currently being parsed.
     * @param       pos_increment : The position incrementation of i due to the parse of the next
     *              values in argv.
     */
    template<typename TpStringArray_>
    void parse(
            std::size_t argc,
            const TpStringArray_& argv,
            std::size_t i,
            std::size_t* pos_increment
    ) noexcept
    {
        arg_parser_type* ap = base_arg_type::get_composite();
        
        key_arg_type::parse();
        
        for (*pos_increment = 1;
             !value_arg_type::max_values_number_reached() &&
                    i + *pos_increment < argc &&
                    ap->string_can_be_value(argv[i + *pos_increment]);
             ++*pos_increment)
        {
            value_arg_type::add_value(argv[i + *pos_increment]);
        }
        
        --*pos_increment;
    }
    
    /**
     * @brief       Parse the argument contained in a eq operator expression.
     * @param       eq_val : The value associated with the key in expression.
     * @param       argc : Contains the number of arguments gotten through the program call.
     * @param       argv : Contains the arguments gotten through the program call.
     * @param       i : The index in argv of the argument currently being parsed.
     * @param       pos_increment : The position incrementation of i due to the parse of the next
     *              values in argv.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TpString_, typename TpStringArray_>
    bool parse_eq_operator(
            TpString_&& eq_val,
            std::size_t argc,
            const TpStringArray_& argv,
            std::size_t i,
            std::size_t* pos_increment
    ) noexcept
    {
        if (!base_arg_type::flag_is_set(arg_flags::ALLOW_EQ_OPERATOR))
        {
            return false;
        }
    
        value_arg_type::add_value(std::forward<TpString_>(eq_val));
        
        parse(argc, argv, i, pos_increment);
        
        return true;
    }
    
    friend class basic_arg_parser<TpAllocator>;
};


/** Class that represents arguments that have keys and values. */
using key_value_arg = basic_key_value_arg<std::allocator<int>>;


}


#endif

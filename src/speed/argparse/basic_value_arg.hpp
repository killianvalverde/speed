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
 * @file        speed/argparse/basic_value_arg.hpp
 * @brief       basic_value_arg class header.
 * @author      Killian Green
 * @date        2016/03/10 - 16:26
 */

#ifndef SPEED_ARGPARSE_BASIC_VALUE_ARG_HPP
#define SPEED_ARGPARSE_BASIC_VALUE_ARG_HPP

#include <vector>

#include "argparse_exception.hpp"
#include "basic_arg_value.hpp"
#include "basic_base_arg.hpp"


namespace speed::argparse {


/**
 * @brief       Class that represents arguments that have values.
 */
template<typename TpAllocator>
class basic_value_arg : public virtual basic_base_arg<TpAllocator>
{
public:
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;
    
    /** Class that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;
    
    /** Class that represents a value for an argument. */
    using arg_value_type = basic_arg_value<TpAllocator>;
    
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
     * @param       min_vals : Minimum number of values for an option.
     * @param       max_vals : Maximum number of values for an option.
     * @param       vals_types : Collection that has the values types that must have the argument
     *              values.
     * @param       regx_vector : Regular expressions that the values have to match. If the vector
     *              is empty all values will match.
     * @param       compo : The composite object of this class.
     */
    template<
            typename TpString1_,
            typename TpString2_,
            typename TpArgValueTypesVector_ = vector_type<arg_value_types>,
            typename TpStringVector_ = vector_type<string_type>
    >
    basic_value_arg(
            TpString1_&& desc,
            TpString2_&& err_id,
            arg_flags flgs,
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
            , vals_()
            , min_vals_(min_vals)
            , max_vals_(max_vals)
            , vals_types_(std::forward<TpArgValueTypesVector_>(vals_types))
            , regx_vector_(std::forward<TpStringVector_>(regx_vector))
    {
        if (min_vals_ > max_vals_)
        {
            max_vals_ = min_vals_;
        }
        if (base_arg_type::flag_is_set(arg_flags::ALLOW_CHAIN) && (min_vals_ != max_vals_))
        {
            base_arg_type::erase_flag(arg_flags::ALLOW_CHAIN);
        }
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_value_arg(const basic_value_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_value_arg(basic_value_arg&& rhs) noexcept
            : base_arg_type(std::move(rhs))
            , vals_(std::move(rhs.vals_))
            , min_vals_(rhs.min_vals_)
            , max_vals_(rhs.max_vals_)
            , vals_types_(std::move(rhs.vals_types_))
            , regx_vector_(std::move(rhs.regx_vector_))
    {
        rhs.min_vals_ = 0;
        rhs.max_vals_ = 0;
    }
    
    /**
     * @brief       Destructor.
     */
    ~basic_value_arg() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_value_arg& operator =(const basic_value_arg& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_value_arg& operator =(basic_value_arg&& rhs) noexcept
    {
        if (this != &rhs)
        {
            base_arg_type::operator =(std::move(rhs));
            vals_ = std::move(rhs.vals_);
            std::swap(min_vals_, rhs.min_vals_);
            std::swap(max_vals_, rhs.max_vals_);
            vals_types_ = std::move(rhs.vals_types_);
            regx_vector_ = std::move(rhs.regx_vector_);
        }
        
        return *this;
    }
    
    /**
     * @brief       Get values collection.
     * @return      The values argument collection.
     */
    inline const vector_type<arg_value_type>& get_values() const noexcept
    {
        return vals_;
    }
    
    /**
     * @brief       Get minimum number of values for an option.
     * @return      The minimum number of values for an option.
     */
    inline std::size_t get_min_values() const noexcept
    {
        return min_vals_;
    }
    
    /**
     * @brief       Get maximum number of values for an option.
     * @return      The maximum number of values for an option.
     */
    inline std::size_t get_max_values() const noexcept
    {
        return max_vals_;
    }
    
    /**
     * @brief       Add a value to the argument.
     * @param       val : Value to add to the arguement.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    template<typename TpString_>
    bool add_value(TpString_&& val)
    {
        bool scs = true;
        arg_value_types vals_types;
        string_type regx_to_match;
        
        if (!max_values_number_reached())
        {
            if (vals_.size() + 1 <= vals_types_.size())
            {
                vals_types = vals_types_[vals_.size()];
            }
            else if (!vals_types_.empty())
            {
                vals_types = vals_types_.back();
            }
            else
            {
                vals_types = arg_value_types::STRING;
            }
            
            if (vals_.size() + 1 <= regx_vector_.size())
            {
                regx_to_match = regx_vector_[vals_.size()];
            }
            else if (!regx_vector_.empty())
            {
                regx_to_match = regx_vector_.back();
            }
            
            vals_.push_back(arg_value_type(std::forward<TpString_>(val),
                                           vals_types,
                                           regx_to_match,
                                           this));
        }
        else
        {
            base_arg_type::set_error_flag(arg_error_flags::MAX_VALUES_ERROR);
            scs = false;
        }
        
        return scs;
    }
    
    /**
     * @brief       Try to add a value to the argument. No error flags will be raised.
     * @param       val : Value to add to the arguement.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    template<typename TpString_>
    bool try_add_value(TpString_&& val)
    {
        bool scs = false;
        arg_value_types vals_types;
        string_type regx_to_match;
        
        if (!max_values_number_reached())
        {
            if (vals_.size() + 1 <= vals_types_.size())
            {
                vals_types = vals_types_[vals_.size()];
            }
            else if (!vals_types_.empty())
            {
                vals_types = vals_types_.back();
            }
            else
            {
                vals_types = arg_value_types::STRING;
            }
            
            if (vals_.size() + 1 <= regx_vector_.size())
            {
                regx_to_match = regx_vector_[vals_.size()];
            }
            else if (!regx_vector_.empty())
            {
                regx_to_match = regx_vector_.back();
            }
            
            arg_value_type av(std::forward<TpString_>(val), vals_types, regx_to_match, this);
            
            if (!av.there_are_errors())
            {
                vals_.push_back(std::move(av));
                scs = true;
            }
        }
        
        return scs;
    }
    
    /**
     * @brief       Get first argument value.
     * @return      The first argument value.
     * @throw       speed::argparse::value_not_found_exception : If argument hasn't any value an
     *              exception is thrown.
     */
    const arg_value_type& get_front_value() const
    {
        if (vals_.empty())
        {
            throw value_not_found_exception();
        }
        
        return vals_.front();
    }
    
    /**
     * @brief       Get first argument value.
     * @param       default_val : The value used to construct an argument value if there isn't any
     *              value.
     * @return      If function was successful the first argument value is returned, otherwise an
     *              argument value constructed with the default value is returned.
     */
    template<typename TpString_>
    const arg_value_type& get_front_value(TpString_&& default_val) const
    {
        if (vals_.empty())
        {
            static arg_value_type default_arg_value;
            default_arg_value = arg_value_type(std::forward<TpString_>(default_val),
                                               arg_value_types::STRING,
                                               string_type(),
                                               this);
            return default_arg_value;
        }
        
        return vals_.front();
    }
    
    /**
     * @brief       Get the value in the specified position.
     * @param       indx : Position of the element.
     * @return      The element specified.
     * @throw       speed::argparse::value_not_found_exception : If there isn't any element in the
     *              specified position an exception is thrown.
     */
    const arg_value_type& get_value_at(std::size_t indx) const
    {
        if (vals_.size() <= indx)
        {
            throw value_not_found_exception();
        }
        
        return vals_.at(indx);
    }
    
    /**
     * @brief       Get the value in the specified position.
     * @param       indx : Position of the element.
     * @param       default_val : The value used to construct an argument value if there isn't any
     *              value.
     * @return      If function was successful the argument value in the specified position is
     *              returned, otherwise an argument value constructed with the default value is
     *              returned.
     */
    template<typename TpString_>
    const arg_value_type& get_value_at(
            std::size_t indx,
            TpString_&& default_val
    ) const
    {
        if (vals_.size() <= indx)
        {
            static arg_value_type default_arg_value;
            default_arg_value = arg_value_type(std::forward<TpString_>(default_val),
                                               arg_value_types::STRING,
                                               string_type(),
                                               this);
            return default_arg_value;
        }
        
        return vals_.at(indx);
    }
    
    /**
     * @brief       Allows knowing whether there are values in the argument.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool there_are_values() const noexcept
    {
        return !vals_.empty();
    }
    
    /**
     * @brief       Allows knowing whether there are less or equal n values in the argument.
     * @param       cnt : The number of values to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool there_are_n_values(std::size_t cnt) const noexcept
    {
        return (cnt <= vals_.size());
    }
    
    /**
     * @brief       Clear values collection.
     */
    void clear_values() noexcept
    {
        vals_.clear();
        base_arg_type::clear_error_flags();
    }
    
    /**
     * @brief       Allows knowing whether the argument has rached the minimal number of values.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    inline bool min_values_number_reached() const noexcept
    {
        return vals_.size() >= min_vals_;
    }
    
    /**
     * @brief       Allows knowing whether the argument can't get more values.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    inline bool max_values_number_reached() const noexcept
    {
        return vals_.size() >= max_vals_;
    }
    
    /**
     * @brief       Print argument errors in standard output.
     * @param       prog_name : The name of the program.
     * @param       colrs_enable : If it is true, colors will be used during the print.
     */
    void print_errors(const string_type& prog_name, bool colrs_enable) const override
    {
        base_arg_type::print_errors(prog_name, colrs_enable);
        
        if (base_arg_type::error_flag_is_set(arg_error_flags::MIN_VALUES_ERROR))
        {
            base_arg_type::print_error_message(string_type(), prog_name, colrs_enable, false);
            std::cout << "Option requires at least " << min_vals_
                      << (min_vals_ > 1 ? " arguments" : " argument") << '\n';
        }
        
        if (base_arg_type::error_flag_is_set(arg_error_flags::MAX_VALUES_ERROR))
        {
            base_arg_type::print_error_message(string_type(), prog_name, colrs_enable, false);
            std::cout << "Option must have a maximum of " << max_vals_
                      << (max_vals_ > 1 ? " arguments" : " argument") << '\n';
        }
        
        if (base_arg_type::error_flag_is_set(arg_error_flags::VALUES_ERROR) &&
            there_are_values())
        {
            for (auto& x : vals_)
            {
                if (x.there_are_errors())
                {
                    x.print_errors(prog_name, base_arg_type::get_error_id(), colrs_enable);
                }
            }
        }
    }

protected:
    /**
     * @brief       Local copy assignment method.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_value_arg& local_assignment(const basic_value_arg& rhs)
    {
        if (this != &rhs)
        {
            vals_ = rhs.vals_;
            min_vals_ = rhs.min_vals_;
            max_vals_ = rhs.max_vals_;
            vals_types_ = rhs.vals_types_;
            regx_vector_ = rhs.regx_vector_;
        }
        
        return *this;
    }
    
    /**
     * @brief       Local move assignment method.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_value_arg& local_assignment(basic_value_arg&& rhs) noexcept
    {
        if (this != &rhs)
        {
            vals_ = std::move(rhs.vals_);
            std::swap(min_vals_, rhs.min_vals_);
            std::swap(max_vals_, rhs.max_vals_);
            vals_types_ = std::move(rhs.vals_types_);
            regx_vector_ = std::move(rhs.regx_vector_);
        }
        
        return *this;
    }
    
    /**
     * @brief       Set error flags if it's required.
     */
    void update_error_flags() noexcept override
    {
        bool in = false;
        
        base_arg_type::update_error_flags();
        
        if (vals_.size() < min_vals_ && base_arg_type::was_found())
        {
            base_arg_type::set_error_flag(arg_error_flags::MIN_VALUES_ERROR);
        }
        else
        {
            base_arg_type::erase_error_flag(arg_error_flags::MIN_VALUES_ERROR);
        }
        
        if (vals_.size() > max_vals_)
        {
            base_arg_type::set_error_flag(arg_error_flags::MAX_VALUES_ERROR);
            while (vals_.size() > max_vals_)
            {
                vals_.pop_back();
            }
        }
        
        for (auto& x : vals_)
        {
            if (x.there_are_errors())
            {
                in = true;
                base_arg_type::set_error_flag(arg_error_flags::VALUES_ERROR);
                break;
            }
        }
        if (!in)
        {
            base_arg_type::erase_error_flag(arg_error_flags::VALUES_ERROR);
        }
    }

private:
    /** Collection that has the values gotten through the program call for an option. */
    vector_type<arg_value_type> vals_;
    
    /** Minimum number of values for an option. */
    std::size_t min_vals_;
    
    /** Maximum number of values for an option. */
    std::size_t max_vals_;
    
    /** Collection that has the values types that must have the values. */
    vector_type<arg_value_types> vals_types_;
    
    /** Regular expressions that the values has to match. If the collection is empty all values will
     *  match. */
    vector_type<string_type> regx_vector_;
    
    friend class basic_arg_value<TpAllocator>;
    friend class basic_arg_parser<TpAllocator>;
};


/** Class that represents arguments that have values. */
using value_arg = basic_value_arg<std::allocator<int>>;
    
    
}


#endif

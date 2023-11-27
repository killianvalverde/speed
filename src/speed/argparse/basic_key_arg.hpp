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
 * @file        speed/argparse/basic_key_arg.hpp
 * @brief       basic_key_arg class header.
 * @author      Killian Green
 * @date        2016/03/10 - 16:26
 */

#ifndef SPEED_ARGPARSE_BASIC_KEY_ARG_HPP
#define SPEED_ARGPARSE_BASIC_KEY_ARG_HPP

#include <vector>

#include "argparse_exception.hpp"
#include "basic_arg_key.hpp"
#include "basic_base_arg.hpp"


namespace speed::argparse {


/**
 * @brief       Class that represents arguments that have keys.
 */
template<typename TpAllocator>
class basic_key_arg : public virtual basic_base_arg<TpAllocator>
{
public:
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;
    
    /** Class that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpAllocator>;
    
    /** Class that represents a key for an argument. */
    using arg_key_type = basic_arg_key<TpAllocator>;
    
    /** Class that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;
    
    /** Class that represents flags container. */
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
     * @param       compo : The composite object of this class.
     */
    template<
            typename TpString1_,
            typename TpString2_,
            typename TpArgKeyVector_ = vector_type<string_type>
    >
    basic_key_arg(
            TpString1_&& desc,
            TpString2_&& err_id,
            arg_flags flgs,
            TpArgKeyVector_&& kys,
            arg_parser_type* compo
    )
            : base_arg_type(std::forward<TpString1_>(desc),
                            std::forward<TpString2_>(err_id),
                            flgs,
                            compo)
            , kys_(std::forward<TpArgKeyVector_>(kys))
    {
        if (base_arg_type::error_id_is_empty() &&
            base_arg_type::flag_is_set(arg_flags::USE_FIRST_KEY_IF_ERROR_ID_EMPTY))
        {
            base_arg_type::set_error_id(get_front_key());
        }
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_key_arg(const basic_key_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_key_arg(basic_key_arg&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_key_arg() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_key_arg& operator =(const basic_key_arg& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_key_arg& operator =(basic_key_arg&& rhs) noexcept
    {
        if (this != &rhs)
        {
            base_arg_type::operator =(std::move(rhs));
            kys_ = std::move(rhs.kys_);
        }
    
        return *this;
    }
    
    /**
     * @brief       Allows knowing whether an argument has a specified id.
     * @param       id : The id to check.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    bool has_id(const string_type& id) const noexcept override
    {
        return std::find(kys_.begin(), kys_.end(), id) != kys_.end();
    }
    
    /**
     * @brief       Get first argument key.
     * @return      The first specified key is returned.
     * @throw       speed::argparse::value_not_found_exception : If the argument hasn't any key an
     *              exception is thrown.
     */
    const arg_key_type& get_front_key() const
    {
        if (kys_.empty())
        {
            throw value_not_found_exception();
        }
        
        return kys_.front();
    }
    
    /**
     * @brief       Get the key in the specified position.
     * @param       indx : Position of the element.
     * @return      The element specified.
     * @throw       speed::argparse::value_not_found_exception : If there isn't any key in the
     *              specified position an exception is thrown.
     */
    const arg_key_type& get_key_at(std::size_t indx) const
    {
        if (kys_.size() <= indx)
        {
            throw value_not_found_exception();
        }
        
        return kys_.at(indx);
    }
    
    /**
     * @brief       Get the necessary length to print short arguments IDs in standard output.
     * @return      The necessary length to print short arguments IDs in standard output.
     */
    std::size_t get_short_ids_length() const noexcept override
    {
        if (base_arg_type::description_is_empty())
        {
            return 0;
        }
        
        std::size_t kys_len = 0;
        
        for (auto& x : kys_)
        {
            if (!x.is_prefix_long())
            {
                speed::lowlevel::try_addml(&kys_len, x.get_string_length(), 2);
            }
        }
        
        return kys_len;
    }
    
    /**
     * @brief       Get the necessary length to print long arguments IDs in standard output.
     * @return      The necessary length to print long arguments IDs in standard output.
     */
    std::size_t get_long_ids_length() const noexcept override
    {
        if (base_arg_type::description_is_empty())
        {
            return 0;
        }
        
        std::size_t kys_len = 0;
        
        for (auto& x : kys_)
        {
            if (x.is_prefix_long())
            {
                speed::lowlevel::try_addml(&kys_len, x.get_string_length(), 2);
            }
        }
        
        return kys_len;
    }
    
    /**
     * @brief       Print the argument information for help menu.
     * @param       args_indent : Indentation used to print arguments help description.
     * @param       max_line_len : The maximum description length that will be printed in a single
     *              line.
     * @param       new_line_indent : The indentation used when a new line is found in a
     *              description.
     * @param       short_id_len : The maximum length of the short keys.
     * @param       long_id_len : The maximum length of the long keys.
     */
    void print_help_text(
            std::size_t args_indent,
            std::size_t max_line_len,
            std::size_t new_line_indent,
            std::size_t short_id_len,
            std::size_t long_id_len
    ) const override
    {
        if (base_arg_type::description_is_empty())
        {
            return;
        }
        
        std::size_t current_id_len = 0;
        std::size_t n_args_printd = 0;
        std::size_t i;
    
        for (i = args_indent; i > 0; i--)
        {
            std::cout << ' ';
        }
        
        for (auto it = kys_.cbegin(); it != kys_.cend(); it++)
        {
            if (!it->is_prefix_long())
            {
                if (n_args_printd > 0)
                {
                    std::cout << ", " << *it;
                    speed::lowlevel::try_addml(&current_id_len, it->get_string_length(), 2);
                }
                else
                {
                    std::cout << *it;
                    speed::lowlevel::try_addm(&current_id_len, it->get_string_length());
                }
    
                speed::lowlevel::try_addm(&n_args_printd, 1);
            }
        }
        
        if (n_args_printd < kys_.size() && n_args_printd > 0)
        {
            std::cout << ", ";
            speed::lowlevel::try_addm(&current_id_len, 2);
        }
        
        if (current_id_len < short_id_len)
        {
            for (i = short_id_len - current_id_len; i > 0; i--)
            {
                std::cout << ' ';
            }
        }
        
        n_args_printd = 0;
        current_id_len = 0;
        for (auto it = kys_.cbegin(); it != kys_.cend(); it++)
        {
            if (it->is_prefix_long())
            {
                if (n_args_printd > 0)
                {
                    std::cout << ", " << *it;
                    speed::lowlevel::try_addml(&current_id_len, it->get_string_length(), 2);
                }
                else
                {
                    std::cout << *it;
                    speed::lowlevel::try_addm(&current_id_len, it->get_string_length());
                }
    
                speed::lowlevel::try_addm(&n_args_printd, 1);
            }
        }
        
        if (current_id_len < long_id_len)
        {
            for (i = long_id_len - current_id_len; i > 0; i--)
            {
                std::cout << ' ';
            }
        }
    
        speed::lowlevel::try_addml(&args_indent, short_id_len, long_id_len);
        speed::lowlevel::try_addm(&new_line_indent, args_indent);
        
        base_arg_type::print_help_text(max_line_len, new_line_indent, args_indent);
    }
    
    /**
     * @brief       Parse the argument.
     */
    void parse()
    {
        arg_parser_type* ap = base_arg_type::get_composite();
        base_arg_type::set_found(true);
        ap->print_help_and_version_if_necessary(this);
    }

protected:
    /**
     * @brief       Local copy assignment method.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     * @throw       std::bad_alloc : If the default allocator is used, an exception is thrown if the
     *              function needs to allocate storage and fails.
     */
    basic_key_arg& local_assignment(const basic_key_arg& rhs)
    {
        if (this != &rhs)
        {
            kys_ = rhs.kys_;
        }
        
        return *this;
    }
    
    /**
     * @brief       Local move assignment method.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_key_arg& local_assignment(basic_key_arg&& rhs) noexcept
    {
        if (this != &rhs)
        {
            kys_ = std::move(rhs.kys_);
        }
        
        return *this;
    }

private:
    /** Argument keys collection. */
    vector_type<arg_key_type> kys_;
    
    friend class basic_arg_parser<TpAllocator>;
};


/** Class that represents arguments that have keys. */
using key_arg = basic_key_arg<std::allocator<int>>;


}


#endif

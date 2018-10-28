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
 * @file        speed/argparse/basic_base_arg.hpp
 * @brief       basic_base_arg class header.
 * @author      Killian Green
 * @date        2015/12/28 - 23:17
 */

#ifndef SPEED_ARGPARSE_BASIC_COMMAND_ARG_HPP
#define SPEED_ARGPARSE_BASIC_COMMAND_ARG_HPP

#include "../containers.hpp"
#include "../iostream.hpp"
#include "../lowlevel.hpp"
#include "arg_error_flags.hpp"
#include "arg_flags.hpp"
#include "argparse_forward_declarations.hpp"
#include "i_help_text.hpp"


namespace speed::argparse {


/**
 * @brief       Class that represents the base of the arguments hierarchy.
 */
template<typename TpAllocator>
class basic_base_arg : public i_help_text
{
public:
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;
    
    /** Class that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<TpAllocator>;
    
    /** Class that represents flags container */
    template<typename T>
    using flags_type = speed::containers::flags<T>;
    
    /** Class that represents sequences of characters. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       desc : Argument description content.
     * @param       err_id : The id used to reference an arguments during the error display.
     * @param       flgs : Flags that dictates the argument behavior.
     * @param       compo : The composite object of this class.
     */
    template<typename TpString1_, typename TpString2_>
    basic_base_arg(
            TpString1_&& desc,
            TpString2_&& err_id,
            arg_flags flgs,
            arg_parser_type* compo
    )
            : desc_(std::forward<TpString1_>(desc))
            , err_id_(std::forward<TpString2_>(err_id))
            , parsd_(false)
            , fnd_(false)
            , flgs_(flgs)
            , err_flgs_(arg_error_flags::NIL)
            , compo_(compo)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_base_arg(const basic_base_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_base_arg(basic_base_arg&& rhs) noexcept
            : desc_(std::move(rhs.desc_))
            , err_id_(std::move(rhs.err_id_))
            , parsd_(std::move(rhs.parsd_))
            , fnd_(std::move(rhs.fnd_))
            , flgs_(std::move(rhs.flgs_))
            , err_flgs_(std::move(rhs.err_flgs_))
            , compo_(std::move(rhs.compo_))
    {
        rhs.parsd_ = false;
        rhs.fnd_ = false;
        rhs.compo_ = nullptr;
    }
    
    /**
     * @brief       Destructor.
     */
    virtual ~basic_base_arg() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_base_arg& operator =(const basic_base_arg& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_base_arg& operator =(basic_base_arg&& rhs) noexcept
    {
        if (this != &rhs)
        {
            desc_ = std::move(rhs.desc_);
            err_id_ = std::move(rhs.err_id_);
            std::swap(parsd_, rhs.parsd_);
            std::swap(fnd_, rhs.fnd_);
            flgs_ = std::move(rhs.flgs_);
            err_flgs_ = std::move(rhs.err_flgs_);
            std::swap(compo_, rhs.compo_);
        }
        
        return *this;
    }
    
    /**
     * @brief       Allows knowing whether the argument description is empty.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    inline bool description_is_empty() const noexcept
    {
        return desc_.empty();
    }
    
    /**
     * @brief       Get the argument error id.
     * @return      The argument error id.
     */
    inline const string_type& get_error_id() const noexcept
    {
        return err_id_;
    }
    
    /**
     * @brief       Get a value that allows knowing whether the argument has been parsed.
     * @return      A value that allows knowing whether the argument has been parsed.
     */
    inline bool was_parsed() const noexcept
    {
        return parsd_;
    }
    
    /**
     * @brief       Get a value that specifies whether the argument has been found in the program
     *              call.
     * @return      The value that specifies whether the argument has been found in the program
     *              call.
     */
    inline bool was_found() const noexcept
    {
        return fnd_;
    }
    
    /**
     * @brief       Allows knowing whether an argument flag is set.
     * @param       flg : The flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool flag_is_set(arg_flags flg) const noexcept
    {
        return flgs_.is_set(flg);
    }
    
    /**
     * @brief       Allows knowing whether an argument flag is set.
     * @param       flg : The flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool error_flag_is_set(arg_error_flags flg) const noexcept
    {
        return err_flgs_.is_set(flg);
    }
    
    /**
     * @brief       Allows knowing whether there are errors flags.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool there_are_errors() const noexcept
    {
        return !err_flgs_.is_empty();
    }
    
    /**
     * @brief       Print the argument information for help menu.
     * @param       max_line_len : The maximum description length that will be printed in a
     *              single line.
     * @param       new_line_indent : The indentation used when a newline is found in a
     *              description.
     * @param       current_line_len : The length of the current line.
     */
    void print_help_text(
            std::size_t max_line_len,
            std::size_t new_line_indent,
            std::size_t current_line_len
    ) const override
    {
        if (desc_.empty())
        {
            return;
        }
    
        std::size_t len_to_next;
    
        for (auto it = desc_.cbegin(); it != desc_.cend(); it++)
        {
            if (*it == '\n')
            {
                current_line_len = new_line_indent;
                std::cout << '\n';
                for (std::size_t i = 0; i < new_line_indent; i++)
                {
                    std::cout << ' ';
                }
            }
            else if (*it == ' ')
            {
                len_to_next = 1;
    
                auto aux_it = it;
                aux_it++;
                for (; aux_it != desc_.cend() && *aux_it != ' '; aux_it++)
                {
                    len_to_next++;
                }
            
                speed::lowlevel::try_addm(&len_to_next, current_line_len);
                if (len_to_next > max_line_len)
                {
                    current_line_len = new_line_indent;
                    std::cout << '\n';
                    for (std::size_t i = 0; i < new_line_indent; i++)
                    {
                        std::cout << ' ';
                    }
                }
                else
                {
                    std::cout << ' ';
                    current_line_len++;
                }
            }
            else
            {
                std::cout << *it;
                current_line_len++;
            }
        }
    
        std::cout << '\n';
    }
    
    /**
     * @brief       Print argument errors in standard output.
     * @param       prog_name : The name of the program.
     * @param       colrs_enable : If it is true, colors will be used during the print.
     */
    virtual void print_errors(const string_type& prog_name, bool colrs_enable) const
    {
        if (error_flag_is_set(arg_error_flags::ALLWAYS_REQUIRED_ERROR))
        {
            std::cout << prog_name << ": ";
            if (!get_error_id().empty())
            {
                if (colrs_enable)
                {
                    std::cout << speed::iostream::set_light_red_text
                              << get_error_id() << ": "
                              << speed::iostream::set_default_text;
                }
                else
                {
                    std::cout << get_error_id() << ": ";
                }
            }
            std::cout << "Option is allways required\n";
        }
        if (error_flag_is_set(arg_error_flags::APPEAR_JUST_ONCE_ERROR))
        {
            std::cout << prog_name << ": ";
            if (!get_error_id().empty())
            {
                if (colrs_enable)
                {
                    std::cout << speed::iostream::set_light_red_text
                              << get_error_id() << ": "
                              << speed::iostream::set_default_text;
                }
                else
                {
                    std::cout << get_error_id() << ": ";
                }
            }
            std::cout << "Option has appeared more than once\n";
        }
    }
    
    /**
     * @brief       Allows throwing an error to the user using the arguent and its syntax. This
     *              method has been made for particular occasions in which an argument error can't
     *              be detected by the argument parser.
     * @param       err_message : The message to print in the standard output.
     * @param       prog_name : The name of the program.
     * @param       colrs_enable : If it is true, colors will be used during the print.
     */
    void print_error_message(
            const string_type& err_message,
            const string_type& prog_name,
            bool colrs_enable
    ) const
    {
        std::cout << prog_name << ": ";
        if (!get_error_id().empty())
        {
            if (colrs_enable)
            {
                std::cout << speed::iostream::set_light_red_text
                          << get_error_id() << ": "
                          << speed::iostream::set_default_text;
            }
            else
            {
                std::cout << get_error_id() << ": ";
            }
        }
        std::cout << err_message << '\n';
    }
    
    /**
     * @brief       Equal operator.
     * @param       rhs : The object to compare.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool operator ==(const basic_base_arg& rhs) const noexcept
    {
        return desc_ == rhs.desc_ &&
               err_id_ == rhs.err_id_ &&
               parsd_ == rhs.parsd_ &&
               fnd_ == rhs.fnd_ &&
               flgs_ == rhs.flgs_ &&
               err_flgs_ == rhs.err_flgs_;
    }
    
    /**
     * @brief       Not equal operator.
     * @param       rhs : The object to compare.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool operator !=(const basic_base_arg& rhs) const noexcept
    {
        return !(rhs == *this);
    }
    
    /**
     * @brief       Allows knowing whether an argument has a specified key.
     * @param       ky : The key to check.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    virtual bool check_key(const string_type& ky) const noexcept = 0;
    
    /**
     * @brief       Get the necessary length to print short arguments keys in standard output.
     * @return      The necessary length to print short arguments keys in standard output.
     */
    virtual std::size_t get_short_keys_length() const noexcept = 0;
    
    /**
     * @brief       Get the necessary length to print long arguments keys in standard output.
     * @return      The necessary length to print long arguments keys in standard output.
     */
    virtual std::size_t get_long_keys_length() const noexcept = 0;
    
    /**
     * @brief       Print the argument information for help menu.
     * @param       args_indent : Indentation used to print arguments help description.
     * @param       max_line_len : The maximum description length that will be printed in a single
     *              line.
     * @param       new_line_indent : The indentation used when a newline is found in a description.
     * @param       short_id_len : The maximum length of the short keys.
     * @param       long_id_len : The maximum length of the long keys.
     */
    virtual void print_help_text(
            std::size_t args_indent,
            std::size_t max_line_len,
            std::size_t new_line_indent,
            std::size_t short_id_len,
            std::size_t long_id_len
    ) const = 0;

protected:
    /**
     * @brief       Allows knowing whether the argument error id is empty.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    inline bool error_id_is_empty() const noexcept
    {
        return err_id_.empty();
    }
    
    /**
     * @brief       Set the argument error id.
     * @param       err_id : The argument error id.
     */
    template<typename TpString_>
    inline void set_error_id(TpString_&& err_id)
    {
        err_id_ = std::forward<TpString_>(err_id);
    }
    
    /**
     * @brief       Set a value that specifies whether the argument has been parsed.
     * @param       parsd : The value that specifies whether the argument has been parsed.
     */
    inline void set_parsed(bool parsd) noexcept
    {
        parsd_ = parsd;
    }
    
    /**
     * @brief       Set a value that specifies whether the argument has been found in the program
     *              call.
     * @param       fnd : The value that specifies whether the argument has been found in the
     *              program call.
     */
    void set_found(bool fnd) noexcept
    {
        if (fnd && fnd_ && flgs_.is_set(arg_flags::APPEAR_JUST_ONCE))
        {
            err_flgs_.set(arg_error_flags::APPEAR_JUST_ONCE_ERROR);
        }
        else
        {
            fnd_ = fnd;
            err_flgs_.erase(arg_error_flags::APPEAR_JUST_ONCE_ERROR);
        }
    }
    
    /**
     * @brief       Erase a argument flag.
     * @param       flg : Flag to erase.
     */
    inline void erase_flag(arg_flags flg) noexcept
    {
        flgs_.erase(flg);
    }
    
    /**
     * @brief       Set a argument error flag.
     * @param       flg : Error flag to raise.
     */
    inline void set_error_flag(arg_error_flags flg) noexcept
    {
        err_flgs_.set(flg);
    }
    
    /**
     * @brief       Erase a argument error flag.
     * @param       flg : Error flag to erase.
     */
    inline void erase_error_flag(arg_error_flags flg) noexcept
    {
        err_flgs_.erase(flg);
    }
    
    /**
     * @brief       Erase all argument flags.
     */
    inline void clear_error_flags() noexcept
    {
        err_flgs_.clear();
    }
    
    /**
     * @brief       Set errors flags if required.
     */
    virtual void update_error_flags() noexcept
    {
        if (flgs_.is_set(arg_flags::ALLWAYS_REQUIRED) && !fnd_ && parsd_)
        {
            err_flgs_.set(arg_error_flags::ALLWAYS_REQUIRED_ERROR);
        }
        else
        {
            err_flgs_.erase(arg_error_flags::ALLWAYS_REQUIRED_ERROR);
        }
    }
    
    /**
     * @brief       Get the composite object of this class.
     * @return      The composite object of this class.
     */
    inline arg_parser_type* get_composite() const noexcept
    {
        return compo_;
    }

private:
    /** Argument description content. */
    string_type desc_;
    
    /** The id used to reference an arguments during the error display. */
    string_type err_id_;
    
    /** Allows knowing whether the argument has been parsed. */
    bool parsd_;
    
    /** Specifies whether the argument has been found in the program call. */
    bool fnd_;
    
    /** Flags that dictates the argument behavior. */
    flags_type<arg_flags> flgs_;
    
    /** Error flags that allows knowing whether there are errors. */
    flags_type<arg_error_flags> err_flgs_;
    
    /** Holds a reference to the composite object. */
    arg_parser_type* compo_;
    
    friend class basic_arg_parser<TpAllocator>;
};


/** Class that represents the base of the arguments hierarchy. */
using base_arg = basic_base_arg<std::allocator<int>>;
    
    
}


#endif

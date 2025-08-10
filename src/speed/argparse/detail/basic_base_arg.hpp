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
 * @file        basic_base_arg.hpp
 * @brief       basic_base_arg class header.
 * @author      Killian Valverde
 * @date        2015/12/28
 */

#ifndef SPEED_ARGPARSE_DETAIL_BASIC_BASE_ARG_HPP
#define SPEED_ARGPARSE_DETAIL_BASIC_BASE_ARG_HPP

#include <string>
#include <unordered_set>
#include <utility>

#include "forward_declarations.hpp"
#include "../../containers/containers.hpp"
#include "../../iostream/iostream.hpp"
#include "../../safety/safety.hpp"
#include "../basic_arg_parser.hpp"
#include "../exception.hpp"
#include "arg_error_flags.hpp"
#include "arg_flags.hpp"

namespace speed::argparse::detail {

/**
 * @brief       Class that represents the base of the arguments hierarchy.
 */
template<typename AllocatorT>
class basic_base_arg
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<T>;
    
    /** String type used in the class. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;
    
    /** String type used in the class. */
    using string_view_type = std::basic_string_view<char, std::char_traits<char>>;
    
    /** Action type */
    using action_type = std::function<void()>;

    /** Pair type used in the class. */
    template<typename T1, typename T2>
    using pair_type = std::pair<T1, T2>;

    /** Flags type used in the class. */
    template<typename T>
    using flags_type = containers::flags<T>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<AllocatorT>;
    
    /**
     * @brief       Constructor with parameters.
     * @param       arg_parsr : Argument parser that holds this object.
     */
    explicit basic_base_arg(arg_parser_type* arg_parsr)
            : arg_parsr_(arg_parsr)
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
    basic_base_arg(basic_base_arg&& rhs) noexcept = default;
    
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
    basic_base_arg& operator =(basic_base_arg&& rhs) noexcept = default;

    /**
     * @brief       Execute the action if there is one.
     */
    void execute_action()
    {
        if (actn_)
        {
            actn_();
        }
    }

    /**
     * @brief       Increase the number of times the arguement has been found in the program call.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    virtual bool increase_occurrence() noexcept
    {
        bool succss = true;
        
        if (max_occurrences_reached())
        {
            err_flgs_.set(arg_error_flags::MAX_OCCURRENCES_ERROR);
            succss = false;
        }
        
        if (presence_holdr_ != nullptr)
        {
            *presence_holdr_ = true;
        }
        
        safety::try_addm(nr_occurrencs_, 1);
        return succss;
    }
    
    /**
     * @brief       Resets the internal state of the object.
     */
    virtual inline void reset() noexcept
    {
        err_flgs_.clear();
        nr_occurrencs_ = 0;
        
        if (presence_holdr_ != nullptr)
        {
            *presence_holdr_ = false;
        }
    }

    /**
     * @brief       Set errors flags if required.
     */
    virtual inline void update_error_flags() noexcept
    {
        if (nr_occurrencs_ < minmax_occurrencs_.first && arg_parsr_->has_parsed())
        {
            err_flgs_.set(arg_error_flags::MIN_OCCURRENCES_ERROR);
        }
        else
        {
            err_flgs_.unset(arg_error_flags::MIN_OCCURRENCES_ERROR);
        }
    }

    /**
     * @brief       Updates the maximum occurrences if auto-update is enabled.
     * @param       sz : Size of the static container.
     */
    void update_max_occurrences(std::size_t sz)
    {
        if (!max_occurrences_update_ || sz <= minmax_occurrencs_.first)
        {
            return;
        }
        
        minmax_occurrencs_.second = sz;
    }
    
    /**
     * @brief       Allows knowing whether there are errors.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool has_errors() const noexcept
    {
        return err_flgs_.is_not_empty();
    }

    /**
     * @brief       Allows knowing whether the argument description is empty.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool is_description_empty() const
    {
        return desc_.empty();
    }

    /**
     * @brief       Allows knowing whether an argument error flag is set.
     * @param       flg : The error flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool is_error_flag_set(arg_error_flags flg) const noexcept
    {
        return err_flgs_.is_set(flg);
    }

    /**
     * @brief       Allows knowing whether the argument error name is empty.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool is_error_name_empty() const noexcept
    {
        return err_name_.empty();
    }

    /**
     * @brief       Allows knowing whether an argument flag is set.
     * @param       flg : The flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool is_flag_set(arg_flags flg) const noexcept
    {
        return flgs_.is_set(flg);
    }

    /**
     * @brief       Allows knowing if the argument in an option.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool is_option() const noexcept
    {
        return minmax_occurrencs_.first == 0;
    }
    
    /**
     * @brief       Allows knowing whether the argument can't get more values.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool max_occurrences_reached() const noexcept
    {
        return nr_occurrencs_ >= minmax_occurrencs_.second;
    }

    /**
     * @brief       Allows knowing whether the argument has been found in the program call.
     * @return      The value that specifies whether the argument has been found in the program
     *              call.
     */
    [[nodiscard]] inline bool was_found() const noexcept
    {
        return nr_occurrencs_ != 0;
    }

    /**
     * @brief       Get the composite object of this class.
     * @return      The composite object of this class.
     */
    [[nodiscard]] inline arg_parser_type* get_arg_parser() const noexcept
    {
        return arg_parsr_;
    }

    /**
     * @brief       Give access to the argument error name.
     * @return      The argument error id.
     */
    [[nodiscard]] inline const string_type& get_error_name() const noexcept
    {
        return err_name_;
    }

    /**
     * @brief       Get the necessary length to print long arguments keys.
     * @return      The necessary length to print long arguments keys.
     */
    [[nodiscard]] virtual std::size_t get_long_keys_length() noexcept = 0;

    /**
     * @brief       Set whether the max occurrences have to auto-update.
     * @param       auto_updat : Wheter the max occurrences have to auto-update.
     */
    [[nodiscard]] bool get_max_occurrences_update() const noexcept
    {
        return max_occurrences_update_;
    }

    /**
     * @brief       Get the necessary length to print the name of the argument.
     * @return      The necessary length to print long argument name.
     */
    [[nodiscard]] virtual std::size_t get_name_length() const = 0;

    /**
     * @brief       Get the program name.
     * @return      The program name.
     */
    [[nodiscard]] const string_type& get_program_name() const noexcept
    {
        return arg_parsr_->get_program_name();
    }

    /**
     * @brief       Get the necessary length to print short arguments keys.
     * @return      The necessary length to print short arguments keys.
     */
    [[nodiscard]] virtual std::size_t get_short_keys_length() noexcept = 0;

    /**
     * @brief       Get a string that represents the kind of argument it is.
     * @return      A string that represents the kind of argument it is.
     */
    [[nodiscard]] inline virtual string_view_type get_tittle() const
    {
        return is_option() ? "Option" : "Command";
    }

    /**
     * @brief       Set the action to execute when the arguement is found in the program call.
     * @param       actn : Action to execute when the arguement is found in the program
     *              call.
     */
    template<typename CallableT>
    inline void set_action(CallableT&& actn)
    {
        actn_ = std::forward<CallableT>(actn);
    }

    /**
     * @brief       Set the argument description.
     * @param       desc : The description to set.
     */
    template<typename StringT_>
    inline void set_description(StringT_&& desc)
    {
        desc_ = std::forward<StringT_>(desc);
    }

    /**
     * @brief       Set an argument error flag.
     * @param       flg : Error flag to set.
     */
    inline void set_error_flag(arg_error_flags flg) noexcept
    {
        err_flgs_.set(flg);
    }

    /**
     * @brief       Set the argument error name.
     * @param       err_name : The argument error name.
     */
    template<typename StringT_>
    inline void set_error_name(StringT_&& err_name) noexcept
    {
        err_name_ = std::forward<StringT_>(err_name);
    }

    /**
     * @brief       Set an argument flag.
     * @param       flg : Flag to set.
     */
    virtual inline void set_flag(arg_flags flg) noexcept
    {
        flgs_.set(flg);
    }

    /**
     * @brief       Set the argument flags.
     * @param       flgs : The argument flags.
     */
    virtual inline void set_flags(arg_flags flgs) noexcept
    {
        flgs_ = flgs;
    }

    /**
     * @brief       Set the help menus assigned with the arguement.
     * @param       hlp_menus_ids : The help menus ids of the help menus assigned.
     */
    template<typename... StringTs_>
    void set_help_menus_assigned(StringTs_&&... hlp_menus_ids)
    {
        arg_parsr_->remove_from_help_menus(this);
        arg_parsr_->register_into_help_menus(this, std::forward<StringTs_>(hlp_menus_ids)...);
    }

    /**
     * @brief       Sets whether the argument is mandatory.
     * @param       enabl : `true` to make the argument mandatory, `false` to make it optional.
     */
    virtual void set_mandatory(bool enabl)
    {
        if (enabl && minmax_occurrencs_.first == 0)
        {
            minmax_occurrencs_.first = 1;
            
            if (minmax_occurrencs_.second == 0)
            {
                minmax_occurrencs_.second = 1;
            }
        }
        if (!enabl)
        {
            minmax_occurrencs_.first = 0;
        }
    }

    /**
     * @brief       Set the minimum and maximum number of time an argument can be find in the
     *              program call.
     * @param       min : The minimum number of occurrences.
     * @param       max : The maxium number of occurrences.
     */
    virtual void set_minmax_occurrences(std::size_t min, std::size_t max)
    {
        if (max == 0)
        {
            throw wrong_max_occurrences_exception();
        }
        if (min > max)
        {
            throw wrong_min_max_interval_exception();
        }

        minmax_occurrencs_.first = min;
        minmax_occurrencs_.second = max;
    }

    /**
     * @brief       Set whether the max occurrences have to auto-update.
     * @param       auto_updat : Wheter the max occurrences have to auto-update.
     */
    void set_max_occurrences_update(bool auto_updat)
    {
        max_occurrences_update_ = auto_updat;
    }

    /**
     * @brief       Set the presence synchronizer.
     * @param       presence_holdr : The presence synchronizer.
     */
    inline void set_presence_holder(bool& presence_holdr) noexcept
    {
        presence_holdr_ = &presence_holdr;
        if (presence_holdr_ != nullptr)
        {
            *presence_holdr_ = nr_occurrencs_ == 0;
        }
    }

    /**
     * @brief       Unset a argument error flag.
     * @param       flg : Error flag to unset.
     */
    inline void unset_error_flag(arg_error_flags flg) noexcept
    {
        err_flgs_.unset(flg);
    }

    /**
     * @brief       Unset an argument flag.
     * @param       flg : Flag to unset.
     */
    virtual inline void unset_flag(arg_flags flg) noexcept
    {
        flgs_.unset(flg);
    }
    
    /**
     * @brief       Print argument errors in standard output.
     */
    virtual void print_errors() const
    {
        auto tittl = get_tittle();
        auto& os = arg_parsr_->get_ostream();
        
        if (err_flgs_.is_set(arg_error_flags::MIN_OCCURRENCES_ERROR))
        {
            print_error_message();
            if (minmax_occurrencs_.first == 1)
            {
                os << tittl << " is allways required\n";
            }
            else
            {
                os << tittl << " must appear at least "
                   << minmax_occurrencs_.first << " times\n";
            }
        }
        if (err_flgs_.is_set(arg_error_flags::MAX_OCCURRENCES_ERROR))
        {
            print_error_message();
            if (minmax_occurrencs_.second == 1)
            {
                os << tittl << " has appeared more than once\n";
            }
            else
            {
                os << tittl << " must appear no more than "
                   << minmax_occurrencs_.first << " times\n";
            }
        }
    }

    /**
     * @brief       Print an error message.
     */
    void print_error_message() const
    {
        auto& os = arg_parsr_->get_ostream();
        
        os << arg_parsr_->get_program_name() << ": ";
        if (!err_name_.empty())
        {
            if (arg_parsr_->colors_enabled())
            {
                os << iostream::set_light_red_text
                   << err_name_ << ": "
                   << iostream::set_default_text;
            }
            else
            {
                os << err_name_ << ": ";
            }
        }
    }

    /**
     * @brief       Print the argument help text.
     * @param       current_line_len : The current length of the current line.
     * @param       max_line_len : The maximum line length that will be printed.
     * @param       new_line_indent : The indentation used after a newline is printed.
     */
    virtual void print_help_text(
            std::size_t current_line_len,
            std::size_t max_line_len,
            std::size_t new_line_indent
    )
    {
        if (desc_.empty())
        {
            return;
        }
        
        auto& os = arg_parsr_->get_ostream();
        
        iostream::print_wrapped(os, desc_, max_line_len, new_line_indent, current_line_len);
        os.put('\n');
    }

    /**
     * @brief       Print the argument help text.
     * @param       args_indent : Indentation used to print arguments help description.
     * @param       max_line_len : The maximum line length that will be printed.
     * @param       new_line_indent : The indentation used after a newline is printed.
     * @param       short_kys_len : The maximum short keys length.
     * @param       long_kys_len : The maximum long keys length.
     */
    virtual void print_help_text(
            std::size_t args_indent,
            std::size_t max_line_len,
            std::size_t new_line_indent,
            std::size_t short_kys_len,
            std::size_t long_kys_len
    ) = 0;

    /**
     * @brief       Print the name of the argument in the standard output.
    */
    virtual void print_name() = 0;

    /**
     * @brief       Print the usage in the standard output.
    */
    virtual void print_usage() = 0;

private:
    /** The argument description. */
    string_type desc_;
    
    /** The name used to reference an arguments during the error display. */
    string_type err_name_;

    /** Minimum and maximum number of times the argument has to be found. */
    pair_type<std::size_t, std::size_t> minmax_occurrencs_ = {0, 1};

    /** Function to execute if the arguement is found during the program call. */
    action_type actn_;

    /** Reference to the argument parser that holds this object. */
    arg_parser_type* arg_parsr_;

    /** Reference to a boolen that will be set to indicate whether the argument has been found. */
    bool* presence_holdr_ = nullptr;

    /** Number of times the argument has been found. */
    std::size_t nr_occurrencs_ = 0;
    
    /** Flags that dictates the argument behavior. */
    flags_type<arg_flags> flgs_ = arg_flags::NIL;
    
    /** Flags that allows knowing whether there are errors. */
    flags_type<arg_error_flags> err_flgs_ = arg_error_flags::NIL;

    /** Dictates whether the max occurrences number will auto-update. */
    bool max_occurrences_update_ = true;
};

}

#endif

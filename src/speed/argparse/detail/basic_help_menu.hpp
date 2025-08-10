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
 *  @file       basic_help_menu.hpp
 *  @brief      basic_help_menu class header.
 *  @author     Killian Valverde
 *  @date       2024/04/16
 */

#ifndef SPEED_ARGPARSE_DETAIL_BASIC_HELP_MENU_HPP
#define SPEED_ARGPARSE_DETAIL_BASIC_HELP_MENU_HPP

#include <memory>
#include <string>
#include <vector>

#include "forward_declarations.hpp"
#include "../../containers/containers.hpp"
#include "../basic_arg_parser.hpp"
#include "basic_arg_constraint.hpp"
#include "basic_base_arg.hpp"
#include "basic_help_arg.hpp"
#include "basic_key_arg.hpp"
#include "basic_key_value_arg.hpp"
#include "basic_positional_arg.hpp"
#include "basic_value_arg.hpp"
#include "basic_version_arg.hpp"
#include "help_menu_flags.hpp"

namespace speed::argparse::detail {

/**
 * @brief       Class used to represent a help menu.
 */
template<typename AllocatorT>
class basic_help_menu
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<T>;

    /** String type used in the class. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;

    /** Vector type used in the class. */
    template<typename T>
    using vector_type = std::vector<T, allocator_type<T>>;

    /** Flags type used in the class. */
    template<typename T>
    using flags_type = containers::flags<T>;
    
    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<AllocatorT>;
    
    /** Type that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<AllocatorT>;
    
    /** Type that represents arguments that have values. */
    using value_arg_type = basic_value_arg<AllocatorT>;
    
    /** Type that represents the argument to get the version information. */
    using version_arg_type = basic_version_arg<AllocatorT>;

    /** Type that represents arguments without keys. */
    using positional_arg_type = basic_positional_arg<AllocatorT>;
    
    /** Type that represents arguments that have keys and values. */
    using key_value_arg_type = basic_key_value_arg<AllocatorT>;

    /** Type that represents the argument to get the help information. */
    using help_arg_type = basic_help_arg<AllocatorT>;
    
    /** Type that represents a constraint for a set of arguments. */
    using arg_dependency_type = basic_arg_constraint<AllocatorT>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<AllocatorT>;

    /**
     * @brief       Constructor with parameters.
     * @param       arg_parsr : Argument parser that holds this object.
     */
    basic_help_menu(arg_parser_type* arg_parsr)
            : arg_parsr_(arg_parsr)
    {
        bse_args_.reserve(10);
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_help_menu(const basic_help_menu& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_help_menu(basic_help_menu&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_help_menu() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_help_menu& operator =(const basic_help_menu& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_help_menu& operator =(basic_help_menu&& rhs) noexcept = default;

    /**
     * @brief       Add an argument into the help menu.
     * @param       bse_arg : Argument to add.
     */
    void add_entry(base_arg_type* bse_arg)
    {
        bse_args_.emplace_back(bse_arg);
    }

    /**
     * @brief       Remove the specified argument from the help menu.
     * @param       bse_arg : Argument to remove.
     */
    void remove_entry(base_arg_type* bse_arg)
    {
        for (auto it = bse_args_.begin(); it != bse_args_.end(); ++it)
        {
            if (bse_arg == *it)
            {
                bse_args_.erase(it);
                break;
            }
        }
    }
    
    /**
     * @brief       Set the arguments indentation during the print.
     * @param       arg_desc_indentation : Arguments indentation during the print.
     */
    void set_args_indentation(std::size_t arg_desc_indentation)
    {
        args_indent_ = arg_desc_indentation;
    }

    /**
     * @brief       Set an epilog.
     * @param       epilg : The epilog to set.
     */
    template<typename StringT_>
    void set_epilog(StringT_&& epilg)
    {
        epilg_ = std::forward<StringT_>(epilg);
    }

    /**
     * @brief       Set a description.
     * @param       desc :  Description to set.
     */
    template<typename StringT_>
    void set_description(StringT_&& desc)
    {
        desc_ = std::forward<StringT_>(desc);
    }

    /**
     * @brief       Set the indentation after printing new lines.
     * @param       new_line_indent : The indentation after printing new lines.
     */
    void set_new_line_indentation(std::size_t new_line_indent)
    {
        new_line_indent_ = new_line_indent;
    }

    /**
     * @brief       Set a flad in the help menu.
     * @param       flg : Flag to set.
     */
    inline void set_flag(help_menu_flags flg) noexcept
    {
        flgs_.set(flg);
    }

    /**
     * @brief       Set the maximum lenght for a line.
     * @param       max_desc_line_length : The maximum length for a line.
     */
    void set_maximum_line_length(std::size_t max_desc_line_length)
    {
        max_line_len_ = max_desc_line_length;
    }

    /**
     * @brief       Set a flad in the help menu.
     * @param       flg : Flag to set.
     */
    inline basic_help_menu& unset_flag(help_menu_flags flg) noexcept
    {
        flgs_.unset(flg);
        return *this;
    }

    /**
     * @brief       Allows knowing whether an argument flag is set.
     * @param       flg : The flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool is_flag_set(help_menu_flags flg) const noexcept
    {
        return flgs_.is_set(flg);
    }

    /**
     * @brief       Print the help menu.
     */
    void print()
    {
        update_max_keys_length();

        print_usage();
        print_description();
        print_options();
        print_commands();
        print_positionals();
        print_constraints();
        print_epilog();

        arg_parsr_->flush_output_stream();
    }

private:
    /**
     * @brief       Update the maximum length needed to print the keys in the help menu.
     */
    void update_max_keys_length() noexcept
    {
        for (auto& bse_arg : bse_args_)
        {
            if (auto* ps_arg = dynamic_cast<positional_arg_type*>(bse_arg))
            {
                if (!ps_arg->is_description_empty() &&
                    flgs_.is_set(help_menu_flags::PRINT_POSITIONALS))
                {
                    update_max_keys_length_from_positional_arg(ps_arg);
                }
            }
            else if (auto* ky_arg = dynamic_cast<key_arg_type*>(bse_arg))
            {
                bool should_prnt =
                        (ky_arg->is_option() && flgs_.is_set(help_menu_flags::PRINT_OPTIONS)) ||
                        (!ky_arg->is_option() && flgs_.is_set(help_menu_flags::PRINT_COMMANDS));
    
                if (should_prnt && !ky_arg->is_description_empty())
                {
                    update_max_keys_length_from_key_arg(ky_arg);
                }
            }
        }
    }
    
    /**
     * @brief       Update the maximum keys length fron a key argument.
     * @param       ky_arg : Key argument to check.
     */
    void update_max_keys_length_from_key_arg(key_arg_type* ky_arg) noexcept
    {
        auto short_kys_len = ky_arg->get_short_keys_length();
        auto long_kys_len = ky_arg->get_long_keys_length();
        
        if (max_short_kys_len_ < short_kys_len)
        {
            max_short_kys_len_ = short_kys_len;
        }
        if (max_long_kys_len_ < long_kys_len)
        {
            max_long_kys_len_ = long_kys_len;
        }
    }

    /**
     * @brief       Update the maximum keys length fron a positional argument.
     * @param       positionl_arg : positional argument to check.
     */
    void update_max_keys_length_from_positional_arg(positional_arg_type* positionl_arg) noexcept
    {
        std::size_t short_kys_len = positionl_arg->get_short_keys_length();
        std::size_t totl = max_short_kys_len_ + max_long_kys_len_;

        if (short_kys_len > totl)
        {
            max_long_kys_len_ =  short_kys_len - max_short_kys_len_;
        }
    }
    
    /**
     * @brief       Print the usage.
     */
    void print_usage()
    {
        if (flgs_.is_set(help_menu_flags::PRINT_USAGE))
        {
            arg_parsr_->print_usage();
        }
    }

    /**
     * @brief       Print the description.
     */
    inline void print_description()
    {
        auto& os = arg_parsr_->get_ostream();
        
        if (flgs_.is_set(help_menu_flags::PRINT_DESCRIPTION) && !desc_.empty())
        {
            iostream::print_wrapped(os, desc_, max_line_len_, 0);
            os << "\n\n";
        }
    }

    /**
     * @brief       Print the options.
     */
    void print_options()
    {
        if (flgs_.is_not_set(help_menu_flags::PRINT_OPTIONS))
        {
            return;
        }
        
        key_arg_type* ky_arg;
        bool fnd = false;
        auto& os = arg_parsr_->get_ostream();

        for (auto& bs_arg : bse_args_)
        {
            ky_arg = dynamic_cast<key_arg_type*>(bs_arg);
            if (ky_arg != nullptr && !ky_arg->is_description_empty() && ky_arg->is_option())
            {
                fnd = true;
                break;
            }
        }
        if (!fnd)
        {
            return;
        }

        os << "Options:\n";

        for (auto& bs_arg : bse_args_)
        {
            ky_arg = dynamic_cast<key_arg_type*>(bs_arg);
            if (ky_arg != nullptr && ky_arg->is_option())
            {
                print_arg(ky_arg);
            }
        }

        os.put('\n');
    }

    /**
     * @brief       Print the commands.
     */
    void print_commands()
    {
        if (flgs_.is_not_set(help_menu_flags::PRINT_COMMANDS))
        {
            return;
        }

        key_arg_type* ky_arg;
        bool fnd = false;
        auto& os = arg_parsr_->get_ostream();

        for (auto& bs_arg : bse_args_)
        {
            ky_arg = dynamic_cast<key_arg_type*>(bs_arg);
            if (ky_arg != nullptr && !ky_arg->is_description_empty() && !ky_arg->is_option())
            {
                fnd = true;
                break;
            }
        }
        if (!fnd)
        {
            return;
        }

        os << "Commands:\n";

        for (auto& bs_arg : bse_args_)
        {
            ky_arg = dynamic_cast<key_arg_type*>(bs_arg);
            if (ky_arg != nullptr && !ky_arg->is_option())
            {
                print_arg(ky_arg);
            }
        }

        os.put('\n');
    }

    /**
     * @brief       Print the positional arguments.
     */
    void print_positionals()
    {
        if (flgs_.is_not_set(help_menu_flags::PRINT_POSITIONALS))
        {
            return;
        }

        positional_arg_type* ps_arg;
        bool fnd = false;
        auto& os = arg_parsr_->get_ostream();

        for (auto& bs_arg : bse_args_)
        {
            ps_arg = dynamic_cast<positional_arg_type*>(bs_arg);
            if (ps_arg != nullptr && !ps_arg->is_description_empty())
            {
                fnd = true;
                break;
            }
        }
        if (!fnd)
        {
            return;
        }

        os << "Values:\n";

        for (auto& bs_arg : bse_args_)
        {
            ps_arg = dynamic_cast<positional_arg_type*>(bs_arg);
            if (ps_arg != nullptr)
            {
                print_arg(ps_arg);
            }
        }

        os.put('\n');
    }

    /**
     * @brief       Print the constraints.
     */
    void print_constraints()
    {
        if (flgs_.is_not_set(help_menu_flags::PRINT_CONSTRAINTS))
        {
            return;
        }
        
        bool fnd = false;
        auto& constrnts = arg_parsr_->get_constraints();
        auto& os = arg_parsr_->get_ostream();
        
        for (auto& constrnt : constrnts)
        {
            if (constrnt.contains_any_of(bse_args_))
            {
                fnd = true;
                break;
            }
        }
        if (!fnd)
        {
            return;
        }

        os << "Constraints:\n";
        
        for (auto& constrnt : constrnts)
        {
            if (constrnt.contains_any_of(bse_args_))
            {
                constrnt.print_help(args_indent_, max_line_len_, new_line_indent_,
                        max_short_kys_len_, max_long_kys_len_);
            }
        }
    }

    /**
     * @brief       Print the epilog.
     */
    void print_epilog()
    {
        if (flgs_.is_set(help_menu_flags::PRINT_EPILOGUE) && !epilg_.empty())
        {
            auto& os = arg_parsr_->get_ostream();
            iostream::print_wrapped(os, epilg_, max_line_len_, 0);
            os << "\n\n";
        }
    }

    /**
     * @brief       Print the argument.
     * @param       bse_arg : Print the base arguement.
     */
    inline void print_arg(base_arg_type* bse_arg)
    {
        if (bse_arg->is_description_empty())
        {
            return;
        }

        if (flgs_.is_not_set(help_menu_flags::PRINT_ARGS_KEYS))
        {
            bse_arg->print_help_text(0, max_line_len_, new_line_indent_);
        }
        else
        {
            bse_arg->print_help_text(args_indent_, max_line_len_, new_line_indent_,
                    max_short_kys_len_, max_long_kys_len_);
        }
    }

private:
    /** The description. */
    string_type desc_;

    /** The epilogue. */
    string_type epilg_;

    /** They args of the help menu. */
    vector_type<base_arg_type*> bse_args_;

    /** Reference to the argument parser that holds this object. */
    arg_parser_type* arg_parsr_;

    /** Indentation of the arugments keys in the help menu. */
    std::size_t args_indent_ = 2;

    /** The maximum line length that will be printed. */
    std::size_t max_line_len_ = 80;

    /** The indentation used after printing a new line. */
    std::size_t new_line_indent_ = 2;

    /** The maximum lenght needed to print the short keys. */
    std::size_t max_short_kys_len_ = 0;

    /** The maximum lenght needed to print the long keys. */
    std::size_t max_long_kys_len_ = 0;

    /** Flags that dictates the help menu behavior. */
    flags_type<help_menu_flags> flgs_ = help_menu_flags::DEFAULT;
};

}

#endif

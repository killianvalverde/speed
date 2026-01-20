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
 *  @file       basic_arg_parser.hpp
 *  @brief      basic_arg_parser class header.
 *  @author     Killian Valverde
 *  @date       2015/12/31
 */

#ifndef SPEED_ARGPARSE_BASIC_ARG_PARSER_HPP
#define SPEED_ARGPARSE_BASIC_ARG_PARSER_HPP

#include <list>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <span>
#include <vector>

#include "detail/forward_declarations.hpp"
#include "../containers/containers.hpp"
#include "../memory/memory.hpp"
#include "../stringutils/stringutils.hpp"
#include "detail/arg_error_flags.hpp"
#include "detail/arg_flags.hpp"
#include "detail/arg_parser_error_flags.hpp"
#include "detail/arg_parser_flags.hpp"
#include "detail/arg_value_error_flags.hpp"
#include "detail/basic_arg_constraint.hpp"
#include "detail/basic_arg_constraint_setter.hpp"
#include "detail/basic_arg_parser_setter.hpp"
#include "detail/basic_help_arg.hpp"
#include "detail/basic_help_arg_setter.hpp"
#include "detail/basic_help_menu.hpp"
#include "detail/basic_help_menu_setter.hpp"
#include "detail/basic_key_arg.hpp"
#include "detail/basic_key_arg_setter.hpp"
#include "detail/basic_key_value_arg.hpp"
#include "detail/basic_key_value_arg_setter.hpp"
#include "detail/basic_positional_arg.hpp"
#include "detail/basic_positional_arg_setter.hpp"
#include "detail/basic_version_arg.hpp"
#include "detail/basic_version_arg_setter.hpp"

namespace speed::argparse {

/**
 * @brief       Class used to parse arguments.
 */
template<typename AllocatorT>
class basic_arg_parser
{
public:
    /** The character type used in the argument parser. */
    using char_type = char;
    
    /** The ostream type used in the argument parser. */
    using ostream_type = std::basic_ostream<char_type>;
    
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<T>;
    
    /** Shared pointer type used in the class. */
    template<typename T>
    using unique_ptr_type = std::unique_ptr<T>;

    /** String type used in the class. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;

    /** String view type used in the class. */
    using string_view_type = std::basic_string_view<char, std::char_traits<char>>;

    /** Vector type used in the class. */
    template<typename T>
    using vector_type = std::vector<T, allocator_type<T>>;
    
    /** Vector type used in the class. */
    template<typename T>
    using list_type = std::list<T, allocator_type<T>>;

    /** Unordered map type used in the class. */
    template<typename KeyT_, typename ValueT_>
    using unordered_map_type = std::unordered_map<
            KeyT_, ValueT_, std::hash<KeyT_>,
            std::equal_to<KeyT_>,
            allocator_type<std::pair<const KeyT_, ValueT_>>>;

    /** Unordered set type used in the class. */
    template<typename KeyT_>
    using unordered_set_type = std::unordered_set<
            KeyT_, std::hash<KeyT_>,
            std::equal_to<KeyT_>,
            allocator_type<KeyT_>>;
    
    /** Span type used in the class. */
    template<typename ValueT>
    using span_type = std::span<ValueT>;

    /** Class that represents a bit field */
    template<typename T>
    using flags_type = containers::flags<T>;
    
private:
    /** Type that represents a key for an argument. */
    using arg_key_type = detail::basic_arg_key<AllocatorT>;

    /** Type that represents a value for an argument. */
    using arg_value_type = detail::basic_arg_value<AllocatorT>;
    
    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = detail::basic_base_arg<AllocatorT>;
    
    /** Type that represents arguments that have keys. */
    using key_arg_type = detail::basic_key_arg<AllocatorT>;
    
    /** Type that represents arguments that have values. */
    using value_arg_type = detail::basic_value_arg<AllocatorT>;
    
    /** Type that represents arguments that have keys and values. */
    using key_value_arg_type = detail::basic_key_value_arg<AllocatorT>;

    /** Type that represents arguments without keys. */
    using positional_arg_type = detail::basic_positional_arg<AllocatorT>;

    /** Type that represents the option to get the help information. */
    using help_arg_type = detail::basic_help_arg<AllocatorT>;
    
    /** Type that represents the argument to get the version information. */
    using version_arg_type = detail::basic_version_arg<AllocatorT>;

    /** Type that represents a constraint for a set of arguments. */
    using arg_constraint_type = detail::basic_arg_constraint<AllocatorT>;

    /** Type that represents a help menu. */
    using help_menu_type = detail::basic_help_menu<AllocatorT>;

    /** Type used to configure an added key argument. */
    using key_arg_setter_type = detail::basic_key_arg_setter<AllocatorT>;

    /** Type used to configure an added key value argument. */
    using key_value_arg_setter_type = detail::basic_key_value_arg_setter<AllocatorT>;

    /** Type used to configure an added positional argument. */
    using positional_arg_setter_type = detail::basic_positional_arg_setter<AllocatorT>;

    /** Type used to configure an added help argument. */
    using help_arg_setter_type = detail::basic_help_arg_setter<AllocatorT>;

    /** Type used to configure an added version argument. */
    using version_arg_setter_type = detail::basic_version_arg_setter<AllocatorT>;

    /** Type used to configure an added argument constraint. */
    using arg_constraint_setter_type = detail::basic_arg_constraint_setter<AllocatorT>;

    /** Type used to configure an added help menu. */
    using help_menu_setter_type = detail::basic_help_menu_setter<AllocatorT>;

    /** Type used to configure the argument parser. */
    using arg_parser_setter_type = detail::basic_arg_parser_setter<AllocatorT>;
    
    /**
    * @brief        Represents the possible states of the parsing DFA (Deterministic Finite
     *              Automaton).
    */
    enum class parsing_dfa_state : std::uint8_t
    {
        /** Initial state before parsing begins. */
        START,
    
        /** Reading the next argument from input. */
        READ_ARG,
    
        /** Parsing a string. */
        PARSE_STRING,
    
        /** Parsing a key argument. */
        PARSE_KEY_ARG,
    
        /** Parsing an equal sign (`=`) used to assign a value to a key. */
        PARSE_EQ_OPERATOR,
    
        /** Parsing grouped short arguments (e.g., `-abc`). */
        PARSE_GROUPING_ARGS,
    
        /** Parsing a positional (non-key) argument. */
        PARSE_POSITIONAL_ARG,
    
        /** Handling an unrecognized or invalid argument. */
        PARSE_UNRECOGNIZED_ARG,
    
        /** State indicating parsing should be terminated. */
        QUIT,
    
        /** Final state after successful parsing. */
        FINISH
    };
    
    /**
     * @brief       Holds the context and state information for the parsing DFA.
     */
    template<typename ArgcT_, typename ArgvT_>
    struct parsing_dfa_context
    {
        /** Number of command-line arguments. */
        ArgcT_ argc = 0;
    
        /** Reference to the array or container of argument values. */
        const ArgvT_& argv;
    
        /** Current DFA state. */
        parsing_dfa_state stte = parsing_dfa_state::START;
    
        /** Current index of the argument being processed. */
        std::size_t idx = 0;
    
        /** Pointer to the current key argument being parsed. */
        key_arg_type* ky_arg = nullptr;
    
        /** Pointer to the previously parsed argument. */
        base_arg_type* prev_arg = nullptr;
    
        /** Iterator to the current positional argument. */
        vector_type<unique_ptr_type<base_arg_type>>::iterator pos_arg_it;
    };

public:
    /**
     * @brief       Constructor with parameters.
     * @param       prog_name : The program name. If you don't specify any, the program name will
     *              be obtained from argv.
     */
    template<
            typename StringT_ = string_type,
            typename = std::enable_if_t<
                    !std::is_base_of<basic_arg_parser, std::decay_t<StringT_>>::value
            >
    >
    explicit basic_arg_parser(StringT_&& prog_name = string_type())
            : prog_name_(std::forward<StringT_>(prog_name))
    {
        bse_arg_list_.reserve(10);
        bse_arg_map_.reserve(20);
    }
    
    /**
     * @brief       Copy constructor [deleted].
     * @param       rhs : Object to copy.
     */
    basic_arg_parser(const basic_arg_parser& rhs) = delete;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_arg_parser(basic_arg_parser&& rhs) noexcept = delete;
    
    /**
     * @brief       Destructor.
     */
    ~basic_arg_parser()
    {
        default_hlp_arg_ = nullptr;
        current_vers_arg_ = nullptr;
    }
    
    /**
     * @brief       Copy assignment operator [deleted].
     * @param       rhs : Object to copy.
     */
    basic_arg_parser& operator =(const basic_arg_parser& rhs) = delete;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     */
    basic_arg_parser& operator =(basic_arg_parser&& rhs) noexcept = delete;

    /**
     * @brief       Allows to get access to an object to configure the argument parser behaviour.
     * @return      An object that will allow to configure the argument parser behaviour.
     */
    arg_parser_setter_type configure() noexcept
    {
        return {this};
    }
    
    /**
     * @brief       Add an argument that has keys.
     * @param       kys : Variadic number of strings to speficy every key that the arguement will
     *              have.
     */
    template<typename... StringTs_>
    key_arg_setter_type add_key_arg(StringTs_&&... kys)
    {
        (assert_valid_key(kys), ...);
        
        unique_ptr_type<key_arg_type> ky_arg = memory::allocate_unique<key_arg_type>(
                allocator_type<key_arg_type>(), *this, std::forward<StringTs_>(kys)...);
        
        key_arg_type* ky_arg_ref = ky_arg.get();
        register_key_arg(std::move(ky_arg));
        
        return {ky_arg_ref};
    }
    
    /**
     * @brief       Add an argument that has keys and a set of values associated.
     * @param       kys : Variadic number of strings to speficy every key that the arguement will
     *              have.
     */
    template<typename... StringTs_>
    key_value_arg_setter_type add_key_value_arg(StringTs_&&... kys)
    {
        (assert_valid_key(kys), ...);
        
        unique_ptr_type<key_value_arg_type> ky_val_arg =
                memory::allocate_unique<key_value_arg_type>(
                        allocator_type<key_value_arg_type>(), *this,
                        std::forward<StringTs_>(kys)...);
        
        key_value_arg_type* ky_val_arg_ref = ky_val_arg.get();
        register_key_value_arg(std::move(ky_val_arg));
        
        return {ky_val_arg_ref};
    }

    /**
     * @brief       Add an argument that just has values and doesn't have keys.
     * @param       ky : The ID that will be used to make refence to this argument in the
     *              parser as well as being used during the printing of the usage message.
     */
    template<typename StringTs_>
    positional_arg_setter_type add_positional_arg(StringTs_&& ky)
    {
        assert_valid_key(ky);
        
        unique_ptr_type<positional_arg_type> positionl_arg =
                memory::allocate_unique<positional_arg_type>(
                        allocator_type<positional_arg_type>(), *this, std::forward<StringTs_>(ky));
        
        positional_arg_type* positionl_arg_ref = positionl_arg.get();
        register_positional_arg(std::move(positionl_arg));
        
        return {positionl_arg_ref};
    }
    
    /**
     * @brief       Add an argument used to print a help menu.
     * @param       kys : Variadic number of strings to speficy every key that the arguement will
     *              have.
     */
    template<typename... StringTs_>
    help_arg_setter_type add_help_arg(StringTs_&&... kys)
    {
        (assert_valid_key(kys), ...);
        
        unique_ptr_type<help_arg_type> hlp_arg = memory::allocate_unique<help_arg_type>(
                allocator_type<help_arg_type>(), *this, std::forward<StringTs_>(kys)...);
        
        help_arg_type* hlp_arg_ref = hlp_arg.get();
        register_help_arg(std::move(hlp_arg));
        
        return {hlp_arg_ref};
    }
    
    /**
     * @brief       Add an argument used to print the version information.
     * @param       kys : Variadic number of strings to speficy every key that the arguement will
     *              have.
     */
    template<typename... StringTs_>
    version_arg_setter_type add_version_arg(StringTs_&&... kys)
    {
        (assert_valid_key(kys), ...);
        assert_valid_version();
        
        unique_ptr_type<version_arg_type> vers_arg =
                memory::allocate_unique<version_arg_type>(
                        allocator_type<version_arg_type>(), *this, std::forward<StringTs_>(kys)...);
        
        version_arg_type* vers_arg_ref = vers_arg.get();
        register_version_arg(std::move(vers_arg));
        
        return {vers_arg_ref};
    }
    
    /**
     * @brief       Add a constraint that verifies that at least one of the specified arguments has
     *              been found in the program call.
     * @param       kys : Variadic number of strings to speficy every argument in which the
     *              constraint applies.
     */
    template<typename... StringTs_>
    arg_constraint_setter_type add_constraint(const StringTs_&... kys)
    {
        return {&constrnts_.emplace_back(this, kys...)};
    }

    /**
     * @brief       Add a help menu to the arguement parser.
     * @param       hlp_menu_id : The ID that the help menu will have and that it will be refered
     *              by from the arguements. If the ID already exists nothing will happened. If the
     *              ID is empty the default help menu will be refered.
     * @return      Object that allows configurint the help menu added.
     */
    template<typename StringT_ = string_type>
    help_menu_setter_type add_help_menu(StringT_&& hlp_menu_id = string_type())
    {
        return {&get_help_menu(std::forward<StringT_>(hlp_menu_id))};
    }
    
    /**
     * @brief       Perform the parsing of the arguements.
     * @param       argc : Contains the number of arguments gotten through the program call.
     * @param       argv : Contains the arguments gotten through the program call.
     */
    template<typename ArgcT_, typename ArgvT_>
    void parse_args(ArgcT_ argc, const ArgvT_& argv)
    {
        parsing_dfa_context<ArgcT_, ArgvT_> parsing_dfa_contxt{ .argc = argc, .argv = argv };
        
        while (parsing_dfa_contxt.stte != parsing_dfa_state::FINISH)
        {
            switch (parsing_dfa_contxt.stte)
            {
            case parsing_dfa_state::START:
                parse_start_state(parsing_dfa_contxt);
                continue;

            case parsing_dfa_state::READ_ARG:
                parse_read_state(parsing_dfa_contxt);
                continue;

            case parsing_dfa_state::PARSE_STRING:
                parse_string_state(parsing_dfa_contxt);
                continue;

            case parsing_dfa_state::PARSE_KEY_ARG:
                parse_key_arg_state(parsing_dfa_contxt);
                continue;

            case parsing_dfa_state::PARSE_EQ_OPERATOR:
                parse_eq_operator_state(parsing_dfa_contxt);
                continue;

            case parsing_dfa_state::PARSE_GROUPING_ARGS:
                parse_grouping_args_state(parsing_dfa_contxt);
                continue;

            case parsing_dfa_state::PARSE_POSITIONAL_ARG:
                parse_positional_arg_state(parsing_dfa_contxt);
                continue;

            case parsing_dfa_state::PARSE_UNRECOGNIZED_ARG:
                parse_unrecognized_args_state(parsing_dfa_contxt);
                continue;

            case parsing_dfa_state::QUIT:
                parse_quit_state(parsing_dfa_contxt);
                continue;
            }
        }
    }
    
    /**
     * @brief       Allows knowing whether there are errors associated with a specific argument in
     *              the argument parser.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename StringT_>
    [[nodiscard]] bool arg_has_errors(const StringT_& ky) const
    {
        base_arg_type* bse_arg = get_base_arg(ky);
        
        if (bse_arg == nullptr)
        {
            throw key_not_found_exception();
        }
        
        return bse_arg->has_errors();
    }
    
    /**
     * @brief       Allows knowing whether there are errors in the argument parser.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool has_errors() const noexcept
    {
        return err_flgs_.is_not_empty();
    }
    
    /**
     * @brief       Allows knowing whether an argument has been found in the program call.
     * @param       ky : Argument key to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename StringT_>
    [[nodiscard]] bool was_found(const StringT_& ky) const
    {
        base_arg_type* bse_arg = get_base_arg(ky);

        if (bse_arg == nullptr)
        {
            throw key_not_found_exception();
        }

        return bse_arg->was_found();
    }

    /**
     * @brief       Allows knowing the number of values that has been found for a specified
     *              arguement that have values associated.
     * @param       ky : The arguement key to check.
     * @return      The number of values that has been found for a specified argument that have
     *              values associated.
     */
    template<typename StringT_>
    [[nodiscard]] std::size_t count_values_found(const StringT_& ky) const
    {
        value_arg_type* val_arg = get_value_arg(ky);

        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }

        return val_arg->get_total_values_count();
    }
    
    /**
     * @brief       Get the key argument front value converted to target.
     * @param       ky : Key of the argument to get the front value.
     * @return      The key argument front value converted to target
     */
    template<typename TargetT_, typename StringT_>
    [[nodiscard]] TargetT_ get_front_as(const StringT_& ky) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }
        
        return val_arg->template get_front_value_as<TargetT_>();
    }
    
    /**
     * @brief       Get the key argument front value converted to target.
     * @param       ky : Key of the argument to get the front value.
     * @param       default_val : The value returned if the operatin fails.
     * @return      If function was successful the key argument front value converted to target 
     *              is returned, otherwise the default value is returned.
     */
    template<typename TargetT_, typename StringT_, typename DefaultValueT_>
    [[nodiscard]] TargetT_ get_front_as(
            const StringT_& ky,
            DefaultValueT_&& default_val
    ) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }
        
        return val_arg->template get_front_value_as<TargetT_>(
                std::forward<DefaultValueT_>(default_val));
    }
    
    /**
     * @brief       Try to get the key argument front value converted to target.
     * @param       ky : Key of the argument to get the front value.
     * @param       res : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TargetT_, typename StringT_, typename DefaultValueT_>
    [[nodiscard]] bool try_get_front_as(
            const StringT_& ky,
            TargetT_& res
    ) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }
        
        return val_arg->template try_get_front_value_as<TargetT_>(res);
    }
    
    /**
     * @brief       Get the key argument index value converted to target.
     * @param       ky : Key of the argument to get the index value.
     * @param       indx : Index of the value to get.
     * @return      The key argument index value converted to target.
     */
    template<typename TargetT_, typename StringT_>
    [[nodiscard]] TargetT_ get_at_as(const StringT_& ky, std::size_t indx) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }
        
        return val_arg->template get_value_at_as<TargetT_>(indx);
    }
    
    /**
     * @brief       Get the key argument index value converted to target.
     * @param       ky : Key of the argument to get the front value.
     * @param       indx : Index of the value to get.
     * @param       default_val : The value returned if the operatin fails.
     * @return      If function was successful the key argument index value converted to target is
     *              returned, otherwise the default value is returned.
     */
    template<typename TargetT_, typename StringT_, typename DefaultValueT_>
    [[nodiscard]] TargetT_ get_at_as(
            const StringT_& ky,
            std::size_t indx,
            DefaultValueT_&& default_val
    ) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }
        
        return val_arg->template get_value_at_as<TargetT_>(
                indx, std::forward<DefaultValueT_>(default_val));
    }
    
    /**
     * @brief       Try to get the key argument index value converted to target.
     * @param       ky : Key of the argument to get the index value.
     * @param       indx : Index of the value to get.
     * @param       res : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TargetT_, typename StringT_>
    [[nodiscard]] bool try_get_at_as(
            const StringT_& ky,
            std::size_t indx,
            TargetT_& res
    ) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }
        
        return val_arg->template try_get_value_at_as<TargetT_>(indx, res);
    }
    
    /**
     * @brief       Get the key argument values converted to target.
     * @param       ky : Key of the argument to get the values.
     * @return      The key argument values converted to target.
     */
    template<typename TargetT_, typename StringT_>
    [[nodiscard]] vector_type<TargetT_> get_all_as(const StringT_& ky) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }

        return val_arg->template get_values_as<TargetT_>();
    }
    
    /**
     * @brief       Get the key argument values converted to target.
     * @param       ky : Key of the argument to get the values.
     * @param       default_val : The value returned if the operatin fails.
     * @return      If function was successful the key argument values converted to target are 
     *              returned, otherwise the default value is returned.
     */
    template<
            typename TargetT_,
            typename StringT_,
            typename DefaultValueT_ = vector_type<TargetT_>
    >
    [[nodiscard]] vector_type<TargetT_> get_all_as(
            const StringT_& ky,
            DefaultValueT_&& default_val
    ) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }

        return val_arg->template get_values_as<TargetT_>(
                std::forward<DefaultValueT_>(default_val));
    }
    
    /**
     * @brief       Try to get the key argument values converted to target.
     * @param       ky : Key of the argument to get the values.
     * @param       res : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TargetT_, typename StringT_>
    [[nodiscard]] bool try_get_all_as(
            const StringT_& ky,
            vector_type<TargetT_>& res
    ) const
    {
        value_arg_type* val_arg = get_value_arg(ky);
        
        if (val_arg == nullptr)
        {
            throw key_not_found_exception();
        }

        return val_arg->template try_get_values_as<TargetT_>(res);
    }
    
    /**
     * @brief       Print a help menu.
     * @param       hlp_menu_id : The ID of the help menu to print. If nothing is pecified the
     *              default help menu will be printed.
     */
    template<typename StringT_ = string_type>
    void print_help(const StringT_& hlp_menu_id = string_type())
    {
        get_help_menu(hlp_menu_id).print();
    }
    
    /**
     * @brief       Print the version information.
     */
    void print_version()
    {
        if (current_vers_arg_ == nullptr)
        {
            throw version_not_found_exception();
        }

        current_vers_arg_->print_version_information();
    }
    
    /**
     * @brief       Print the argument parser errors.
     */
    void print_errors()
    {
        if (err_flgs_.is_empty())
        {
            return;
        }
        
        use_error_stream();
        auto& os = get_ostream();
        
        if (err_flgs_.is_set(detail::arg_parser_error_flags::ARGS_ERROR))
        {
            for (auto& bse_arg : bse_arg_list_)
            {
                bse_arg->print_errors();
            }
        }
        
        if (err_flgs_.is_set(detail::arg_parser_error_flags::ARGS_CONSTRAINTS_ERROR))
        {
            for (auto& constrnt : constrnts_)
            {
                constrnt.print_errors();
            }
        }
        
        if (err_flgs_.is_set(detail::arg_parser_error_flags::UNRECOGNIZED_ARGS_ERROR))
        {
            for (auto& unrecog_arg : unrecog_args_)
            {
                os << prog_name_ << ": ";
                if (!err_id_.empty())
                {
                    if (flgs_.is_set(detail::arg_parser_flags::USE_COLORS))
                    {
                        os << iostream::set_light_red_text
                           << err_id_ << ": "
                           << iostream::set_default_text;
                    }
                    else
                    {
                        os << err_id_ << ": ";
                    }
                }
                os << "Unrecognized option '" << unrecog_arg << "'\n";
            }
        }

        if (!prog_name_.empty() && default_hlp_arg_ != nullptr)
        {
            os << "Try '" << prog_name_ << ' ' << default_hlp_arg_->get_front_key()
               << "' for more information.\n";
        }
        
        if (flgs_.is_set(detail::arg_parser_flags::PRINT_HELP_AFTER_PRINTING_ERRORS))
        {
            print_help();
        }
        else
        {
            flush_output_stream();
        }
        
        if (flgs_.is_set(detail::arg_parser_flags::PKILL_AFTER_PRINTING_ERRORS))
        {
            exit_program(SPEED_EXIT_CODE_USAGE_ERROR);
        }
        
        use_output_stream();
    }

private:
    /**
     * @brief       Assert the validity of the specified key.
     * @param       ky : Key to check.
     */
    template<typename StringT_>
    void assert_valid_key(const StringT_& ky) const
    {
        if (arg_key_exists(ky))
        {
            throw key_already_exists_exception();
        }
    }

    /**
     * @brief       Assert the validity of adding a version argument.
     */
    void assert_valid_version() const
    {
        if (current_vers_arg_ != nullptr)
        {
            throw version_already_exists_exception();
        }
    }

    /**
     * @brief       Register in the data stuctures a specified key argument and its keys.
     * @param       ky_arg : Key argument to register.
     */
    void register_key_arg(unique_ptr_type<key_arg_type> ky_arg)
    {
        for (auto& ky : ky_arg->get_keys())
        {
            bse_arg_map_.emplace(ky.get_string(), ky_arg.get());
        }
        
        register_into_help_menus(ky_arg.get());
        bse_arg_list_.emplace_back(std::move(ky_arg));
    }

    /**
     * @brief       Register in the data stuctures a specified key value argument and its keys.
     * @param       ky_val_arg : Key value argument to register.
     */
    void register_key_value_arg(unique_ptr_type<key_value_arg_type> ky_val_arg)
    {
        register_key_arg(std::move(ky_val_arg));
    }

    /**
     * @brief       Register in the data stuctures a specified positional argument and its key.
     * @param       positionl_arg : positional argument to register.
     */
    void register_positional_arg(unique_ptr_type<positional_arg_type> positionl_arg)
    {
        bse_arg_map_.emplace(positionl_arg->get_key(), positionl_arg.get());
        register_into_help_menus(positionl_arg.get());
        bse_arg_list_.emplace_back(std::move(positionl_arg));
    }

    /**
     * @brief       Register in the data stuctures a specified help argument and its keys.
     * @param       hlp_arg : Help argument to register.
     */
    void register_help_arg(unique_ptr_type<help_arg_type> hlp_arg)
    {
        if (default_hlp_arg_ == nullptr)
        {
            default_hlp_arg_ = hlp_arg.get();
        }
        register_key_arg(std::move(hlp_arg));
    }

    /**
     * @brief       Register in the data stuctures a specified version argument and its keys.
     * @param       vers_arg : Version argument to register.
     */
    void register_version_arg(unique_ptr_type<version_arg_type> vers_arg)
    {
        if (current_vers_arg_ == nullptr)
        {
            current_vers_arg_ = vers_arg.get();
        }
        register_key_arg(std::move(vers_arg));
    }

    /**
     * @brief       Register the specified arguement into the default help menu.
     * @param       bse_arg : Specified argument to register into the default help menu.
     */
    void register_into_help_menus(base_arg_type* bse_arg)
    {
        get_help_menu().add_entry(bse_arg);
    }

    /**
     * @brief       Register the specified arguement into the help menus specified by their IDs.
     * @param       bse_arg : Argument to register.
     * @param       hlp_menus_ids : IDs of the help menu in which register the argument.
     */
    template<typename... StringTs_>
    void register_into_help_menus(base_arg_type* bse_arg, StringTs_&&... hlp_menus_ids)
    {
        (get_help_menu(std::forward<StringTs_>(hlp_menus_ids)).add_entry(bse_arg), ...);
    }

    /**
     * @brief       Remove an arguement to every help menu specified by their IDs.
     * @param       bse_arg : Argument to remove.
     */
    void remove_from_help_menus(base_arg_type* bse_arg)
    {
        for (auto& x : hlp_menu_map_)
        {
            x.second->remove_entry(bse_arg);
        }
    }

    /**
     * @brief       Exit the program with the value specified.
     * @param       val : Value that the program will return.
     */
    void exit_program(int val) const noexcept
    {
        this->~basic_arg_parser();
        exit(val);
    }
    
    /**
     * @brief       Flushes the current output stream.
     */
    void flush_output_stream()
    {
        std::flush(*ostrm);
    }
    
    /**
     * @brief       Initializes the parsing DFA to its starting state.
     * @param       contxt : Reference to the parsing DFA context that will be initialized.
     */
    template<typename ArgcT_, typename ArgvT_>
    void parse_start_state(parsing_dfa_context<ArgcT_, ArgvT_>& contxt)
    {
        reset_args_parse();
        
        if (contxt.argc > 0 && prog_name_.empty())
        {
            prog_name_ = contxt.argv[0];
        }
        
        contxt.idx = 1;
        contxt.stte = parsing_dfa_state::READ_ARG;
        contxt.pos_arg_it = get_first_positional_arg(bse_arg_list_.begin());
    }
    
    /**
     * @brief       Handles the DFA state for reading the next argument.
     * @param       contxt : Reference to the parsing DFA context that holds the current state.
     */
    template<typename ArgcT_, typename ArgvT_>
    void parse_read_state(parsing_dfa_context<ArgcT_, ArgvT_>& contxt)
    {
        if (contxt.idx >= contxt.argc)
        {
            contxt.stte = parsing_dfa_state::QUIT;
            return;
        }
        
        contxt.stte = parsing_dfa_state::PARSE_STRING;
    }
    
    /**
     * @brief       Handles the DFA state for parsing a string.
     * @param       contxt : Reference to the parsing DFA context containing the current state.
     */
    template<typename ArgcT_, typename ArgvT_>
    void parse_string_state(parsing_dfa_context<ArgcT_, ArgvT_>& contxt)
    {
        contxt.ky_arg = get_key_arg(contxt.argv[contxt.idx]);
        
        if (contxt.ky_arg == nullptr)
        {
            contxt.stte = parsing_dfa_state::PARSE_EQ_OPERATOR;
            return;
        }
        
        contxt.stte = parsing_dfa_state::PARSE_KEY_ARG;
    }
    
    /**
     * @brief       Handles the DFA state for processing a key argument.
     * @param       contxt : Reference to the parsing DFA context containing the current state.
     */
    template<typename ArgcT_, typename ArgvT_>
    void parse_key_arg_state(parsing_dfa_context<ArgcT_, ArgvT_>& contxt)
    {
        parse_key_arg(contxt);
        ++contxt.idx;
        contxt.prev_arg = contxt.ky_arg;
        contxt.stte = parsing_dfa_state::READ_ARG;
    }
    
    /**
     * @brief       Handles the DFA state for parsing arguments with an equal-sign assignment.
     * @param       contxt : Reference to the parsing DFA context containing the current state.
     */
    template<typename ArgcT_, typename ArgvT_>
    void parse_eq_operator_state(parsing_dfa_context<ArgcT_, ArgvT_>& contxt)
    {
        string_view_type cur_argv = contxt.argv[contxt.idx];
        string_view_type ky_strv;
        string_view_type val_strv;
        std::size_t eq_pos;
        key_value_arg_type* ky_val_arg;
        
        if (cur_argv.size() > 2 &&
            (eq_pos = cur_argv.find('=', 1)) != string_type::npos)
        {
            ky_strv = cur_argv.substr(0, eq_pos);
            val_strv = cur_argv.substr(eq_pos + 1);
    
            if (!ky_strv.empty() &&
                !val_strv.empty() &&
                (ky_val_arg = get_key_value_arg(ky_strv)) != nullptr &&
                        ky_val_arg->is_flag_set(detail::arg_flags::ASSIGNMENT_OPERATOR))
            {
                ky_val_arg->add_value(val_strv);
                contxt.ky_arg = ky_val_arg;
                contxt.stte = parsing_dfa_state::PARSE_KEY_ARG;
                return;
            }
        }
        
        contxt.stte = parsing_dfa_state::PARSE_GROUPING_ARGS;
    }
    
    /**
     * @brief       Handles the DFA state for parsing grouped single-character arguments.
     * @param       contxt : Reference to the parsing DFA context containing the current state.
     */
    template<typename ArgcT_, typename ArgvT_>
    void parse_grouping_args_state(parsing_dfa_context<ArgcT_, ArgvT_>& contxt)
    {
        string_view_type cur_argv = contxt.argv[contxt.idx];
        string_view_type ky_prefix = get_key_prefix(cur_argv);
        string_type ky_builder;
        bool valid_groupng;
        
        ky_builder.reserve(ky_prefix.size() + 1);
        ky_builder = ky_prefix;
        ky_builder += ' ';
        
        auto for_each_grouping_key = [&](auto&& actn) -> bool
        {
            std::size_t i = ky_prefix.size();
            for (; i < cur_argv.size(); ++i)
            {
                ky_builder[ky_prefix.size()] = cur_argv[i];
                key_arg_type* ky_arg = get_key_arg(ky_builder);
                if (!actn(ky_arg))
                {
                    return false;
                }
            }
            return i > ky_prefix.size();
        };
        
        valid_groupng = for_each_grouping_key([&](key_arg_type* ky_arg) -> bool
        {
            return ky_arg && ky_arg->is_flag_set(detail::arg_flags::GROUPING);
        });
        
        if (!valid_groupng)
        {
            contxt.stte = parsing_dfa_state::PARSE_POSITIONAL_ARG;
            return;
        }
        
        for_each_grouping_key([&](key_arg_type* ky_arg) -> bool
        {
            contxt.ky_arg = ky_arg;
            parse_key_arg(contxt);
            return true;
        });
        
        ++contxt.idx;
        contxt.stte = parsing_dfa_state::READ_ARG;
        contxt.prev_arg = static_cast<base_arg_type*>(contxt.ky_arg);
    }
    
    /**
     * @brief       Handles the DFA state for parsing positional arguments.
     * @param       contxt : Reference to the parsing DFA context containing the current state.
     */
    template<typename ArgcT_, typename ArgvT_>
    void parse_positional_arg_state(parsing_dfa_context<ArgcT_, ArgvT_>& contxt)
    {
        string_view_type cur_argv = contxt.argv[contxt.idx];
        positional_arg_type* pos_arg;
        bool insertd = false;
        
        while (contxt.pos_arg_it != bse_arg_list_.end() &&
               dynamic_cast<positional_arg_type*>(contxt.pos_arg_it->get())
                        ->max_values_reached())
        {
            contxt.pos_arg_it = get_next_positional_arg(contxt.pos_arg_it);
        }
        
        while (contxt.pos_arg_it != bse_arg_list_.end())
        {
            pos_arg = dynamic_cast<positional_arg_type*>(contxt.pos_arg_it->get());
            bool prefix_err = is_value_prefix_invalid(pos_arg, cur_argv);
            
            if (prefix_err || !pos_arg->try_add_value(cur_argv))
            {
                if (pos_arg->min_values_reached() ||
                    (pos_arg->get_current_dimension_values_count() > 0 && prefix_err))
                {
                    contxt.pos_arg_it = get_next_positional_arg(contxt.pos_arg_it);
                    continue;
                }
                else if (!prefix_err)
                {
                    pos_arg->add_value(cur_argv);
                    insertd = true;
                }
                
                break;
            }
            
            insertd = true;
            break;
        }
        
        if (!insertd)
        {
            contxt.stte = parsing_dfa_state::PARSE_UNRECOGNIZED_ARG;
            return;
        }
        
        if (static_cast<base_arg_type*>(pos_arg) != contxt.prev_arg)
        {
            pos_arg->execute_action();
            pos_arg->increase_occurrence();
        }
        
        ++contxt.idx;
        contxt.stte = parsing_dfa_state::READ_ARG;
        contxt.prev_arg = pos_arg;
    }
    
    /**
     * @brief       Handles the DFA state for parsing unrecognized arguments.
     * @param       contxt : Reference to the parsing DFA context containing the current state.
     */
    template<typename ArgcT_, typename ArgvT_>
    void parse_unrecognized_args_state(parsing_dfa_context<ArgcT_, ArgvT_>& contxt)
    {
        if (unrecog_args_.size() < max_unrecog_args_)
        {
            unrecog_args_.emplace_back(contxt.argv[contxt.idx]);
        }
        
        ++contxt.idx;
        contxt.stte = parsing_dfa_state::READ_ARG;
        contxt.prev_arg = nullptr;
    }
    
    /**
     * @brief       Handles the DFA state to terminate parsing.
     * @param       contxt : Reference to the parsing DFA context containing the current state.
     */
    template<typename ArgcT_, typename ArgvT_>
    void parse_quit_state(parsing_dfa_context<ArgcT_, ArgvT_>& contxt)
    {
        parsd_ = true;
        update_error_flags();
        trigger_prints();
        contxt.stte = parsing_dfa_state::FINISH;
    }
    
    /**
     * @brief       Parses a key argument and processes its associated values.
     * @param       contxt : Reference to the parsing DFA context containing the current state.
     */
    template<typename ArgcT_, typename ArgvT_>
    void parse_key_arg(parsing_dfa_context<ArgcT_, ArgvT_>& contxt)
    {
        std::size_t pos_inc = 0;
        
        if (!contxt.ky_arg->increase_occurrence())
        {
            return;
        }
        
        contxt.ky_arg->execute_action();

        if (auto* ky_val_arg = dynamic_cast<key_value_arg_type*>(contxt.ky_arg);
            ky_val_arg != nullptr)
        {
            for (pos_inc = 1;
                 !ky_val_arg->max_values_reached() &&
                         contxt.idx + pos_inc < contxt.argc &&
                         string_can_be_value(ky_val_arg, contxt.argv[contxt.idx + pos_inc]);
                 ++pos_inc)
            {
                if (ky_val_arg->min_values_reached())
                {
                    if (!ky_val_arg->try_add_value(contxt.argv[contxt.idx + pos_inc]))
                    {
                        break;
                    }
                }
                else
                {
                    ky_val_arg->add_value(contxt.argv[contxt.idx + pos_inc]);
                }
            }

            --pos_inc;
        }
        
        contxt.ky_arg->parse_sub_arg_parser(contxt.argc, contxt.argv, contxt.idx, pos_inc);
        contxt.idx += pos_inc;
    }

    /**
     * @brief       Reset the arguments parse.
     */
    void reset_args_parse() noexcept
    {
        parsd_ = false;
        
        for (auto& bse_arg : bse_arg_list_)
        {
            bse_arg->reset();
        }
        
        unrecog_args_.clear();
        update_error_flags();
    }

    /**
     * @brief       Set error flags if it's required.
     */
    void update_error_flags() noexcept
    {
        bool err_fnd = false;

        for (auto& bse_arg : bse_arg_list_)
        {
            bse_arg->update_error_flags();
            if (bse_arg->has_errors() && !err_fnd)
            {
                err_fnd = true;
                err_flgs_.set(detail::arg_parser_error_flags::ARGS_ERROR);
            }
        }
        if (!err_fnd)
        {
            err_flgs_.unset(detail::arg_parser_error_flags::ARGS_ERROR);
        }

        if (!unrecog_args_.empty())
        {
            err_flgs_.set(detail::arg_parser_error_flags::UNRECOGNIZED_ARGS_ERROR);
        }
        else
        {
            err_flgs_.unset(detail::arg_parser_error_flags::UNRECOGNIZED_ARGS_ERROR);
        }

        err_fnd = false;
        for (auto& constrnt : constrnts_)
        {
            if (constrnt.is_violed())
            {
                err_fnd = true;
                err_flgs_.set(detail::arg_parser_error_flags::ARGS_CONSTRAINTS_ERROR);
                break;
            }
        }
        if (!err_fnd)
        {
            err_flgs_.unset(detail::arg_parser_error_flags::ARGS_CONSTRAINTS_ERROR);
        }
    }

    /**
     * @brief       Update the key arguments after the prifixes changed.
     */
    void update_key_args_prefixes()
    {
        key_arg_type* ky_arg;

        for (auto& bse_arg : bse_arg_list_)
        {
            if ((ky_arg = dynamic_cast<key_arg_type*>(bse_arg.get())) != nullptr)
            {
                ky_arg->update_prefixes();
            }
        }
    }
    
    /**
     * @brief       Redirects the output to the standard error stream.
     */
    void use_error_stream()
    {
        ostrm = &speed::iostream::get_cerr<char_type>();
    }
    
    /**
     * @brief       Redirects the output to the standard output stream.
     */
    void use_output_stream()
    {
        ostrm = &speed::iostream::get_cout<char_type>();
    }

    /**
     * @brief       Allows knowing whether a string contains an eq operator.
     * @param       str : The string to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool arg_has_eq_operator(string_view_type str) const
    {
        key_arg_type* ky_arg;
        std::size_t eq_pos;

        if (str.size() > 2)
        {
            eq_pos = str.find('=', 1);
            if (eq_pos != string_type::npos)
            {
                ky_arg = get_key_arg(str.substr(0, eq_pos));
                return ky_arg != nullptr &&
                        ky_arg->is_flag_set(detail::arg_flags::ASSIGNMENT_OPERATOR);
            }
        }

        return false;
    }

    /**
     * @brief       Allows knowing whether an argument key has been already added.
     * @param       ky : Argument key to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename StringT_>
    [[nodiscard]] bool arg_key_exists(const StringT_& ky) const noexcept
    {
        return bse_arg_map_.contains(ky);
    }

    /**
     * @brief       Allows knowing whether a string are composed by chained arguments.
     * @param       str : The string to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool args_are_chained(string_view_type str) const
    {
        string_type ky_builder(get_key_prefix(str));
        key_arg_type* ky_arg = nullptr;

        ky_builder += ' ';
        for (auto i = ky_builder.size() - 1; i < str.size(); ++i)
        {
            ky_builder[ky_builder.size() - 1] = str[i];
            ky_arg = get_key_arg(ky_builder);
            if (ky_arg == nullptr || !ky_arg->is_flag_set(detail::arg_flags::GROUPING))
            {
                return false;
            }
        }

        return ky_arg != nullptr;
    }

    /**
     * @brief       Allows knowing whether colors are enalbled.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool colors_enabled() const noexcept
    {
        return flgs_.is_set(detail::arg_parser_flags::USE_COLORS);
    }

    /**
     * @brief       Allows knowing whether the parse of the arguements have been done.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool has_parsed() const noexcept
    {
        return parsd_;
    }

    /**
     * @brief       Allows knowing whether an error flag is set.
     * @param       flg : Flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_error_flag_set(detail::arg_parser_error_flags flg) const noexcept
    {
        return err_flgs_.is_set(flg);
    }

    /**
     * @brief       Allows knowing whether a flag is set.
     * @param       flg : Flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_flag_set(detail::arg_parser_flags flg) const noexcept
    {
        return flgs_.is_set(flg);
    }

    /**
     * @brief       Allows knowing whether a key prefix is long.
     * @param       ky : String that contains the prefix
     * @return      If function was successful true is returned, if not false is returned.
     */
    [[nodiscard]] bool is_key_prefix_long(string_view_type ky) const
    {
        std::size_t long_prefx_len = 0;
        std::size_t short_prefx_len = 0;
        std::size_t cur_len;

        for (auto& long_prefx : long_prefxs_)
        {
            cur_len = long_prefx.length();

            if (ky.compare(0, cur_len, long_prefx) == 0)
            {
                if (long_prefx_len < cur_len)
                {
                    long_prefx_len = cur_len;
                }
            }
        }

        for (auto& short_prefx : short_prefxs_)
        {
            cur_len = short_prefx.length();

            if (ky.compare(0, cur_len, short_prefx) == 0)
            {
                if (short_prefx_len < cur_len)
                {
                    short_prefx_len = cur_len;
                }
            }
        }

        if (long_prefx_len > short_prefx_len)
        {
            return true;
        }

        return false;
    }

    /**
     * @brief       Allows knowing whether a string can't be an argument value due to the
     *              presece of a prefix while the value argument doesn't allow it.
     * @param       val_arg : The value argument to consider for the checking.
     * @param       str : The string to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename StringT_>
    [[nodiscard]] bool is_value_prefix_invalid(
            value_arg_type* val_arg,
            const StringT_& str
    ) const
    {
        return value_has_prefix(str) &&
                !val_arg->is_flag_set(detail::arg_flags::VALUES_WITH_PREFIX);
    }

    /**
     * @brief       Allows knowing whether a string can be interpreted as an argument value.
     * @param       val_arg : The value argument that the value is currecntly related.
     * @param       str : The string to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename StringT_>
    [[nodiscard]] bool string_can_be_value(value_arg_type* val_arg, const StringT_& str) const
    {
        return !is_value_prefix_invalid(val_arg, str) &&
               (!arg_key_exists(str) || val_arg->is_flag_set(detail::arg_flags::KEYS_AS_VALUES)) &&
               !arg_has_eq_operator(str) && !args_are_chained(str);
    }
    
    /**
     * @brief       Allows knowing whether a specified value has a prefix.
     * @param       val : Value to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool value_has_prefix(string_view_type val) const
    {
        for (auto& long_prefx : long_prefxs_)
        {
            if (val.compare(0, long_prefx.length(), long_prefx) == 0)
            {
                return true;
            }
        }
        
        for (auto& short_prefx : short_prefxs_)
        {
            if (val.compare(0, short_prefx.length(), short_prefx) == 0)
            {
                return true;
            }
        }
        
        return false;
    }

    /**
     * @brief       Get a reference to a base argument.
     * @param       ky : Key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned,
     *              otherwise a null pointer is returned.
     */
    template<typename StringT_>
    [[nodiscard]] base_arg_type* get_base_arg(const StringT_& ky) const noexcept
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            return it->second;
        }
        
        return nullptr;
    }
    
    /**
     * @brief       Get a reference to a key argument.
     * @param       ky : Key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned, if not
     *              a null pointer is returned.
     */
    template<typename StringT_>
    [[nodiscard]] key_arg_type* get_key_arg(const StringT_& ky) const noexcept
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            return dynamic_cast<key_arg_type*>(it->second);
        }
        
        return nullptr;
    }
    
    /**
     * @brief       Get a reference to a value argument.
     * @param       ky : Key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned, if not
     *              a null pointer is returned.
     */
    template<typename StringT_>
    [[nodiscard]] value_arg_type* get_value_arg(const StringT_& ky) const noexcept
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            return dynamic_cast<value_arg_type*>(it->second);
        }
        
        return nullptr;
    }
    
    /**
     * @brief       Get a reference to a key value argument.
     * @param       ky : Key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned, if not
     *              a null pointer is returned.
     */
    template<typename StringT_>
    [[nodiscard]] key_value_arg_type* get_key_value_arg(const StringT_& ky) const noexcept
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            return dynamic_cast<key_value_arg_type*>(it->second);
        }
        
        return nullptr;
    }
    
    /**
     * @brief       Get a reference to a positional argument.
     * @param       ky : Key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned, if not
     *              a null pointer is returned.
     */
    template<typename StringT_>
    [[nodiscard]] positional_arg_type* get_positional_arg(const StringT_& ky) const noexcept
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            return dynamic_cast<positional_arg_type*>(it->second);
        }
        
        return nullptr;
    }

    /**
     * @brief       Get a reference to a help argument.
     * @param       ky : Key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned, if not
     *              a null pointer is returned.
     */
    template<typename StringT_>
    [[nodiscard]] help_arg_type* get_help_arg(const StringT_& ky) const noexcept
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            return dynamic_cast<help_arg_type*>(it->second);
        }
        
        return nullptr;
    }
    
    /**
     * @brief       Retrieves the list of argument constraints.
     * @return      A reference to the list containing pointers to the argument constraint types.
     */
    [[nodiscard]] const list_type<arg_constraint_type>& get_constraints() const noexcept
    {
        return constrnts_;
    }

    /**
     * @brief       Get the help menu specified an ID.
     * @param       hlp_menu_id : ID of the help menu to get.
     * @return      The help menu related witht he specified ID.
     */
    template<typename StringT_ = string_type>
    [[nodiscard]] help_menu_type& get_help_menu(StringT_&& hlp_menu_id = string_type()) noexcept
    {
        auto it = hlp_menu_map_.find(hlp_menu_id);
        
        if (it == hlp_menu_map_.end())
        {
            it = hlp_menu_map_.emplace(
                    std::forward<StringT_>(hlp_menu_id),
                    memory::allocate_unique<help_menu_type>(
                            allocator_type<help_menu_type>(), *this)).first;
        }

        return *(it->second);
    }
    
    /**
     * @brief       Finds the first positional argument in the argument list starting from the given
     *              iterator.
     * @param       it : An iterator to the starting position in a vector of `unique_ptr` to
     *              `base_arg_type`.
     * @return      An iterator pointing to the first `positional_arg_type` element found, or to the
     *              end of the list.
     */
    vector_type<unique_ptr_type<base_arg_type>>::iterator get_first_positional_arg(
            vector_type<unique_ptr_type<base_arg_type>>::iterator it
    ) const noexcept
    {
        while (it != bse_arg_list_.end() &&
               dynamic_cast<positional_arg_type*>(it->get()) == nullptr)
        {
            ++it;
        }
        
        return it;
    }
    
    /**
     * @brief       Get the prefix of the specified key.
     * @param       ky : String that contains the prefix.
     * @return      If function was successful the prefix is returned, if not an empty string is
     *              returned.
     */
    [[nodiscard]] string_view_type get_key_prefix(string_view_type ky) const
    {
        auto res_long_prefx_it = long_prefxs_.end();
        auto res_short_prefx_it = short_prefxs_.end();
        
        auto find_prefix = [&](const auto& prefixs, auto& res_it)
        {
            for (auto prefx_it = prefixs.begin(); prefx_it != prefixs.end(); ++prefx_it)
            {
                std::size_t prefx_len = prefx_it->length();
                if (ky.compare(0, prefx_len, *prefx_it) == 0 &&
                    (res_it == prefixs.end() || res_it->length() < prefx_len))
                {
                    res_it = prefx_it;
                }
            }
        };
        
        find_prefix(long_prefxs_, res_long_prefx_it);
        find_prefix(short_prefxs_, res_short_prefx_it);
        
        if (res_long_prefx_it != long_prefxs_.end() && res_short_prefx_it != short_prefxs_.end())
        {
            return res_long_prefx_it->length() >= res_short_prefx_it->length() ?
                    *res_long_prefx_it : *res_short_prefx_it;
        }
        
        return res_long_prefx_it != long_prefxs_.end() ? *res_long_prefx_it :
                res_short_prefx_it != short_prefxs_.end() ? *res_short_prefx_it :
                string_view_type();
    }
    
    /**
     * @brief       Retrieves the next positional argument from the given iterator position.
     * @param       it : An iterator pointing to the current position in the base argument list.
     * @return      An iterator pointing to the next positional argument, or `bse_arg_list_.end()`
     *              if none is found.
     */
    vector_type<unique_ptr_type<base_arg_type>>::iterator get_next_positional_arg(
            vector_type<unique_ptr_type<base_arg_type>>::iterator it
    ) const noexcept
    {
        if (it != bse_arg_list_.end())
        {
            ++it;
        }
        
        return get_first_positional_arg(std::move(it));
    }
    
    /**
     * @brief       Get the number of arguments that are options.
     * @return      The number of arguments that are options.
     */
    [[nodiscard]] std::size_t get_options_count() const noexcept
    {
        key_arg_type* ky_arg;
        std::size_t nr_options_bldr = 0;
        std::size_t nr_term_not_always_requird = 0;

        for (auto& bse_arg : bse_arg_list_)
        {
            ky_arg = dynamic_cast<key_arg_type*>(bse_arg.get());
            if (ky_arg != nullptr && ky_arg->is_option())
            {
                if (!ky_arg->is_flag_set(detail::arg_flags::TERMINAL) &&
                    !ky_arg->is_flag_set(detail::arg_flags::PKILL_AFTER_TRIGGERING))
                {
                    safety::try_addm(nr_options_bldr, 1);
                }
                else
                {
                    safety::try_addm(nr_term_not_always_requird, 1);
                }
            }
        }

        return (nr_options_bldr == 0 && nr_term_not_always_requird > 0) ? 1 : nr_options_bldr;
    }
    
    [[nodiscard]] ostream_type& get_ostream() noexcept
    {
        return *ostrm;
    }
    
    /**
     * @brief       Get the program name.
     * @return      The program name.
     */
    [[nodiscard]] const string_type& get_program_name() const noexcept
    {
        return prog_name_;
    }

    /**
     * @brief       Set the error id used by the parser for generic errors.
     * @param       err_id : Error id used by the parser for generic errors.
     */
    template<typename StringT_>
    void set_error_id(StringT_&& err_id) noexcept
    {
        err_id_ = std::forward<StringT_>(err_id);
    }

    /**
     * @brief       Set an argument parser flag.
     * @param       flg : Flag to set.
     */
    void set_flag(detail::arg_parser_flags flg) noexcept
    {
        flgs_.set(flg);
    }

    /**
     * @brief       Set the long prefixes.
     * @param       prefxs : The long prefixes.
     */
    template<typename... StringTs_>
    void set_long_prefixes(StringTs_&&... prefxs)
    {
        long_prefxs_.clear();
        (long_prefxs_.emplace(std::forward<StringTs_>(prefxs)), ...);
        update_key_args_prefixes();
    }

    /**
     * @brief       Set the maximum amount of unrecognized arguments.
     * @param       max : The maximum amount of unrecognized arguments.
     */
    void set_maximum_unrecognized_args(std::size_t max) noexcept
    {
        max_unrecog_args_ = max;
    }

    /**
     * @brief       Set the short prefixes.
     * @param       prefxs : The short prefixes.
     */
    template<typename... StringTs_>
    void set_short_prefixes(StringTs_&&... prefxs)
    {
        short_prefxs_.clear();
        (short_prefxs_.emplace(std::forward<StringTs_>(prefxs)), ...);
        update_key_args_prefixes();
    }

    /**
     * @brief       Set the program name.
     * @param       prog_name : The program name.
     */
    template<typename StringT_>
    void set_program_name(StringT_&& prog_name)
    {
        prog_name_ = std::forward<StringT_>(prog_name);
    }

    /**
     * @brief       Unset an argument parser flag.
     * @param       flg : Flag to unset.
     */
    void unset_flag(detail::arg_parser_flags flg) noexcept
    {
        flgs_.unset(flg);
    }

    /**
     * @brief       Print the usage message.
     */
    void print_usage()
    {
        std::size_t nr_options = get_options_count();
        auto& os = get_ostream();

        os << "Usage: ";

        if (!prog_name_.empty())
        {
            os << prog_name_ << ' ';
        }
        else
        {
            os << "???" << ' ';
        }

        if (nr_options > 0)
        {
            os << "[OPTION]";

            if (nr_options > 1)
            {
                os << "...";
            }
        }

        print_commands_usage();
        print_values_usage();
        print_constraints_usage();

        os << "\n\n";
    }

    /**
     * @brief       Print the commands usage.
     */
    void print_commands_usage()
    {
        key_arg_type* ky_arg;
        auto& os = get_ostream();

        for (auto& bse_arg : bse_arg_list_)
        {
            ky_arg = dynamic_cast<key_arg_type*>(bse_arg.get());
            if (ky_arg != nullptr && !ky_arg->is_option())
            {
                os.put(' ');
                ky_arg->print_usage();
            }
        }
    }

    /**
     * @brief       Pring the values usage.
     */
    void print_values_usage()
    {
        positional_arg_type* positionl_arg;
        auto& os = get_ostream();

        for (auto& bse_arg : bse_arg_list_)
        {
            positionl_arg = dynamic_cast<positional_arg_type*>(bse_arg.get());
            if (positionl_arg != nullptr)
            {
                os.put(' ');
                positionl_arg->print_usage();
            }
        }
    }

    /**
     * @brief       Print the constraints usage.
     */
    void print_constraints_usage()
    {
        // if (constrnts_.empty())
        // {
        //     return;
        // }
        //
        // auto& os = get_ostream();
        // os.put(' ');
        //
        // if (constrnts_.size() > 1)
        // {
        //     os << "{CONSTRAINTS}";
        // }
        // else
        // {
        //     os << "{CONSTRAINT}";
        // }
    }

    /**
     * @brief       Print the help menu or version information if it is necessary.
     */
    void trigger_prints()
    {
        help_arg_type* hlp_arg;

        for (auto& bse_arg : bse_arg_list_)
        {
            hlp_arg = dynamic_cast<help_arg_type*>(bse_arg.get());
            if (hlp_arg != nullptr &&
                hlp_arg->was_found() &&
                hlp_arg->is_flag_set(detail::arg_flags::TRIGGER_HELP_PRINTING))
            {
                hlp_arg->print_help_menu_assigned();
                if (hlp_arg->is_flag_set(detail::arg_flags::PKILL_AFTER_TRIGGERING))
                {
                    exit_program(0);
                }
            }
        }

        if (current_vers_arg_ != nullptr &&
            current_vers_arg_->was_found() &&
            current_vers_arg_->is_flag_set(detail::arg_flags::TRIGGER_VERSION_PRINTING))
        {
            print_version();
            if (current_vers_arg_->is_flag_set(detail::arg_flags::PKILL_AFTER_TRIGGERING))
            {
                exit_program(0);
            }
        }
        
        if (flgs_.is_set(detail::arg_parser_flags::PRINT_ERRORS))
        {
            print_errors();
        }
    }

private:
    /** Map allowing access to a base argument from one of its keys. */
    unordered_map_type<string_view_type, base_arg_type*> bse_arg_map_;

    /** Map allowing access to a help menu from its key. */
    unordered_map_type<string_type, unique_ptr_type<help_menu_type>> hlp_menu_map_;

    /** Contains the short prefixes for arguments. */
    unordered_set_type<string_type> short_prefxs_ = {"-"};

    /** Contains the long prefixes for arguments. */
    unordered_set_type<string_type> long_prefxs_ = {"--"};

    /** Contains the program name. */
    string_type prog_name_;

    /** Error id that will be used for global errors. */
    string_type err_id_ = "error";

    /** Contains all the arguments. */
    vector_type<unique_ptr_type<base_arg_type>> bse_arg_list_;

    /** Contains the unrecognized arguments if an error happen. */
    vector_type<string_type> unrecog_args_;

    /** Collection of arguments constraints. */
    list_type<arg_constraint_type> constrnts_;

    /** Reference to the first added help argument. */
    help_arg_type* default_hlp_arg_ = nullptr;

    /** Contains the current version argument. */
    version_arg_type* current_vers_arg_ = nullptr;
    
    ostream_type* ostrm = &speed::iostream::get_cout<char_type>();
    
    /** Contains the maximum number of unrecognized args to be catched. */
    std::size_t max_unrecog_args_ = 1;
    
    /** Flags that dictates the argument parser behavior. */
    flags_type<detail::arg_parser_flags> flgs_ = detail::arg_parser_flags::DEFAULT_ARG_PARSER_FLAGS;
    
    /** Argument parser error flags. */
    flags_type<detail::arg_parser_error_flags> err_flgs_ = detail::arg_parser_error_flags::NIL;

    /** Indicates whether the parsing has been done. */
    bool parsd_ = false;
    
    friend class detail::basic_arg_key<AllocatorT>;
    friend class detail::basic_arg_value<AllocatorT>;
    friend class detail::basic_base_arg<AllocatorT>;
    friend class detail::basic_key_arg<AllocatorT>;
    friend class detail::basic_value_arg<AllocatorT>;
    friend class detail::basic_key_value_arg<AllocatorT>;
    friend class detail::basic_positional_arg<AllocatorT>;
    friend class detail::basic_help_arg<AllocatorT>;
    friend class detail::basic_version_arg<AllocatorT>;
    friend class detail::basic_arg_constraint<AllocatorT>;
    friend class detail::basic_help_menu<AllocatorT>;
    friend class detail::basic_arg_parser_setter<AllocatorT>;
};

/** Class used to parse arguments. */
using arg_parser = basic_arg_parser<std::allocator<int>>;

}

#endif

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
 *  @file       speed/argparse/basic_arg_parser.hpp
 *  @brief      basic_arg_parser class header.
 *  @author     Killian
 *  @date       2015/12/31 - 04:51
 */

#ifndef SPEED_ARGPARSE_BASIC_ARG_PARSER_HPP
#define SPEED_ARGPARSE_BASIC_ARG_PARSER_HPP

#include <unordered_map>
#include <unordered_set>

#include "../containers.hpp"
#include "../stringutils.hpp"
#include "arg_parser_error_flags.hpp"
#include "arg_parser_flags.hpp"
#include "relational_constraint_types.hpp"
#include "basic_keyless_arg.hpp"
#include "basic_help_arg.hpp"
#include "basic_help_text.hpp"
#include "basic_key_value_arg.hpp"
#include "basic_relational_constraint.hpp"
#include "basic_version_arg.hpp"


namespace speed::argparse {


/**
 * @brief       Class used to parse arguments.
 */
template<typename TpAllocator>
class basic_arg_parser
{
public:
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAllocator::template rebind<T>::other;
    
    /** Class that represents a text in the help information.*/
    using i_help_text_type = i_help_text;
    
    /** Class that represents a text in the help information.*/
    using help_text_type = basic_help_text<TpAllocator>;
    
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
    
    /** Class that represents a relational constraint for a set of arguments. */
    using relational_constraint_type = basic_relational_constraint<TpAllocator>;
    
    /** Class that represents a bit field */
    template<typename T>
    using flags_type = speed::containers::flags<T>;
    
    /** Class that represent sequences of characters. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;
    
    /** Class that represents a sequence container. */
    template<typename T>
    using initializer_list_type = std::initializer_list<T>;
    
    /** Class that represents a vector. */
    template<typename T>
    using vector_type = std::vector<T, allocator_type<T>>;
    
    /** Class that represents associative containers that store elements formed by the combination
     * of a key value and a mapped value. */
    template<typename TpKey_, typename TpValue_>
    using unordered_map_type = std::unordered_map<
            TpKey_, TpValue_, std::hash<TpKey_>,
            std::equal_to<TpKey_>,
            allocator_type<std::pair<const TpKey_, TpValue_>>
    >;
    
    /** Class that represents containers that store unique elements in no particular order, and
     * which allow for fast retrieval of individual elements based on their value. */
    template<typename TpKey_>
    using unordered_set_type = std::unordered_set<
            TpKey_, std::hash<TpKey_>,
            std::equal_to<TpKey_>,
            allocator_type<TpKey_>
    >;
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       prog_name : The program name, used for print help menu.
     * @param       short_prefxs : Collection that contains the arguments short prefixes.
     * @param       long_prefxs : Collection that contains the arguments long prefixes.
     * @param       arg_desc_indentation : Indentation used to print arguments in the help menu.
     * @param       max_desc_line_length : The maximum description length that will be printed in a
     *              single line in the help menu.
     * @param       desc_new_line_indentation : The indentation used when a new line is found in a
     *              description in the help menu.
     * @param       default_hlp_menu_id : The default help menu id used to store descriptions when
     *              any help menu id is specified.
     * @param       err_id : The argument parser error id.
     * @param       max_unrecog_args : The muximum number of unrecognized arguments that will be
     *              catched.
     * @param       flgs : Flags that dictates the argument parser behavior.
     * @throw       speed::argparse::default_help_menu_id_empty_exception : An exception is thrown
     *              if the default help menu id is empty.
     */
    template<
            typename TpString1_ = string_type,
            typename TpString2_ = string_type,
            typename TpString3_ = string_type,
            typename TpStringSet1_ = unordered_set_type<string_type>,
            typename TpStringSet2_ = unordered_set_type<string_type>,
            typename = std::enable_if_t<
                    !std::is_base_of<basic_arg_parser, std::decay_t<TpString1_>>::value
            >
    >
    basic_arg_parser(
            TpString1_&& prog_name = string_type(),
            TpStringSet1_&& short_prefxs = {"-"},
            TpStringSet2_&& long_prefxs = {"--"},
            std::size_t arg_desc_indentation = 2u,
            std::size_t max_desc_line_length = 80u,
            std::size_t desc_new_line_indentation = 2u,
            TpString2_&& default_hlp_menu_id = string_type("--help"),
            TpString3_&& err_id = string_type("error"),
            std::size_t max_unrecog_args = std::numeric_limits<std::size_t>::max(),
            arg_parser_flags flgs = arg_parser_flags::DEFAULT_ARG_PARSER_FLAGS
    )
            : prog_name_(std::forward<TpString1_>(prog_name))
            , short_prefxs_(std::forward<TpStringSet1_>(short_prefxs))
            , long_prefxs_(std::forward<TpStringSet2_>(long_prefxs))
            , arg_desc_indentation_(arg_desc_indentation)
            , max_desc_line_length_(max_desc_line_length)
            , desc_new_line_indentation_(desc_new_line_indentation)
            , i_hlp_text_list_()
            , default_hlp_menu_id_(std::forward<TpString2_>(default_hlp_menu_id))
            , hlp_arg_map_()
            , hlp_menus_()
            , current_vers_arg_(nullptr)
            , kyless_args_list_()
            , bse_arg_map_()
            , err_id_(std::forward<TpString3_>(err_id))
            , unrecog_args_()
            , max_unrecog_args_(max_unrecog_args)
            , relational_constrs_()
            , flgs_(flgs)
            , err_flgs_(arg_parser_error_flags::NIL)
            , help_text_type_alloc_()
            , key_arg_type_alloc_()
            , help_arg_type_alloc_()
            , version_arg_type_alloc_()
            , key_value_arg_type_alloc_()
            , keyless_arg_type_alloc_()
    {
        if (default_hlp_menu_id_.empty())
        {
            throw default_help_menu_id_empty_exception();
        }
    
        hlp_menus_.insert(std::make_pair(default_hlp_menu_id_, vector_type<i_help_text_type*>()));
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
    basic_arg_parser(basic_arg_parser&& rhs) noexcept
            : prog_name_(std::move(rhs.prog_name_))
            , short_prefxs_(std::move(rhs.short_prefxs_))
            , long_prefxs_(std::move(rhs.long_prefxs_))
            , arg_desc_indentation_(std::move(rhs.arg_desc_indentation_))
            , max_desc_line_length_(std::move(rhs.max_desc_line_length_))
            , desc_new_line_indentation_(std::move(rhs.desc_new_line_indentation_))
            , i_hlp_text_list_(std::move(rhs.i_hlp_text_list_))
            , default_hlp_menu_id_(std::move(rhs.default_hlp_menu_id_))
            , hlp_arg_map_(std::move(rhs.hlp_arg_map_))
            , hlp_menus_ (std::move(rhs.hlp_menus_))
            , current_vers_arg_(std::move(rhs.current_vers_arg_))
            , kyless_args_list_(std::move(rhs.kyless_args_list_))
            , bse_arg_map_(std::move(rhs.bse_arg_map_))
            , err_id_(std::move(rhs.err_id_))
            , unrecog_args_(std::move(rhs.unrecog_args_))
            , max_unrecog_args_(std::move(rhs.max_unrecog_args_))
            , relational_constrs_(std::move(relational_constrs_))
            , flgs_(std::move(rhs.flgs_))
            , err_flgs_(std::move(rhs.err_flgs_))
            , help_text_type_alloc_(std::move(rhs.help_text_type_alloc_))
            , key_arg_type_alloc_(std::move(rhs.key_arg_type_alloc_))
            , help_arg_type_alloc_(std::move(rhs.help_arg_type_alloc_))
            , version_arg_type_alloc_(std::move(rhs.version_arg_type_alloc_))
            , key_value_arg_type_alloc_(std::move(rhs.key_value_arg_type_alloc_))
            , keyless_arg_type_alloc_(std::move(rhs.keyless_arg_type_alloc_))
    {
        rhs.arg_desc_indentation_ = 0;
        rhs.current_vers_arg_ = nullptr;
        rhs.max_unrecog_args_ = 0;
    }
    
    /**
     * @brief       Destructor.
     */
    ~basic_arg_parser()
    {
        for (auto& x : i_hlp_text_list_)
        {
            delete_i_help_text(x);
        }
    
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
    basic_arg_parser& operator =(basic_arg_parser&& rhs) noexcept
    {
        if (this != &rhs)
        {
            prog_name_ = std::move(rhs.prog_name_);
            short_prefxs_ = std::move(rhs.short_prefxs_);
            long_prefxs_ = std::move(rhs.long_prefxs_);
            std::swap(arg_desc_indentation_, rhs.arg_desc_indentation_);
            std::swap(max_desc_line_length_, rhs.max_desc_line_length_);
            std::swap(desc_new_line_indentation_, rhs.desc_new_line_indentation_);
            i_hlp_text_list_ = std::move(rhs.i_hlp_text_list_);
            default_hlp_menu_id_ = std::move(rhs.default_hlp_menu_id_);
            hlp_arg_map_ = std::move(rhs.hlp_arg_map_);
            hlp_menus_ = std::move(rhs.hlp_menus_);
            std::swap(current_vers_arg_, rhs.current_vers_arg_);
            kyless_args_list_ = std::move(rhs.kyless_args_list_);
            bse_arg_map_ = std::move(rhs.bse_arg_map_);
            err_id_ = std::move(rhs.err_id_);
            unrecog_args_ = std::move(rhs.unrecog_args_);
            std::swap(max_unrecog_args_, rhs.max_unrecog_args_);
            flgs_ = std::move(rhs.flgs_);
            err_flgs_ = std::move(rhs.err_flgs_);
            help_text_type_alloc_ = std::move(rhs.help_text_type_alloc_);
            key_arg_type_alloc_ = std::move(rhs.key_arg_type_alloc_);
            help_arg_type_alloc_ = std::move(rhs.help_arg_type_alloc_);
            version_arg_type_alloc_ = std::move(rhs.version_arg_type_alloc_);
            key_value_arg_type_alloc_ = std::move(rhs.key_value_arg_type_alloc_);
            keyless_arg_type_alloc_ = std::move(rhs.keyless_arg_type_alloc_);
        }
        
        return *this;
    }
    
    /**
     * @brief       Add the description text of the program.
     * @param       desc : Argument description content.
     * @param       hlp_menus_ids : The help menus ids in which insert the description.
     */
    // TODO[P1](KillianValverde@gmail.com): Implement the description concept.
    // template<typename TpString1_, typename TpStringVector_ = vector_type<string_type>>
    // void add_description(TpString1_&& desc, TpStringVector_&& hlp_menus_ids = {})
    // {
    // }
    
    /**
     * @brief       Add a string that represents a text in the help information.
     * @param       desc : Argument description content.
     * @param       hlp_menus_ids : The help menus ids in which insert the text.
     */
    template<typename TpString1_, typename TpStringVector_ = vector_type<string_type>>
    void add_help_text(TpString1_&& desc, TpStringVector_&& hlp_menus_ids = {})
    {
        help_text_type* help_text = help_text_type_alloc_.allocate(1);
        help_text_type_alloc_.construct(help_text, std::forward<TpString1_>(desc));
        
        i_hlp_text_list_.push_back(help_text);
        add_help_menu_entry(std::forward<TpStringVector_>(hlp_menus_ids), help_text);
    }
    
    /**
     * @brief       Add an argument that has keys.
     * @param       kys : Strings vector that contains the argument keys.
     * @param       desc : Argument description content. If the string is empty the argument will
     *              not be displayed when the help information will be printed.
     * @param       flgs : Flags that dictates the argument behavior.
     * @param       hlp_menus_ids : The help menus ids in which insert the description.
     * @param       err_id : The id used to reference an arguments during the error display. If
     *              the string is empty the argument will display the first key.
     * @throw       speed::argparse::no_key_specified_exception : An exception is thrown if there is
     *              no key specified.
     * @throw       speed::argparse::key_already_exists_exception : An exception is thrown if an
     *              argument with one of the keys already exist.
     */
    template<
            typename TpString1_,
            typename TpString2_ = string_type,
            typename TpStringVector1_ = vector_type<string_type>,
            typename TpStringVector2_ = vector_type<string_type>
    >
    void add_key_arg(
            TpStringVector1_&& kys,
            TpString1_&& desc,
            arg_flags flgs = arg_flags::DEFAULT_ARG_FLAGS,
            TpStringVector2_&& hlp_menus_ids = {},
            TpString2_&& err_id = string_type()
    )
    {
        if (kys.empty())
        {
            throw no_key_specified_exception();
        }
        
        for (auto& x : kys)
        {
            if (arg_key_exists(x))
            {
                throw key_already_exists_exception();
            }
        }
    
        key_arg_type* ky_arg = key_arg_type_alloc_.allocate(1);
        key_arg_type_alloc_.construct(ky_arg,
                                      std::forward<TpString1_>(desc),
                                      std::forward<TpString2_>(err_id),
                                      flgs,
                                      get_arg_key_list_from_strings(kys),
                                      this);
        
        for (auto& x : kys)
        {
            bse_arg_map_.emplace(std::is_rvalue_reference<TpStringVector1_&&>::value ?
                                         std::move(x) : x, static_cast<base_arg_type*>(ky_arg));
        }
        
        i_hlp_text_list_.push_back(ky_arg);
        if (!ky_arg->description_is_empty())
        {
            add_help_menu_entry(std::forward<TpStringVector2_>(hlp_menus_ids), ky_arg);
        }
    }
    
    /**
     * @brief       Add an argument that represents the option to get the help information.
     * @param       kys : Strings vector that contains the argument keys. The first key will be
     *              used to reference the help menu associated.
     * @param       desc : Argument description content. If the string is empty the argument will
     *              not be displayed when the help information will be printed.
     * @param       flgs : Flags that dictates the argument behavior.
     * @param       hlp_menus_ids : The help menus ids in which insert the description.
     * @param       err_id : The id used to reference an arguments during the error display If the
     *              string is empty the argument will display the first key.
     * @throw       speed::argparse::no_key_specified_exception : An exception is thrown if there is
     *              no key specified.
     * @throw       speed::argparse::key_already_exists_exception : An exception is thrown if an
     *              argument with one of the keys already exist.
     * @throw       speed::argparse::help_argument_already_exists_exception : An exception is thrown
     *              if an help argument already exist.
     */
    template<
            typename TpString1_,
            typename TpString2_ = string_type,
            typename TpStringVector1_ = vector_type<string_type>,
            typename TpStringVector2_ = vector_type<string_type>
    >
    void add_help_arg(
            TpStringVector1_&& kys,
            TpString1_&& desc,
            arg_flags flgs = arg_flags::DEFAULT_ARG_FLAGS,
            TpStringVector2_&& hlp_menus_ids = {},
            TpString2_&& err_id = string_type()
    )
    {
        if (kys.empty())
        {
            throw no_key_specified_exception();
        }
        
        for (auto& x : kys)
        {
            if (arg_key_exists(x))
            {
                throw key_already_exists_exception();
            }
        }
        
        if (flgs_.is_set(arg_parser_flags::EXIT_ON_PRINT_HELP))
        {
            flgs |= arg_flags::IS_TERMINAL;
        }
    
        help_arg_type* help_arg = help_arg_type_alloc_.allocate(1);
        help_arg_type_alloc_.construct(help_arg,
                                       std::forward<TpString1_>(desc),
                                       std::forward<TpString2_>(err_id),
                                       flgs,
                                       get_arg_key_list_from_strings(kys),
                                       this);
    
        for (auto& x : kys)
        {
            hlp_arg_map_.insert(std::make_pair(x, help_arg));

            bse_arg_map_.emplace(std::is_rvalue_reference<TpStringVector1_&&>::value ?
                                         std::move(x) : x, static_cast<base_arg_type*>(help_arg));
        }
        
        i_hlp_text_list_.push_back(help_arg);
        if (!help_arg->description_is_empty())
        {
            add_help_menu_entry(std::forward<TpStringVector2_>(hlp_menus_ids), help_arg);
        }
    }
    
    /**
     * @brief       Add an argument that represents the option to get the version information.
     * @param       kys : Strings vector that contains the argument keys.
     * @param       desc : Argument description content. If the string is empty the argument will
     *              not be displayed when the help information will be printed.
     * @param       vers_information : Argument version information content.
     * @param       flgs : Flags that dictates the argument behavior.
     * @param       hlp_menus_ids : The help menus ids in which insert the description.
     * @param       err_id : The id used to reference an arguments during the error display If the
     *              string is empty the argument will display the first key.
     * @throw       speed::argparse::no_key_specified_exception : An exception is thrown if there is
     *              no key specified.
     * @throw       speed::argparse::key_already_exists_exception : An exception is thrown if an
     *              argument with one of the keys already exist.
     * @throw       speed::argparse::version_argument_already_exists_exception : An exception is
     *              thrown if an version argument already exist.
     */
    template<
            typename TpString1_,
            typename TpString2_ = string_type,
            typename TpString3_ = string_type,
            typename TpStringVector1_ = vector_type<string_type>,
            typename TpStringVector2_ = vector_type<string_type>
    >
    void add_version_arg(
            TpStringVector1_&& kys,
            TpString1_&& desc,
            TpString2_&& vers_information,
            arg_flags flgs = arg_flags::DEFAULT_ARG_FLAGS,
            TpStringVector2_&& hlp_menus_ids = {},
            TpString3_&& err_id = string_type()
    )
    {
        if (kys.empty())
        {
            throw no_key_specified_exception();
        }
    
        for (auto& x : kys)
        {
            if (arg_key_exists(x))
            {
                throw key_already_exists_exception();
            }
        }
    
        if (current_vers_arg_ != nullptr)
        {
            throw version_arg_already_exists_exception();
        }
    
        if (flgs_.is_set(arg_parser_flags::EXIT_ON_PRINT_VERSION))
        {
            flgs |= arg_flags::IS_TERMINAL;
        }
    
        version_arg_type* vers_arg = version_arg_type_alloc_.allocate(1);
        version_arg_type_alloc_.construct(vers_arg,
                                          std::forward<TpString1_>(desc),
                                          std::forward<TpString3_>(err_id),
                                          flgs,
                                          get_arg_key_list_from_strings(kys),
                                          std::forward<TpString2_>(vers_information),
                                          this);
    
        for (auto& x : kys)
        {
            bse_arg_map_.emplace(std::is_rvalue_reference<TpStringVector1_&&>::value ?
                                         std::move(x) : x, static_cast<base_arg_type*>(vers_arg));
        }
        
        i_hlp_text_list_.push_back(vers_arg);
        current_vers_arg_ = vers_arg;
    
        if (!vers_arg->description_is_empty())
        {
            add_help_menu_entry(std::forward<TpStringVector2_>(hlp_menus_ids), vers_arg);
        }
    }
    
    /**
     * @brief       Add an argument that represents the option to get the version information
     *              (Programs with GPLv3 licence).
     * @param       kys : Strings vector that contains the argument keys.
     * @param       desc : Argument description content. If the string is empty the argument will
     *              not be displayed when the help information will be printed.
     * @param       prog_version : The version that will be displayed in the vesion information.
     * @param       dte : The program release date that will be displayed in the vesion information.
     * @param       authr : The author name that will be displayed in the vesion information.
     * @param       flgs : Flags that dictates the argument behavior.
     * @param       hlp_menus_ids : The help menus ids in which insert the description.
     * @param       err_id : The id used to reference an arguments during the error display If the
     *              string is empty the argument will display the first key.
     * @throw       speed::argparse::no_key_specified_exception : An exception is thrown if there is
     *              no key specified.
     * @throw       speed::argparse::key_already_exists_exception : An exception is thrown if an
     *              argument with one of the keys already exist.
     * @throw       speed::argparse::version_argument_already_exists_exception : An exception is
     *              thrown if an version argument already exist.
     */
    template<
            typename TpString1_,
            typename TpString2_,
            typename TpString3_,
            typename TpString4_,
            typename TpString5_ = string_type,
            typename TpStringVector1_ = vector_type<string_type>,
            typename TpStringVector2_ = vector_type<string_type>
    >
    void add_gplv3_version_arg(
            TpStringVector1_&& kys,
            TpString1_&& desc,
            const TpString2_& prog_version,
            const TpString3_& dte,
            const TpString4_& authr,
            arg_flags flgs = arg_flags::DEFAULT_ARG_FLAGS,
            TpStringVector2_&& hlp_menus_ids = {},
            TpString5_&& err_id = string_type()
    )
    {
        string_type vers_information;
        vers_information.append(prog_name_);
        vers_information.append(" ");
        vers_information.append(prog_version);
        vers_information.append("\nCopyright (C) ");
        vers_information.append(dte);
        vers_information.append(
                " Free Software Foundation, Inc.\n"
                "License GPLv3+: GNU GPL version 3 or later "
                "<http://gnu.org/licenses/gpl.html>.\n"
                "This is free software: you are free to change and redistribute it.\n"
                "There is NO WARRANTY, to the extent permitted by law.\n\n"
                "Written by ");
        vers_information.append(authr);
        vers_information.append(".");
    
        add_version_arg(std::forward<TpStringVector1_>(kys),
                        std::forward<TpString1_>(desc),
                        std::move(vers_information),
                        flgs,
                        std::forward<TpStringVector2_>(hlp_menus_ids),
                        std::forward<TpString5_>(err_id));
    }
    
    /**
     * @brief       Add an argument that has keys and values.
     * @param       kys : Strings vector that contains the argument keys.
     * @param       desc : Argument description content. If the string is empty the argument will
     *              not be displayed when the help information will be printed.
     * @param       vals_types : Vector that has the values types that must have the argument
     *              values.
     * @param       min_vals : Minimum number of values for an option.
     * @param       max_vals : Maximum number of values for an option.
     * @param       flgs : Flags that dictates the argument behavior.
     * @param       regx_collection : Regular expressions that the values have to match. If the
     *              string is empty all values will match.
     * @param       hlp_menus_ids : The help menus ids in which insert the description.
     * @param       err_id : The id used to reference an arguments during the error display. If the
     *              string is empty the argument will display the first key.
     * @throw       speed::argparse::no_key_specified_exception : An exception is thrown if there is
     *              no key specified.
     * @throw       speed::argparse::key_already_exists_exception : An exception is thrown if an
     *              argument with one of the keys already exist.
     */
    template<
            typename TpString1_,
            typename TpString2_ = string_type,
            typename TpStringVector1_ = vector_type<string_type>,
            typename TpStringVector2_ = vector_type<string_type>,
            typename TpStringVector3_ = vector_type<string_type>,
            typename TpArgValueTypesVector_ = vector_type<arg_value_types>
    >
    void add_key_value_arg(
            TpStringVector1_&& kys,
            TpString1_&& desc,
            TpArgValueTypesVector_&& vals_types = {arg_value_types::STRING},
            std::size_t min_vals = 1,
            std::size_t max_vals = 1,
            arg_flags flgs = arg_flags::DEFAULT_ARG_FLAGS,
            TpStringVector2_&& regx_collection = vector_type<string_type>(),
            TpStringVector3_&& hlp_menus_ids = {},
            TpString2_&& err_id = string_type()
    )
    {
        if (kys.empty())
        {
            throw no_key_specified_exception();
        }
    
        for (auto& x : kys)
        {
            if (arg_key_exists(x))
            {
                throw key_already_exists_exception();
            }
        }
    
        key_value_arg_type* ky_val_arg = key_value_arg_type_alloc_.allocate(1);
        key_value_arg_type_alloc_.construct(ky_val_arg,
                                            std::forward<TpString1_>(desc),
                                            std::forward<TpString2_>(err_id),
                                            flgs,
                                            get_arg_key_list_from_strings(kys),
                                            min_vals,
                                            max_vals,
                                            std::forward<TpArgValueTypesVector_>(vals_types),
                                            std::forward<TpStringVector2_>(regx_collection),
                                            this);
    
        for (auto& x : kys)
        {
            bse_arg_map_.emplace(std::is_rvalue_reference<TpStringVector1_&&>::value ?
                                         std::move(x) : x, static_cast<base_arg_type*>(ky_val_arg));
        }
        
        i_hlp_text_list_.push_back(ky_val_arg);
        if (!ky_val_arg->description_is_empty())
        {
            add_help_menu_entry(std::forward<TpStringVector3_>(hlp_menus_ids), ky_val_arg);
        }
    }
    
    /**
     * @brief       Add an argument that represents an argument without keys.
     * @param       usage_ky : The key used to reference an arguments during the help usage sentence
     *              display.
     * @param       help_ky : The key used to reference an arguments during the help display.
     * @param       desc : Argument description content. If the string is empty the argument
     *              will not be displayed when the help information will be printed.
     * @param       vals_types : Vector that has the values types that must have the argument
     *              values.
     * @param       min_vals : Minimum number of values for an option.
     * @param       max_vals : Maximum number of values for an option.
     * @param       flgs : Flags that dictates the argument behavior.
     * @param       regx_collection : Regular expressions that the values have to match. If the
     *              string is empty all values will match.
     * @param       hlp_menus_ids : The help menus ids in which insert the description.
     * @param       err_id : The id used to reference an arguments during the error display.
     * @throw       speed::argparse::no_key_specified_exception : An exception is thrown if there is
     *              no key specified.
     * @throw       speed::argparse::key_already_exists_exception : An exception is thrown if an
     *              argument with one of the keys already exist.
     */
    template<
            typename TpString1_,
            typename TpString2_,
            typename TpString3_,
            typename TpString4_ = string_type,
            typename TpArgValueTypesVector_ = vector_type<arg_value_types>,
            typename TpStringVector1_ = vector_type<string_type>,
            typename TpStringVector2_ = vector_type<string_type>
    >
    void add_keyless_arg(
            TpString1_&& usage_ky,
            TpString2_&& help_ky,
            TpString3_&& desc,
            TpArgValueTypesVector_&& vals_types = {arg_value_types::STRING},
            std::size_t min_vals = 1,
            std::size_t max_vals = 1,
            arg_flags flgs = arg_flags::DEFAULT_KEYLESS_ARG_FLAGS,
            TpStringVector1_&& regx_collection = vector_type<string_type>(),
            TpStringVector2_&& hlp_menus_ids = {},
            TpString4_&& err_id = string_type()
    )
    {
        if (speed::stringutils::get_string_length(usage_ky) == 0 ||
            speed::stringutils::get_string_length(help_ky) == 0)
        {
            throw no_key_specified_exception();
        }
    
        if (arg_key_exists(usage_ky) || arg_key_exists(help_ky))
        {
            throw key_already_exists_exception();
        }
    
        keyless_arg_type* kyless_arg = keyless_arg_type_alloc_.allocate(1);
        keyless_arg_type_alloc_.construct(kyless_arg,
                                          std::forward<TpString3_>(desc),
                                          std::forward<TpString4_>(err_id),
                                          flgs,
                                          min_vals,
                                          max_vals,
                                          std::forward<TpArgValueTypesVector_>(vals_types),
                                          std::forward<TpStringVector1_>(regx_collection),
                                          usage_ky,
                                          help_ky,
                                          this);
        
        bse_arg_map_.emplace(std::forward<TpString1_>(usage_ky),
                             static_cast<base_arg_type*>(kyless_arg));
        bse_arg_map_.emplace(std::forward<TpString2_>(help_ky),
                             static_cast<base_arg_type*>(kyless_arg));
        
        kyless_args_list_.push_back(kyless_arg);

        i_hlp_text_list_.push_back(kyless_arg);
        if (!kyless_arg->description_is_empty())
        {
            add_help_menu_entry(std::forward<TpStringVector2_>(hlp_menus_ids), kyless_arg);
        }
    }
    
    /**
     * @brief       Add arguments relational constraints to the parser.
     * @param       args : Arguments in which apply the constaints.
     * @param       constrs : Argument relational constraints to apply.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              one of the specified keys an exception is thrown.
     */
    template<typename TpStringVector = vector_type<string_type>>
    void add_args_relational_constraints(
            const TpStringVector& args,
            relational_constraint_types constrs
    )
    {
        vector_type<base_arg_type*> bse_arg_vector;
        base_arg_type* bse_arg;
        
        for (auto& x : args)
        {
            bse_arg = get_base_arg_reference(x);
            
            if (bse_arg == nullptr)
            {
                throw arg_not_found_exception();
            }
    
            bse_arg_vector.push_back(bse_arg);
        }
        
        relational_constrs_.push_back(
                relational_constraint_type(std::move(bse_arg_vector), constrs));
    }
    
    /**
     * @brief       Get a reference to a base argument.
     * @param       ky : Id of the argument to get.
     * @return      If function was successful the argument found is returned.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              the specified key an exception is thrown.
     */
    const base_arg_type& get_base_arg(const string_type& ky) const
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            return *(it->second);
        }
        
        throw arg_not_found_exception();
    }
    
    /**
     * @brief       Get a reference to a key argument.
     * @param       ky : Key of the argument to get.
     * @return      If function was successful the argument found is returned.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              the specified key an exception is thrown.
     * @throw       speed::argparse::bad_argument_type_exception : If the argument can't be cast to
     *              the specified type an exception is thrown.
     */
    const key_arg_type& get_key_arg(const string_type& ky) const
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            key_arg_type* ky_arg = dynamic_cast<key_arg_type*>(it->second);
            if (ky_arg == nullptr)
            {
                throw bad_arg_type_exception();
            }
            
            return *ky_arg;
        }
    
        throw arg_not_found_exception();
    }
    
    /**
     * @brief       Get a reference to a value argument.
     * @param       ky : Key of the argument to get.
     * @return      If function was successful the argument found is returned.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              the specified key an exception is thrown.
     * @throw       speed::argparse::bad_argument_type_exception : If the argument can't be cast to
     *              the specified type an exception is thrown.
     */
    const value_arg_type& get_value_arg(const string_type& ky) const
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            value_arg_type* val_arg = dynamic_cast<value_arg_type*>(it->second);
            if (val_arg == nullptr)
            {
                throw bad_arg_type_exception();
            }
    
            return *val_arg;
        }
    
        throw arg_not_found_exception();
    }
    
    /**
     * @brief       Get a reference to a key value argument.
     * @param       ky : Key of the argument to get.
     * @return      If function was successful the argument found is returned.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              the specified key an exception is thrown.
     * @throw       speed::argparse::bad_argument_type_exception : If the argument can't be cast to
     *              the specified type an exception is thrown.
     */
    const key_value_arg_type& get_key_value_arg(const string_type& ky) const
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            key_value_arg_type* key_value_arg = dynamic_cast<key_value_arg_type*>(it->second);
            if (key_value_arg == nullptr)
            {
                throw bad_arg_type_exception();
            }
    
            return *key_value_arg;
        }
    
        throw arg_not_found_exception();
    }
    
    /**
     * @brief       Get a reference to an keyless argument.
     * @param       ky : Key of the argument to get.
     * @return      If function was successful the argument found is returned.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              the specified key an exception is thrown.
     * @throw       speed::argparse::bad_argument_type_exception : If the argument can't be cast to
     *              the specified type an exception is thrown.
     */
    const keyless_arg_type& get_keyless_arg(const string_type& ky) const
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            keyless_arg_type* foreign_arg = dynamic_cast<keyless_arg_type*>(it->second);
            if (foreign_arg == nullptr)
            {
                throw bad_arg_type_exception();
            }
    
            return *foreign_arg;
        }
    
        throw arg_not_found_exception();
    }
    
    /**
     * @brief       Parse all the arguments added to the parser. It's used to associate arguments
     *              with the values from the program call.
     * @param       argc : Contains the number of arguments gotten through the program call.
     * @param       argv : Contains the arguments gotten through the program call.
     * @throw       speed::argparse::help_menu_not_found_exception : If there is a help menu to
     *              print and the help menu entry does not exists an exception is thrown.
     */
    template<typename TpStringArray_>
    void parse_args(std::size_t argc, const TpStringArray_& argv)
    {
        string_type cur_argv;
        base_arg_type *bse_arg = nullptr;
        key_arg_type *ky_arg = nullptr;
        help_arg_type *hlp_arg = nullptr;
        key_value_arg_type *ky_val_arg = nullptr;
        keyless_arg_type *kyless_arg = nullptr;
        keyless_arg_type *old_forgn_arg = nullptr;
        std::size_t pos_increment;
        vector_type<key_arg_type*> chaind_args;
        string_type eq_val;
        
        reset_args_parse();
        
        if (argc > 0 && prog_name_.empty())
        {
            prog_name_ = argv[0];
        }
        
        for (std::size_t i = 1; i < argc; ++i)
        {
            cur_argv = argv[i];
            
            // Normal arguments
            if ((ky_arg = get_key_arg_reference(cur_argv)) != nullptr)
            {
                ky_val_arg = dynamic_cast<key_value_arg_type*>(ky_arg);
                if (ky_val_arg != nullptr)
                {
                    ky_val_arg->parse(argc, argv, i, &pos_increment);
                    i += pos_increment;
                }
                else
                {
                    ky_arg->parse();
                }
            }
            // Chained arguments
            else if (try_get_chained_args(cur_argv, &chaind_args))
            {
                for (auto& x : chaind_args)
                {
                    ky_val_arg = dynamic_cast<key_value_arg_type*>(x);
                    if (ky_val_arg != nullptr)
                    {
                        ky_val_arg->parse(argc, argv, i, &pos_increment);
                        i += pos_increment;
                    }
                    else
                    {
                        x->parse();
                    }
                }
            }
            // EQ arguments
            else if (try_get_key_value_arg_splitted_by_eq_operator(cur_argv, &ky_val_arg, &eq_val))
            {
                ky_val_arg->parse_eq_operator(std::move(eq_val), argc, argv, i, &pos_increment);
                i += pos_increment;
            }
            // keyless arguments
            else if (try_add_value_to_keyless_arg(cur_argv, &kyless_arg))
            {
                if (old_forgn_arg != kyless_arg)
                {
                    kyless_arg->set_found(true);
                }
                
                old_forgn_arg = kyless_arg;
            }
            // Unrecognized arguments
            else if (unrecog_args_.size() < max_unrecog_args_)
            {
                unrecog_args_.push_back(std::move(cur_argv));
            }
        }
        
        // Set all arguments as parsed.
        for (auto& x : i_hlp_text_list_)
        {
            if ((bse_arg = dynamic_cast<base_arg_type*>(x)) != nullptr)
            {
                bse_arg->set_parsed(true);
            }
        }
        
        // Set argument parser flags if there are errors.
        update_error_flags();
        
        // Print argument parser errors if necessary
        if (flgs_.is_set(arg_parser_flags::PRINT_ERRORS) && !err_flgs_.is_empty())
        {
            print_errors();
        }
    }
    
    /**
     * @brief       Reset the arguments parse.
     */
    void reset_args_parse() noexcept
    {
        value_arg_type* value_arg;
        
        for (auto& x : bse_arg_map_)
        {
            x.second->set_parsed(false);
            x.second->set_found(false);
            
            if ((value_arg = dynamic_cast<value_arg_type*>(x.second)) != nullptr)
            {
                value_arg->clear_values();
            }
        }
        
        unrecog_args_.clear();
    
        update_error_flags();
    }
    
    /**
     * @brief       Allows knowing whether an argument has been found in the program call.
     * @param       ky : Argument key to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool arg_found(const string_type& ky) const noexcept
    {
        base_arg_type* bse_arg = get_base_arg_reference(ky);
        return bse_arg != nullptr && bse_arg->was_found();
    }
    
    /**
     * @brief       Allows knowing whether the argument that represents the option to get the help
     *              information has been found in the program call.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool help_arg_found() const noexcept
    {
        for (auto& x : hlp_arg_map_)
        {
            if (x.second->was_found())
            {
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * @brief       Allows knowing whether the argument that represents the option to get the
     *              version information has been found in the program call.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool version_arg_found() const noexcept
    {
        return (current_vers_arg_ != nullptr && current_vers_arg_->was_found());
    }
    
    /**
     * @brief       Get the first argument value with the specified key.
     * @param       ky : Key of the argument to get the value.
     * @return      The argument first value found.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              the specified key an exception is thrown.
     * @throw       speed::argparse::value_not_found_exception : If argument hasn't any value an
     *              exception is thrown.
     */
    const arg_value_type& get_front_arg_value(const string_type& ky) const
    {
        value_arg_type* val_arg = get_value_arg_reference(ky);
        
        if (val_arg == nullptr)
        {
            throw arg_not_found_exception();
        }
        
        return val_arg->get_front_value();
    }
    
    /**
     * @brief       Get the first argument value with the specified key.
     * @param       ky : Key of the argument to get the value.
     * @param       default_val : The value used to construct an argument value if there isn't any
     *              value.
     * @return      If function was successful the first argument value is returned, otherwise an
     *              argument value constructed with the default value is returned.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              the specified key an exception is thrown.
     */
    template<typename TpString_>
    const arg_value_type& get_front_arg_value(
            const string_type& ky,
            TpString_&& default_val
    ) const
    {
        value_arg_type* val_arg = get_value_arg_reference(ky);
        
        if (val_arg == nullptr)
        {
            throw arg_not_found_exception();
        }
        
        return val_arg->get_front_value(std::forward<TpString_>(default_val));
    }
    
    /**
     * @brief       Get the first argument value converted to target with the specified key.
     * @param       ky : Key of the argument to get the value.
     * @return      The argument first value found.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              the specified key an exception is thrown.
     * @throw       speed::argparse::value_not_found_exception : If argument hasn't any value an
     *              exception is thrown.
     * @throw       speed::type_casting::type_casting_exception : If no conversion could be
     *              performed an exception is thrown.
     */
    template<typename TpTarget_>
    TpTarget_ get_front_arg_value_as(const string_type& ky) const
    {
        value_arg_type* val_arg = get_value_arg_reference(ky);
        
        if (val_arg == nullptr)
        {
            throw arg_not_found_exception();
        }
        
        return val_arg->get_front_value().template as<TpTarget_>();
    }
    
    /**
     * @brief       Get the first argument value converted to target with the specified key.
     * @param       ky : Key of the argument to get the value.
     * @param       default_val : The value returned if the operatin fails.
     * @return      If function was successful the first argument value is returned, otherwise an
     *              argument value constructed with the default value is returned.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              the specified key an exception is thrown.
     */
    template<typename TpTarget_, typename TpDefaultValue_>
    TpTarget_ get_front_arg_value_as(
            const string_type& ky,
            TpDefaultValue_&& default_val
    ) const
    {
        value_arg_type* val_arg = get_value_arg_reference(ky);
        
        if (val_arg == nullptr)
        {
            throw arg_not_found_exception();
        }
        
        if (val_arg->there_are_values())
        {
            return val_arg->get_front_value().template as<TpTarget_>(
                    std::forward<TpDefaultValue_>(default_val));
        }
        
        return default_val;
    }
    
    /**
     * @brief       Get the 'n' argument value with the specified key.
     * @param       ky : Key of the argument to get the value.
     * @param       indx : Position of the element.
     * @return      The element specified.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              the specified key an exception is thrown.
     * @throw       speed::argparse::value_not_found_exception : If there isn't any element in the
     *              specified position an exception is thrown.
     */
    const arg_value_type& get_arg_value_at(
            const string_type& ky,
            std::size_t indx
    ) const
    {
        value_arg_type* val_arg = get_value_arg_reference(ky);
        
        if (val_arg == nullptr)
        {
            throw arg_not_found_exception();
        }
        
        return val_arg->get_value_at(indx);
    }
    
    /**
     * @brief       Get the 'n' argument value with the specified key.
     * @param       ky : Key of the argument to get the value.
     * @param       indx : Position of the element.
     * @param       default_val : The value used to construct an argument value if there isn't any
     *              value.
     * @return      If function was successful the argument value in the specified position is
     *              returned, otherwise an argument value constructed with the default value is
     *              returned.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              the specified key an exception is thrown.
     */
    template<typename TpString_>
    const arg_value_type& get_arg_value_at(
            const string_type& ky,
            std::size_t indx,
            TpString_&& default_val
    ) const
    {
        value_arg_type* val_arg = get_value_arg_reference(ky);
        
        if (val_arg == nullptr)
        {
            throw arg_not_found_exception();
        }
        
        return val_arg->get_value_at(indx, std::forward<TpString_>(default_val));
    }
    
    /**
     * @brief       Get the 'n' argument value with the specified key converted to target.
     * @param       ky : Key of the argument to get the value.
     * @param       indx : Position of the element.
     * @return      The element specified.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              the specified key an exception is thrown.
     * @throw       speed::argparse::value_not_found_exception : If there isn't any element in the
     *              specified position an exception is thrown.
     */
    template<typename TpTarger_>
    TpTarger_ get_arg_value_at_as(
            const string_type& ky,
            std::size_t indx
    ) const
    {
        value_arg_type* val_arg = get_value_arg_reference(ky);
        
        if (val_arg == nullptr)
        {
            throw arg_not_found_exception();
        }
        
        return val_arg->get_value_at(indx).template as<TpTarger_>();
    }
    
    /**
     * @brief       Get the 'n' argument value with the specified key converted to target.
     * @param       ky : Key of the argument to get the value.
     * @param       indx : Position of the element.
     * @param       default_val : The value used to construct an argument value if there isn't any
     *              value.
     * @return      If function was successful the argument value in the specified position is
     *              returned, otherwise an argument value constructed with the default value is
     *              returned.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              the specified key an exception is thrown.
     */
    template<typename TpTarget_, typename TpDefaultValue_>
    TpTarget_ get_arg_value_at_as(
            const string_type& ky,
            std::size_t indx,
            TpDefaultValue_&& default_val
    ) const
    {
        value_arg_type* val_arg = get_value_arg_reference(ky);
        
        if (val_arg == nullptr)
        {
            throw arg_not_found_exception();
        }
    
        if (val_arg->there_are_n_values(indx + 1))
        {
            return val_arg->get_value_at(indx).template as<TpTarget_>(
                    std::forward<TpDefaultValue_>(default_val));
        }
    
        return default_val;
    }
    
    /**
     * @brief       Get all the argument values with the specified key.
     * @param       ky : Key of the argument to get the values.
     * @return      All arguments values found.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              the specified key an exception is thrown.
     */
    const vector_type<arg_value_type>& get_arg_values(const string_type& ky) const
    {
        value_arg_type* val_arg = get_value_arg_reference(ky);
        
        if (val_arg == nullptr)
        {
            throw arg_not_found_exception();
        }
        
        return val_arg->get_values();
    }
    
    /**
     * @brief       Get all the argument values with the specified key.
     * @param       ky : Key of the argument to get the values.
     * @return      All arguments values found.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              the specified key an exception is thrown.
     * @throw       speed::type_casting::type_casting_exception : If no conversion could be
     *              performed an exception is thrown.
     */
    template<typename TpTarget_>
    vector_type<TpTarget_> get_arg_values_as(const string_type& ky) const
    {
        value_arg_type* val_arg = get_value_arg_reference(ky);
        
        if (val_arg == nullptr)
        {
            throw arg_not_found_exception();
        }
    
        vector_type<TpTarget_> res;
        for (auto& x : val_arg->get_values())
        {
            res.push_back(x.template as<TpTarget_>());
        }
        
        return res;
    }
    
    /**
     * @brief       Get all the argument values with the specified key.
     * @param       ky : Key of the argument to get the values.
     * @param       default_val : The value returned if the operation fails.
     * @return      All arguments values found.
     * @throw       speed::argparse::argument_not_found_exception : If there isn't any argument with
     *              the specified key an exception is thrown.
     */
    template<typename TpTarget_, typename TpDefaultValue_ = vector_type<TpTarget_>>
    vector_type<TpTarget_> get_arg_values_as(
            const string_type& ky,
            TpDefaultValue_&& default_val
    ) const
    {
        value_arg_type* val_arg = get_value_arg_reference(ky);
        
        if (val_arg == nullptr)
        {
            throw arg_not_found_exception();
        }
    
        if (!val_arg->there_are_values())
        {
            return std::forward<TpDefaultValue_>(default_val);
        }
        
        vector_type<TpTarget_> res;
        TpTarget_ elem;
        for (auto& x : val_arg->get_values())
        {
            if (!x.template try_as<TpTarget_>(&elem))
            {
                return std::forward<TpDefaultValue_>(default_val);
            }
            
            res.push_back(std::move(elem));
        }
        
        return res;
    }
    
    /**
     * @brief       Allows knowing whether a flag is set in the argument parser.
     * @param       flg : Flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool flag_is_set(arg_parser_flags flg) const noexcept
    {
        return flgs_.is_set(flg);
    }
    
    /**
     * @brief       Allows knowing whether a error flag is set in the argument parser.
     * @param       flg : Flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool error_flag_is_set(arg_parser_error_flags flg) const noexcept
    {
        return err_flgs_.is_set(flg);
    }
    
    /**
     * @brief       Allows knowing whether there are flags errors in the argument parser.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool there_are_errors() const noexcept
    {
        return !err_flgs_.is_empty();
    }
    
    /**
     * @brief       Print the help information in standard output.
     * @param       hlp_menu_id : The id of the help menu to print.
     * @throw       speed::argparse::help_menu_not_found_exception : If there isn't any help menu
     *              with the specified id an exception is thrown.
     */
    void print_help(const string_type& hlp_menu_id = string_type()) const
    {
        typename unordered_map_type<string_type, vector_type<i_help_text_type*>>::const_iterator
                help_menus_it;
        const vector_type<i_help_text_type*> *hlp_menu_entries;
        base_arg_type* bse_arg;
        key_arg_type* ky_arg;
        std::size_t n_ky_args = 0;
        bool allways_requird_args = false;
        std::size_t shrt_id_length = 0;
        std::size_t lng_id_length = 0;
        std::size_t curr_id_length;
        bool first_bse_arg_found = true;
        
        // Get a reference to the help menu to print.
        if (hlp_menu_id.empty())
        {
            help_menus_it = hlp_menus_.find(default_hlp_menu_id_);
        }
        else
        {
            help_menus_it = hlp_menus_.find(hlp_menu_id);
        }
        
        if (help_menus_it != hlp_menus_.end())
        {
            hlp_menu_entries = &(help_menus_it->second);
        }
        else
        {
            throw help_menu_not_found_exception();
        }
        
        // Compute the length of the arguments keys in the stdout.
        if (flgs_.is_set(arg_parser_flags::PRINT_ARGS_ID_WHEN_PRINT_HELP))
        {
            for (auto& x : *hlp_menu_entries)
            {
                bse_arg = dynamic_cast<base_arg_type*>(x);
                if (bse_arg != nullptr)
                {
                    curr_id_length = bse_arg->get_short_ids_length();
                    if (shrt_id_length < curr_id_length)
                    {
                        shrt_id_length = curr_id_length;
                    }
    
                    curr_id_length = bse_arg->get_long_ids_length();
                    if (lng_id_length < curr_id_length)
                    {
                        lng_id_length = curr_id_length;
                    }
                }
            }
        }
        
        // Display the usage sentence if it is required by the user.
        if (flgs_.is_set(arg_parser_flags::PRINT_USAGE_WHEN_PRINT_HELP))
        {
            // Get the number of no terminal key arguments.
            for (auto& x : i_hlp_text_list_)
            {
                ky_arg = dynamic_cast<key_arg_type*>(x);
                
                if (ky_arg != nullptr &&
                    !ky_arg->flag_is_set(arg_flags::IS_TERMINAL))
                {
                    ++n_ky_args;
    
                    if (n_ky_args > 1)
                    {
                        break;
                    }
                }
            }
            
            // If there are not key arguments but there are terminal key arguments...
            if (n_ky_args == 0 && there_are_terminal_key_args())
            {
                n_ky_args = 1;
            }
    
            // Check if there are always required arguments.
            for (auto& x : bse_arg_map_)
            {
                ky_arg = dynamic_cast<key_arg_type*>(x.second);
        
                if (ky_arg != nullptr && ky_arg->flag_is_set(arg_flags::ALLWAYS_REQUIRED))
                {
                    allways_requird_args = true;
                    break;
                }
            }
            
            // Check if the relational constraints imply that a key argument is always required.
            if (!allways_requird_args)
            {
                for (auto& x : relational_constrs_)
                {
                    if (x.key_arg_always_required())
                    {
                        allways_requird_args = true;
                        break;
                    }
                }
            }
            
            // Print usage sentence.
            std::cout << "Usage: " << prog_name_;
            
            if (n_ky_args > 0)
            {
                if (allways_requird_args)
                {
                    std::cout << " OPTION";
                }
                else
                {
                    std::cout << " [OPTION]";
                }
                
                if (n_ky_args > 1)
                {
                    std::cout << "...";
                }
            }
    
            for (auto& x : kyless_args_list_)
            {
                x->print_usage_id();
            }
    
            std::cout << "\n\n";
        }
        
        // Print all arguments help text.
        for (auto& x : *hlp_menu_entries)
        {
            if (dynamic_cast<help_text_type*>(x) != nullptr ||
                !flgs_.is_set(arg_parser_flags::PRINT_ARGS_ID_WHEN_PRINT_HELP))
            {
                x->print_help_text(max_desc_line_length_, desc_new_line_indentation_, 0);
            }
            else if ((bse_arg = dynamic_cast<base_arg_type*>(x)) != nullptr)
            {
                if (first_bse_arg_found)
                {
                    if (flgs_.is_set(arg_parser_flags::PRINT_OPTIONS_SENTENCE_WHEN_PRINT_HELP))
                    {
                        help_text_type ht("Options:");
                        ht.print_help_text(max_desc_line_length_, desc_new_line_indentation_, 0);
                    }
                    
                    first_bse_arg_found = false;
                }
                
                bse_arg->print_help_text(arg_desc_indentation_,
                                         max_desc_line_length_,
                                         desc_new_line_indentation_,
                                         shrt_id_length,
                                         lng_id_length);
            }
        }
        
        if (flgs_.is_set(arg_parser_flags::EXIT_ON_PRINT_HELP))
        {
            exit_program(0);
        }
    }
    
    /**
     * @brief       Print the version information in standard output.
     */
    void print_version() const
    {
        if (current_vers_arg_ != nullptr)
        {
            current_vers_arg_->print_version_information();
            std::cout << std::endl;
        }
        
        if (flgs_.is_set(arg_parser_flags::EXIT_ON_PRINT_VERSION))
        {
            exit_program(0);
        }
    }
    
    /**
     * @brief       Print the argument parser errors in standard output.
     */
    void print_errors() const
    {
        if (!err_flgs_.is_empty())
        {
            base_arg_type* base_arg;
            
            if (err_flgs_.is_set(arg_parser_error_flags::ARGS_ERROR))
            {
                for (auto& x : i_hlp_text_list_)
                {
                    base_arg = dynamic_cast<base_arg_type*>(x);
                    if (base_arg != nullptr && base_arg->there_are_errors())
                    {
                        base_arg->print_errors(prog_name_, flgs_.is_set(
                                arg_parser_flags::USE_COLORS_WHEN_PRINT_ERRORS));
                    }
                }
            }
            
            if (err_flgs_.is_set(
                    arg_parser_error_flags::ARGS_RELATIONAL_CONSTRAINTS_ERROR))
            {
                for (auto& x : relational_constrs_)
                {
                    x.print_errors(prog_name_, flgs_.is_set(
                            arg_parser_flags::USE_COLORS_WHEN_PRINT_ERRORS));
                }
            }
            
            if (err_flgs_.is_set(arg_parser_error_flags::UNRECOGNIZED_ARGS_ERROR))
            {
                for (auto& x : unrecog_args_)
                {
                    std::cout << prog_name_ << ": ";
                    if (!err_id_.empty())
                    {
                        if (flgs_.is_set(arg_parser_flags::USE_COLORS_WHEN_PRINT_ERRORS))
                        {
                            speed::system::terminal::set_text_attribute(
                                    std::cout, speed::system::terminal::text_attribute::LIGHT_RED);
                            std::cout << err_id_ << ": ";
                            speed::system::terminal::set_text_attribute(
                                    std::cout, speed::system::terminal::text_attribute::DEFAULT);
                        }
                        else
                        {
                            std::cout << err_id_ << ": ";
                        }
                    }
                    std::cout << "Unrecognized option '" << x << "'\n";
                }
            }
            
            if (flgs_.is_set(arg_parser_flags::PRINT_TRY_HELP_WHEN_PRINT_ERRORS))
            {
                std::cout << "Try '" << prog_name_ << " " << default_hlp_menu_id_
                          << "' for more information.\n";
            }
            
            if (flgs_.is_set(arg_parser_flags::PRINT_HELP_WHEN_PRINT_ERRORS) &&
                    err_flgs_.is_set(arg_parser_error_flags::ARGS_ERROR))
            {
                print_help(default_hlp_menu_id_);
            }
            
            if (flgs_.is_set(arg_parser_flags::EXIT_ON_PRINT_ARGS_ERRORS))
            {
                exit_program(-1);
            }
        }
    }
    
    /**
     * @brief       Allows throwing an error to the user using the arguent parser and its syntax.
     *              This method has been made for particular occasions in which an argument error is
     *              not compatible with the argument parser.
     * @param       err_message : The message to print in the standard output.
     * @param       arg_ky : The key of the argument to associate with the
     *              error.
     */
    template<typename TpString_>
    void print_error_message(
            const TpString_& err_message,
            const string_type& arg_ky = string_type()
    ) const
    {
        base_arg_type* bse_arg = get_base_arg_reference(arg_ky);
        
        if (bse_arg != nullptr)
        {
            bse_arg->print_error_message(err_message, prog_name_, flgs_.is_set(
                    arg_parser_flags::USE_COLORS_WHEN_PRINT_ERRORS));
        }
        else
        {
            std::cout << prog_name_ << ": " << err_message << '\n';
        }
        
        if (flgs_.is_set(arg_parser_flags::PRINT_TRY_HELP_WHEN_PRINT_ERRORS))
        {
            std::cout << "Try '" << prog_name_ << " " << default_hlp_menu_id_
                      << "' for more information." << std::endl;
        }
        
        if (flgs_.is_set(arg_parser_flags::PRINT_HELP_WHEN_PRINT_ERRORS) &&
                err_flgs_.is_set(arg_parser_error_flags::ARGS_ERROR))
        {
            print_help(default_hlp_menu_id_);
        }
        
        if (flgs_.is_set(arg_parser_flags::EXIT_ON_PRINT_ARGS_ERRORS))
        {
            exit_program(-1);
        }
    }

private:
    /**
     * @brief       Get a reference to a base argument.
     * @param       ky : Id of the argument to get.
     * @return      If function was successful a reference to the argument found is returned,
     *              otherwise a null pointer is returned.
     */
    base_arg_type* get_base_arg_reference(const string_type& ky) const noexcept
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
    key_arg_type* get_key_arg_reference(const string_type& ky) const noexcept
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
    value_arg_type* get_value_arg_reference(const string_type& ky) const noexcept
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
    key_value_arg_type* get_key_value_arg_reference(const string_type& ky) const noexcept
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            return dynamic_cast<key_value_arg_type*>(it->second);
        }
        
        return nullptr;
    }
    
    /**
     * @brief       Get a reference to an keyless argument.
     * @param       ky : Key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned, if not
     *              a null pointer is returned.
     */
    keyless_arg_type* get_keyless_arg_reference(const string_type& ky) const noexcept
    {
        auto it = bse_arg_map_.find(ky);
        
        if (it != bse_arg_map_.end())
        {
            return dynamic_cast<keyless_arg_type*>(it->second);
        }
        
        return nullptr;
    }
    
    /**
     * @brief       Allows knowing whether a key prefix is long.
     * @param       ky : String that contains the prefix
     * @return      If function was successful true is returned, if not false is returned.
     */
    bool is_key_prefix_long(const string_type& ky) const
    {
        string_type prefx_builder;
        string_type short_prefx;
        string_type long_prefx;
        
        for (auto& x : ky)
        {
            prefx_builder += x;
            
            if (short_prefxs_.count(prefx_builder) > 0)
            {
                short_prefx = prefx_builder;
            }
            
            if (long_prefxs_.count(prefx_builder) > 0)
            {
                long_prefx = prefx_builder;
            }
        }
        
        if (long_prefx.length() > short_prefx.length())
        {
            return true;
        }
        
        return false;
    }
    
    /**
     * @brief       Get an arg keys list from a strings list.
     * @param       kys : The list of string keys.
     * @return      A list of arg keys.
     */
    vector_type<arg_key_type> get_arg_key_list_from_strings(const vector_type<string_type>& kys)
    {
        vector_type<arg_key_type> arg_ky_list;
        
        for (auto& ky : kys)
        {
            arg_ky_list.push_back(arg_key_type(ky, is_key_prefix_long(ky)));
        }
        
        return arg_ky_list;
    }
    
    /**
     * @brief       Add a help menu entry, if no menu exists it will be created.
     * @param       hlp_menus_ids : The help menus in which add the entry.
     * @param       ihlp_text : The entry to add in the help menu.
     */
    template<typename TpStringVector_ = vector_type<string_type>>
    void add_help_menu_entry(TpStringVector_&& hlp_menus_ids, i_help_text_type *ihlp_text)
    {
        if (hlp_menus_ids.empty())
        {
            hlp_menus_[default_hlp_menu_id_].push_back(ihlp_text);
        }
        else
        {
            for (auto& hlp_menu_id : hlp_menus_ids)
            {
                auto it = hlp_menus_.find(hlp_menu_id);
                
                if (it != hlp_menus_.end())
                {
                    it->second.push_back(ihlp_text);
                }
                else
                {
                    std::pair<decltype(it), bool> res;
                    if (std::is_rvalue_reference<TpStringVector_&&>::value)
                    {
                        res = hlp_menus_.insert(std::make_pair(std::move(hlp_menu_id),
                                                               vector_type<i_help_text_type*>()));
                    }
                    else
                    {
                        res = hlp_menus_.insert(std::make_pair(hlp_menu_id,
                                                               vector_type<i_help_text_type*>()));
                    }
                    
                    if (res.second)
                    {
                        res.first->second.push_back(ihlp_text);
                    }
                }
            }
        }
        
        if (std::is_rvalue_reference<TpStringVector_&&>::value)
        {
            hlp_menus_ids.clear();
        }
    }
    
    /**
     * @brief       Allows knowing whether an argument key has been added to the parser.
     * @param       ky : Argument key to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool arg_key_exists(const string_type& ky) const noexcept
    {
        return bse_arg_map_.count(ky) > 0;
    }
    
    /**
     * @brief       Get the prefix of the specified key.
     * @param       ky : String that contains the prefix.
     * @return      If function was successful the prefix is returned, if not an empty string is
     *              returned.
     */
    string_type get_key_prefix(const string_type& ky) const
    {
        string_type prefx_builder;
        string_type short_prefx;
        string_type long_prefx;
        
        for (auto& x : ky)
        {
            prefx_builder += x;
            
            if (short_prefxs_.count(prefx_builder) > 0)
            {
                short_prefx = prefx_builder;
            }
            
            if (long_prefxs_.count(prefx_builder) > 0)
            {
                long_prefx = prefx_builder;
            }
        }
        
        return (long_prefx.length() >= short_prefx.length()) ? long_prefx : short_prefx;
    }
    
    /**
     * @brief       Allows knowing whether a string are composed by chained arguments.
     * @param       str : The string to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool chained_args_exists(const string_type& str) const
    {
        string_type ky_prefix = get_key_prefix(str);
        string_type ky_builder;
        key_arg_type* ky_arg;
        bool succss = false;
        
        for (auto i = ky_prefix.size(); i < str.size(); ++i)
        {
            ky_builder = ky_prefix;
            ky_builder += str[i];
            
            ky_arg = get_key_arg_reference(ky_builder);
            if (ky_arg == nullptr || !ky_arg->flag_is_set(arg_flags::ALLOW_CHAIN))
            {
                return false;
            }
            succss = true;
        }
        
        return succss;
    }
    
    /**
     * @brief       Allows knowing whether a string contains a key, an eq operator and an associated
     *              value.
     * @param       str : The string to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool arg_has_eq_operator(const string_type& str) const
    {
        key_arg_type* ky_arg;
        std::size_t eq_pos;
        
        if (str.size() > 2)
        {
            eq_pos = str.find('=', 1);
            if (eq_pos != string_type::npos)
            {
                ky_arg = get_key_arg_reference(str.substr(0, eq_pos));
                return ky_arg != nullptr && ky_arg->flag_is_set(arg_flags::ALLOW_EQ_OPERATOR);
            }
        }
        
        return false;
    }
    
    /**
     * @brief       Allows knowing whether a string can be interpreted as an argument value.
     * @param       str : The string to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool string_can_be_value(const string_type& str) const
    {
        return !arg_key_exists(str) && !chained_args_exists(str) && !arg_has_eq_operator(str);
    }
    
    /**
     * @brief       Get an argument collection composed by the chained arguments specified in a
     *              string.
     * @param       str : String that contains the chained arguments.
     * @param       chaind_args : The collection in which the chained arguments will be set.
     * @return      If function was successful a collection with the references of the chained
     *              arguments is returned, if not an empty collection is returned.
     */
    bool try_get_chained_args(
            const string_type& str,
            vector_type<key_arg_type*> *chaind_args
    ) const
    {
        string_type ky_prefix = get_key_prefix(str);
        string_type ky_builder;
        key_arg_type* ky_arg;
        chaind_args->clear();
        
        for (auto i = ky_prefix.size(); i < str.size(); i++)
        {
            ky_builder = ky_prefix;
            ky_builder += str[i];
            
            ky_arg = get_key_arg_reference(ky_builder);
            
            if (ky_arg != nullptr && ky_arg->flag_is_set(arg_flags::ALLOW_CHAIN))
            {
                chaind_args->push_back(ky_arg);
            }
            else
            {
                return false;
            }
        }
        
        return !chaind_args->empty();
    }
    
    /**
     * @brief       Try to get a key value argument splitted by the eq operator.
     * @param       cur_argv : The argument to be splitted.
     * @param       ky_val_arg : Holds the address of the key value argument specified in the
     *              expression.
     * @param       val : The value associated to the key.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool try_get_key_value_arg_splitted_by_eq_operator(
            const string_type& cur_argv,
            key_value_arg_type** ky_val_arg,
            string_type* val
    ) const
    {
        std::size_t eq_pos;
        string_type ky;
        
        if (cur_argv.size() > 2)
        {
            eq_pos = cur_argv.find('=', 1);
            if (eq_pos != string_type::npos)
            {
                ky = cur_argv.substr(0, eq_pos);
                *val = cur_argv.substr(eq_pos + 1);
                if (!ky.empty() && !val->empty())
                {
                    *ky_val_arg = get_key_value_arg_reference(ky);
                    
                    return *ky_val_arg != nullptr &&
                           (*ky_val_arg)->flag_is_set(arg_flags::ALLOW_EQ_OPERATOR);
                }
            }
        }
        
        return false;
    }
    
    /**
     * @brief       Try to add a value to the next keyless argument that accept this value.
     * @param       cur_argv : The keyless argument value.
     * @param       kyless_arg : The pointer to get the argument.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TpString_>
    bool try_add_value_to_keyless_arg(
            TpString_&& cur_argv,
            keyless_arg_type** kyless_arg
    ) const
    {
        *kyless_arg = nullptr;
        
        if (!string_can_be_value(cur_argv) ||
            (flgs_.is_set(arg_parser_flags::KEYLESS_ARG_VALUE_CANNOT_BE_ADDED_WITH_PREFIX) &&
             !get_key_prefix(cur_argv).empty()))
        {
            return false;
        }
        
        for (auto& x : kyless_args_list_)
        {
            if (x->try_add_value(cur_argv))
            {
                *kyless_arg = x;
                return true;
            }
        }
        
        for (auto& x : kyless_args_list_)
        {
            if (!x->max_values_number_reached())
            {
                x->add_value(std::forward<TpString_>(cur_argv));
                *kyless_arg = x;
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * @brief       Set error flags if it's required.
     */
    void update_error_flags() noexcept
    {
        base_arg_type* bse_arg;
        bool in = false;
        
        for (auto& x : i_hlp_text_list_)
        {
            if ((bse_arg = dynamic_cast<base_arg_type*>(x)) != nullptr)
            {
                bse_arg->update_error_flags();
                if (bse_arg->there_are_errors() && !in)
                {
                    in = true;
                    err_flgs_.set(arg_parser_error_flags::ARGS_ERROR);
                }
            }
        }
        if (!in)
        {
            err_flgs_.erase(arg_parser_error_flags::ARGS_ERROR);
        }
        
        if (!unrecog_args_.empty())
        {
            err_flgs_.set(arg_parser_error_flags::UNRECOGNIZED_ARGS_ERROR);
        }
        else
        {
            err_flgs_.erase(arg_parser_error_flags::UNRECOGNIZED_ARGS_ERROR);
        }
        
        // Update realation constraints errors.
        in = false;
        for (auto& x : relational_constrs_)
        {
            if (x.is_relational_constraint_violed())
            {
                in = true;
                err_flgs_.set(arg_parser_error_flags::ARGS_RELATIONAL_CONSTRAINTS_ERROR);
                break;
            }
        }
        if (!in)
        {
            err_flgs_.erase(arg_parser_error_flags::ARGS_RELATIONAL_CONSTRAINTS_ERROR);
        }
    }
    
    /**
     * @brief       Allows knowing whether there are terminal key arguments in the parser.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool there_are_terminal_key_args() const noexcept
    {
        key_arg_type* key_arg;
        
        for (auto& x : i_hlp_text_list_)
        {
            if ((key_arg = dynamic_cast<key_arg_type*>(x)) != nullptr)
            {
                if (key_arg->flag_is_set(arg_flags::IS_TERMINAL))
                {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    /**
     * @brief       Print the help menu associated with the argument or the version menu if it is
     *              necessary
     * @param       ky_arg : The argument to check.
     * @throw       speed::argparse::help_menu_not_found_exception : If there isn't any help menu
     *              with the specified id an exception is thrown.
     */
    void print_help_and_version_if_necessary(key_arg_type *ky_arg)
    {
        help_arg_type* hlp_arg;
        version_arg_type* vers_arg;
    
        hlp_arg = dynamic_cast<help_arg_type*>(ky_arg);
        if (hlp_arg != nullptr)
        {
            if (hlp_arg->was_found() &&
                flgs_.is_set(arg_parser_flags::PRINT_HELP_WHEN_HELP_ARG_FOUND))
            {
                print_help(hlp_arg->get_front_key());
            }
        }
    
        vers_arg = dynamic_cast<version_arg_type*>(ky_arg);
        if (vers_arg != nullptr)
        {
            if (vers_arg->was_found() &&
                flgs_.is_set(arg_parser_flags::PRINT_VERSION_WHEN_VERSION_ARG_FOUND))
            {
                print_version();
            }
        }
    }
    
    /**
     * @brief       Destroy and deallocate the specified argument.
     * @param       arg : The specified argument.
     */
    inline void delete_i_help_text(i_help_text_type *&arg) noexcept
    {
        help_text_type* hlp_text;
        key_arg_type* ky_arg;
        help_arg_type* hlp_arg;
        version_arg_type* vers_arg;
        key_value_arg_type* ky_val_arg;
        keyless_arg_type* forgn_arg;
        
        if ((forgn_arg = dynamic_cast<keyless_arg_type*>(arg)) != nullptr)
        {
            allocator_type<keyless_arg_type> foreign_arg_type_alloc;
            foreign_arg_type_alloc.destroy(forgn_arg);
            foreign_arg_type_alloc.deallocate(forgn_arg, 1);
        }
        else if ((ky_val_arg = dynamic_cast<key_value_arg_type*>(arg)) != nullptr)
        {
            allocator_type<key_value_arg_type> key_value_arg_type_alloc;
            key_value_arg_type_alloc.destroy(ky_val_arg);
            key_value_arg_type_alloc.deallocate(ky_val_arg, 1);
        }
        else if ((vers_arg = dynamic_cast<version_arg_type*>(arg)) != nullptr)
        {
            allocator_type<version_arg_type> version_arg_type_alloc;
            version_arg_type_alloc.destroy(vers_arg);
            version_arg_type_alloc.deallocate(vers_arg, 1);
        }
        else if ((hlp_arg = dynamic_cast<help_arg_type*>(arg)) != nullptr)
        {
            allocator_type<help_arg_type> help_arg_type_alloc;
            help_arg_type_alloc.destroy(hlp_arg);
            help_arg_type_alloc.deallocate(hlp_arg, 1);
        }
        else if ((ky_arg = dynamic_cast<key_arg_type*>(arg)) != nullptr)
        {
            allocator_type<key_arg_type> key_arg_type_alloc;
            key_arg_type_alloc.destroy(ky_arg);
            key_arg_type_alloc.deallocate(ky_arg, 1);
        }
        else if ((hlp_text = dynamic_cast<help_text_type*>(arg)) != nullptr)
        {
            allocator_type<help_text_type> help_text_type_alloc;
            help_text_type_alloc.destroy(hlp_text);
            help_text_type_alloc.deallocate(hlp_text, 1);
        }
        
        arg = nullptr;
    }
    
    /**
     * @brief       Exit the program with the value specified.
     * @param       val : Value that the program will return.
     */
    inline void exit_program(int val) const noexcept
    {
        this->~basic_arg_parser();
        exit(val);
    }

private:
    /** Contains the program name that use the argument parser. */
    string_type prog_name_;
    
    /** Collection that contains the short prefixes arguments. The default short prefix is '-' */
    unordered_set_type<string_type> short_prefxs_;
    
    /** Collection that contains the long prefixes arguments. The default long prefix is '--' */
    unordered_set_type<string_type> long_prefxs_;
    
    /** Indentation used to print arguments help description. */
    std::size_t arg_desc_indentation_;
    
    /** The maximum description length that will be printed in a single line. */
    std::size_t max_desc_line_length_;
    
    /** The indentation used when a new line is found in a description. */
    std::size_t desc_new_line_indentation_;
    
    /** Collection that contains all the arguments. */
    vector_type<i_help_text_type*> i_hlp_text_list_;
    
    /** The default help menu id. */
    string_type default_hlp_menu_id_;
    
    /** Collection used to allow O(1) access on help arguments. */
    unordered_map_type<string_type, help_arg_type*> hlp_arg_map_;
    
    /** Contains all help menus. */
    unordered_map_type<string_type, vector_type<i_help_text_type*>> hlp_menus_;
    
    /** Contains the current version argument. */
    version_arg_type* current_vers_arg_;
    
    /** Collection that contains all the keyless arguments. */
    vector_type<keyless_arg_type*> kyless_args_list_;
    
    /** Collection used to allow O(1) access on base arguments. */
    unordered_map_type<string_type, base_arg_type*> bse_arg_map_;
    
    /** Error id that will be used for global errors. */
    string_type err_id_;
    
    /** Contains the unrecognized argument if an error happen. */
    vector_type<string_type> unrecog_args_;
    
    /** Contains the maximum number of unrecognized args to be catched. */
    std::size_t max_unrecog_args_;
    
    /** Flags that dictates the argument parser behavior. */
    vector_type<relational_constraint_type> relational_constrs_;
    
    /** Flags that dictates the argument parser behavior. */
    flags_type<arg_parser_flags> flgs_;
    
    /** Error flags that allows knowing whether there are errors. */
    flags_type<arg_parser_error_flags> err_flgs_;

    /** Alocator of the help_text_type. */
    allocator_type<help_text_type> help_text_type_alloc_;

    /** Alocator of the key_arg_type. */
    allocator_type<key_arg_type> key_arg_type_alloc_;

    /** Alocator of the help_arg_type. */
    allocator_type<help_arg_type> help_arg_type_alloc_;

    /** Alocator of the version_arg_type. */
    allocator_type<version_arg_type> version_arg_type_alloc_;

    /** Alocator of the key_value_arg_type. */
    allocator_type<key_value_arg_type> key_value_arg_type_alloc_;

    /** Alocator of the keyless_arg_type. */
    allocator_type<keyless_arg_type> keyless_arg_type_alloc_;
    
    friend class basic_base_arg<TpAllocator>;
    friend class basic_key_arg<TpAllocator>;
    friend class basic_value_arg<TpAllocator>;
    friend class basic_help_arg<TpAllocator>;
    friend class basic_version_arg<TpAllocator>;
    friend class basic_key_value_arg<TpAllocator>;
    friend class basic_keyless_arg<TpAllocator>;
};


/** Class used to parse arguments. */
using arg_parser = basic_arg_parser<std::allocator<int>>;



}


#endif

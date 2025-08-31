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
 * @file        directory_iteration.hpp
 * @brief       directory_iteration class header.
 * @author      Killian Valverde
 * @date        2024/10/15
 */

#ifndef SPEED_FILESYSTEM_DIRECTORY_ITERATION_HPP
#define SPEED_FILESYSTEM_DIRECTORY_ITERATION_HPP

#include <filesystem>
#include <regex>
#include <set>
#include <stack>

#include "detail/forward_declarations.hpp"
#include "../containers/containers.hpp"
#include "../stringutils/stringutils.hpp"
#include "../system/system.hpp"
#include "../type_casting/type_casting.hpp"
#include "operations.hpp"

namespace speed::filesystem {

/**
 * @brief       Class that configures and produce directories iterators.
 */
class directory_iteration
{
public:
    /** Character type used in the class. */
    using char_type = std::filesystem::path::value_type;

    /** String type used in the class. */
    using string_type = std::filesystem::path::string_type;

    /** Directory entity type. */
    using directory_entity_type = system::filesystem::directory_entity;

    /** Regex type used in the class. */
    using regex_type = std::basic_regex<char_type, std::regex_traits<char_type>>;

    /**
     * @brief       Class that represents const iterators.
     */
    class const_iterator
    {
    public:
        /** The class itself. */
        using self_type = const_iterator;

        /** The value encapsulated by the iterator. */
        using value_type = std::filesystem::path;

        /** Constructor with parameters. */
        explicit const_iterator(const directory_iteration* composit);

        /** Destructor. */
        ~const_iterator() noexcept;

        /**
         * @brief       Move to the forward node.
         * @return      A reference to an iterator pointing the current node.
         */
        self_type& operator ++();

        /**
         * @brief       Allows knowing whether the two iterators are the same.
         * @param       rhs : The value to compare.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        bool operator ==(const self_type& rhs) const noexcept;

        /**
         * @brief       Allows knowing whether the iterator is past-the-end or not.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        [[nodiscard]] bool end() const noexcept
        {
            return end_;
        }

        /**
         * @brief       Get the reference of the current node value.
         * @return      The reference of the current node value.
         */
        const value_type& operator *() const noexcept
        {
            return cur_fle_;
        }

        /**
         * @brief       Get the address of the current node value.
         * @return      The address of the current node value.
         */
        const value_type* operator ->() const noexcept
        {
            return &cur_fle_;
        }

    private:
        /**
         * @brief       Tries to open the current directory.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        bool open_directory();

        /**
         * @brief       Tries to read the next entry in the current directory.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        bool read_directory();

        /**
         * @brief       Close and exit the current directory.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        void close_directory();

        /**
         * @brief       Exit the current directory.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        void exit_directory();

        /**
         * @brief       Check if the current file is valid.
         * @return      If function was successful true is returned, otherwise false is returned.
         */
        [[nodiscard]] bool is_file_valid();
        
        /**
         * @brief       Searches for the first occurrence of a substring in a string.
         * @param       str : The null-terminated string to search in.
         * @param       substr : The null-terminated substring to search for.
         * @return      Pointer to the first occurrence of `substr` in `str` if found. `nullptr` if
         *              `substr` is not found. If `substr` is an empty string, returns `str`.
         */
        [[nodiscard]] bool find_substr() const;

        /**
         * @brief       Compares a string to a pattern with wildcard characters '*' and '?'.
         * @param       str : Pointer to the string to match.
         * @param       pattrn : Pointer to the pattern containing wildcards.
         * @return      `true` if `str` matches the `pattrn` with wildcards; otherwise, `false`.
         */
        [[nodiscard]] bool matches_wildcard() const;

    private:
        /** Current directory. */
        std::filesystem::path cur_dir_;

        /** Current file. */
        std::filesystem::path cur_fle_;

        /** Stack of directories entities used to explore recursivelly the filesystem. */
        std::stack<directory_entity_type> directory_entity_stck_;

        /** Set of visited inodes to avoid infinite recursions in case of fs corruptions. */
        std::set<system::filesystem::inode_t> vistd_inos_;

        /** Pointer to the composite object. */
        const directory_iteration* composit_;

        /** Current level of recursivity. */
        std::uint64_t current_recursivity_levl_ = 0;

        /** Represents whether or not the iteration is finished. */
        bool end_ = false;
    };

    /**
     * @brief       Constructor with parameters.
     * @param       root_pth : Path where to do the iteration.
     */
    template<
            typename PathT_,
            typename = std::enable_if_t<
                    !std::is_base_of<directory_iteration, std::decay_t<PathT_>>::value
            >
    >
    explicit directory_iteration(PathT_&& root_pth)
            : root_pth_(std::forward<PathT_>(root_pth))
    {
        if (root_pth_.native().find(SPEED_ALT_PATH_SEPARATOR_CHAR) != string_type::npos)
        {
            root_pth_ = get_normalized_path(root_pth_);
        }
    }

    /**
     * @brief       Get a const iterator for the first file.
     * @return      A const iterator for the first file.
     */
    [[nodiscard]] const_iterator begin() noexcept
    {
        return const_iterator(this);
    }

    /**
     * @brief       Get a const iterator to the past-the-end file.
     * @return      A const iterator to the past-the-end file.
     */
    [[nodiscard]] const_iterator end() noexcept
    {
        return const_iterator(nullptr);
    }

    /**
     * @brief       Get a const iterator for the first file.
     * @return      A const iterator for the first file.
     */
    [[nodiscard]] const_iterator cbegin() const noexcept
    {
        return const_iterator(this);
    }

    /**
     * @brief       Get a const iterator to the past-the-end file.
     * @return      A const iterator to the past-the-end file.
     */
    [[nodiscard]] const_iterator cend() const noexcept
    {
        return const_iterator(nullptr);
    }

    /**
     * @brief       Specify the access modes that the files are mandatory to have.
     * @param       access_mods : Access modes that the files are mandatory to have.
     * @return      The object who call the method.
     */
    directory_iteration& access_modes(system::filesystem::access_modes access_mods)
    {
        access_mods_ = access_mods;
        return *this;
    }

    /**
     * @brief       Sets the case sensitivity option for directory iteration.
     * @param       enabl : `true` to enable case-sensitive matching, `false` for case-insensitive
     *              matching.
     * @return      The object who call the method.
     */
    directory_iteration& case_insensitive(bool enabl)
    {
        if (case_insensitve_ != enabl)
        {
            case_insensitve_ = enabl;
            
            if (!regex_to_mtch_str_.empty())
            {
                update_regex();
            }
        }
        
        return *this;
    }

    /**
     * @brief       Specify the file types that will be considered during the iteration.
     * @param       file_typs : File types that will be considered during the iteration.
     * @return      The object who call the method.
     */
    directory_iteration& file_types(system::filesystem::file_types file_typs)
    {
        file_typs_ = file_typs;
        return *this;
    }
    
    /**
     * @brief       Enables or disables inode tracking to prevent duplicate file entries.
     * @param       enabl : Set to `true` to enable inode tracking, or `false` to disable it.
     * @return      The object who call the method.
     */
    directory_iteration& inode_tracking(bool enabl)
    {
        inode_trackr_ = enabl;
        return *this;
    }

    /**
     * @brief       Specify the level of recursivity that.
     * @param       recursivity_levl : Access modes that the files are mandatory to have.
     * @return      The object who call the method.
     */
    directory_iteration& recursivity_level(std::uint64_t recursivity_levl)
    {
        max_recursivity_levl_ = recursivity_levl;
        return *this;
    }

    /**
     * @brief       Specify the regex that all the file names have to match.
     * @param       regex_to_mtch : Regex string that all the file names have to match.
     * @return      The object who call the method.
     */
    template<typename StringT_>
    directory_iteration& regex_to_match(StringT_&& regex_to_mtch)
    {
        regex_to_mtch_str_ = type_casting::type_cast<string_type>(
                std::forward<StringT_>(regex_to_mtch));
        
        update_regex();
        return *this;
    }

    /**
     * @brief       Specify wether or not the directory symbolic links will be resolved.
     * @param       enabl : If true the directories symbolic links will be followed.
     * @return      The object who call the method.
     */
    directory_iteration& resolve_directory_symlinks(bool enabl)
    {
        resolve_directory_symlnks_ = enabl;
        return *this;
    }

    /**
     * @brief       Specify wether or not the entries symbolic links will be resolved.
     * @param       enabl : If true the entries symbolic links will be resolved.
     * @return      The object who call the method.
     */
    directory_iteration& resolve_entries_symlinks(bool enabl)
    {
        resolve_directory_symlnks_ = enabl;
        return *this;
    }

    /**
     * @brief       Specify the substring that all the file names must have.
     * @param       substring_to_mtch : Wildcard string that all the file names have to match.
     * @return      The object who call the method.
     */
    template<typename StringT_>
    directory_iteration& substring_to_match(StringT_&& substring_to_mtch)
    {
        substring_to_mtch_ = type_casting::type_cast<string_type>(
                std::forward<StringT_>(substring_to_mtch));
        
        return *this;
    }

    /**
     * @brief       Specify the wildcard that all the file names have to match.
     * @param       wildcard_to_mtch : Wildcard string that all the file names have to match.
     * @return      The object who call the method.
     */
    template<typename StringT_>
    directory_iteration& wildcard_to_match(StringT_&& wildcard_to_mtch)
    {
        wildcard_to_mtch_ = type_casting::type_cast<string_type>(
                std::forward<StringT_>(wildcard_to_mtch));
        
        return *this;
    }
    
private:
    /**
     * @brief       Specify the regex that all the file names have to match.
     * @param       regex_to_mtch : Regex string that all the file names have to match.
     * @return      The object who call the method.
     */
    void update_regex();

private:
    /** The root directory of the iteration. */
    std::filesystem::path root_pth_;
    
    /** Wildcard that all the iterated files have to match. */
    string_type substring_to_mtch_;
    
    /** Wildcard that all the iterated files have to match. */
    string_type wildcard_to_mtch_;

    /** Regex that all the iterated files have to match. */
    string_type regex_to_mtch_str_;

    /** Regex that all the iterated files have to match. */
    regex_type regex_to_mtch_;

    /** Maximum level of recursivity allowed. */
    std::uint64_t max_recursivity_levl_ = ~0ull;

    /** Access mods that all the iterated files have to match. */
    system::filesystem::access_modes access_mods_ = system::filesystem::access_modes::NIL;

    /** List of file types that are allowed to be iterated. */
    system::filesystem::file_types file_typs_ = system::filesystem::file_types::NIL;
    
    /** Specify wheter or not the regex will be case sensitive. */
    bool case_insensitve_ = true;
    
    /** Specify wheter or not the inodes will be tracked. */
    bool inode_trackr_ = false;

    /** Specify wheter or not follow symbolic links during the iteration. */
    bool resolve_directory_symlnks_ = false;

    /** Specify wheter or not resolve directory symbolic links during the iteration. */
    bool resolve_entries_symlnks_ = false;

    friend class const_iterator;
};

}

#endif

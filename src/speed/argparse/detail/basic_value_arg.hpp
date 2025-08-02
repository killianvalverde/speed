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
 * @file        basic_value_arg.hpp
 * @brief       basic_value_arg class header.
 * @author      Killian Valverde
 * @date        2016/03/10
 */

#ifndef SPEED_ARGPARSE_DETAIL_BASIC_VALUE_ARG_HPP
#define SPEED_ARGPARSE_DETAIL_BASIC_VALUE_ARG_HPP

#include <regex>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "forward_declarations.hpp"
#include "../../type_casting/type_casting.hpp"
#include "../basic_arg_parser.hpp"
#include "../exception.hpp"
#include "arg_flags.hpp"
#include "basic_arg_value.hpp"
#include "basic_base_arg.hpp"
#include "basic_type_caster.hpp"
#include "type_traits.hpp"

namespace speed::argparse::detail {

/**
 * @brief       Class that represents arguments that have values.
 */
template<typename AllocatorT>
class basic_value_arg : public virtual basic_base_arg<AllocatorT>
{
public:
    /** Allocator type used in the class. */
    template<typename T>
    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<T>;
    
    /** Shared pointer type used in the class. */
    template<typename T>
    using unique_ptr_type = std::unique_ptr<T>;

    /** String type used in the class. */
    using string_type = std::basic_string<char, std::char_traits<char>, allocator_type<char>>;

    /** Regex type used in the class. */
    using regex_type = std::basic_regex<char, std::regex_traits<char>>;

    /** Array type used in the class. */
    template<typename T, std::size_t N>
    using array_type = std::array<T, N>;

    /** Vector type used in the class. */
    template<typename T>
    using vector_type = std::vector<T, allocator_type<T>>;

    /** Forward list type used in the class. */
    template<typename T>
    using list_type = std::list<T, allocator_type<T>>;

    /** Pair type used in the class. */
    template<typename T1, typename T2>
    using pair_type = std::pair<T1, T2>;
    
    /** Tuple type used in the class. */
    template<typename... Ts>
    using tuple_type = std::tuple<Ts...>;
    
    /** Type that represents a validator lambda. */
    using assertion_type = std::function<bool(const string_type&)>;

    /** Type that represents the caster base type. */
    using type_caster_base_type = type_caster_base<string_type>;

    /** Type that represents the caster type. */
    template<typename T>
    using type_caster_type = basic_type_caster<string_type, T>;

    /** Type that represents a value for an argument. */
    using arg_value_type = basic_arg_value<AllocatorT>;
    
    /** Type that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<AllocatorT>;

    /** Type that represents the argument parser. */
    using arg_parser_type = basic_arg_parser<AllocatorT>;

    /**
     * @brief       Constructor with parameters.
     * @param       arg_parsr : Argument parser that holds this object.
     */
    explicit basic_value_arg(arg_parser_type* arg_parsr)
            : base_arg_type(arg_parsr)
    {
        nr_vals_.emplace_back(0);
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
    basic_value_arg(basic_value_arg&& rhs) noexcept = default;
    
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
            minmax_vals_ = std::move(rhs.minmax_vals_);
            vals_ = std::move(rhs.vals_);
            nr_vals_ = std::move(rhs.nr_vals_);
            castrs_ = std::move(rhs.castrs_);
            assertns_ = std::move(rhs.assertns_);
            regxes_ = std::move(rhs.regxes_);
        }
        
        return *this;
    }

    /**
     * @brief       Add a value to the argument.
     * @param       val : Value to add to the arguement.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    template<typename StringT_>
    bool add_value(StringT_&& val)
    {
        if (!max_values_reached())
        {
            vals_.emplace_back(std::forward<StringT_>(val), get_next_caster(),
                    get_next_assertion(), get_next_regex(), base_arg_type::get_arg_parser(), this);
            ++nr_vals_.back();
            return true;
        }

        base_arg_type::set_error_flag(arg_error_flags::MAX_VALUES_ERROR);
        return false;
    }

    /**
     * @brief       Try to add a value to the argument. The value will just be added to the
     *              argument if this process will not raise any error flag.
     * @param       val : Value to add to the arguement.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    template<typename StringT_>
    bool try_add_value(StringT_&& val)
    {
        if (!max_values_reached())
        {
            arg_value_type arg_val(std::forward<StringT_>(val), get_next_caster(),
                    get_next_assertion(), get_next_regex(), base_arg_type::get_arg_parser(), this);

            if (!arg_val.has_errors())
            {
                vals_.emplace_back(std::move(arg_val));
                ++nr_vals_.back();
                return true;
            }
        }

        return false;
    }

    /**
     * @brief       Specifies whether the argument has been found in the program call.
     * @param       fnd : The value that specifies whether the argument has been found in the
     *              program call.
     */
    bool increase_occurrence() noexcept override
    {
        if (base_arg_type::was_found() &&
            holdr_is_nested_contnr_ &&
            !base_arg_type::max_occurrences_reached())
        {
            for (auto& castr : castrs_)
            {
                castr->request_addition();
            }
            nr_vals_.emplace_back(0);
        }
        
        return base_arg_type::increase_occurrence();
    }

    /**
     * @brief       Clear values collection.
     */
    inline void reset() noexcept override
    {
        vals_.clear();
        nr_vals_.clear();
        nr_vals_.emplace_back(0);
        base_arg_type::reset();
    }
    
    /**
     * @brief       Set error flags if it's required.
     */
    void update_error_flags() noexcept override
    {
        bool min_val_err = false;
        bool val_err = false;
        
        base_arg_type::update_error_flags();
        
        if (base_arg_type::was_found())
        {
            for (auto& nr_val : nr_vals_)
            {
                if (nr_val < minmax_vals_.first)
                {
                    base_arg_type::set_error_flag(arg_error_flags::MIN_VALUES_ERROR);
                    min_val_err = true;
                    break;
                }
            }
        }
        if (!min_val_err)
        {
            base_arg_type::unset_error_flag(arg_error_flags::MIN_VALUES_ERROR);
        }
        
        for (auto& val : vals_)
        {
            if (val.has_errors())
            {
                val_err = true;
                base_arg_type::set_error_flag(arg_error_flags::VALUES_ERROR);
                break;
            }
        }
        if (!val_err)
        {
            base_arg_type::unset_error_flag(arg_error_flags::VALUES_ERROR);
        }
    }

    /**
     * @brief       Updates the minimum and maximum allowed values if auto-update is enabled.
     * @param       min : The new minimum value.
     * @param       max : The new maximum value.
     */
    void update_minmax_values(std::size_t min, std::size_t max)
    {
        if (!max_vals_auto_update_)
        {
            return;
        }

        set_minmax_values(min, max);
    }
    
    /**
     * @brief       Allows knowing whether the argument can't get more values.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool max_values_reached() const noexcept
    {
        return nr_vals_.back() >= minmax_vals_.second;
    }

    /**
     * @brief       Allows knowing whether the argument has rached the minimal number of values.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    [[nodiscard]] inline bool min_values_reached() const noexcept
    {
        return nr_vals_.back() >= minmax_vals_.first;
    }

    /**
     * @brief       Get the front arg value converted to target.
     * @return      The front arg value converted to target.
     */
    template<typename TargetT_>
    [[nodiscard]] inline TargetT_ get_front_value_as() const
    {
        if (vals_.empty())
        {
            throw value_not_found_exception();
        }

        return vals_.front().template as<TargetT_>();
    }
    
    /**
     * @brief       Get the front arg value converted to target.
     * @param       default_val : The value returned if the conversion fails.
     * @return      If function was successful the arg value converted to target is returned, 
     *              otherwise the specified default value is returned.
     */
    template<typename TargetT_, typename TargetT_DefaultValue_>
    [[nodiscard]] inline TargetT_ get_front_value_as(TargetT_DefaultValue_&& default_val) const noexcept
    {
        return vals_.empty() ? std::forward<TargetT_DefaultValue_>(default_val) : 
                vals_.front().template as<TargetT_>(std::forward<TargetT_DefaultValue_>(default_val));
    }
    
    /**
     * @brief       Try to get the front arg value converted to target.
     * @param       res : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TargetT_>
    [[nodiscard]] inline bool try_get_front_value_as(TargetT_& res) const noexcept
    {
        return !vals_.empty() && vals_.front().template try_as<TargetT_>(res);
    }

    /**
     * @brief       Get the specified arg value converted to target.
     * @param       indx : Position of the element.
     * @return      The specified arg value converted to target.
     */
    template<typename TargetT_>
    [[nodiscard]] inline TargetT_ get_value_at_as(std::size_t indx) const
    {
        if (indx >= vals_.size())
        {
            throw value_not_found_exception();
        }

        return vals_.at(indx).template as<TargetT_>();
    }
    
    /**
     * @brief       Get the specified arg value converted to target.
     * @param       indx : Position of the element.
     * @param       default_val : The value returned if the conversion fails.
     * @return      If function was successful the arg value converted to target is returned, 
     *              otherwise the specified default value is returned.
     */
    template<typename TargetT_, typename TargetT_DefaultValue_>
    [[nodiscard]] inline TargetT_ get_value_at_as(
            std::size_t indx, 
            TargetT_DefaultValue_&& default_val
    ) const noexcept
    {
        return indx >= vals_.size() ? std::forward<TargetT_DefaultValue_>(default_val) :
                vals_.at(indx).template as<TargetT_>(
                        std::forward<TargetT_DefaultValue_>(default_val));
    }
    
    /**
     * @brief       Try to get the specified arg value converted to target.
     * @param       indx : Position of the element.
     * @param       res : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TargetT_>
    [[nodiscard]] inline bool try_get_value_at_as(std::size_t indx, TargetT_& res) const noexcept
    {
        return indx < vals_.size() && vals_.at(indx).template try_as<TargetT_>(res);
    }

    /**
     * @brief       Get all the arg values converted to target.
     * @return      All the arg values converted to target.
     */
    template<typename TargetT_>
    [[nodiscard]] inline vector_type<TargetT_> get_values_as() const
    {
        vector_type<TargetT_> ret_vals;

        for (auto& x : vals_)
        {
            ret_vals.emplace_back(x.template as<TargetT_>());
        }

        return ret_vals;
    }

    /**
     * @brief       Get all the arg values converted to target.
     * @param       default_val : The value returned if the conversion fails.
     * @return      If function was successful all the arg values converted to target are returned, 
     *              otherwise the specified default value is returned.
     */
    template<typename TargetT_, typename TargetT_DefaultValue_ = vector_type<TargetT_>>
    [[nodiscard]] inline vector_type<TargetT_> get_values_as(
        TargetT_DefaultValue_&& default_val
    ) const noexcept
    {
        vector_type<TargetT_> res;
        TargetT_ val_res;

        for (auto& val : vals_)
        {
            if (val.template try_as<TargetT_>(&val_res))
            {
                res.emplace_back(std::move(val_res));
            }
            else
            {
                return std::forward<TargetT_DefaultValue_>(default_val);
            }
        }

        return res.empty() ? std::forward<TargetT_DefaultValue_>(default_val) : res;
    }

    /**
     * @brief       Try to get all the arg values converted to target.
     * @param       res : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TargetT_>
    [[nodiscard]] inline vector_type<TargetT_> try_get_values_as(
            vector_type<TargetT_>& res
    ) const noexcept
    {
        TargetT_ val_res;

        res->clear();

        for (auto& val : vals_)
        {
            if (val.template try_as<TargetT_>(&val_res))
            {
                res->emplace_back(std::move(val_res));
            }
            else
            {
                res->clear();
                return false;
            }
        }

        return !res->empty();
    }

    /**
     * @brief       Get the current dimension number of values of the arguement.
     * @return      The current dimension number of values of the argument.
     */
    [[nodiscard]] std::size_t get_current_dimension_values_count() const noexcept
    {
        return nr_vals_.back();
    }

    /**
     * @brief       Get maximum number of values for an option.
     * @return      The maximum number of values for an option.
     */
    [[nodiscard]] inline std::size_t get_max_values() const noexcept
    {
        return minmax_vals_.second;
    }

    /**
     * @brief       Get minimum number of values for an option.
     * @return      The minimum number of values for an option.
     */
    [[nodiscard]] inline std::size_t get_min_values() const noexcept
    {
        return minmax_vals_.first;
    }

    /**
     * @brief       Get the assertion associated with the current value.
     * @return      The assertion associated with the current value.
     */
    [[nodiscard]] assertion_type* get_next_assertion()
    {
        if (nr_vals_.back() < assertns_.size())
        {
            auto it = assertns_.begin();
            std::advance(it, nr_vals_.back());
            return &*it;
        }
        else if (!assertns_.empty())
        {
            return &assertns_.back();
        }

        return nullptr;
    }

    /**
     * @brief       Get the caster associated witht the current value.
     * @return      The caster associated witht the current value.
     */
    [[nodiscard]] type_caster_base_type* get_next_caster()
    {
        if (vals_.size() < castrs_.size())
        {
            return castrs_[vals_.size()].get();
        }
        else if (!castrs_.empty())
        {
            return castrs_.back().get();
        }

        return nullptr;
    }

    /**
     * @brief       Get the regex associated with the current value.
     * @return      The regex associated with the current value.
     */
    [[nodiscard]] regex_type* get_next_regex()
    {
        if (nr_vals_.back() < regxes_.size())
        {
            auto it = regxes_.begin();
            std::advance(it, nr_vals_.back());
            return &*it;
        }
        else if (!regxes_.empty())
        {
            return &regxes_.back();
        }

        return nullptr;
    }

    /**
     * @brief       Get the actual number of values of the arguement.
     * @return      The actual number of values of the argument.
     */
    [[nodiscard]] std::size_t get_total_values_count() const noexcept
    {
        return vals_.size();
    }

    /**
     * @brief       Set the objects that will be holding the casted values.
     * @param       holdrs : The objects that will holding the casted values.
     */
    template<typename... HolderTs_>
    void set_holders(HolderTs_&... holdrs)
    {
        castrs_.clear();
        (castrs_.emplace_back(memory::allocate_unique<type_caster_type<HolderTs_>>(
                allocator_type<type_caster_type<HolderTs_>>(), &holdrs)), ...);
        
        if constexpr (sizeof...(holdrs) == 1)
        {
            ((holdr_is_nested_contnr_ = is_nested_supported_container_v<HolderTs_>), ...);
        }
        else
        {
            holdr_is_nested_contnr_ = false;
        }
        
        update_data_from_holders(holdrs...);
    }

    /**
     * @brief       Set whether the max values have to auto-update.
     * @param       auto_updat : Wheter the max values have to auto-update.
     */
    void set_max_values_auto_update(bool auto_updat)
    {
        max_vals_auto_update_ = auto_updat;
    }

    /**
     * @brief       Set the minimum and maximum values.
     * @param       min : The minimum values.
     * @param       max : The maxium values.
     */
    virtual void set_minmax_values(std::size_t min, std::size_t max)
    {
        if (min > max)
        {
            throw wrong_min_max_interval_exception();
        }

        minmax_vals_.first = min;
        minmax_vals_.second = max;
    }

    /**
     * @brief       Set the regexes that will be associated with the values.
     * @param       regxes : Regexes that will be associated with the values.
     */
    template<typename... RegexTs_>
    void set_regexes(RegexTs_&&... regxes)
    {
        regxes_.clear();
        (regxes_.emplace_back(std::forward<RegexTs_>(regxes)), ...);
    }

    /**
     * @brief       Set the functions to execute in order to know if the values are valid.
     * @param       assertns : Functions to execute in order to know if the values are valid.
     */
    template<typename... AssertionTs_>
    inline void set_assertions(AssertionTs_&&... assertns)
    {
        assertns_.clear();
        (assertns_.emplace_back(std::forward<AssertionTs_>(assertns)), ...);
    }
    
    /**
     * @brief       Print argument errors in standard output.
     */
    void print_errors() const override
    {
        if (!base_arg_type::has_errors())
        {
            return;
        }

        base_arg_type::print_errors();
        
        if (base_arg_type::is_error_flag_set(arg_error_flags::MIN_VALUES_ERROR))
        {
            base_arg_type::print_error_message();
            std::cout << base_arg_type::get_tittle() << " requires at least " << minmax_vals_.first
                      << (minmax_vals_.first > 1 ? " values" : " value") << '\n';
        }
        
        if (base_arg_type::is_error_flag_set(arg_error_flags::MAX_VALUES_ERROR))
        {
            base_arg_type::print_error_message();
            std::cout << base_arg_type::get_tittle() << " must have a maximum of "
                      << minmax_vals_.second
                      << (minmax_vals_.second > 1 ? " values" : " value") << '\n';
        }
        
        if (base_arg_type::is_error_flag_set(arg_error_flags::VALUES_ERROR) && !vals_.empty())
        {
            for (auto& val : vals_)
            {
                if (val.has_errors())
                {
                    val.print_errors();
                }
            }
        }
    }
 
protected:
    /** @cond */
    template<typename... ContainerTs_>
    void update_data_from_holders(ContainerTs_&... holdrs)
    {
        constexpr std::size_t N_PARAMETERS = sizeof...(ContainerTs_);
        update_minmax_values(N_PARAMETERS, N_PARAMETERS);
    }
    
    template<typename ValueT_, std::size_t N>
    void update_data_from_holders(array_type<ValueT_, N>& holdr)
    {
        update_minmax_values(1, N);
    }
    
    template<typename ValueT_, std::size_t N>
    requires (is_supported_container_v<ValueT_>)
    void update_data_from_holders(array_type<ValueT_, N>& holdr)
    {
        bool updat = base_arg_type::get_max_occurrences_update();
        base_arg_type::update_max_occurrences(N);
        base_arg_type::set_max_occurrences_update(false);
        update_data_from_holders(*((ValueT_*) &updat));
        base_arg_type::set_max_occurrences_update(updat);
    }
    
    template<typename T1_, typename T2_>
    void update_data_from_holders(pair_type<T1_, T2_>& holdr)
    {
        update_minmax_values(2, 2);
    }
    
    template<typename... ContainerTs_>
    void update_data_from_holders(tuple_type<ContainerTs_...>& holdr)
    {
        constexpr std::size_t TUPLE_SIZE = std::tuple_size<tuple_type<ContainerTs_...>>::value;
        update_minmax_values(TUPLE_SIZE, TUPLE_SIZE);
    }
    /** @endcond */

private:
    /** Collection that has the values gotten through the program call for an argument. */
    vector_type<arg_value_type> vals_;
    
    /** Virtual total number of values. */
    vector_type<std::size_t> nr_vals_;
    
    /** Type casters used to validate the values syntax. */
    vector_type<unique_ptr_type<type_caster_base_type>> castrs_;
    
    /** Functions to execute in order to know if the values are valid. */
    list_type<assertion_type> assertns_;
    
    /** Regular expressions that the values has to match. */
    list_type<regex_type> regxes_;

    /** Minimum and maximum number of values for an option. */
    pair_type<std::size_t, std::size_t> minmax_vals_ = {1, 1};

    /** Dictates whether the max values number will auto-update. */
    bool max_vals_auto_update_ = true;
    
    /** Indicates if holder is a supported nested container. */
    bool holdr_is_nested_contnr_ = false;
};

}

#endif

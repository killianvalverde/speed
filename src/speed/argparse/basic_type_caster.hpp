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
 * @file        speed/argparse/basic_type_caster.hpp
 * @brief       basic_type_caster main header.
 * @author      Killian Valverde
 * @date        2024/05/15
 */

#ifndef SPEED_ARGPARSE_BASIC_TYPE_CASTER_HPP
#define SPEED_ARGPARSE_BASIC_TYPE_CASTER_HPP

#include <utility>

#include "../type_casting/type_casting.hpp"


namespace speed::argparse {


/**
 * @brief       Classs used as a type casting strategy holder.
 */
template<
        typename TpTarget,
        typename TpSource,
        template<typename...> class TpContainer,
        typename TpComponent = speed::type_casting::type_caster_base<TpSource>
>
class basic_type_caster : public TpComponent
{
public:
    /** Type that represents the target. */
    using target_type = TpTarget;

    /** Type that represents the source. */
    using source_type = TpSource;

    /** Type that represents the container. */
    using container_type = TpContainer<TpTarget>;

    /** Type that represents the component. */
    using component_type = TpComponent;

    /** Type that represents a pair. */
    template<typename T1, typename T2>
    using pair_type = std::pair<T1, T2>;

    /**
     * @brief       Allows knowing whether or not an argument is valid.
     * @param       arg : The value to check.
     * @param       err_code : If function fails it holds the error code.
     * @return      If the argument is valid true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) const noexcept override
    {
        target_type res;

        if (speed::type_casting::try_type_cast(arg, &res, err_code))
        {
            return component_type::is_valid(arg, err_code);
        }

        return false;
    }

    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       res : The result of the operation if it was successful.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool try_type_cast(
            const source_type& arg,
            void* res,
            std::error_code* err_code = nullptr
    ) const noexcept override
    {
        auto* dat = (pair_type<void*, void*>*)res;
        auto* hldr = (target_type*)dat->first;
        auto* vec = (container_type*)dat->second;
        target_type val;

        if (hldr == nullptr)
        {
            hldr = &val;
        }

        if (!speed::type_casting::try_type_cast(arg, hldr, err_code))
        {
            return false;
        }

        if (vec != nullptr)
        {
            vec->push_back((hldr == &val) ? std::move(val) : *hldr);
        }

        return true;
    }
};


}


#endif

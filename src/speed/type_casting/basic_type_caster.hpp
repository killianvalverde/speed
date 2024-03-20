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
 * @file        speed/type_casting/basic_type_caster.hpp
 * @brief       basic_type_caster main header.
 * @author      Killian Valverde
 * @date        2024/01/25
 */

#ifndef SPEED_TYPE_CASTING_BASIC_TYPE_CASTER_HPP
#define SPEED_TYPE_CASTING_BASIC_TYPE_CASTER_HPP


#include "i_type_caster.hpp"


namespace speed::type_casting {


/**
 * @brief       Classs used as a type casting strategy holder.
 */
template<typename TpTarget, typename TpSource, typename TpComponent = i_type_caster<TpSource>>
class basic_type_caster : public TpComponent
{
public:
    /** Type that represents the target type. */
    using target_type = TpTarget;

    /** Type that represents the source type. */
    using source_type = TpSource;

    /** Type that represents the component type. */
    using component_type = TpComponent;

    /**
     * @brief       Allows knowing whether or not an argument is valid.
     * @param       arg : The value to check.
     * @param       err_message : If the value is not valid it contains the error message thrown by 
     *              the exception.
     * @return      If the argument is valid true is returned, otherwise false is returned.
     */
    [[nodiscard]] bool is_valid(
            const void* arg, 
            std::string* err_message = nullptr
    ) const noexcept override
    {
        try
        {
            target_type res;

            if (err_message != nullptr)
            {
                err_message->clear();
            }
            
            return type_cast(*(const source_type*)arg, &res) ? 
                    component_type::is_valid(arg) : 
                    false;
        }
        catch (const type_casting_exception& tce)
        {
            if (err_message != nullptr)
            {
                std::size_t fnd;

                *err_message = tce.what();

                fnd = err_message->find(" exception");
                if (fnd != std::string::npos) 
                {
                    err_message->erase(fnd, 10);
                }
            }
        }
        catch (...)
        {
        }

        return false;
    }
};


/**
 * @brief       Classs used as a string casting strategy holder.
 */
template<typename TpTarget, typename TpComponent = i_string_caster>
using string_caster = basic_type_caster<TpTarget, std::string, TpComponent>;


}


#endif

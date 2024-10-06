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
 * @file        speed/type_casting/type_caster_base.hpp
 * @brief       type_caster_base main header.
 * @author      Killian Valverde
 * @date        2024/01/24
 */

#ifndef SPEED_TYPE_CASTING_TYPE_CASTER_BASE_HPP
#define SPEED_TYPE_CASTING_TYPE_CASTER_BASE_HPP

#include <any>

#include "try_type_cast.hpp"


namespace speed::type_casting {


/**
 * @brief       Interface used as base of type casting strategies holders.
 */
template<typename TpSource>
class type_caster_base
{
public:
     /** Type that represents the source type. */
     using source_type = TpSource;

     /** Default constructor. */
     type_caster_base() = default;

     /** Default destructor. */
    virtual ~type_caster_base() = default;
    
    /**
     * @brief       Allows knowing whether or not an argument is valid.
     * @param       arg : The value to check.
     * @param       err_code : If function fails it holds the error code.
     * @return      If the argument is valid true is returned, otherwise false is returned.
     */
    [[nodiscard]] virtual bool is_valid(
            const source_type& arg, 
            std::error_code* err_code = nullptr
    ) const noexcept
    {
        return true;
    }


    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] virtual bool try_type_cast(
            const source_type& arg,
            std::error_code* err_code = nullptr
    ) noexcept
    {
        return false;
    }
    
    /**
     * @brief       Try to convert the source to the target.
     * @param       arg : The value to convert.
     * @param       res : The result of the operation if it was successful.
     * @param       err_code : If function fails it holds the error code.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    [[nodiscard]] virtual bool try_type_cast(
            const source_type& arg, 
            std::any res,
            std::error_code* err_code = nullptr
    ) noexcept
    {
        return false;
    }
};


}


#endif

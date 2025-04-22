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
 * @file        scalar.hpp
 * @brief       scalar class header.
 * @author      Killian Valverde
 * @date        2018/01/10
 */

#ifndef SPEED_SCALARS_SCALAR_HPP
#define SPEED_SCALARS_SCALAR_HPP

#include <type_traits>

namespace speed::scalars {

/**
 * @brief       Class that represents a scalar type encapsulation.
 */
template<
        typename TpScalar,
        typename = std::enable_if_t<std::is_scalar<TpScalar>::value>
>
class scalar
{
public:
    /** Class that represents the underlying scalar type. */
    using scalar_type = TpScalar;

    /**
     * @brief       Default constructor.
     */
    scalar() = default;

    /**
     * @brief       Constructor with parameters.
     * @param       value : Value for the scalar type.
     */
    scalar(scalar_type value)
            : value_(value)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    scalar(const scalar& rhs)
            : value_(rhs.value_)
    {
    }
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    template<
            typename TpScalar_ = scalar_type,
            typename = std::enable_if_t<!std::is_const<TpScalar_>::value>
    >
    scalar(scalar&& rhs) noexcept
            : value_(rhs.value_)
    {
        rhs.value_ = static_cast<scalar_type>(0);
    }
    
    /**
     * @brief       Destructor.
     */
    virtual ~scalar() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : The object to copy.
     * @return      The object who call the method.
     */
    template<
            typename TpScalar_ = scalar_type,
            typename = std::enable_if_t<!std::is_const<TpScalar_>::value>
    >
    scalar& operator =(const scalar& rhs)
    {
        if (this != &rhs)
        {
            value_ = rhs.value_;
        }
        
        return *this;
    }
    
    /**
     * @brief       Move assignement operator.
     * @param       rhs : The object to move.
     * @return      The object who call the method.
     */
    template<
            typename TpScalar_ = scalar_type,
            typename = std::enable_if_t<!std::is_const<TpScalar_>::value>
    >
    scalar& operator =(scalar&& rhs) noexcept
    {
        if ((scalar_type*)this != &rhs)
        {
            scalar_type aux = value_;
            value_ = rhs.value_;
            rhs.value_ = aux;
        }
        
        return *this;
    }
    
    /**
     * @brief       Allows access to the underlying value.
     * @return      A reference to the underlying value.
     */
    scalar_type& value() noexcept
    {
        return value_;
    }
    
    /**
     * @brief       Implicit conversion to the underlying type.
     * @return      A reference to the underlying value.
     */
    operator scalar_type&() noexcept
    {
        return value_;
    }

    /**
     * @brief       Overloading of the reference operator.
     * @return      A pointer to the underlying scalar value.
     */
    scalar_type* operator &() noexcept
    {
        return &value_;
    }
    
    /**
     * @brief       Overloading of the indirection operator.
     * @return      A reference to the value pointed by the scalar value.
     */
    template<
            typename TpScalar_ = scalar_type,
            typename = std::enable_if_t<std::is_pointer<TpScalar_>::value>
    >
    std::remove_pointer_t<scalar_type>& operator *() noexcept
    {
        return *value_;
    }
    
    /**
     * @brief       Overloading of the indirection operator.
     * @return      A reference to the value pointed by the scalar value.
     */
    template<
            typename TpScalar_ = scalar_type,
            typename = std::enable_if_t<std::is_pointer<TpScalar_>::value>
    >
    scalar_type operator ->() noexcept
    {
        return value_;
    }

private:
    /** The underlying value. */
    scalar_type value_;
};

}

#endif

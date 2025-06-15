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
 * @file        operations.hpp
 * @brief       operations functions header.
 * @author      Killian Valverde
 * @date        2017/01/28
 */

#ifndef SPEED_SAFETY_OPERATIONS_HPP
#define SPEED_SAFETY_OPERATIONS_HPP

#include <limits>
#include <stdexcept>

#include "../system/system.hpp"
#include "exception.hpp"

namespace speed::safety {

/**
 * @brief       Fonction that allows to do an addition with overflow detection.
 * @param       trg : Integral to increment.
 * @param       vals : Values to add to the integral.
 */
template<typename TpTarget, typename... TpValues>
inline TpTarget add(TpTarget trg, const TpValues&... vals)
{
    constexpr TpTarget max_trg = std::numeric_limits<TpTarget>::max();
    
    ((trg > max_trg - static_cast<TpTarget>(vals)
            ? throw overflow_exception()
            : trg += static_cast<TpTarget>(vals)), ...);
    
    return trg;
}

/**
 * @brief       Fonction that allows to do an addition with overflow detection.
 * @param       trg : Integral to increment.
 * @param       vals : Values to add to the integral.
 * @return      If the function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename... TpValues>
inline bool try_add(TpTarget* trg, const TpValues& ... vals) noexcept
{
    constexpr TpTarget max_trg = std::numeric_limits<TpTarget>::max();
    bool scs = true;
    
    ((scs
            ? (*trg > max_trg - static_cast<TpTarget>(vals)
                    ? scs = false
                    : *trg += static_cast<TpTarget>(vals))
            : 0), ...);
    
    return scs;
}

/**
 * @brief       Fonction that allows to do an addition with overflow detection and in the overflow
 *              case the maximum value of the type will be set to target.
 * @param       trg : Integral to increment.
 * @param       vals : Values to add to the integral.
 */
template<typename TpTarget, typename... TpValues>
inline TpTarget addm(TpTarget trg, const TpValues&... vals) noexcept
{
    constexpr TpTarget max_trg = std::numeric_limits<TpTarget>::max();
    bool saturatd = false;

    ((saturatd || trg > max_trg - static_cast<TpTarget>(vals)
            ? (trg = max_trg, saturatd = true)
            : trg += static_cast<TpTarget>(vals)), ...);
    
    return trg;
}

/**
 * @brief       Fonction that allows to do an addition with overflow detection and in the overflow
 *              case the maximum value of the type will be set to target.
 * @param       trg : Integral to increment.
 * @param       vals : Values to add to the integral.
 * @return      If the function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename... TpValues>
inline bool try_addm(
        TpTarget* trg,
        const TpValues& ... vals
) noexcept
{
    constexpr TpTarget max_trg = std::numeric_limits<TpTarget>::max();
    bool scs = true;
    
    ((scs
            ? (*trg > max_trg - static_cast<TpTarget>(vals)
                    ? (*trg = max_trg, scs = false)
                    : *trg += static_cast<TpTarget>(vals))
            : 0), ...);
    
    return scs;
}

}

#endif

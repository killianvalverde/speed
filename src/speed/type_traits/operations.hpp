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
 * @file        speed/type_traits/operations.hpp
 * @brief       type_traits functions header.
 * @author      Killian Valverde
 * @date        2016/08/05
 */

#ifndef SPEED_TYPE_TRAITS_OPERATIONS_HPP
#define SPEED_TYPE_TRAITS_OPERATIONS_HPP

#include <filesystem>
#include <string>
#include <type_traits>
#include <vector>


namespace speed::type_traits {


/** @cond */
namespace __private {
template<typename>
struct __is_char_helper : public std::false_type {};

template<>
struct __is_char_helper<char> : public std::true_type {};

template<>
struct __is_char_helper<signed char> : public std::true_type {};

template<>
struct __is_char_helper<unsigned char> : public std::true_type {};
} /* __private */
/** @endcond */


/**
 * @brief       Trait class that identifies whether T is a 'char' type.
 */
template<typename Tp>
struct is_char
        : public __private::__is_char_helper<typename std::remove_cv<Tp>::type>::type
{
};


/** @cond */
namespace __private {
template<typename>
struct __is_wchar_helper : public std::false_type {};

template<>
struct __is_wchar_helper<wchar_t> : public std::true_type {};
} /* __private */
/** @endcond */


/**
 * @brief       Trait class that identifies whether T is a 'wchat_t' type.
 */
template<typename Tp>
struct is_wchar
        : public __private::__is_wchar_helper<typename std::remove_cv<Tp>::type>::type
{
};


/** @cond */
namespace __private {
template<typename>
struct __is_character_helper : public std::false_type {};

template<>
struct __is_character_helper<char> : public std::true_type {};

template<>
struct __is_character_helper<signed char> : public std::true_type {};

template<>
struct __is_character_helper<unsigned char> : public std::true_type {};

template<>
struct __is_character_helper<wchar_t> : public std::true_type {};

template<>
struct __is_character_helper<char16_t> : public std::true_type {};

template<>
struct __is_character_helper<char32_t> : public std::true_type {};
} /* __private */
/** @endcond */


/**
 * @brief       Trait class that identifies whether T is a character type.
 */
template<typename Tp>
struct is_character
        : public __private::__is_character_helper<
                typename std::remove_cv<Tp>::type
          >::type
{
};


/**
 * @brief       Trait class that identifies whether T is a character pointer type.
 */
template<typename Tp>
struct is_character_pointer
        : public std::__and_<
                std::is_pointer<Tp>,
                is_character<typename std::remove_pointer<Tp>::type>
          >::type
{
};


/**
 * @brief       Trait class that identifies whether T is a character that can be used in standard io
 *              operations.
 */
template<typename Tp>
struct is_stdio_character
        : public std::__or_<is_char<Tp>, is_wchar<Tp>>::type
{
};


/** @cond */
namespace __private {
template<typename>
struct __is_float_helper : public std::false_type {};

template<>
struct __is_float_helper<float> : public std::true_type {};
} /* __private */
/** @endcond */


/**
 * @brief       Trait class that identifies whether T is a 'float' type.
 */
template<typename Tp>
struct is_float
        : public __private::__is_float_helper<typename std::remove_cv<Tp>::type>::type
{
};


/** @cond */
namespace __private {
template<typename>
struct __is_double_helper : public std::false_type {};

template<>
struct __is_double_helper<double> : public std::true_type {};
} /* __private */
/** @endcond */


/**
 * @brief       Trait class that identifies whether T is a 'double' type.
 */
template<typename Tp>
struct is_double
        : public __private::__is_double_helper<typename std::remove_cv<Tp>::type>::type
{
};


/** @cond */
namespace __private {
template<typename>
struct __is_long_double_helper : public std::false_type {};

template<>
struct __is_long_double_helper<long double> : public std::true_type {};
} /* __private */
/** @endcond */


/**
 * @brief       Trait class that identifies whether T is a 'double' type.
 */
template<typename Tp>
struct is_long_double
        : public __private::__is_long_double_helper<
                typename std::remove_cv<Tp>::type
          >::type
{
};


/** @cond */
namespace __private {
template<typename>
struct __is_basic_string_helper : public std::false_type {};

template<
        typename TpChar,
        typename TpCharTraits,
        typename TpCharAlloc
>
struct __is_basic_string_helper<std::basic_string<TpChar, TpCharTraits, TpCharAlloc>>
        : public is_character<TpChar>
{
};
} /* __private */
/** @endcond */


/**
 * @brief       Trait class that identifies whether T is a 'std::basic_string' type.
 */
template<typename Tp>
struct is_basic_string
        : public __private::__is_basic_string_helper<
                typename std::remove_cv<Tp>::type
          >::type
{
};


/** @cond */
namespace __private {
template<typename>
struct __is_basic_string_vector_helper : public std::false_type {};

template<
        typename TpBasicString,
        typename TpBasicStringAlloc
>
struct __is_basic_string_vector_helper<std::vector<TpBasicString, TpBasicStringAlloc>>
        : public is_basic_string<TpBasicString>
{
};
} /* __private */
/** @endcond */


/**
 * @brief       Trait class that identifies whether T is a 'std::basic_string' type.
 */
template<typename Tp>
struct is_basic_string_vector
        : public __private::__is_basic_string_vector_helper<
                typename std::remove_cv<Tp>::type
          >::type
{
};


/** @cond */
namespace __private {
template<typename>
struct __is_basic_ostream_helper : public std::false_type {};

template<
        typename TpChar,
        typename TpCharTraits
>
struct __is_basic_ostream_helper<std::basic_ostream<TpChar, TpCharTraits>>
        : public is_stdio_character<TpChar>
{
};
} /* __private */
/** @endcond */


/**
 * @brief       Trait class that identifies whether T is a 'std::basic_ostream' type.
 */
template<typename Tp>
struct is_basic_ostream
        : public __private::__is_basic_ostream_helper<
                typename std::remove_cv<Tp>::type
          >::type
{
};


/** @cond */
namespace __private {
template<typename>
struct __is_path_helper : public std::false_type {};

template<>
struct __is_path_helper<std::filesystem::path> : public std::true_type {};
} /* __private */
/** @endcond */


/**
 * @brief       Trait class that identifies whether T is a 'std::basic_ostream' type.
 */
template<typename Tp>
struct is_path
        : public __private::__is_path_helper<
                typename std::remove_cv<Tp>::type
          >::type
{
};


/**
 * @brief       Trait class that try to obtains the underlying type of enum type T.
 */
template<typename TpEnum>
struct try_underlying_type
{
    /** The underlying type of enum type T. */
    typedef std::conditional_t<
            TpEnum(-1) < TpEnum(0),
            std::make_signed_t<TpEnum>,
            std::make_unsigned_t<TpEnum>
    > type;
};


/** Trait class that try to obtains the underlying type of enum type T. Alias of
 *  'try_underlying_type<T>::type'. */
template<typename TpEnum>
using try_underlying_type_t = typename try_underlying_type<TpEnum>::type;


/**
 * @brief       Alias that simplifies to inherit from the base class in the context of a CRTP
 *              pattern.
 */
template<
        template<typename...> class TpBase,
        template<typename...> class TpDerived,
        typename TpActual,
        typename TpCondition,
        typename... Ts
>
using basic_crtp_base = typename std::conditional<
        std::is_same_v<TpActual, TpCondition>,
        TpBase<Ts..., TpDerived<Ts..., TpActual>>,
        TpBase<Ts..., TpActual>
>::type;


/**
 * @brief       Alias that simplifies getting access to the self type in a CRTP context.
 */
template<
        template<typename...> class TpSelf,
        typename TpActual,
        typename TpCondition,
        typename... Ts
>
using basic_crtp_self = typename std::conditional<
        std::is_same_v<TpActual, TpCondition>,
        TpSelf<Ts...>,
        TpActual
>::type;


}


#endif

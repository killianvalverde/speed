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
 * @file        speed/iostream/basic_ostream.hpp
 * @brief       basic_ostream related functions header.
 * @author      Killian Green
 * @date        2016/08/24 - 09:50
 */

#ifndef SPEED_IOSTREAM_BASIC_OSTREAM_HPP
#define SPEED_IOSTREAM_BASIC_OSTREAM_HPP

#include <iostream>

#include "../system.hpp"


namespace speed::iostream {


/**
 * @brief       Get the current 'std::basic_ostream' object used to print TpChar in standard output.
 * @return      The current 'std::basic_ostream' object used to print TpChar in standard output.
 */
template<typename TpChar>
inline std::basic_ostream<TpChar>& get_cout() noexcept;


/**
 * @brief       Get the current 'std::ostream' object used to print in standard output.
 * @return      The current 'std::ostream' object used to print in standard output.
 */
template<>
inline std::ostream& get_cout<char>() noexcept
{
    return std::cout;
}


/**
 * @brief       Get the current 'std::wostream' object used to print in standard output.
 * @return      The current 'std::wostream' object used to print in standard output.
 */
template<>
inline std::wostream& get_cout<wchar_t>() noexcept
{
    return std::wcout;
}


/**
 * @brief       Inserts a new-line character in the stream.
 * @param       os : Output stream object effected.
 * @return      Argument os.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& newl(std::basic_ostream<TpChar, TpTraits>& os)
{
    return os.put(os.widen('\n'));
}


/**
 * @brief       Set the default text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_default_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::DEFAULT);
    return os;
}


/**
 * @brief       Set black text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_black_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::BLACK);
    return os;
}


/**
 * @brief       Set red text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_red_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::RED);
    return os;
}


/**
 * @brief       Set green text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_green_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::GREEN);
    return os;
}


/**
 * @brief       Set brown text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_brown_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::BROWN);
    return os;
}


/**
 * @brief       Set blue text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_blue_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::BLUE);
    return os;
}


/**
 * @brief       Set purple text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_purple_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::PURPLE);
    return os;
}


/**
 * @brief       Set cyan text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_cyan_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::CYAN);
    return os;
}


/**
 * @brief       Set light gray text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_light_gray_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::LIGHT_GRAY);
    return os;
}


/**
 * @brief       Set dark gray text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_dark_gray_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::DARK_GRAY);
    return os;
}


/**
 * @brief       Set light red text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_light_red_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::LIGHT_RED);
    return os;
}


/**
 * @brief       Set light green text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_light_green_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::LIGHT_GREEN);
    return os;
}


/**
 * @brief       Set yellow text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_yellow_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::YELLOW);
    return os;
}


/**
 * @brief       Set light blue text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_light_blue_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::LIGHT_BLUE);
    return os;
}


/**
 * @brief       Set light purple text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_light_purple_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::LIGHT_PURPLE);
    return os;
}


/**
 * @brief       Set light cyan text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_light_cyan_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::LIGHT_CYAN);
    return os;
}


/**
 * @brief       Set white text in terminal.
 * @param       os : Ostream in which set the attribute.
 * @return      os is returned.
 */
template<typename TpChar, typename TpTraits>
inline std::basic_ostream<TpChar, TpTraits>& set_white_text(
        std::basic_ostream<TpChar, TpTraits>& os
)
{
    speed::system::terminal::set_text_attribute(
            os, speed::system::terminal::text_attribute::WHITE);
    return os;
}


}


#endif

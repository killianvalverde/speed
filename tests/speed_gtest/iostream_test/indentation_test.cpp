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
 * @file        speed_gtest/iostream_test/indentation_test.cpp
 * @brief       indentation unit test.
 * @author      Killian Valverde
 * @date        2018/06/09
 */

#include <gtest/gtest.h>

#include "speed/iostream/iostream.hpp"


TEST(iostream_indentation, operator_pre_increment)
{
    speed::iostream::indentation indent(2, 0);
    speed::iostream::ios_redirect ios_redirect(std::cout);
    std::string expected_result("  \n    \n      \n");
    
    ios_redirect.redirect_to_embedded_stringstream();
    
    std::cout << ++indent << speed::iostream::newl;
    std::cout << ++indent << speed::iostream::newl;
    std::cout << ++indent << speed::iostream::newl;
    
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str() == expected_result);
}


TEST(iostream_indentation, operator_post_increment)
{
    speed::iostream::indentation indent(2, 0);
    speed::iostream::ios_redirect ios_redirect(std::cout);
    std::string expected_result("\n  \n    \n");
    
    ios_redirect.redirect_to_embedded_stringstream();
    
    std::cout << indent++ << speed::iostream::newl;
    std::cout << indent++ << speed::iostream::newl;
    std::cout << indent++ << speed::iostream::newl;
    
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str() == expected_result);
}


TEST(iostream_indentation, operator_pre_decrement)
{
    speed::iostream::indentation indent(2, 0);
    speed::iostream::ios_redirect ios_redirect(std::cout);
    std::string expected_result("  \n    \n      \n    \n  \n\n");
    
    ios_redirect.redirect_to_embedded_stringstream();
    
    std::cout << ++indent << speed::iostream::newl;
    std::cout << ++indent << speed::iostream::newl;
    std::cout << ++indent << speed::iostream::newl;
    std::cout << --indent << speed::iostream::newl;
    std::cout << --indent << speed::iostream::newl;
    std::cout << --indent << speed::iostream::newl;
    
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str() == expected_result);
}


TEST(iostream_indentation, operator_post_decrement)
{
    speed::iostream::indentation indent(2, 0);
    speed::iostream::ios_redirect ios_redirect(std::cout);
    std::string expected_result("\n  \n    \n      \n    \n  \n");
    
    ios_redirect.redirect_to_embedded_stringstream();
    
    std::cout << indent++ << speed::iostream::newl;
    std::cout << indent++ << speed::iostream::newl;
    std::cout << indent++ << speed::iostream::newl;
    std::cout << indent-- << speed::iostream::newl;
    std::cout << indent-- << speed::iostream::newl;
    std::cout << indent-- << speed::iostream::newl;
    
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str() == expected_result);
}

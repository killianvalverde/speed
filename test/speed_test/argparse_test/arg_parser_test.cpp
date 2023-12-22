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
 * @file        speed_test/argparse_test/arg_parser_test.cpp
 * @brief       arg_parser unit test.
 * @author      Killian Green
 * @date        2017/05/16
 */

#include "gtest/gtest.h"
#include "speed/argparse.hpp"
#include "speed/iostream.hpp"
#include "speed/lowlevel.hpp"


TEST(argparse_arg_parser, add_help_text)
{
    speed::iostream::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_embedded_stringstream();

    std::string help_text_1 = "world";
    std::string help_text_2 = "bye";
    std::string expected_result("hello\nworld\nbye\n");
    
    speed::argparse::arg_parser ap1("", {"-"}, {"--"}, 2u, 80u, 2u, "--help", "error", ~0u,
                            speed::argparse::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                  ~speed::argparse::apf_t::EXIT_ON_PRINT_HELP &
                                  ~speed::argparse::apf_t::PRINT_USAGE_WHEN_PRINT_HELP);
    
    ap1.add_help_text("hello");
    ap1.add_help_text(help_text_1);
    ap1.add_help_text(std::move(help_text_2));
    ap1.add_help_arg({"--help"}, "");
    ap1.print_help();
    EXPECT_TRUE(!help_text_1.empty());
    EXPECT_TRUE(help_text_2.empty());
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str() == expected_result);
}


TEST(argparse_arg_parser, add_key_arg)
{
    std::vector<std::string> keys = {"-r", "--recursive"};
    std::string description = "Recursive behavior";
    std::vector<std::string> help_menus_ids = {"--help"};
    std::string error_id = "error";
    
    speed::argparse::arg_parser ap1;
    ap1.add_key_arg(keys, description, speed::argparse::af_t::DEFAULT_ARG_FLAGS,
                    help_menus_ids, error_id);
    EXPECT_TRUE(!keys.empty());
    EXPECT_TRUE(!description.empty());
    EXPECT_TRUE(!help_menus_ids.empty());
    EXPECT_TRUE(!error_id.empty());
    EXPECT_THROW(ap1.add_key_arg({}, "...", {}), speed::argparse::argparse_exception);
    EXPECT_THROW(ap1.add_key_arg({"-r"}, "...", {}), speed::argparse::argparse_exception);
    
    speed::argparse::arg_parser ap2;
    ap2.add_key_arg(std::move(keys), std::move(description),
                    speed::argparse::af_t::DEFAULT_ARG_FLAGS,
                    std::move(help_menus_ids), std::move(error_id));
    EXPECT_TRUE(keys.empty());
    EXPECT_TRUE(description.empty());
    EXPECT_TRUE(help_menus_ids.empty());
    EXPECT_TRUE(error_id.empty());
    EXPECT_THROW(ap2.add_key_arg({}, "..."), speed::argparse::argparse_exception);
    EXPECT_THROW(ap2.add_key_arg({"-r"}, "..."), speed::argparse::argparse_exception);
}


TEST(argparse_arg_parser, add_help_arg)
{
    std::vector<std::string> keys = {"-h", "--help"};
    std::string description = "Display this help and exit";
    std::vector<std::string> help_menus_ids = {"--help"};
    std::string error_id = "error";
    
    speed::argparse::arg_parser ap1;
    ap1.add_help_arg(keys, description, speed::argparse::af_t::DEFAULT_ARG_FLAGS, help_menus_ids,
                     error_id);
    EXPECT_TRUE(!keys.empty());
    EXPECT_TRUE(!description.empty());
    EXPECT_TRUE(!help_menus_ids.empty());
    EXPECT_TRUE(!error_id.empty());
    EXPECT_THROW(ap1.add_key_arg({}, "..."), speed::argparse::argparse_exception);
    EXPECT_THROW(ap1.add_key_arg({"-h", "--help"}, "..."), speed::argparse::argparse_exception);
    
    speed::argparse::arg_parser ap2;
    ap2.add_help_arg(std::move(keys), std::move(description),
                     speed::argparse::af_t::DEFAULT_ARG_FLAGS, std::move(help_menus_ids),
                     std::move(error_id));
    EXPECT_TRUE(keys.empty());
    EXPECT_TRUE(description.empty());
    EXPECT_TRUE(help_menus_ids.empty());
    EXPECT_TRUE(error_id.empty());
    EXPECT_THROW(ap2.add_key_arg({}, "..."), speed::argparse::argparse_exception);
    EXPECT_THROW(ap2.add_key_arg({"-h", "--help"}, "..."), speed::argparse::argparse_exception);
}


TEST(argparse_arg_parser, add_version_arg)
{
    std::vector<std::string> keys = {"-v", "--version"};
    std::string description = "Display version information";
    std::vector<std::string> help_menus_ids = {"--help"};
    std::string version_information = "1.0.0";
    std::string error_id = "error";
    
    speed::argparse::arg_parser ap1;
    ap1.add_version_arg(keys, description, version_information,
                        speed::argparse::af_t::DEFAULT_ARG_FLAGS, help_menus_ids, error_id);
    EXPECT_TRUE(!keys.empty());
    EXPECT_TRUE(!description.empty());
    EXPECT_TRUE(!version_information.empty());
    EXPECT_TRUE(!help_menus_ids.empty());
    EXPECT_TRUE(!error_id.empty());
    EXPECT_THROW(ap1.add_version_arg({}, "...", "..."), speed::argparse::argparse_exception);
    EXPECT_THROW(ap1.add_version_arg({"-v", "--version"}, "...", "..."),
                 speed::argparse::argparse_exception);
    EXPECT_THROW(ap1.add_version_arg({"-?", "--???"}, "...", "..."),
                 speed::argparse::argparse_exception);
    
    speed::argparse::arg_parser ap2;
    ap2.add_version_arg(std::move(keys), std::move(description), std::move(version_information),
                        speed::argparse::af_t::DEFAULT_ARG_FLAGS, std::move(help_menus_ids),
                        std::move(error_id));
    EXPECT_TRUE(keys.empty());
    EXPECT_TRUE(description.empty());
    EXPECT_TRUE(version_information.empty());
    EXPECT_TRUE(help_menus_ids.empty());
    EXPECT_TRUE(error_id.empty());
    EXPECT_THROW(ap2.add_version_arg({}, "...", "..."), speed::argparse::argparse_exception);
    EXPECT_THROW(ap2.add_version_arg({"-v", "--version"}, "...", "..."),
                 speed::argparse::argparse_exception);
    EXPECT_THROW(ap2.add_version_arg({"-?", "--???"}, "...", "..."),
                 speed::argparse::argparse_exception);
}


TEST(argparse_arg_parser, add_gplv3_version_arg)
{
    std::vector<std::string> keys = {"-v", "--version"};
    std::string description = "Display version information";
    std::vector<std::string> help_menus_ids = {"--help"};
    std::string program_version = "1.0.0";
    std::string date = "2017";
    std::string author = "Killian";
    std::string error_id = "error";
    
    speed::argparse::arg_parser ap1;
    ap1.add_gplv3_version_arg(keys, description, program_version, date, author,
                              speed::argparse::af_t::DEFAULT_ARG_FLAGS, help_menus_ids, error_id);
    EXPECT_TRUE(!keys.empty());
    EXPECT_TRUE(!description.empty());
    EXPECT_TRUE(!program_version.empty());
    EXPECT_TRUE(!date.empty());
    EXPECT_TRUE(!author.empty());
    EXPECT_TRUE(!help_menus_ids.empty());
    EXPECT_TRUE(!error_id.empty());
    EXPECT_THROW(ap1.add_gplv3_version_arg({}, "...", "...", "...", "..."),
                 speed::argparse::argparse_exception);
    EXPECT_THROW(ap1.add_gplv3_version_arg({"-v", "--version"}, "...", "...", "...", "..."),
                 speed::argparse::argparse_exception);
    EXPECT_THROW(ap1.add_gplv3_version_arg({"-?", "--???"}, "...", "...", "...", "..."),
                 speed::argparse::argparse_exception);
    
    speed::argparse::arg_parser ap2;
    ap2.add_gplv3_version_arg(std::move(keys), std::move(description), std::move(program_version),
                              std::move(date), std::move(author),
                              speed::argparse::af_t::DEFAULT_ARG_FLAGS, std::move(help_menus_ids),
                              std::move(error_id));
    EXPECT_TRUE(keys.empty());
    EXPECT_TRUE(description.empty());
    EXPECT_TRUE(help_menus_ids.empty());
    EXPECT_TRUE(error_id.empty());
    EXPECT_THROW(ap2.add_gplv3_version_arg({}, "...", "...", "...", "..."),
                 speed::argparse::argparse_exception);
    EXPECT_THROW(ap2.add_gplv3_version_arg({"-v", "--version"}, "...", "...", "...", "..."),
                 speed::argparse::argparse_exception);
    EXPECT_THROW(ap2.add_gplv3_version_arg({"-?", "--???"}, "...", "...", "...", "..."),
                 speed::argparse::argparse_exception);
}


TEST(argparse_arg_parser, add_key_value_arg)
{
    std::vector<std::string> keys = {"-s", "--seconds"};
    std::string description = "The number of seconds";
    std::vector<speed::argparse::avt_t> values_types = {speed::argparse::avt_t::INT64};
    std::vector<std::string> regx_vector = {".*"};
    std::vector<std::string> help_menus_ids = {"--help"};
    std::string error_id = "error";
    
    speed::argparse::arg_parser ap1;
    ap1.add_key_value_arg(keys, description, values_types, 1, 1,
                          speed::argparse::af_t::DEFAULT_ARG_FLAGS, regx_vector, help_menus_ids,
                          error_id);
    EXPECT_TRUE(!keys.empty());
    EXPECT_TRUE(!description.empty());
    EXPECT_TRUE(!values_types.empty());
    EXPECT_TRUE(!regx_vector.empty());
    EXPECT_TRUE(!help_menus_ids.empty());
    EXPECT_TRUE(!error_id.empty());
    EXPECT_THROW(ap1.add_key_value_arg({}, "..."), speed::argparse::argparse_exception);
    EXPECT_THROW(ap1.add_key_value_arg({"-s"}, "..."), speed::argparse::argparse_exception);
    
    speed::argparse::arg_parser ap2;
    ap2.add_key_value_arg(std::move(keys), std::move(description), std::move(values_types), 1, 1,
                          speed::argparse::af_t::DEFAULT_ARG_FLAGS, std::move(regx_vector),
                          std::move(help_menus_ids), std::move(error_id));
    EXPECT_TRUE(keys.empty());
    EXPECT_TRUE(description.empty());
    EXPECT_TRUE(values_types.empty());
    EXPECT_TRUE(regx_vector.empty());
    EXPECT_TRUE(help_menus_ids.empty());
    EXPECT_TRUE(error_id.empty());
    EXPECT_THROW(ap2.add_key_value_arg({}, "..."), speed::argparse::argparse_exception);
    EXPECT_THROW(ap2.add_key_value_arg({"-s"}, "..."), speed::argparse::argparse_exception);
}


TEST(argparse_arg_parser, add_foreign_arg)
{
    std::string usage_key = "FILE";
    std::string help_key = "File";
    std::string description = "The file path";
    std::vector<speed::argparse::avt_t> values_types = {speed::argparse::avt_t::STRING};
    std::vector<std::string> regx_vector = {".*"};
    std::vector<std::string> help_menus_ids = {"--help"};
    std::string error_id = "error";
    
    speed::argparse::arg_parser ap1;
    ap1.add_keyless_arg(usage_key, help_key, description, values_types, 1, 1,
                        speed::argparse::af_t::DEFAULT_ARG_FLAGS, regx_vector,
                        help_menus_ids, error_id);
    EXPECT_TRUE(!usage_key.empty());
    EXPECT_TRUE(!help_key.empty());
    EXPECT_TRUE(!description.empty());
    EXPECT_TRUE(!values_types.empty());
    EXPECT_TRUE(!regx_vector.empty());
    EXPECT_TRUE(!help_menus_ids.empty());
    EXPECT_TRUE(!error_id.empty());
    EXPECT_THROW(ap1.add_keyless_arg("", "", "...", {}), speed::argparse::argparse_exception);
    EXPECT_THROW(ap1.add_keyless_arg("FILE", "...", "...", {}),
                 speed::argparse::argparse_exception);
    EXPECT_THROW(ap1.add_keyless_arg("...", "File", "...", {}),
                 speed::argparse::argparse_exception);
    
    speed::argparse::arg_parser ap2;
    ap2.add_keyless_arg(std::move(usage_key), std::move(help_key), std::move(description),
                        std::move(values_types), 1, 1, speed::argparse::af_t::DEFAULT_ARG_FLAGS,
                        std::move(regx_vector), std::move(help_menus_ids), std::move(error_id));
    EXPECT_TRUE(usage_key.empty());
    EXPECT_TRUE(help_key.empty());
    EXPECT_TRUE(description.empty());
    EXPECT_TRUE(values_types.empty());
    EXPECT_TRUE(regx_vector.empty());
    EXPECT_TRUE(help_menus_ids.empty());
    EXPECT_TRUE(error_id.empty());
    EXPECT_THROW(ap2.add_keyless_arg("", "", "..."), speed::argparse::argparse_exception);
    EXPECT_THROW(ap2.add_keyless_arg("FILE", "...", "..."), speed::argparse::argparse_exception);
    EXPECT_THROW(ap2.add_keyless_arg("...", "File", "..."), speed::argparse::argparse_exception);
}


TEST(argparse_arg_parser, add_arg_dependencies_t1)
{
    speed::argparse::arg_parser ap;
    
    ap.add_key_arg({"-r", "--recursive"}, "Recursive");
    ap.add_key_arg({"-a", "--all"}, "All");
    ap.add_key_arg({"-f", "--force"}, "Force");
    EXPECT_NO_THROW(ap.add_args_dependencies(
            {"-r", "-a"}, speed::argparse::adf_t::AT_LEAST_ONE_FOUND));
}


TEST(argparse_arg_parser, add_arg_dependencies_t2)
{
    speed::argparse::arg_parser ap;
    
    ap.add_key_arg({"-r", "--recursive"}, "Recursive");
    ap.add_key_arg({"-a", "--all"}, "All");
    ap.add_key_arg({"-f", "--force"}, "Force");
    EXPECT_THROW(ap.add_args_dependencies(
            {"-m", "-a"}, speed::argparse::adf_t::AT_LEAST_ONE_FOUND),
            speed::argparse::argparse_exception);
}


TEST(argparse_arg_parser, parse_args_t1)
{
    std::vector<const char*> argv = {};
    
    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"--seconds", "-s"}, "Set seconds", {speed::argparse::avt_t::UINT64});
    ap.add_key_value_arg({"--mintes", "-m"}, "Set minutes", {speed::argparse::avt_t::UINT64});
    ap.add_key_value_arg({"--hours", "-h"}, "Set hours", {speed::argparse::avt_t::UINT64});
    EXPECT_NO_THROW(ap.parse_args(argv.size(), argv));
}


TEST(argparse_arg_parser, parse_args_t2)
{
    std::vector<const char*> argv = {
        "speed",
        "-s", "10",
        "-m", "20",
        "-h", "30"
    };
    
    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"--seconds", "-s"}, "Set seconds", {speed::argparse::avt_t::UINT64});
    ap.add_key_value_arg({"--mintes", "-m"}, "Set minutes", {speed::argparse::avt_t::UINT64});
    ap.add_key_value_arg({"--hours", "-h"}, "Set hours", {speed::argparse::avt_t::UINT64});
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.get_front_arg_value("-s").get_value() == "10");
    EXPECT_TRUE(ap.get_front_arg_value("-m").get_value() == "20");
    EXPECT_TRUE(ap.get_front_arg_value("-h").get_value() == "30");
}


TEST(argparse_arg_parser, parse_args_t3)
{
    std::vector<const char*> argv = {
            "speed",
            "-s", "10",
            "-m", "20", "200",
            "-h", "30", "300", "3000"
    };
    
    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"--seconds", "-s"}, "Set seconds", {speed::argparse::avt_t::UINT64});
    ap.add_key_value_arg({"--mintes", "-m"}, "Set minutes", {speed::argparse::avt_t::UINT64}, 1, 2);
    ap.add_key_value_arg({"--hours", "-h"}, "Set hours", {speed::argparse::avt_t::UINT64}, 1, 3);
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.get_front_arg_value("-s").get_value() == "10");
    EXPECT_TRUE(ap.get_front_arg_value("-m").get_value() == "20");
    EXPECT_TRUE(ap.get_front_arg_value("-h").get_value() == "30");
    EXPECT_TRUE(ap.get_arg_value_at("-m", 1).get_value() == "200");
    EXPECT_TRUE(ap.get_arg_value_at("-h", 1).get_value() == "300");
    EXPECT_TRUE(ap.get_arg_value_at("-h", 2).get_value() == "3000");
}


TEST(argparse_arg_parser, parse_args_t4)
{
    std::vector<const char*> argv = {"speed", "-smh", "10", "20", "30"};
    
    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"--seconds", "-s"}, "Set seconds", {speed::argparse::avt_t::UINT64});
    ap.add_key_value_arg({"--mintes", "-m"}, "Set minutes", {speed::argparse::avt_t::UINT64});
    ap.add_key_value_arg({"--hours", "-h"}, "Set hours", {speed::argparse::avt_t::UINT64});
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.get_front_arg_value("-s").get_value() == "10");
    EXPECT_TRUE(ap.get_front_arg_value("-m").get_value() == "20");
    EXPECT_TRUE(ap.get_front_arg_value("-h").get_value() == "30");
}


TEST(argparse_arg_parser, parse_args_t5)
{
    std::vector<const char*> argv = {"speed", "-sm", "10", "-h", "30"};
    
    speed::argparse::arg_parser ap("", {"-"}, {"--"}, 2u, 80u, 2u, "--help", "error", ~0u,
                                   speed::argparse::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                   ~speed::argparse::apf_t::PRINT_ERRORS);
    ap.add_key_value_arg({"--seconds", "-s"}, "Set seconds", {speed::argparse::avt_t::UINT64});
    ap.add_key_value_arg({"--minutes", "-m"}, "Set minutes", {speed::argparse::avt_t::UINT64});
    ap.add_key_value_arg({"--hours", "-h"}, "Set hours", {speed::argparse::avt_t::UINT64});
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.arg_found("-m"));
    EXPECT_TRUE(ap.there_are_errors());
    EXPECT_TRUE(ap.get_front_arg_value("-h").get_value() == "30");
}


TEST(argparse_arg_parser, parse_args_t6)
{
    std::vector<const char*> argv = {"speed", "-n", "Mario", "-sm", "30", "2"};
    
    speed::argparse::arg_parser ap("", {"-"}, {"--"}, 2u, 80u, 2u, "--help", "error", ~0u,
                                   speed::argparse::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                   ~speed::argparse::apf_t::PRINT_ERRORS);
    ap.add_key_value_arg({"--names", "-n"}, "Set names", {speed::argparse::avt_t::STRING}, 1, 2);
    ap.add_key_value_arg({"--seconds", "-s"}, "Set seconds", {speed::argparse::avt_t::UINT64});
    ap.add_key_value_arg({"--minutes", "-m"}, "Set minutes", {speed::argparse::avt_t::UINT64});
    ap.add_key_value_arg({"--hours", "-h"}, "Set hours", {speed::argparse::avt_t::UINT64});
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(!ap.there_are_errors());
    EXPECT_TRUE(ap.get_front_arg_value("-n").get_value() == "Mario");
    EXPECT_TRUE(ap.get_front_arg_value("-s").get_value() == "30");
    EXPECT_TRUE(ap.get_front_arg_value("-m").get_value() == "2");
}


TEST(argparse_arg_parser, parse_args_t7)
{
    std::vector<const char*> argv = {
            "speed",
            "-s=10",
            "-m=20",
            "-h=30"
    };
    
    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"--seconds", "-s"}, "Set seconds", {speed::argparse::avt_t::UINT64});
    ap.add_key_value_arg({"--mintes", "-m"}, "Set minutes", {speed::argparse::avt_t::UINT64});
    ap.add_key_value_arg({"--hours", "-h"}, "Set hours", {speed::argparse::avt_t::UINT64});
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.get_front_arg_value("-s").get_value() == "10");
    EXPECT_TRUE(ap.get_front_arg_value("-m").get_value() == "20");
    EXPECT_TRUE(ap.get_front_arg_value("-h").get_value() == "30");
}


TEST(argparse_arg_parser, parse_args_t8)
{
    std::vector<const char*> argv = {
            "speed",
            ".",
            "-s", "10"
    };
    
    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"--seconds", "-s"}, "Set seconds", {speed::argparse::avt_t::UINT64});
    ap.add_keyless_arg("FILE", "File", "The file path");
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.get_front_arg_value("File").get_value() == ".");
}


TEST(argparse_arg_parser, parse_args_t9)
{
    std::vector<const char*> argv = {
            "speed",
            ".",
            "-s", "10"
    };
    
    speed::argparse::arg_parser ap("", {"-"}, {"--"}, 2u, 80u, 2u, "--help", "error", ~0u,
                                   speed::argparse::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                   ~speed::argparse::apf_t::PRINT_ERRORS);
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.error_flag_is_set(speed::argparse::apef_t::UNRECOGNIZED_ARGS_ERROR));
}


TEST(argparse_arg_parser, parse_args_t10)
{
    std::string expected_result = "speed: -r, -a: At least one of the arguments has to be found\n";
    speed::iostream::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_embedded_stringstream();
    
    std::vector<const char*> argv = {
            "speed",
            "-f"
    };
    
    speed::argparse::arg_parser ap("", {"-"}, {"--"}, 2u, 80u, 2u, "--help", "error", ~0u,
                           speed::argparse::apf_t::PRINT_ERRORS);
    
    ap.add_key_arg({"-r", "--recursive"}, "Recursive");
    ap.add_key_arg({"-a", "--all"}, "All");
    ap.add_key_arg({"-f", "--force"}, "Force");
    ap.add_args_dependencies({"-r", "-a"}, speed::argparse::adf_t::AT_LEAST_ONE_FOUND);
    
    ap.parse_args(argv.size(), argv);
    
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str() == expected_result);
}


TEST(argparse_arg_parser, parse_args_t11)
{
    std::string expected_result;
    speed::iostream::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_embedded_stringstream();
    
    std::vector<const char*> argv = {
            "speed",
            "-r"
    };
    
    speed::argparse::arg_parser ap("", {"-"}, {"--"}, 2u, 80u, 2u, "--help", "error", ~0u,
                           speed::argparse::apf_t::PRINT_ERRORS);
    
    ap.add_key_arg({"-r", "--recursive"}, "Recursive");
    ap.add_key_arg({"-a", "--all"}, "All");
    ap.add_key_arg({"-f", "--force"}, "Force");
    ap.add_args_dependencies({"-r", "-a"}, speed::argparse::adf_t::AT_LEAST_ONE_FOUND);
    
    ap.parse_args(argv.size(), argv);
    
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str() == expected_result);
}


TEST(argparse_arg_parser, reset_args_parse)
{
    std::vector<const char*> argv = {
            "speed",
            "/home/user/Desktop/file.cpp",
            "-s", "10"
    };
    
    speed::argparse::arg_parser ap("", {"-"}, {"--"}, 2u, 80u, 2u, "--help", "error", ~0u,
                                   speed::argparse::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                   ~speed::argparse::apf_t::PRINT_ERRORS);
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.error_flag_is_set(speed::argparse::apef_t::UNRECOGNIZED_ARGS_ERROR));
    ap.reset_args_parse();
    EXPECT_TRUE(!ap.error_flag_is_set(speed::argparse::apef_t::UNRECOGNIZED_ARGS_ERROR));
}


TEST(argparse_arg_parser, arg_found)
{
    std::vector<const char*> argv = {"speed", "-a"};
    
    speed::argparse::arg_parser ap;
    ap.add_key_arg({"-a", "--all"}, "Print all");
    ap.add_key_arg({"-l", "--long"}, "Long format");
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.arg_found("-a"));
    EXPECT_TRUE(!ap.arg_found("-l"));
}


TEST(argparse_arg_parser, help_arg_found)
{
    std::vector<const char*> argv1 = {"speed", "--help"};
    std::vector<const char*> argv2 = {"speed", "--all"};
    
    speed::argparse::arg_parser ap("", {"-"}, {"--"}, 2u, 80u, 2u, "--help", "error", ~0u,
                                   speed::argparse::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                   ~speed::argparse::apf_t::PRINT_HELP_WHEN_HELP_ARG_FOUND &
                                   ~speed::argparse::apf_t::PRINT_VERSION_WHEN_VERSION_ARG_FOUND);
    ap.add_help_arg({"--help"}, "Display this help and exit");
    ap.add_key_arg({"--all"}, "Display version information");
    ap.parse_args(argv1.size(), argv1);
    EXPECT_TRUE(ap.help_arg_found());
    ap.parse_args(argv2.size(), argv2);
    EXPECT_TRUE(!ap.help_arg_found());
}


TEST(argparse_arg_parser, version_arg_found)
{
    std::vector<const char*> argv1 = {"speed", "--all"};
    std::vector<const char*> argv2 = {"speed", "--version"};
    
    speed::argparse::arg_parser ap("", {"-"}, {"--"}, 2u, 80u, 2u, "--help", "error", ~0u,
                                   speed::argparse::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                   ~speed::argparse::apf_t::PRINT_HELP_WHEN_HELP_ARG_FOUND &
                                   ~speed::argparse::apf_t::PRINT_VERSION_WHEN_VERSION_ARG_FOUND);
    ap.add_key_arg({"--all"}, "Display this help and exit");
    ap.add_version_arg({"--version"}, "Display version information", "1.0.0");
    ap.parse_args(argv1.size(), argv1);
    EXPECT_TRUE(!ap.version_arg_found());
    ap.parse_args(argv2.size(), argv2);
    EXPECT_TRUE(ap.version_arg_found());
}


TEST(argparse_arg_parser, get_front_arg_value_1)
{
    std::vector<const char*> argv = {"speed", "-s", "30"};
    
    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds to set", {speed::argparse::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {speed::argparse::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_THROW(ap.get_front_arg_value("-h"), speed::argparse::argparse_exception);
    EXPECT_THROW(ap.get_front_arg_value("-m"), speed::argparse::argparse_exception);
    EXPECT_TRUE(!ap.get_front_arg_value("-s").there_are_errors());
}


TEST(argparse_arg_parser, get_front_arg_value_2)
{
    std::vector<const char*> argv = {"speed", "-s", "30"};
    
    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds to set", {speed::argparse::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {speed::argparse::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_THROW(ap.get_front_arg_value("-h", "30"), speed::argparse::argparse_exception);
    EXPECT_TRUE(ap.get_front_arg_value("-m", "20").as<int>() == 20);
    EXPECT_TRUE(ap.get_front_arg_value("-s", "10").as<int>() == 30);
}


TEST(argparse_arg_parser, get_front_arg_value_as_1)
{
    std::vector<const char*> argv = {"speed", "-s", "30"};
    
    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds to set", {speed::argparse::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {speed::argparse::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_THROW(ap.get_front_arg_value_as<int>("-h"), speed::argparse::argparse_exception);
    EXPECT_THROW(ap.get_front_arg_value_as<int>("-m"), speed::argparse::argparse_exception);
    EXPECT_TRUE(ap.get_front_arg_value_as<int>("-s") == 30);
}


TEST(argparse_arg_parser, get_front_arg_value_as_2)
{
    std::vector<const char*> argv = {"speed", "-s", "30"};
    
    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds to set", {speed::argparse::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {speed::argparse::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_THROW(ap.get_front_arg_value_as<int>("-h", 30), speed::argparse::argparse_exception);
    EXPECT_TRUE(ap.get_front_arg_value_as<int>("-m", 20) == 20);
    EXPECT_TRUE(ap.get_front_arg_value_as<int>("-s", 10) == 30);
}


TEST(argparse_arg_parser, get_arg_value_at_1)
{
    std::vector<const char*> argv = {"speed", "-s", "30"};

    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds to set", {speed::argparse::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {speed::argparse::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_THROW(ap.get_arg_value_at("-h", 0), speed::argparse::argparse_exception);
    EXPECT_THROW(ap.get_arg_value_at("-m", 1), speed::argparse::argparse_exception);
    EXPECT_TRUE(!ap.get_arg_value_at("-s", 0).there_are_errors());
}


TEST(argparse_arg_parser, get_arg_value_at_2)
{
    std::vector<const char*> argv = {"speed", "-s", "30"};

    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds to set", {speed::argparse::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {speed::argparse::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_THROW(ap.get_arg_value_at("-h", 0, "30"), speed::argparse::argparse_exception);
    EXPECT_TRUE(ap.get_arg_value_at("-m", 1, "20").as<int>() == 20);
    EXPECT_TRUE(ap.get_arg_value_at("-s", 0, "10").as<int>() == 30);
}


TEST(argparse_arg_parser, get_arg_value_at_as_1)
{
    std::vector<const char*> argv = {"speed", "-s", "30"};
    
    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds to set", {speed::argparse::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {speed::argparse::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_THROW(ap.get_arg_value_at_as<int>("-h", 0), speed::argparse::argparse_exception);
    EXPECT_THROW(ap.get_arg_value_at_as<int>("-m", 1), speed::argparse::argparse_exception);
    EXPECT_TRUE(ap.get_arg_value_at_as<int>("-s", 0) == 30);
}


TEST(argparse_arg_parser, get_arg_value_at_as_2)
{
    std::vector<const char*> argv = {"speed", "-s", "30"};
    
    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds to set", {speed::argparse::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {speed::argparse::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_THROW(ap.get_arg_value_at_as<int>("-h", 0, 30), speed::argparse::argparse_exception);
    EXPECT_TRUE(ap.get_arg_value_at_as<int>("-m", 1, 20) == 20);
    EXPECT_TRUE(ap.get_arg_value_at_as<int>("-s", 0, 10) == 30);
}


TEST(argparse_arg_parser, get_arg_values)
{
    std::vector<const char*> argv = {"speed", "-s", "30"};

    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds to set", {speed::argparse::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {speed::argparse::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_THROW(ap.get_arg_values("-h"), speed::argparse::argparse_exception);
    EXPECT_TRUE(ap.get_arg_values("-s").size() == 1);
    EXPECT_TRUE(ap.get_arg_values("-s").front().get_value() == "30");
}


TEST(argparse_arg_parser, get_arg_values_as_1)
{
    std::vector<const char*> argv = {"speed", "-s", "30"};
    
    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds to set", {speed::argparse::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {speed::argparse::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_THROW(ap.get_arg_values("-h"), speed::argparse::argparse_exception);
    EXPECT_TRUE(ap.get_arg_values_as<int>("-s").size() == 1);
    EXPECT_TRUE(ap.get_arg_values_as<int>("-s").front() == 30);
}


TEST(argparse_arg_parser, get_arg_values_as_2)
{
    std::vector<const char*> argv = {"speed", "-d", "."};
    
    speed::argparse::arg_parser ap;
    ap.add_key_value_arg({"-d"}, "Seconds to set", {speed::argparse::avt_t::R_DIR});
    ap.add_key_value_arg({"-f"}, "Minutes to set", {speed::argparse::avt_t::R_DIR});
    ap.parse_args(argv.size(), argv);
    EXPECT_THROW(ap.get_arg_values("-h"), speed::argparse::argparse_exception);
    EXPECT_TRUE(ap.get_arg_values_as<std::filesystem::path>("-d").size() == 1);
    EXPECT_TRUE(ap.get_arg_values_as<std::filesystem::path>("-d").front() == ".");
}


TEST(argparse_arg_parser, flag_is_raised)
{
    speed::argparse::arg_parser ap("", {"-"}, {"--"}, 2u, 80u, 2u, "--help", "error", ~0u,
                         speed::argparse::apf_t::PRINT_HELP_WHEN_HELP_ARG_FOUND);
    EXPECT_TRUE(ap.flag_is_set(speed::argparse::apf_t::PRINT_HELP_WHEN_HELP_ARG_FOUND));
    EXPECT_TRUE(!ap.flag_is_set(speed::argparse::apf_t::PRINT_VERSION_WHEN_VERSION_ARG_FOUND));
}


TEST(argparse_arg_parser, error_flag_is_raised)
{
    std::vector<const char*> argv = {"speed", "-h", "30"};

    speed::argparse::arg_parser ap("", {"-"}, {"--"}, 2u, 80u, 2u, "--help", "error", ~0u,
                                   speed::argparse::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                   ~speed::argparse::apf_t::PRINT_ERRORS);
    ap.add_key_value_arg({"-s"}, "Seconds to set", {speed::argparse::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {speed::argparse::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.error_flag_is_set(speed::argparse::apef_t::UNRECOGNIZED_ARGS_ERROR));
}


TEST(argparse_arg_parser, there_are_errors)
{
    std::vector<const char*> argv = {"speed", "-h", "30"};

    speed::argparse::arg_parser ap("", {"-"}, {"--"}, 2u, 80u, 2u, "--help", "error", ~0u,
                                   speed::argparse::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                   ~speed::argparse::apf_t::PRINT_ERRORS);
    ap.add_key_value_arg({"-s"}, "Seconds to set", {speed::argparse::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {speed::argparse::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.there_are_errors());
}


TEST(argparse_arg_parser, print_help)
{
    speed::iostream::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_embedded_stringstream();
    
    std::vector<const char*> argv = {"speed", "--help"};
    
    speed::argparse::arg_parser ap("", {"-"}, {"--"}, 2u, 80u, 2u, "--help", "error", ~0u,
                                   speed::argparse::apf_t::PRINT_ARGS_ID_WHEN_PRINT_HELP);
    ap.add_key_value_arg({"-s"}, "Seconds to set", {speed::argparse::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {speed::argparse::avt_t::INT32});
    ap.add_help_arg({"--help"}, "Display this help and exit.");
    ap.add_keyless_arg("FILE", "File", "", {speed::argparse::avt_t::R_REG_FILE}, 1u, ~0u);
    ap.parse_args(argv.size(), argv);
    EXPECT_NO_THROW(ap.print_help());
}

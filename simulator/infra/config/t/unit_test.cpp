/**
 * Unit tests for configuration parser
 * @author Denis Los
 */

// Google test library
#include <gtest/gtest.h>

// Testing module
#include "../config.h"

// Utils
#include "infra/macro.h"

namespace config {
    AliasedRequiredValue<std::string> string_config = { "b", "string_config_name", "string config description"};
    AliasedRequiredValue<uint64> uint64_config = { "n", "uint64_config_name", "uint64 config description"};

    AliasedSwitch bool_config_1 = { "d", "bool_config_1", "first bool config description"};
    AliasedSwitch bool_config_2 = { "f", "bool_config_2", "second bool config description"};
} // namespace config

//
// To check whether the returned values
// are equal to passed arguments
//
TEST( config_parse, Pass_Valid_Args_1)
{
    const uint64 mandatory_int_value = 145;
    const std::string mandatory_string_value{ "file.elf"};

    const char* argv[] =
    {
        "mipt-mips",
        "-b", "file.elf",
        "-n", "145",
        "-f"
    };
    const int argc = countof(argv);

    // should not throw any exceptions
    ASSERT_NO_THROW( config::handleArgs( argc, argv));

    ASSERT_EQ( config::uint64_config, mandatory_int_value);
    ASSERT_FALSE( mandatory_string_value.compare( config::string_config));
    ASSERT_EQ( config::bool_config_1, false);
    ASSERT_EQ( config::bool_config_2, true);
}

//
// To check whether returned values
// are equal to passed arguments
//
TEST( config_parse,  Pass_Valid_Args_2)
{
    const uint64 mandatory_int_value = 356;
    const std::string mandatory_string_value{ "run_test.elf"};

    const char* argv[] =
    {
        "mipt-mips",
        "-b", "run_test.elf",
        "-n", "356",
        "-d"
    };
    const int argc = countof(argv);

    // should not throw any exceptions
    ASSERT_NO_THROW( config::handleArgs( argc, argv));

    ASSERT_EQ( config::uint64_config, mandatory_int_value);
    ASSERT_FALSE( mandatory_string_value.compare( config::string_config));
    ASSERT_EQ( config::bool_config_1, true);
    ASSERT_EQ( config::bool_config_2, false);
}

//
// Pass no arguments
//
TEST( config_parse, Pass_No_Args)
{
    const char* argv[] =
    {
        "mipt-mips"
    };
    const int argc = countof(argv);

    // should throw
    ASSERT_THROW( config::handleArgs( argc, argv), std::exception);
}

//
// Pass arguments without a string_config_name option
//
TEST( config_parse, Pass_Args_Without_Binary_Option)
{
    const char* argv[] =
    {
        "mipt-mips",
        "--uint64_config_name", "356",
    };
    const int argc = countof(argv);
    
    // should throw
    ASSERT_THROW( config::handleArgs( argc, argv), std::exception);
}

//
// Pass arguments without a numsteps option
//
TEST( config_parse,  Pass_Args_Without_Numsteps_Option)
{
    const char* argv[] =
    {
        "mipt-mips",
        "--string_config_name", "test.elf", 
    };
    const int argc = countof(argv);

    // should throw
    ASSERT_THROW( config::handleArgs( argc, argv), std::exception);
}

//
// Pass arguments with unrecognised option
//
TEST( config_parse, Pass_Args_With_Unrecognised_Option)
{
    const char* argv[] =
    {
        "mipt-mips",
        "--string_config_name", "test.elf",
        "-n", "356",
        "-koption"
    };
    const int argc = countof(argv);

    // should throw
    ASSERT_THROW( config::handleArgs( argc, argv), std::exception);
}

#if 0
//
// Pass a binary option multiple times
//
TEST( config_parse,  Pass_Binary_Option_Multiple_Times)
{
    const char* argv[] =
    {
        "mipt-mips",
        "-b", "run_test_1.elf",
        "--string_config_name", "run_test_2.elf",
        "-n", "412",
    };
    const int argc = countof(argv);
    
    // should throw
    ASSERT_THROW( config::handleArgs( argc, argv), std::exception);
}
#endif

//
// Pass a binary option without an argument
//
TEST( config_parse,  Pass_Binary_Option_Without_Arg)
{
    const char* argv[] =
    {
        "mipt-mips",
        "-b",
        "-n", "412",
    };
    const int argc = countof(argv);

    // should throw
    ASSERT_THROW( config::handleArgs( argc, argv), std::exception);
}

//
// Pass a numsteps option without an argument
//
TEST( config_parse,  Pass_Numsteps_Option_Without_Arg)
{
    const char* argv[] =
    {
        "mipt-mips",
        "-b", "run_test.elf",
        "-n",
        "-f",
        "-d"
    };
    const int argc = countof(argv);

    // should exit with EXIT_FAILURE
    ASSERT_THROW( config::handleArgs( argc, argv), std::exception);
}

#if 0
//
// To check whether providing configuration parser
// with the same option is a failure
//
TEST( config_provide_options, Provide_Config_Parser_With_Binary_Option_Twice)
{
    const char* argv[] =
    {
        "mipt-mips",
        "-b", "test.elf",
        "-n", "100"
    };
    const int argc = countof(argv);

    // should not throw any exceptions
    ASSERT_NO_THROW( config::handleArgs( argc, argv));


    auto test_function = []()
    {
        config::RequiredValue<std::string> second_binary_file_option =
            {
                "string_config_name,b",
                "input string_config_name file"
            };
    };

    // should exit with EXIT_FAILURE
    ASSERT_EXIT( test_function(), ::testing::ExitedWithCode( EXIT_FAILURE), "ERROR.*");
}
#endif

int main( int argc, char** argv)
{
    ::testing::InitGoogleTest( &argc, argv);
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    return RUN_ALL_TESTS();
}


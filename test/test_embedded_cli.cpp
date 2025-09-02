#include <gtest/gtest.h>

extern "C" {
#include "embedded_cli.h"
}

TEST(EmbeddedCliTest, Init) {
    embedded_cli_init(128);
}

bool test_command_executed = false;
void test_command_handler() {
    test_command_executed = true;
}

TEST(EmbeddedCliTest, CommandParsing) {
    embedded_cli_init(128);

    cli_command_t test_command = {
        .name = "test_command",
        .handler = test_command_handler,
        .args = NULL,
        .arg_count = 0
    };

    cli_command_t commands[] = { test_command };
    embedded_cli_register_commands(commands, 1);

    test_command_executed = false;
    embedded_cli_process("test_command");
    ASSERT_TRUE(test_command_executed);
}

int parsed_int_arg = 0;
char parsed_string_arg[128] = {0};
float parsed_float_arg = 0.0f;

void arg_test_command_handler() {
    // This handler isn't strictly needed for this test, but it's good practice
}

TEST(EmbeddedCliTest, ArgumentParsing) {
    embedded_cli_init(128);

    cli_arg_t args[] = {
        {
            .name = "--int_arg",
            .type = CLI_ARG_TYPE_INT,
            .value = &parsed_int_arg
        },
        {
            .name = "--string_arg",
            .type = CLI_ARG_TYPE_STRING,
            .value = parsed_string_arg
        }
    };

    cli_command_t test_command = {
        .name = "arg_test_command",
        .handler = arg_test_command_handler,
        .args = args,
        .arg_count = 2
    };

    cli_command_t commands[] = { test_command };
    embedded_cli_register_commands(commands, 1);

    parsed_int_arg = 0;
    memset(parsed_string_arg, 0, sizeof(parsed_string_arg));

    embedded_cli_process("arg_test_command --int_arg 42 --string_arg \"hello world\"");

    ASSERT_EQ(parsed_int_arg, 42);
    ASSERT_STREQ(parsed_string_arg, "hello world");
}

TEST(EmbeddedCliTest, HelpCommand) {
    embedded_cli_init(128);

    cli_command_t test_command = {
        .name = "test_command",
        .handler = test_command_handler,
        .args = NULL,
        .arg_count = 0
    };

    cli_command_t commands[] = { test_command };
    embedded_cli_register_commands(commands, 1);

    // Redirect stdout to a string stream to capture the output
    testing::internal::CaptureStdout();
    embedded_cli_process("help");
    std::string output = testing::internal::GetCapturedStdout();

    ASSERT_NE(output.find("Available commands:"), std::string::npos);
    ASSERT_NE(output.find("test_command"), std::string::npos);
    ASSERT_NE(output.find("help"), std::string::npos);
}

TEST(EmbeddedCliTest, ArgumentTruncation) {
    const int max_len = 16; // Buffer size of 16 can hold 15 chars + null
    embedded_cli_init(max_len);

    char truncated_string_arg[max_len] = {0};

    cli_arg_t args[] = {
        {
            .name = "--string_arg",
            .type = CLI_ARG_TYPE_STRING,
            .value = truncated_string_arg
        }
    };

    cli_command_t test_command = {
        .name = "trunc_test",
        .handler = arg_test_command_handler,
        .args = args,
        .arg_count = 1
    };

    cli_command_t commands[] = { test_command };
    // Suppress help output for this test
    testing::internal::CaptureStdout();
    embedded_cli_register_commands(commands, 1);
    testing::internal::GetCapturedStdout();

    memset(truncated_string_arg, 0, sizeof(truncated_string_arg));

    embedded_cli_process("trunc_test --string_arg \"this is a very long string that should be truncated\"");

    // The buffer is 16 bytes, so it should hold the first 15 characters.
    ASSERT_STREQ(truncated_string_arg, "this is a very ");
}

TEST(EmbeddedCliTest, CommandNameTruncation) {
    const int max_len = 8;
    embedded_cli_init(max_len);

    cli_command_t test_command = {
        .name = "short",
        .handler = test_command_handler,
        .args = NULL,
        .arg_count = 0
    };

    cli_command_t commands[] = { test_command };
    testing::internal::CaptureStdout();
    embedded_cli_register_commands(commands, 1);
    testing::internal::GetCapturedStdout();

    test_command_executed = false;
    // This command name is longer than max_len, it should not be found.
    embedded_cli_process("shortcommandthatistoolong");
    ASSERT_FALSE(test_command_executed);

    // This command name should be found.
    embedded_cli_process("short");
    ASSERT_TRUE(test_command_executed);
}

TEST(EmbeddedCliTest, FloatArgumentParsing) {
    embedded_cli_init(128);

    cli_arg_t args[] = {
        {
            .name = "--float_arg",
            .type = CLI_ARG_TYPE_FLOAT,
            .value = &parsed_float_arg
        }
    };

    cli_command_t test_command = {
        .name = "float_test",
        .handler = arg_test_command_handler,
        .args = args,
        .arg_count = 1
    };

    cli_command_t commands[] = { test_command };
    embedded_cli_register_commands(commands, 1);

    parsed_float_arg = 0.0f;

    embedded_cli_process("float_test --float_arg 3.14");

    ASSERT_FLOAT_EQ(parsed_float_arg, 3.14f);
}
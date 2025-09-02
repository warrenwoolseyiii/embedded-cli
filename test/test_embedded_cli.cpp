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
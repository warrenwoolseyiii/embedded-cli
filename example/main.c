#include <stdio.h>
#include <string.h>
#include "embedded_cli.h"

int int_value = 0;
char string_value[128] = {0};

void print_args_handler() {
    printf("Received arguments:\n");
    printf("  --int_arg: %d\n", int_value);
    printf("  --string_arg: %s\n", string_value);
}

int main() {
    embedded_cli_init(128);

    cli_arg_t args[] = {
        {
            .name = "--int_arg",
            .type = CLI_ARG_TYPE_INT,
            .value = &int_value
        },
        {
            .name = "--string_arg",
            .type = CLI_ARG_TYPE_STRING,
            .value = string_value
        }
    };

    cli_command_t print_command = {
        .name = "print_args",
        .handler = print_args_handler,
        .args = args,
        .arg_count = 2
    };

    cli_command_t commands[] = { print_command };
    embedded_cli_register_commands(commands, 1);

    const char* command_string = "print_args --int_arg 123 --string_arg \"hello from example\"";
    printf("Processing command: %s\n", command_string);
    embedded_cli_process(command_string);

    return 0;
}
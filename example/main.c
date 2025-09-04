#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "embedded_cli.h"

int int_value = 0;
char string_value[128] = {0};
float float_value = 0.0f;
bool should_quit = false;

void print_args_handler() {
    printf("Received arguments:\n");
    printf("  --int_arg: %d\n", int_value);
    printf("  --string_arg: %s\n", string_value);
    printf("  --float_arg: %f\n", float_value);
}

void quit_handler() {
    printf("Exiting.\n");
    should_quit = true;
}

int main() {
    embedded_cli_init(128, printf);

    cli_arg_t print_args[] = {
        {
            .name = "--int_arg",
            .type = CLI_ARG_TYPE_INT,
            .value = &int_value
        },
        {
            .name = "--string_arg",
            .type = CLI_ARG_TYPE_STRING,
            .value = string_value
        },
        {
            .name = "--float_arg",
            .type = CLI_ARG_TYPE_FLOAT,
            .value = &float_value
        }
    };

    cli_command_t commands[] = {
        {
            .name = "print_args",
            .handler = print_args_handler,
            .args = print_args,
            .arg_count = 3
        },
        {
            .name = "quit",
            .handler = quit_handler,
            .args = NULL,
            .arg_count = 0
        }
    };
    embedded_cli_register_commands(commands, 2);

    char input_buffer[256];

    while (!should_quit) {
        printf("> ");
        fflush(stdout);

        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }

        // Remove trailing newline
        input_buffer[strcspn(input_buffer, "\r\n")] = 0;

        if (strlen(input_buffer) > 0) {
            embedded_cli_process(input_buffer);
        }
    }

    return 0;
}
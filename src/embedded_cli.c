#include "embedded_cli.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

static uint16_t max_arg_length = 0;
static const cli_command_t* registered_commands = NULL;
static uint8_t registered_command_count = 0;
static int (*printf)(const char *format, ...) = NULL;

static void print_help() {
    printf("Available commands:\n");
    for (uint8_t i = 0; i < registered_command_count; i++) {
        printf("  %s", registered_commands[i].name);
        for (uint8_t j = 0; j < registered_commands[i].arg_count; j++) {
            const char* arg_type_str = "unknown";
            if (registered_commands[i].args[j].type == CLI_ARG_TYPE_INT) {
                arg_type_str = "<int>";
            } else if (registered_commands[i].args[j].type == CLI_ARG_TYPE_STRING) {
                arg_type_str = "<string>";
            } else if (registered_commands[i].args[j].type == CLI_ARG_TYPE_FLOAT) {
                arg_type_str = "<float>";
            }
            printf(" %s %s", registered_commands[i].args[j].name, arg_type_str);
        }
        printf("\n");
    }
    printf("  help\n");
}

void embedded_cli_init(uint16_t new_max_arg_length, int (*user_print_function)(const char *format, ...)) {
    max_arg_length = new_max_arg_length;
    printf = user_print_function;
}

void embedded_cli_register_commands(const cli_command_t* commands, uint8_t command_count) {
    if (commands == NULL) {
        return;
    }
    registered_commands = commands;
    registered_command_count = command_count;
    print_help();
}

void embedded_cli_process(const char* command_string) {
    if (command_string == NULL) {
        return;
    }

    if (strcmp(command_string, "help") == 0) {
        print_help();
        return;
    }

    char temp_buffer[max_arg_length];
    const char* p = command_string;
    int buffer_idx = 0;

    // Extract command name
    while (*p != ' ' && *p != '\0') {
        if (buffer_idx < max_arg_length - 1) {
            temp_buffer[buffer_idx++] = *p;
        }
        p++;
    }
    temp_buffer[buffer_idx] = '\0';

    const cli_command_t* command = NULL;
    for (uint8_t i = 0; i < registered_command_count; i++) {
        if (strcmp(temp_buffer, registered_commands[i].name) == 0) {
            command = &registered_commands[i];
            break;
        }
    }

    if (command == NULL) {
        return; // Command not found
    }

    // Parse arguments
    uint8_t parsed_arg_count = 0;
    while (*p != '\0') {
        while (*p == ' ') p++; // Skip whitespace
        if (*p == '\0') break;

        buffer_idx = 0;
        while (*p != ' ' && *p != '\0') {
            if (buffer_idx < max_arg_length - 1) {
                temp_buffer[buffer_idx++] = *p;
            }
            p++;
        }
        temp_buffer[buffer_idx] = '\0';

        for (uint8_t i = 0; i < command->arg_count; i++) {
            if (strcmp(temp_buffer, command->args[i].name) == 0) {
                parsed_arg_count++;
                const cli_arg_t* arg = &command->args[i];
                while (*p == ' ') p++; // Skip whitespace

                if (arg->type == CLI_ARG_TYPE_INT) {
                    buffer_idx = 0;
                    while (*p != ' ' && *p != '\0') {
                        if (buffer_idx < max_arg_length - 1) {
                            temp_buffer[buffer_idx++] = *p;
                        }
                        p++;
                    }
                    temp_buffer[buffer_idx] = '\0';
                    if (arg->value) {
                        *(int*)arg->value = atoi(temp_buffer);
                    }
                } else if (arg->type == CLI_ARG_TYPE_STRING) {
                    if (*p == '"') {
                        p++; // Skip opening quote
                        buffer_idx = 0;
                        while (*p != '"' && *p != '\0') {
                            if (buffer_idx < max_arg_length - 1) {
                                ((char*)arg->value)[buffer_idx++] = *p;
                            }
                            p++;
                        }
                        ((char*)arg->value)[buffer_idx] = '\0';
                        if (*p == '"') p++; // Skip closing quote
                    }
                } else if (arg->type == CLI_ARG_TYPE_FLOAT) {
                    buffer_idx = 0;
                    while (*p != ' ' && *p != '\0') {
                        if (buffer_idx < max_arg_length - 1) {
                            temp_buffer[buffer_idx++] = *p;
                        }
                        p++;
                    }
                    temp_buffer[buffer_idx] = '\0';
                    if (arg->value) {
                        *(float*)arg->value = atof(temp_buffer);
                    }
                }
                break;
            }
        }
    }

    if (parsed_arg_count != command->arg_count) {
        printf("Error: Incomplete arguments for command '%s'\n", command->name);
        print_help();
        return;
    }

    if (command->handler) {
        command->handler();
    }
}

#include "embedded_cli.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

static uint16_t max_arg_length = 0;
static const cli_command_t* registered_commands = NULL;
static uint8_t registered_command_count = 0;

void embedded_cli_init(uint16_t new_max_arg_length) {
    max_arg_length = new_max_arg_length;
}

void embedded_cli_register_commands(const cli_command_t* commands, uint8_t command_count) {
    registered_commands = commands;
    registered_command_count = command_count;
}

void embedded_cli_process(const char* command_string) {
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
                }
                break;
            }
        }
    }

    if (command->handler) {
        command->handler();
    }
}

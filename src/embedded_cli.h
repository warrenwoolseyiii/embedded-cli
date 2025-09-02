#ifndef EMBEDDED_CLI_H
#define EMBEDDED_CLI_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Enum for argument types
 */
typedef enum {
    CLI_ARG_TYPE_INT,
    CLI_ARG_TYPE_STRING,
} cli_arg_type_t;

/**
 * @brief Structure for a command argument
 */
typedef struct {
    const char* name;
    cli_arg_type_t type;
    void* value;
} cli_arg_t;

/**
 * @brief Function pointer for command handler
 */
typedef void (*cli_command_handler_t)(void);

/**
 * @brief Structure for a command
 */
typedef struct {
    const char* name;
    cli_command_handler_t handler;
    cli_arg_t* args;
    uint8_t arg_count;
} cli_command_t;

#endif // EMBEDDED_CLI_H
/**
 * @brief Initializes the embedded CLI.
 *
 * @param max_arg_length The maximum length for a single argument string.
 */
void embedded_cli_init(uint16_t max_arg_length);

/**
 * @brief Registers the commands with the CLI.
 *
 * @param commands An array of command structures.
 * @param command_count The number of commands in the array.
 */
void embedded_cli_register_commands(const cli_command_t* commands, uint8_t command_count);

/**
 * @brief Processes an incoming command string.
 *
 * @param command_string The null-terminated command string to process.
 */
void embedded_cli_process(const char* command_string);
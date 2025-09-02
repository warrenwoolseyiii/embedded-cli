# Embedded CLI

A lightweight, configurable command-line interface (CLI) library for embedded systems.

## Features

*   Minimal resource footprint, suitable for microcontrollers.
*   Configurable command and argument handling.
*   Tagged argument parsing for both integer and string types.
*   Built-in help command (`help`) that automatically lists all registered commands.
*   Test-driven development with Google Test.
*   CMake build system for easy compilation.

## Building the Project

The project uses CMake to manage the build process.

```bash
mkdir build
cd build
cmake ..
make
```

## Running the Unit Tests

The unit tests are built along with the library and can be run using `ctest`.

```bash
cd build
ctest
```

## Running the Example

An interactive example application is provided in the `example` directory. It demonstrates how to register commands and process input from `stdin`.

```bash
cd build
./example/example_app
```

## API Usage

### `embedded_cli.h`

The main header file contains the public API for the library.

#### Data Structures

*   `cli_arg_type_t`: Enum for argument types (`CLI_ARG_TYPE_INT`, `CLI_ARG_TYPE_STRING`).
*   `cli_arg_t`: Structure representing a command argument.
*   `cli_command_t`: Structure representing a CLI command.

#### Functions

*   `void embedded_cli_init(uint16_t max_arg_length)`: Initializes the CLI library.
*   `void embedded_cli_register_commands(const cli_command_t* commands, uint8_t command_count)`: Registers an array of commands with the CLI and prints a list of available commands.
*   `void embedded_cli_process(const char* command_string)`: Processes an incoming command string. Handles the built-in `help` command.

### Example

```c
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "embedded_cli.h"

int int_value = 0;
char string_value[128] = {0};
bool should_quit = false;

void print_args_handler() {
    printf("Received arguments:\n");
    printf("  --int_arg: %d\n", int_value);
    printf("  --string_arg: %s\n", string_value);
}

void quit_handler() {
    printf("Exiting.\n");
    should_quit = true;
}

int main() {
    embedded_cli_init(128);

    cli_arg_t print_args[] = {
        { "--int_arg", CLI_ARG_TYPE_INT, &int_value },
        { "--string_arg", CLI_ARG_TYPE_STRING, string_value }
    };

    cli_command_t commands[] = {
        { "print_args", print_args_handler, print_args, 2 },
        { "quit", quit_handler, NULL, 0 }
    };
    embedded_cli_register_commands(commands, 2);

    char input_buffer[256];

    while (!should_quit) {
        printf("> ");
        fflush(stdout);

        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }

        input_buffer[strcspn(input_buffer, "\r\n")] = 0;

        if (strlen(input_buffer) > 0) {
            embedded_cli_process(input_buffer);
        }
    }

    return 0;
}

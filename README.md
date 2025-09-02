# Embedded CLI

A lightweight, configurable command-line interface (CLI) library for embedded systems.

## Features

*   Minimal resource footprint, suitable for microcontrollers.
*   Configurable command and argument handling.
*   Tagged argument parsing for both integer and string types.
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

An example application is provided in the `example` directory.

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
*   `void embedded_cli_register_commands(const cli_command_t* commands, uint8_t command_count)`: Registers an array of commands with the CLI.
*   `void embedded_cli_process(const char* command_string)`: Processes an incoming command string.

### Example

```c
#include <stdio.h>
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
        { "--int_arg", CLI_ARG_TYPE_INT, &int_value },
        { "--string_arg", CLI_ARG_TYPE_STRING, string_value }
    };

    cli_command_t print_command = { "print_args", print_args_handler, args, 2 };
    cli_command_t commands[] = { print_command };
    embedded_cli_register_commands(commands, 1);

    const char* command_string = "print_args --int_arg 123 --string_arg \"hello from example\"";
    embedded_cli_process(command_string);

    return 0;
}

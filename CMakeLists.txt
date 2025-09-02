cmake_minimum_required(VERSION 3.14)
project(embedded-cli C)

enable_testing()

include(FetchContent)

# Add the src directory to the include path
include_directories(src)

# Add the library
add_library(embedded_cli src/embedded_cli.c)

# Add the test directory
add_subdirectory(test)

# Add the example directory
add_subdirectory(example)
cmake_minimum_required(VERSION 3.10)
project(embedded-cli-example C)

# Include the parent directory to find embedded_cli.h
include_directories(../src)

# Add the example executable
add_executable(example_app main.c)

# Link the libraries
target_link_libraries(example_app embedded_cli)
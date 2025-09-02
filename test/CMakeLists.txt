project(embedded-cli-tests CXX)

# Include GTest using FetchContent
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Include the parent directory to find embedded_cli.h
include_directories(../src)

# Add the test executable
add_executable(run_tests test_embedded_cli.cpp)

# Link the libraries
target_link_libraries(run_tests embedded_cli GTest::gtest_main)

# Add test
include(GoogleTest)
gtest_discover_tests(run_tests)
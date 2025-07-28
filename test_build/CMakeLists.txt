cmake_minimum_required(VERSION 3.21)
project(TestBuild LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)

# Add the OptionsGreeks library
add_subdirectory(../libraries/OptionsGreeks OptionsGreeks)

# Create a simple test
add_executable(simple_test simple_test.cpp)
target_link_libraries(simple_test PRIVATE OptionsGreeks GTest::gtest GTest::gtest_main)

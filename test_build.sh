#!/bin/bash

# Simple test build script to diagnose compilation issues

set -e

echo "Testing OptionsGreeks library compilation..."

# Create a minimal test directory
mkdir -p test_build
cd test_build

# Create a minimal CMakeLists.txt
cat > CMakeLists.txt << 'EOF'
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
EOF

# Create a minimal test file
cat > simple_test.cpp << 'EOF'
#include <gtest/gtest.h>
#include <OptionsGreeks/OptionsGreeks.hpp>

TEST(SimpleTest, BasicFunctionality) {
    double price = OptionsGreeks::GetOptionPrice(100.0, 100.0, 0.2, 0.05, 0.25, true);
    EXPECT_GT(price, 0.0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
EOF

echo "Configuring with CMake..."
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

echo "Building..."
cmake --build build

echo "Running test..."
./build/simple_test

echo "Test completed successfully!"

# Cleanup
cd ..
rm -rf test_build
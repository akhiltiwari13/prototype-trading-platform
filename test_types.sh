#!/bin/bash

# Test type definitions

set -e

echo "Testing type definitions..."

# Create test directory
mkdir -p type_test
cd type_test

# Create CMakeLists.txt for type test
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.21)
project(TypeTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)

# Include MarketDataProvider headers
target_include_directories(test_types PRIVATE ../libraries/MarketDataProvider/include)

add_executable(test_types ../test_types.cpp)
target_link_libraries(test_types PRIVATE Boost::headers)
EOF

echo "Configuring type test..."
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=../build/Debug/generators/conan_toolchain.cmake

echo "Building type test..."
cmake --build build

echo "Running type test..."
./build/test_types

echo "Type test completed successfully!"

# Cleanup
cd ..
rm -rf type_test
#!/bin/bash

# Test SQLite integration

set -e

echo "Testing SQLite integration..."

# Create test directory
mkdir -p sqlite_test
cd sqlite_test

# Create CMakeLists.txt for SQLite test
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.21)
project(SQLiteTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)

add_executable(test_sqlite ../test_sqlite.cpp)
target_link_libraries(test_sqlite PRIVATE sqlite3::sqlite3)
EOF

echo "Configuring SQLite test..."
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=../build/Debug/generators/conan_toolchain.cmake

echo "Building SQLite test..."
cmake --build build

echo "Running SQLite test..."
./build/test_sqlite

echo "SQLite test completed successfully!"

# Cleanup
cd ..
rm -rf sqlite_test

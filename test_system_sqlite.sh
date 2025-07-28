#!/bin/bash

# Test system SQLite integration

set -e

echo "Testing system SQLite integration..."

# Check if SQLite3 is available on the system
if ! command -v sqlite3 &> /dev/null; then
    echo "SQLite3 not found. Installing via Homebrew..."
    brew install sqlite3
fi

echo "SQLite3 version: $(sqlite3 --version)"

# Create test directory
mkdir -p system_sqlite_test
cd system_sqlite_test

# Create CMakeLists.txt for system SQLite test
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.21)
project(SystemSQLiteTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)

add_executable(test_system_sqlite ../test_sqlite.cpp)
target_link_libraries(test_system_sqlite PRIVATE SQLite::SQLite3)
EOF

echo "Configuring system SQLite test..."
cmake -S . -B build

echo "Building system SQLite test..."
cmake --build build

echo "Running system SQLite test..."
./build/test_system_sqlite

echo "System SQLite test completed successfully!"

# Cleanup
cd ..
rm -rf system_sqlite_test
#!/bin/bash

# Fix build issues script

set -e

echo "Fixing build issues..."

# Check if SQLite3 is available on the system
if ! command -v sqlite3 &> /dev/null; then
    echo "SQLite3 not found. Installing via Homebrew..."
    if command -v brew &> /dev/null; then
        brew install sqlite3
    else
        echo "Please install SQLite3 development libraries manually"
        exit 1
    fi
fi

echo "SQLite3 version: $(sqlite3 --version)"

# Clean everything
echo "Cleaning build directory..."
rm -rf build/

# Install dependencies with correct configuration
echo "Installing Conan dependencies..."
conan install . --output-folder=build --build=missing -s build_type=Debug

# Configure CMake with correct toolchain path
echo "Configuring CMake..."
cd build
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=build/Debug/generators/conan_toolchain.cmake \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_BUILD_TYPE=Debug

# Build the project
echo "Building project..."
cmake --build . --target all

echo "copying compile-commands.json to project-root..."
cp ./compile_commands.json ../

echo "Build completed successfully!"

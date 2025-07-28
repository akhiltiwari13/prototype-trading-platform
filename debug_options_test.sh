#!/bin/bash

# Debug OptionsGreeks test

set -e

echo "Building and running OptionsGreeks test..."

cd build

# Build the test
cmake --build . --target OptionsGreeksTests

# Run with verbose output
echo "Running OptionsGreeks test with verbose output..."
./tests/libraries/OptionsGreeks/OptionsGreeksTests --gtest_output=verbose

echo "Test completed"
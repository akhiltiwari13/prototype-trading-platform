#!/bin/bash

# Test MarketDataProvider compilation

set -e

echo "Testing MarketDataProvider compilation..."

# Create test directory
mkdir -p market_data_test
cd market_data_test

# Create CMakeLists.txt for MarketDataProvider test
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.21)
project(MarketDataTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(Boost REQUIRED)

# Add the MarketDataProvider library
add_subdirectory(../libraries/MarketDataProvider MarketDataProvider)

# Create a simple test
add_executable(test_market_data test_market_data.cpp)
target_link_libraries(test_market_data PRIVATE MarketDataProvider)
EOF

# Create a minimal test file
cat > test_market_data.cpp << 'EOF'
#include <MarketDataProvider/LadderBuilder.hpp>
#include <MarketDataProvider/Structure.hpp>
#include <iostream>

int main() {
    using namespace MarketDataProvider;
    
    LadderBuilder builder(12345);
    
    OrderMessage order;
    order._orderId = 1.0;
    order._token = 12345;
    order._orderType = 'B';
    order._price = 100;
    order._quantity = 50;
    
    builder.processNewOrder(order);
    
    LadderDepth depth = builder.getLadderDepth();
    
    std::cout << "LadderBuilder test completed successfully!" << std::endl;
    std::cout << "Token: " << depth._token << std::endl;
    
    return 0;
}
EOF

echo "Configuring MarketDataProvider test..."
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=../build/Debug/generators/conan_toolchain.cmake

echo "Building MarketDataProvider test..."
cmake --build build

echo "Running MarketDataProvider test..."
./build/test_market_data

echo "MarketDataProvider test completed successfully!"

# Cleanup
cd ..
rm -rf market_data_test
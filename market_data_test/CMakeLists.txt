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

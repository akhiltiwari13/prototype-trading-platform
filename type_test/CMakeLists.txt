cmake_minimum_required(VERSION 3.21)
project(TypeTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)

# Include MarketDataProvider headers
target_include_directories(test_types PRIVATE ../libraries/MarketDataProvider/include)

add_executable(test_types ../test_types.cpp)
target_link_libraries(test_types PRIVATE Boost::headers)

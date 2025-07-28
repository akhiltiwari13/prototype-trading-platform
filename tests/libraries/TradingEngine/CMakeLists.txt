cmake_minimum_required(VERSION 3.21)

project(TradingEngineTests VERSION 1.0 LANGUAGES CXX)

# Create test executable
add_executable(${PROJECT_NAME} test_trading_engine.cpp)

# Set target properties
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        TradingEngine
        DatabaseLayer
        GTest::gtest
        GTest::gtest_main
)

# Add test to CTest
add_test(NAME TradingEngineTests COMMAND ${PROJECT_NAME})

# Compiler-specific optimizations for Apple Silicon
if(APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    target_compile_options(${PROJECT_NAME} PRIVATE -mcpu=apple-m1 -O2)
endif()
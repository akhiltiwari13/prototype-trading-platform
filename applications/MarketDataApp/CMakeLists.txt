cmake_minimum_required(VERSION 3.21)

project(MarketDataApp VERSION 1.0 LANGUAGES CXX)

# Create the executable
add_executable(${PROJECT_NAME} main.cpp)

# Set target properties
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        MarketDataProvider
        fmt::fmt
        spdlog::spdlog
        nlohmann_json::nlohmann_json
        Boost::system
)

# Compiler-specific optimizations for Apple Silicon
if(APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    target_compile_options(${PROJECT_NAME} PRIVATE -mcpu=apple-m1 -O3)
endif()

# Install the executable
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
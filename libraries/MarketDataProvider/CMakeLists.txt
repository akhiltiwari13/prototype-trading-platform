cmake_minimum_required(VERSION 3.21)

project(MarketDataProvider VERSION 1.0 LANGUAGES CXX)

# Create the library
add_library(${PROJECT_NAME} STATIC
    src/StreamManager.cpp
    src/LadderBuilder.cpp
)

# Set target properties
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Link dependencies
target_link_libraries(${PROJECT_NAME} 
    PUBLIC
        fmt::fmt
        spdlog::spdlog
        Boost::headers
        Boost::system
)

# Compiler-specific optimizations for Apple Silicon
if(APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    target_compile_options(${PROJECT_NAME} PRIVATE -mcpu=apple-m1 -O3)
endif()

# Export targets
install(TARGETS ${PROJECT_NAME}
    EXPORT MarketDataProviderTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT MarketDataProviderTargets
    FILE MarketDataProviderTargets.cmake
    NAMESPACE AlternateTradingPlatform::
    DESTINATION lib/cmake/MarketDataProvider
)
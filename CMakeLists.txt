cmake_minimum_required(VERSION 3.21)

project(AlternateTradingPlatform VERSION 1.0 LANGUAGES CXX)

message(STATUS "***AlternateTradingPlatform pre-build*** ${PROJECT_SOURCE_DIR}. building with cmake version: ${CMAKE_VERSION}")

if(APPLE)
  message(STATUS "Building on macOS (Apple Silicon/Intel)")
  # Apple Silicon (M1/M2) specific optimizations
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    message(STATUS "Detected Apple Silicon (ARM64)")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
  endif()
elseif(UNIX)
  message(STATUS "Building on Linux/Unix")
else()
  message(STATUS "Building on an unsupported/untested platform.")
endif()

# Enable C++20 features and disable compiler-specific extensions
# This ensures portability and adherence to the C++ standard
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CPPCHECK "")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Compiler-specific flags for better ARM64 support
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=apple-m1")
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
endif()

# Conan integration
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(magic_enum REQUIRED)
find_package(Boost REQUIRED)
find_package(Tracy REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(GTest REQUIRED)

set(Boost_NO_WARN_NEW_VERSIONS ON)
set(Boost_USE_STATIC_LIBS ON)

add_subdirectory(libraries)
add_subdirectory(applications)
add_subdirectory(tests)
add_subdirectory(simulation)

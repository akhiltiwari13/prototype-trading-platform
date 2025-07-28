cmake_minimum_required(VERSION 3.21)

project(TradingFrontendApp VERSION 1.0 LANGUAGES CXX)

# Create the executable
add_executable(${PROJECT_NAME} 
    main.cpp
    TradingGUI.cpp
)

# Set target properties
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Find SDL2
find_package(SDL2 QUIET)
if(NOT SDL2_FOUND)
    message(STATUS "SDL2 not found via find_package, trying pkg-config")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
endif()

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        fmt::fmt
        spdlog::spdlog
        nlohmann_json::nlohmann_json
)

# Link SDL2
if(TARGET SDL2::SDL2)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
elseif(SDL2_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})
    target_compile_options(${PROJECT_NAME} PRIVATE ${SDL2_CFLAGS_OTHER})
else()
    message(WARNING "SDL2 not found. GUI application may not build correctly.")
    message(STATUS "Install SDL2 with: brew install sdl2")
endif()

# Compiler-specific optimizations for Apple Silicon
if(APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    target_compile_options(${PROJECT_NAME} PRIVATE -mcpu=apple-m1 -O3)
endif()

# Install the executable
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
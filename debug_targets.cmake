cmake_minimum_required(VERSION 3.21)
project(DebugTargets)

# Find sqlite3
find_package(sqlite3 REQUIRED)

# Print all targets
get_property(all_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
get_property(imported_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY IMPORTED_TARGETS)

message(STATUS "All targets: ${all_targets}")
message(STATUS "Imported targets: ${imported_targets}")

# Check common SQLite target names
set(POSSIBLE_TARGETS 
    "sqlite3::sqlite3"
    "SQLite::SQLite3" 
    "sqlite3"
    "SQLite3"
    "sqlite"
    "SQLite"
)

foreach(target ${POSSIBLE_TARGETS})
    if(TARGET ${target})
        message(STATUS "FOUND TARGET: ${target}")
        get_target_property(target_type ${target} TYPE)
        message(STATUS "  Type: ${target_type}")
        get_target_property(target_location ${target} LOCATION)
        message(STATUS "  Location: ${target_location}")
    else()
        message(STATUS "NOT FOUND: ${target}")
    endif()
endforeach()
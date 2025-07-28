# Tests for Alternate Trading Platform

enable_testing()

add_subdirectory(libraries)

# Add a simple test to verify CTest is working
add_test(NAME SimpleTest COMMAND ${CMAKE_COMMAND} -E echo "CTest is working")
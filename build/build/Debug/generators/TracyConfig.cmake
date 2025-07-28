########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(Tracy_FIND_QUIETLY)
    set(Tracy_MESSAGE_MODE VERBOSE)
else()
    set(Tracy_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/TracyTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${tracy_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(Tracy_VERSION_STRING "0.11.1")
set(Tracy_INCLUDE_DIRS ${tracy_INCLUDE_DIRS_DEBUG} )
set(Tracy_INCLUDE_DIR ${tracy_INCLUDE_DIRS_DEBUG} )
set(Tracy_LIBRARIES ${tracy_LIBRARIES_DEBUG} )
set(Tracy_DEFINITIONS ${tracy_DEFINITIONS_DEBUG} )


# Only the last installed configuration BUILD_MODULES are included to avoid the collision
foreach(_BUILD_MODULE ${tracy_BUILD_MODULES_PATHS_DEBUG} )
    message(${Tracy_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()



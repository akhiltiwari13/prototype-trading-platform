# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(tracy_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(tracy_FRAMEWORKS_FOUND_DEBUG "${tracy_FRAMEWORKS_DEBUG}" "${tracy_FRAMEWORK_DIRS_DEBUG}")

set(tracy_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET tracy_DEPS_TARGET)
    add_library(tracy_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET tracy_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${tracy_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${tracy_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### tracy_DEPS_TARGET to all of them
conan_package_library_targets("${tracy_LIBS_DEBUG}"    # libraries
                              "${tracy_LIB_DIRS_DEBUG}" # package_libdir
                              "${tracy_BIN_DIRS_DEBUG}" # package_bindir
                              "${tracy_LIBRARY_TYPE_DEBUG}"
                              "${tracy_IS_HOST_WINDOWS_DEBUG}"
                              tracy_DEPS_TARGET
                              tracy_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "tracy"    # package_name
                              "${tracy_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${tracy_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Debug ########################################

    ########## COMPONENT Tracy::TracyClient #############

        set(tracy_Tracy_TracyClient_FRAMEWORKS_FOUND_DEBUG "")
        conan_find_apple_frameworks(tracy_Tracy_TracyClient_FRAMEWORKS_FOUND_DEBUG "${tracy_Tracy_TracyClient_FRAMEWORKS_DEBUG}" "${tracy_Tracy_TracyClient_FRAMEWORK_DIRS_DEBUG}")

        set(tracy_Tracy_TracyClient_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET tracy_Tracy_TracyClient_DEPS_TARGET)
            add_library(tracy_Tracy_TracyClient_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET tracy_Tracy_TracyClient_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${tracy_Tracy_TracyClient_FRAMEWORKS_FOUND_DEBUG}>
                     $<$<CONFIG:Debug>:${tracy_Tracy_TracyClient_SYSTEM_LIBS_DEBUG}>
                     $<$<CONFIG:Debug>:${tracy_Tracy_TracyClient_DEPENDENCIES_DEBUG}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'tracy_Tracy_TracyClient_DEPS_TARGET' to all of them
        conan_package_library_targets("${tracy_Tracy_TracyClient_LIBS_DEBUG}"
                              "${tracy_Tracy_TracyClient_LIB_DIRS_DEBUG}"
                              "${tracy_Tracy_TracyClient_BIN_DIRS_DEBUG}" # package_bindir
                              "${tracy_Tracy_TracyClient_LIBRARY_TYPE_DEBUG}"
                              "${tracy_Tracy_TracyClient_IS_HOST_WINDOWS_DEBUG}"
                              tracy_Tracy_TracyClient_DEPS_TARGET
                              tracy_Tracy_TracyClient_LIBRARIES_TARGETS
                              "_DEBUG"
                              "tracy_Tracy_TracyClient"
                              "${tracy_Tracy_TracyClient_NO_SONAME_MODE_DEBUG}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET Tracy::TracyClient
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${tracy_Tracy_TracyClient_OBJECTS_DEBUG}>
                     $<$<CONFIG:Debug>:${tracy_Tracy_TracyClient_LIBRARIES_TARGETS}>
                     )

        if("${tracy_Tracy_TracyClient_LIBS_DEBUG}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET Tracy::TracyClient
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         tracy_Tracy_TracyClient_DEPS_TARGET)
        endif()

        set_property(TARGET Tracy::TracyClient APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Debug>:${tracy_Tracy_TracyClient_LINKER_FLAGS_DEBUG}>)
        set_property(TARGET Tracy::TracyClient APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Debug>:${tracy_Tracy_TracyClient_INCLUDE_DIRS_DEBUG}>)
        set_property(TARGET Tracy::TracyClient APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Debug>:${tracy_Tracy_TracyClient_LIB_DIRS_DEBUG}>)
        set_property(TARGET Tracy::TracyClient APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Debug>:${tracy_Tracy_TracyClient_COMPILE_DEFINITIONS_DEBUG}>)
        set_property(TARGET Tracy::TracyClient APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Debug>:${tracy_Tracy_TracyClient_COMPILE_OPTIONS_DEBUG}>)


    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET Tracy::TracyClient APPEND PROPERTY INTERFACE_LINK_LIBRARIES Tracy::TracyClient)

########## For the modules (FindXXX)
set(tracy_LIBRARIES_DEBUG Tracy::TracyClient)

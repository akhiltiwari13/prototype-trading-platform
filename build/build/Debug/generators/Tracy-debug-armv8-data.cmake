########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

list(APPEND tracy_COMPONENT_NAMES Tracy::TracyClient)
list(REMOVE_DUPLICATES tracy_COMPONENT_NAMES)
if(DEFINED tracy_FIND_DEPENDENCY_NAMES)
  list(APPEND tracy_FIND_DEPENDENCY_NAMES )
  list(REMOVE_DUPLICATES tracy_FIND_DEPENDENCY_NAMES)
else()
  set(tracy_FIND_DEPENDENCY_NAMES )
endif()

########### VARIABLES #######################################################################
#############################################################################################
set(tracy_PACKAGE_FOLDER_DEBUG "/Users/akhil/.conan2/p/b/tracy7db401a98a0fa/p")
set(tracy_BUILD_MODULES_PATHS_DEBUG )


set(tracy_INCLUDE_DIRS_DEBUG "${tracy_PACKAGE_FOLDER_DEBUG}/include")
set(tracy_RES_DIRS_DEBUG )
set(tracy_DEFINITIONS_DEBUG "-DTRACY_ENABLE")
set(tracy_SHARED_LINK_FLAGS_DEBUG )
set(tracy_EXE_LINK_FLAGS_DEBUG )
set(tracy_OBJECTS_DEBUG )
set(tracy_COMPILE_DEFINITIONS_DEBUG "TRACY_ENABLE")
set(tracy_COMPILE_OPTIONS_C_DEBUG )
set(tracy_COMPILE_OPTIONS_CXX_DEBUG )
set(tracy_LIB_DIRS_DEBUG "${tracy_PACKAGE_FOLDER_DEBUG}/lib")
set(tracy_BIN_DIRS_DEBUG )
set(tracy_LIBRARY_TYPE_DEBUG STATIC)
set(tracy_IS_HOST_WINDOWS_DEBUG 0)
set(tracy_LIBS_DEBUG TracyClient)
set(tracy_SYSTEM_LIBS_DEBUG )
set(tracy_FRAMEWORK_DIRS_DEBUG )
set(tracy_FRAMEWORKS_DEBUG )
set(tracy_BUILD_DIRS_DEBUG )
set(tracy_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(tracy_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${tracy_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${tracy_COMPILE_OPTIONS_C_DEBUG}>")
set(tracy_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${tracy_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${tracy_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${tracy_EXE_LINK_FLAGS_DEBUG}>")


set(tracy_COMPONENTS_DEBUG Tracy::TracyClient)
########### COMPONENT Tracy::TracyClient VARIABLES ############################################

set(tracy_Tracy_TracyClient_INCLUDE_DIRS_DEBUG "${tracy_PACKAGE_FOLDER_DEBUG}/include")
set(tracy_Tracy_TracyClient_LIB_DIRS_DEBUG "${tracy_PACKAGE_FOLDER_DEBUG}/lib")
set(tracy_Tracy_TracyClient_BIN_DIRS_DEBUG )
set(tracy_Tracy_TracyClient_LIBRARY_TYPE_DEBUG STATIC)
set(tracy_Tracy_TracyClient_IS_HOST_WINDOWS_DEBUG 0)
set(tracy_Tracy_TracyClient_RES_DIRS_DEBUG )
set(tracy_Tracy_TracyClient_DEFINITIONS_DEBUG "-DTRACY_ENABLE")
set(tracy_Tracy_TracyClient_OBJECTS_DEBUG )
set(tracy_Tracy_TracyClient_COMPILE_DEFINITIONS_DEBUG "TRACY_ENABLE")
set(tracy_Tracy_TracyClient_COMPILE_OPTIONS_C_DEBUG "")
set(tracy_Tracy_TracyClient_COMPILE_OPTIONS_CXX_DEBUG "")
set(tracy_Tracy_TracyClient_LIBS_DEBUG TracyClient)
set(tracy_Tracy_TracyClient_SYSTEM_LIBS_DEBUG )
set(tracy_Tracy_TracyClient_FRAMEWORK_DIRS_DEBUG )
set(tracy_Tracy_TracyClient_FRAMEWORKS_DEBUG )
set(tracy_Tracy_TracyClient_DEPENDENCIES_DEBUG )
set(tracy_Tracy_TracyClient_SHARED_LINK_FLAGS_DEBUG )
set(tracy_Tracy_TracyClient_EXE_LINK_FLAGS_DEBUG )
set(tracy_Tracy_TracyClient_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(tracy_Tracy_TracyClient_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${tracy_Tracy_TracyClient_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${tracy_Tracy_TracyClient_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${tracy_Tracy_TracyClient_EXE_LINK_FLAGS_DEBUG}>
)
set(tracy_Tracy_TracyClient_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${tracy_Tracy_TracyClient_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${tracy_Tracy_TracyClient_COMPILE_OPTIONS_C_DEBUG}>")
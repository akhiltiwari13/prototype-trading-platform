#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PrototypeTradingPlatform::TradingEngine" for configuration "Debug"
set_property(TARGET PrototypeTradingPlatform::TradingEngine APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PrototypeTradingPlatform::TradingEngine PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libTradingEngine.a"
  )

list(APPEND _cmake_import_check_targets PrototypeTradingPlatform::TradingEngine )
list(APPEND _cmake_import_check_files_for_PrototypeTradingPlatform::TradingEngine "${_IMPORT_PREFIX}/lib/libTradingEngine.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

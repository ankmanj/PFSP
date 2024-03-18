#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PFSP_plugin_mode_manager" for configuration "Debug"
set_property(TARGET PFSP_plugin_mode_manager APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PFSP_plugin_mode_manager PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG "pfsp::PFSP_core_mode_manager"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libPFSP_plugin_mode_manager.so"
  IMPORTED_SONAME_DEBUG "libPFSP_plugin_mode_manager.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS PFSP_plugin_mode_manager )
list(APPEND _IMPORT_CHECK_FILES_FOR_PFSP_plugin_mode_manager "${_IMPORT_PREFIX}/lib/libPFSP_plugin_mode_manager.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

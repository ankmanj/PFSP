#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PFSP::PFSP_mw_start" for configuration "Debug"
set_property(TARGET PFSP::PFSP_mw_start APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PFSP::PFSP_mw_start PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libPFSP_mw_start.so"
  IMPORTED_SONAME_DEBUG "libPFSP_mw_start.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS PFSP::PFSP_mw_start )
list(APPEND _IMPORT_CHECK_FILES_FOR_PFSP::PFSP_mw_start "${_IMPORT_PREFIX}/lib/libPFSP_mw_start.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

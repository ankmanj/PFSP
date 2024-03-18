#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PFSP_component2" for configuration "Debug"
set_property(TARGET PFSP_component2 APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PFSP_component2 PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libPFSP_component2.so"
  IMPORTED_SONAME_DEBUG "libPFSP_component2.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS PFSP_component2 )
list(APPEND _IMPORT_CHECK_FILES_FOR_PFSP_component2 "${_IMPORT_PREFIX}/lib/libPFSP_component2.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

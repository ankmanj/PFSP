#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PFSP_MW" for configuration ""
set_property(TARGET PFSP_MW APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(PFSP_MW PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "rt"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libPFSP_MW.so"
  IMPORTED_SONAME_NOCONFIG "libPFSP_MW.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS PFSP_MW )
list(APPEND _IMPORT_CHECK_FILES_FOR_PFSP_MW "${_IMPORT_PREFIX}/lib/libPFSP_MW.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

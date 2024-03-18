#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PFSP_base_host_component_1" for configuration "Debug"
set_property(TARGET PFSP_base_host_component_1 APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PFSP_base_host_component_1 PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG "PFSP_component2"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libPFSP_base_host_component_1.so"
  IMPORTED_SONAME_DEBUG "libPFSP_base_host_component_1.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS PFSP_base_host_component_1 )
list(APPEND _IMPORT_CHECK_FILES_FOR_PFSP_base_host_component_1 "${_IMPORT_PREFIX}/lib/libPFSP_base_host_component_1.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

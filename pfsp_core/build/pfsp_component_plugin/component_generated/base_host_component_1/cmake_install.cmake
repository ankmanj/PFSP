# Install script for directory: /home/junghun/Source/PFSP_FinalVersion/Backup_PFSP/PFSP/pfsp_core/pfsp_component_plugin/component_generated/base_host_component_1

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libPFSP_base_host_component_1.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libPFSP_base_host_component_1.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libPFSP_base_host_component_1.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/junghun/Source/PFSP_FinalVersion/Backup_PFSP/PFSP/pfsp_core/build/pfsp_component_plugin/component_generated/base_host_component_1/libPFSP_base_host_component_1.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libPFSP_base_host_component_1.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libPFSP_base_host_component_1.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libPFSP_base_host_component_1.so"
         OLD_RPATH "/home/junghun/Source/PFSP_FinalVersion/Backup_PFSP/PFSP/pfsp_core/build/build/component2:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libPFSP_base_host_component_1.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/PFSP_base_host_component_1/cmake/FindPFSP_base_host_component_1.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/PFSP_base_host_component_1/cmake/FindPFSP_base_host_component_1.cmake"
         "/home/junghun/Source/PFSP_FinalVersion/Backup_PFSP/PFSP/pfsp_core/build/pfsp_component_plugin/component_generated/base_host_component_1/CMakeFiles/Export/lib/PFSP_base_host_component_1/cmake/FindPFSP_base_host_component_1.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/PFSP_base_host_component_1/cmake/FindPFSP_base_host_component_1-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/PFSP_base_host_component_1/cmake/FindPFSP_base_host_component_1.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/PFSP_base_host_component_1/cmake" TYPE FILE FILES "/home/junghun/Source/PFSP_FinalVersion/Backup_PFSP/PFSP/pfsp_core/build/pfsp_component_plugin/component_generated/base_host_component_1/CMakeFiles/Export/lib/PFSP_base_host_component_1/cmake/FindPFSP_base_host_component_1.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/PFSP_base_host_component_1/cmake" TYPE FILE FILES "/home/junghun/Source/PFSP_FinalVersion/Backup_PFSP/PFSP/pfsp_core/build/pfsp_component_plugin/component_generated/base_host_component_1/CMakeFiles/Export/lib/PFSP_base_host_component_1/cmake/FindPFSP_base_host_component_1-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/PFSP_base_host_component_1/cmake" TYPE FILE FILES "/home/junghun/Source/PFSP_FinalVersion/Backup_PFSP/PFSP/pfsp_core/pfsp_component_plugin/component_generated/base_host_component_1/PFSP_base_host_component_1Config.cmake")
endif()


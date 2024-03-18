# Project name
project(PFSP_core_mode_manager)

# Set default language setting
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PLATFORM x86_64)
set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(${PROJECT_NAME} SHARED "")

# Set header file list
set(header_path "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Set function definition file list
set(src_path "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB src_files "${src_path}/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${src_files})

set(pfsp_header_path "${CMAKE_SOURCE_DIR}/pfsp_include")
target_include_directories(${PROJECT_NAME} 
                                PRIVATE 
                                        ${pfsp_header_path} 
                                PUBLIC 
                                        $<BUILD_INTERFACE:${header_path}>
                                )


set(include_dest "lib/${PROJECT_NAME}/include")
set(lib_dest "lib")
set(cmake_dest "lib/${PROJECT_NAME}/cmake")

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets
        LIBRARY DESTINATION "${lib_dest}"
        ARCHIVE DESTINATION "${lib_dest}"
        RUNTIME DESTINATION "${lib_dest}"
        PUBLIC_HEADER DESTINATION "${cmake_dest}")
install(EXPORT ${PROJECT_NAME}-targets
        NAMESPACE pfsp::
        FILE ${PROJECT_NAME}-targets.cmake
        DESTINATION "${cmake_dest}")
install(FILES ${PROJECT_NAME}-config.cmake DESTINATION "${cmake_dest}")
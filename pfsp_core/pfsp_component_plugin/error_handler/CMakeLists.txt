# Project name
project(PFSP_plugin_error_handler C CXX)

add_definitions("-DDEBUG -D_DEBUG -DLINUX -DPTHREAD -D\"__is_empty(Type)=1\" -DISOC99_SOURCE -D_GNU_SOURCE")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PLATFORM x86_64)
set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ldl -pthread")
add_compile_options(-Wall -fPIC)
# add_executable(${PROJECT_NAME} "")
add_library(${PROJECT_NAME} SHARED "")

# Set header file list
set(header_path "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Set function definition file list
set(src_path "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB src_files "${src_path}/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${src_files})

target_link_libraries(${PROJECT_NAME} 
                        PRIVATE 
                                PFSP_core_error_handler
                        )

set(pfsp_header_path "${CMAKE_SOURCE_DIR}/pfsp_include")
set(core_error_handler_header_path "${CMAKE_SOURCE_DIR}/pfsp_component_core/error_handler/include")
# set(host_application_header_path "${CMAKE_SOURCE_DIR}/pfsp_component_plugin/include")
set(lib_src_header_path "${CMAKE_SOURCE_DIR}/pfsp_lib/PFSPMiddleware/include/src")
set(lib_xml_header_path "${CMAKE_SOURCE_DIR}/pfsp_lib/PFSPMiddleware/include")
target_include_directories(${PROJECT_NAME} 
                                PRIVATE 
                                        ${pfsp_header_path} 
                                        ${core_error_handler_header_path} 
# ${host_application_header_path}
                                        ${lib_src_header_path} 
                                        ${lib_xml_header_path}
                                PUBLIC 
                                        $<BUILD_INTERFACE:${header_path}>
                                )

set(include_dest "lib/${PROJECT_NAME}/include")
set(lib_dest "lib")
set(cmake_dest "lib/${PROJECT_NAME}/cmake")

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
        LIBRARY DESTINATION "${lib_dest}"
        ARCHIVE DESTINATION "${lib_dest}"
        RUNTIME DESTINATION "${lib_dest}"
        PUBLIC_HEADER DESTINATION "${cmake_dest}")
install(EXPORT ${PROJECT_NAME}
        NAMESPACE 
        FILE Find${PROJECT_NAME}.cmake
        DESTINATION "${cmake_dest}")
install(FILES ${PROJECT_NAME}Config.cmake DESTINATION "${cmake_dest}")
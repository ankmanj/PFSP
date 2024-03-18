# Project name
project(PFSP_mw_start C CXX)

add_definitions("-DDEBUG -D_DEBUG -DLINUX -DPTHREAD -D\"__is_empty(Type)=1\" -DISOC99_SOURCE -D_GNU_SOURCE")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ldl -pthread")
add_compile_options(-Wall -fPIC)

add_library(${PROJECT_NAME} SHARED "")
# add_library(${PROJECT_NAME} STATIC "")
# add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/pfsp_mw_start.cpp)

# Set function definition file list
set(src_path "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB src_files "${src_path}/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${src_files})

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/pfsp_lib/PFSPMiddleware/cmake")
#find_package(PFSP_MW REQUIRED)
#include_directories(${CMAKE_SOURCE_DIR}/pfsp_lib/PFSPMiddleware/include)
target_link_libraries(${PROJECT_NAME} 
                        PUBLIC 
#                                PFSP_base_host_component_0
#                                PFSP_base_host_component_1
                                ${CMAKE_DL_LIBS} 
                                ${CMAKE_SOURCE_DIR}/pfsp_lib/libs/libPFSP_MW.so rt
                        )

# Set header file list
set(main_source_path "${CMAKE_SOURCE_DIR}")
set(header_path "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(lib_src_header_path "${CMAKE_SOURCE_DIR}/pfsp_lib/PFSPMiddleware/include/src")
set(lib_xml_header_path "${CMAKE_SOURCE_DIR}/pfsp_lib/PFSPMiddleware/include")
target_include_directories(${PROJECT_NAME} 
                                PRIVATE 
                                        ${main_source_path} 
                                        ${lib_xml_header_path} 
                                        ${lib_src_header_path} 
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
        NAMESPACE PFSP::
        FILE Find${PROJECT_NAME}.cmake
        DESTINATION "${cmake_dest}")
install(FILES ${PROJECT_NAME}Config.cmake DESTINATION "${cmake_dest}")
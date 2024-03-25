# PFSP
# Ensure cmake version
cmake_minimum_required(VERSION 3.5 )
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)


# Project name
project(PFSP)
message(${CMAKE_PROJECT_NAME})

#Compile option
add_compile_options(-Wall -fPIC)

set(host_component_path "../host_components")

add_subdirectory(pfsp_component_core)
add_subdirectory(pfsp_component_plugin)
add_subdirectory(${host_component_path} build)
add_subdirectory(pfsp_middleware_start)
add_subdirectory(pfsp_main)

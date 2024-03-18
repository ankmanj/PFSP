project(PFSP_FirstComponent C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PLATFORM x86_64)
set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ldl")
add_compile_options(-Wall -fPIC)

add_library(${PROJECT_NAME} SHARED "")

set(header_path "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(src_path "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB src_files "${src_path}/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${src_files})

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${header_path}>)

set(include_dest "lib/${PROJECT_NAME}/include")
set(lib_dest "lib")
set(cmake_dest "lib/${PROJECT_NAME}/cmake")

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} LIBRARY DESTINATION "${lib_dest}" ARCHIVE DESTINATION "${lib_dest}" RUNTIME DESTINATION "${lib_dest}" PUBLIC_HEADER DESTINATION "${cmake_dest}")
install(EXPORT ${PROJECT_NAME} NAMESPACE FILE Find${PROJECT_NAME}.cmake DESTINATION "${cmake_dest}")
install(FILES ${PROJECT_NAME}Config.cmake DESTINATION "${cmake_dest}")

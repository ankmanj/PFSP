# Project name
project(PFSP_main C CXX)

# Set default language setting
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PLATFORM x86_64)
set(CMAKE_VERBOSE_MAKEFILE true)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/pfsp_main.cpp)

target_link_libraries(${PROJECT_NAME} 
                        PRIVATE 
                                PFSP_mw_start
                        )

set(pfsp_mw_adaptation_path "${CMAKE_SOURCE_DIR}/pfsp_middleware_start/include")
target_include_directories(${PROJECT_NAME} 
                                PRIVATE 
                                        ${pfsp_mw_adaptation_path}
                                )
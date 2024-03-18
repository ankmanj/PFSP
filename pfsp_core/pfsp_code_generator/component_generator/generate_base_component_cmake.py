import os
import sys
import argparse
import glob
import json

from pprint import PrettyPrinter
from types import FunctionType 

class GenerateBaseComponentCmake():
    def __init__(self, component_name, component_class_name):
        self.__component_name = component_name
        self.__class_name = component_class_name

    def set_cmake_information(self, cmake_dirname, cmake_file_name, component_id, external_input_component_id):
        self.__component_id = component_id
        self.__external_input_component_id = external_input_component_id
        self.__cmake_dirname = cmake_dirname
        self.__cmake_file_name = cmake_file_name

    def write_contents(self):

        with open(os.path.join(self.__cmake_dirname, self.__cmake_file_name), 'w') as f:

            f.write(f'project(PFSP_base_host_component_{self.__component_id} C CXX)\n\n')

            f.write('add_definitions("-DDEBUG -D_DEBUG -DLINUX -DPTHREAD -DISOC99_SOURCE -D_GNU_SOURCE")\n\n')

            f.write('set(CMAKE_CXX_STANDARD 17)\n')
            f.write('set(CMAKE_CXX_EXTENSIONS OFF)\n')
            f.write('set(CMAKE_CXX_STANDARD_REQUIRED ON)\n\n')

            f.write('set(PLATFORM x86_64)\n')
            f.write('set(CMAKE_VERBOSE_MAKEFILE true)\n')
            f.write('set(CMAKE_EXPORT_COMPILE_COMMANDS ON)\n\n')

            f.write(f'set(CMAKE_CXX_FLAGS "${{CMAKE_CXX_FLAGS}} -g -ldl -pthread")\n')
            f.write('add_compile_options(-Wall -fPIC)\n\n')

            f.write(f'add_library(${{PROJECT_NAME}} SHARED "")\n\n')

            f.write(f'set(header_path "${{CMAKE_CURRENT_SOURCE_DIR}}/include")\n\n')

            f.write(f'set(src_path "${{CMAKE_CURRENT_SOURCE_DIR}}/src")\n')
            f.write(f'file(GLOB src_files "${{src_path}}/*.cpp")\n')
            f.write(f'target_sources(${{PROJECT_NAME}} PRIVATE ${{src_files}})\n\n')

            f.write(f'target_link_libraries(${{PROJECT_NAME}} PRIVATE PFSP_{self.__component_name})\n\n')

            f.write(f'set(pfsp_header_path "${{CMAKE_SOURCE_DIR}}/pfsp_include")\n')
            f.write(f'set(template_include "${{CMAKE_SOURCE_DIR}}/pfsp_component_plugin/component_common")\n')
            f.write(f'set(smart_converter_common_include "${{CMAKE_SOURCE_DIR}}/pfsp_component_core/smart_converter")\n')
            f.write(f'set(lib_src_header_path "${{CMAKE_SOURCE_DIR}}/pfsp_lib/PFSPMiddleware/include/src")\n')
            f.write(f'set(lib_xml_header_path "${{CMAKE_SOURCE_DIR}}/pfsp_lib/PFSPMiddleware/include")\n\n')

            f.write(f'target_include_directories(${{PROJECT_NAME}} PRIVATE ${{pfsp_header_path}} ${{template_include}} ${{smart_converter_common_include}} ${{lib_src_header_path}} ${{lib_xml_header_path}} PUBLIC $<BUILD_INTERFACE:${{header_path}}>)\n\n')

            f.write(f'set(include_dest "lib/${{PROJECT_NAME}}/include")\n')
            f.write('set(lib_dest "lib")\n')
            f.write(f'set(cmake_dest "lib/${{PROJECT_NAME}}/cmake")\n\n')

            f.write(f'install(TARGETS ${{PROJECT_NAME}} EXPORT ${{PROJECT_NAME}} LIBRARY DESTINATION "${{lib_dest}}" ARCHIVE DESTINATION "${{lib_dest}}" RUNTIME DESTINATION "${{lib_dest}}" PUBLIC_HEADER DESTINATION "${{cmake_dest}}")\n')
            f.write(f'install(EXPORT ${{PROJECT_NAME}} NAMESPACE FILE Find${{PROJECT_NAME}}.cmake DESTINATION "${{cmake_dest}}")\n')
            f.write(f'install(FILES ${{PROJECT_NAME}}Config.cmake DESTINATION "${{cmake_dest}}")\n')

            f.close()


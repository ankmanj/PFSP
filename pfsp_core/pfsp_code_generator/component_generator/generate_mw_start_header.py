import os
import sys
import argparse
import glob
import json

from pprint import PrettyPrinter
from types import FunctionType 

class GenerateMwStarterHeader():
    def __init__(self, component_name, component_class_name, number_of_components):
        self.__component_name = component_name
        self.__class_name = component_class_name
        self.__number_of_components = number_of_components

    def set_mw_starter_header_file_information(self, dirname, file_name, component_id):
        self.__mw_starter_header_dirname = dirname
        self.__mw_starter_header_file_name = file_name
        self.__component_id = component_id

    def write_contents(self):
        if self.__component_id == 0 and self.__component_id < self.__number_of_components:
            self.__write_default_initial_code()
            self.__write_each_host_component_lib()
        elif self.__component_id == 0 and self.__component_id == self.__number_of_components:
            self.__write_default_initial_code()
            self.__write_each_host_component_lib()
            self.__write_default_ending_code()
        elif self.__component_id > 0 and self.__component_id < self.__number_of_components:
            self.__write_each_host_component_lib()
        elif self.__component_id > 0 and self.__component_id == self.__number_of_components:
            self.__write_each_host_component_lib()
            self.__write_default_ending_code()

    def __write_default_initial_code(self):
        with open(os.path.join(self.__mw_starter_header_dirname, self.__mw_starter_header_file_name), 'a+') as f:
            f.write('#pragma once\n\n')

            f.write('#include <cassert>\n')
            f.write('#include <dlfcn.h>\n\n')

            f.write('class PfspMwStart\n')
            f.write('{\n')
            f.write('public:\n')
            f.write('    PfspMwStart(void);\n')
            f.write('    virtual ~PfspMwStart(void);\n\n')

            f.write('    int PfspMwStartMain(int argc, char* argv[]);\n\n')
            f.write('protected:\n\n')

            f.write('private:\n')
            f.write('    void* loading_lib(const char* name)\n')
            f.write('    {\n')
            f.write('        void* handle = dlopen (name, RTLD_NOW | RTLD_GLOBAL);\n')
            f.write('        if( handle == 0 )\n')
            f.write('        {\n')
            f.write('            return 0;\n')
            f.write('        }\n')
            f.write('        return handle;\n')
            f.write('    }\n\n')

            f.write('    void LoadLibsOfAllComponents()\n')
            f.write('    {\n')
            f.write('        void* libhandle = 0;\n\n')

            f.write('        libhandle = loading_lib("libPFSP_MW.so");\n')
            f.write('        assert(libhandle);\n\n')

            f.write('        libhandle = loading_lib("libPFSP_plugin_error_handler.so");\n')
            f.write('        assert(libhandle);\n\n')

            f.write('        libhandle = loading_lib("libPFSP_plugin_mode_manager.so");\n')
            f.write('        assert(libhandle);\n\n')
            f.close()

    def __write_default_ending_code(self):
        with open(os.path.join(self.__mw_starter_header_dirname, self.__mw_starter_header_file_name), 'a+') as f:
            f.write('    }\n')
            f.write('};\n')
            f.close()

    def __write_each_host_component_lib(self):
        with open(os.path.join(self.__mw_starter_header_dirname, self.__mw_starter_header_file_name), 'a+') as f:
            f.write(f'        libhandle = loading_lib("libPFSP_base_host_component_{self.__component_id}.so");\n')
            f.write('        assert(libhandle);\n\n')
            f.close()
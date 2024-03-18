import os
import sys
import argparse
import glob
import json

from pprint import PrettyPrinter
from types import FunctionType 

class GenerateMainShellScript():
    def __init__(self, component_name, component_class_name, number_of_components):
        self.__component_name = component_name
        self.__class_name = component_class_name
        real_host_component_class_name = component_class_name.replace("Base","")
        self.__host_class_name = real_host_component_class_name
        self.__number_of_components = number_of_components

    def set_shell_script_file_information(self, dirname, file_name, component_id):
        self.__shell_script_dirname = dirname
        self.__shell_script_file_name = file_name
        self.__component_id = component_id

    def write_contents(self):
        if self.__component_id == 0 and self.__component_id < self.__number_of_components:
            self.__write_default_initial_code()
            self.__write_each_component_lib()
        elif self.__component_id == 0 and self.__component_id == self.__number_of_components:
            self.__write_default_initial_code()
            self.__write_each_component_lib()
            self.__write_default_ending_code()
        elif self.__component_id > 0 and self.__component_id < self.__number_of_components:
            self.__write_each_component_lib()
        elif self.__component_id > 0 and self.__component_id == self.__number_of_components:
            self.__write_each_component_lib()
            self.__write_default_ending_code()

    def __write_default_initial_code(self):
        with open(os.path.join(self.__shell_script_dirname, self.__shell_script_file_name), 'a+') as f:
            f.write('#!/bin/bash\n\n')

            f.write('current_Path=$(pwd)\n\n')

            f.write('cd $current_Path/pfsp_lib\n')
            f.write('lib_path="libs"\n')
            f.write('if [ -d $lib_path ]; then\n')
            f.write('    sudo rm -rf $lib_path\n')
            f.write('    mkdir $lib_path\n')
            f.write('else\n')
            f.write('    mkdir $lib_path\n')
            f.write('fi\n\n')

            f.write('cd $current_Path/pfsp_lib/PFSPMiddleware\n')
            f.write('cp -rf libPFSP_MW.so $current_Path/pfsp_lib/libs\n\n')

            f.write('cd $current_Path\n')
            f.write('build_path="build"\n')
            f.write('if [ -d $build_path ]; then\n')
            f.write('    sudo rm -rf $build_path\n')
            f.write('    mkdir $build_path\n')
            f.write('else\n')
            f.write('    mkdir $build_path\n')
            f.write('fi\n\n')

            f.write('cd $current_Path/$build_path\n')
            f.write('cmake ..\n')
            f.write('make\n\n')

            f.write('cd $current_Path/$build_path/pfsp_component_plugin/error_handler\n')
            f.write('cp -rf libPFSP_plugin_error_handler.so $current_Path/pfsp_lib/libs\n\n')

            f.write('cd $current_Path/$build_path/pfsp_component_plugin/mode_manager\n')
            f.write('cp -rf libPFSP_plugin_mode_manager.so $current_Path/pfsp_lib/libs\n\n')
            f.close()

    def __write_default_ending_code(self):
        with open(os.path.join(self.__shell_script_dirname, self.__shell_script_file_name), 'a+') as f:
            f.write('export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$current_Path/pfsp_lib/libs\n')
            f.write('cd $current_Path/$build_path/pfsp_main\n')
            f.write('sudo ./PFSP_main\n')
            f.close()

    def __write_each_component_lib(self):
        with open(os.path.join(self.__shell_script_dirname, self.__shell_script_file_name), 'a+') as f:
            f.write(f'cd $current_Path/$build_path/pfsp_component_plugin/component_generated/base_host_component_{self.__component_id}\n')
            f.write(f'cp -rf libPFSP_base_host_component_{self.__component_id}.so $current_Path/pfsp_lib/libs\n\n')

            f.write(f'cd $current_Path/$build_path/$build_path/{self.__component_name}\n')
            f.write(f'cp -rf libPFSP_{self.__component_name}.so $current_Path/pfsp_lib/libs\n\n')
            f.close()
import os
from cpp_generator import *
from code_generator import *
from lib.data_map_gen.json_function_gen import *
from types import FunctionType
from cpp_json_path import CPPJSONPath

from generate_base_component import GenerateBaseComponent
from generate_base_component_cmake import GenerateBaseComponentCmake

from generate_host_component import GenerateHostComponent
from generate_host_component_cmake import GenerateHostComponentCmake

from generate_mw_start_header import GenerateMwStarterHeader

from generate_main_shell_script import GenerateMainShellScript
from generate_sub_shell_script import GenerateSubShellScript

class GenerationCordinator:
    def __init__(self):
        self.__cpp_generated_classes = None
        self.__obj_cpp_json_path = CPPJSONPath()

    def set_component_information(self, component_id, external_input_component_id, component_name, component_class_name):
        self.__component_id = component_id
        self.__external_input_component_id = external_input_component_id
        self.__component_name = component_name
        self.__component_class_name = component_class_name

    def set_component_files_information(self, dest_path, header_file, cpp_file):
        self.__obj_cpp_json_path.dest_path = dest_path
        self.__gen_path = self.__obj_cpp_json_path.dest_path
        self.__header_file = header_file
        self.__cpp_file = cpp_file

    def generate_base_component(self):
        obj = GenerateBaseComponent(self.__component_name, self.__component_class_name, self.__header_file, self.__cpp_file)
        obj.get_component_information(self.__component_id, self.__external_input_component_id)
        obj.gen_methods()
        obj.gen_members()
        obj.get_includes()
        obj.generate_header_impl_files()

    def set_base_component_cmake_information(self, cmake_dirname, cmake_file_name):
        self.__cmake_dirname = cmake_dirname
        self.__cmake_file_name = cmake_file_name

    def generate_base_component_cmake_file(self):
        obj = GenerateBaseComponentCmake(self.__component_name, self.__component_class_name)
        obj.set_cmake_information(self.__cmake_dirname, self.__cmake_file_name, self.__component_id, self.__external_input_component_id)
        obj.write_contents()

    def generate_host_component(self, header_file_name):
        obj = GenerateHostComponent(header_file_name, self.__component_id, self.__component_name, self.__component_class_name, self.__header_file, self.__cpp_file)
        obj.get_component_information(self.__external_input_component_id)
        obj.gen_methods()
        obj.gen_members()
        obj.get_includes()
        obj.generate_header_impl_files()

    def set_host_component_cmake_information(self, cmake_dirname, cmake_file_name):
        self.__host_component_cmake_dirname = cmake_dirname
        self.__host_component_cmake_file_name = cmake_file_name

    def generate_host_component_cmake_file(self):
        obj = GenerateHostComponentCmake(self.__component_name, self.__component_class_name)
        obj.set_cmake_information(self.__host_component_cmake_dirname, self.__host_component_cmake_file_name, self.__component_id, self.__external_input_component_id)
        obj.write_contents()

    def set_mw_starter_header_information(self, header_dirname, header_name):
        self.__mw_starter_header_dirname = header_dirname
        self.__mw_starter_header_file_name = header_name

    def generate_mw_starter_header_file(self, number_of_components):
        obj = GenerateMwStarterHeader(self.__component_name, self.__component_class_name, number_of_components)
        obj.set_mw_starter_header_file_information(self.__mw_starter_header_dirname, self.__mw_starter_header_file_name, self.__component_id)
        obj.write_contents()

    def set_main_shell_script_information(self, dirname, file_name):
        self.__shell_script_dirname = dirname
        self.__shell_script_file_name = file_name

    def generate_main_shell_script_file(self, number_of_components):
        obj = GenerateMainShellScript(self.__component_name, self.__component_class_name, number_of_components)
        obj.set_shell_script_file_information(self.__shell_script_dirname, self.__shell_script_file_name, self.__component_id)
        obj.write_contents()

    def set_sub_shell_script_information(self, dirname, file_name):
        self.__shell_script_dirname = dirname
        self.__shell_script_file_name = file_name

    def generate_sub_shell_script_file(self, number_of_components):
        obj = GenerateSubShellScript(self.__component_name, self.__component_class_name, number_of_components)
        obj.set_shell_script_file_information(self.__shell_script_dirname, self.__shell_script_file_name, self.__component_id)
        obj.write_contents()

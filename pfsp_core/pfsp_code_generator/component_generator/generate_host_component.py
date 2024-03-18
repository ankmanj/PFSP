from cpp_generator import * 
from code_generator import *
from types import FunctionType 

class GenerateHostComponent():
    def __init__(self, header_file_name, component_id, component_name, component_class_name, header_file, cpp_file):
        self.__component_id = component_id
        self.__component_name = component_name
        self.__class_name = component_class_name

        temp_name = 'HostComponent{}InternalDataT'
        self.__wrapped_host_internal_data_name = temp_name.format(self.__component_id)
        self.__host_component_header_file_name = header_file_name
        self.__cpp_class = CppClass(name = component_class_name, 
                                is_struct = False, 
                                is_template = False,
                                parent_class = 'PfspHostComponentBase<' + self.__wrapped_host_internal_data_name + '>')

        #Names of the header & the implementation files 
        self.__cpp_header_file_path = CppFile(header_file)
        self.__cpp_impl_file = CppFile(cpp_file)
        self.__cpp_header_file = CppFile(self.__host_component_header_file_name)

    def get_component_information(self, external_input_component_id):
        self.__external_input_component_id = external_input_component_id

    def gen_methods(self):
        def empty_body(self,cpp):
            cpp("")

        def HostCompInit_body(self,cpp):
            cpp("bool result = true;\n")

            # Trash code for ...
            cpp("host_message test;\n\n")
            cpp("host_message test;\n")
            cpp("host_message test;\n")
            cpp("host_message test;\n")
            cpp("host_message test;\n\n\n")

            cpp("host_message.state = SWComponentStateE::eSWComponentStateInitialized;")
            cpp("host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;")
            cpp("host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;\n")

            cpp("return result;")

        def HostCompRun_body(self,cpp):
            cpp("bool result = true;\n")

            cpp("host_message.state = SWComponentStateE::eSWComponentStateRun;")
            cpp("host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;")
            cpp("host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;\n")

            cpp("return result;")

        def HostCompStop_body(self,cpp):
            cpp("bool result = true;\n")

            cpp("host_message.state = SWComponentStateE::eSWComponentStateStopDone;")
            cpp("host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;")
            cpp("host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;\n")

            cpp("return result;")

        def HostCompShutdown_body(self,cpp):
            cpp("bool result = true;\n")

            cpp("host_message.state = SWComponentStateE::eSWComponentStateShutdownDone;")
            cpp("host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;")
            cpp("host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;\n")

            cpp("return result;")

        cons_method = CppFunction(name = self.__class_name,
                                is_template = False,
                                ret_type = None,
                                implementation_handle = empty_body)

        dest_method = CppFunction(name = '~'+self.__class_name,
                                is_template = False,
                                ret_type = None,
                                is_virtual = True,
                                implementation_handle = empty_body)

        HostCompInit_method = CppFunction(name = 'HostCompInit',
                                            is_template = False,
                                            ret_type = 'bool',
                                            is_virtual = True,
                                            implementation_handle = HostCompInit_body)
        HostCompInit_method.add_argument("const std::string component_name")
        HostCompInit_method.add_argument("HostComponentStatusT& host_message")

        HostCompRun_method = CppFunction(name = 'HostCompRun',
                                        is_template = False,
                                        ret_type = 'bool',
                                        is_virtual = True,
                                        implementation_handle = HostCompRun_body)
        HostCompRun_method.add_argument("HostComponentStatusT& host_message")

        HostCompStop_method = CppFunction(name = 'HostCompStop',
                                        is_template = False,
                                        ret_type = 'bool',
                                        is_virtual = True,
                                        implementation_handle = HostCompStop_body)
        HostCompStop_method.add_argument("HostComponentStatusT& host_message")

        HostCompShutdown_method = CppFunction(name = 'HostCompShutdown',
                                            is_template = False,
                                            ret_type = 'bool',
                                            is_virtual = True,
                                            implementation_handle = HostCompShutdown_body)
        HostCompShutdown_method.add_argument("HostComponentStatusT& host_message")

        self.__cpp_class.add_method(cons_method)
        self.__cpp_class.add_method(dest_method)
        self.__cpp_class.add_method(HostCompInit_method)
        self.__cpp_class.add_method(HostCompRun_method)
        self.__cpp_class.add_method(HostCompStop_method)
        self.__cpp_class.add_method(HostCompShutdown_method)

    def gen_members(self):
        var = CppVariable(name = 'internal_data_',
                        type = self.__wrapped_host_internal_data_name + '*',
                        is_class_member = True,
                        initialization_value = None)
        self.__cpp_class.add_variable(var)

        # var = CppVariable(name = 'internal_data_',
        #                 type = 'std::shared_ptr<' + self.__wrapped_host_internal_data_name + '>',
        #                 is_class_member = True,
        #                 initialization_value = None)
        # self.__cpp_class.add_variable(var)

    def get_includes(self):
        cpp_include = CppInclude()

        # Trash code to insert "pragma once" instead of the code below
        cpp_include.add_includes('a.h')

        temp_name = '{}_internal_data.h'
        internal_data_header_name = temp_name.format(self.__component_name)

        cpp_include.add_includes(internal_data_header_name)
        cpp_include.add_includes('../../../pfsp_core/pfsp_host_component_base/pfsp_host_component_base.h')
        self.__cpp_class.add_includes(cpp_include)

    def generate_header_impl_files(self):
        self.__cpp_class.render_to_string_declaration(self.__cpp_header_file_path)
        self.__cpp_class.render_to_string_implementation(self.__cpp_impl_file, self.__cpp_header_file)

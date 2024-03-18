from cpp_generator import * 
from code_generator import *
from types import FunctionType 

class GenerateBaseComponent():
    def __init__(self, component_name, component_class_name, header_file, cpp_file):
        self.__component_name = component_name
        self.__class_name = component_class_name
        self.__cpp_class = CppClass(name = component_class_name, 
                                is_struct = False, 
                                is_template = False,
                                parent_class = 'AService, INotify')

        #Names of the header & the implementation files 
        self.__cpp_header_file_path = CppFile(header_file)
        self.__cpp_impl_file = CppFile(cpp_file)
        self.__cpp_header_file = CppFile('base_host_component.h')

    def get_component_information(self, component_id, external_input_component_id):
        self.__component_id = component_id
        self.__external_input_component_id = external_input_component_id

    def gen_methods(self):
        def empty_body(self,cpp):
            cpp("")

        def initialize_body(self,cpp):
            cpp("int16_t result = application_service_.Initialize(component_name, component_id,number_of_components);\n")
            cpp("return result;")

        def MwCompStart_body(self,cpp):
            cpp("int16_t result = application_service_.Start();\n")
            cpp("return result;")

        def MwCompStop_body(self,cpp):
            cpp("int16_t result = application_service_.Stop();\n")
            cpp("return result;")

        def MwCompFinalize_body(self,cpp):
            cpp("int16_t result = application_service_.Finalize();\n")
            cpp("return result;")

        def MwCompMessage_body(self,cpp):
            cpp("int16_t result = application_service_.Message(poMessage);\n")
            cpp("return result;")

        def MwCompCommand_body(self,cpp):
            cpp("return -1;")

        def MwCompNotify_body(self,cpp):
            cpp("")
            

        cons_method = CppFunction(name = self.__class_name,
                                is_template = False,
                                ret_type = None,
                                implementation_handle = empty_body)

        dest_method = CppFunction(name = '~'+self.__class_name,
                                is_template = False,
                                ret_type = None,
                                is_virtual = True,
                                implementation_handle = empty_body)

        MwCompInitialize_method = CppFunction(name = 'MwCompInitialize',
                                            is_template = False,
                                            ret_type = 'int16_t',
                                            is_virtual = True,
                                            implementation_handle = initialize_body)
        MwCompInitialize_method.add_argument("std::string component_name")
        MwCompInitialize_method.add_argument("uint32_t component_id")
        MwCompInitialize_method.add_argument("uint32_t number_of_components")

        MwCompStart_method = CppFunction(name = 'MwCompStart',
                                        is_template = False,
                                        ret_type = 'int16_t',
                                        is_virtual = True,
                                        implementation_handle = MwCompStart_body)
        MwCompStart_method.add_argument("void")

        MwCompStop_method = CppFunction(name = 'MwCompStop',
                                        is_template = False,
                                        ret_type = 'int16_t',
                                        is_virtual = True,
                                        implementation_handle = MwCompStop_body)
        MwCompStop_method.add_argument("void")

        MwCompFinalize_method = CppFunction(name = 'MwCompFinalize',
                                            is_template = False,
                                            ret_type = 'int16_t',
                                            is_virtual = True,
                                            implementation_handle = MwCompFinalize_body)
        MwCompFinalize_method.add_argument("void")

        MwCompMessage_method = CppFunction(name = 'MwCompMessage',
                                        is_template = False,
                                        ret_type = 'int16_t',
                                        is_virtual = True,
                                        implementation_handle = MwCompMessage_body)
        MwCompMessage_method.add_argument("AServiceMessage *poMessage")

        MwCompCommand_method = CppFunction(name = 'MwCompCommand',
                                        is_template = False,
                                        ret_type = 'int16_t',
                                        is_virtual = True,
                                        implementation_handle = MwCompCommand_body)
        MwCompCommand_method.add_argument("int32_t nArgc")
        MwCompCommand_method.add_argument("char_t *pszArgv[]")

        MwCompNotify_method = CppFunction(name = 'MwCompNotify',
                                        is_template = False,
                                        ret_type = 'void',
                                        is_virtual = True,
                                        implementation_handle = MwCompNotify_body)
        MwCompNotify_method.add_argument("void *pvArg")

        self.__cpp_class.add_method(cons_method)
        self.__cpp_class.add_method(dest_method)
        self.__cpp_class.add_method(MwCompInitialize_method)
        self.__cpp_class.add_method(MwCompStart_method)
        self.__cpp_class.add_method(MwCompStop_method)
        self.__cpp_class.add_method(MwCompFinalize_method)
        self.__cpp_class.add_method(MwCompMessage_method)
        self.__cpp_class.add_method(MwCompCommand_method)
        self.__cpp_class.add_method(MwCompNotify_method)
        self.__cpp_class.add_header_specific_lines(self.__add_header_specific_lines())
        self.__cpp_class.add_impl_specific_lines(self.__add_impl_specific_lines())

    def gen_members(self):
        temp_name = '{}'
        real_host_component_class_name = self.__class_name.replace("Base","")
        host_class_name = temp_name.format(real_host_component_class_name)

        temp_name = 'SmartConverterCore{}'
        wrapped_host_smart_converter_name = temp_name.format(self.__component_id)

        temp_name = 'HostComponent{}'
        wrapped_host_component_name = temp_name.format(self.__component_id)

        temp_name = 'HostComponent{}'
        wrapped_external_input_host_component_name = temp_name.format(self.__external_input_component_id)

        temp_name = 'eHostApplicationComponentOffset{}'
        wrapped_host_component_offset = temp_name.format(self.__component_id)

        temp_name = '{}ExternalInputT'
        wrapped_host_component_external_input = temp_name.format(wrapped_host_component_name)

        temp_name = '{}InternalDataT'
        wrapped_host_component_internal_data = temp_name.format(wrapped_host_component_name)

        temp_name = '{}ExternalOutputT'
        wrapped_host_component_external_output = temp_name.format(wrapped_host_component_name)

        self.__cpp_class.add_variable(CppVariable(name = 'component', type = host_class_name))

        #This is a hack for now, The constructor arguments shall be a part of the initialization_value
        var = CppVariable(name = 'smart_converter_core_ = std::make_shared<' + wrapped_host_smart_converter_name + '>("' 
                                                                            + wrapped_host_component_name + '")',
                        type = 'std::shared_ptr< SmartConverterCoreBase<' + wrapped_host_component_external_input + ', ' 
                                                                        + wrapped_host_component_internal_data + ', '
                                                                        + wrapped_host_component_external_output + '> >' ,
                        is_class_member = True,
                        initialization_value = None)
        self.__cpp_class.add_variable(var)



        #This is a hack for now, The constructor arguments shall be a part of the initialization_value
        var = CppVariable(name = 'application_service_{ ' + '"' + wrapped_host_component_name + '", ' 
                                                        + '"' + wrapped_external_input_host_component_name + '", ' 
                                                        + wrapped_host_component_offset 
                                                        + ', this, &component, smart_converter_core_ }',
                        type = 'CHostApplicationService<' + wrapped_host_component_external_input + ', ' 
                                                        + wrapped_host_component_internal_data + ', ' 
                                                        + wrapped_host_component_external_output + '>',
                        is_class_member = True,
                        initialization_value = None)
        self.__cpp_class.add_variable(var)

    def get_includes(self):
        cpp_include = CppInclude()

        # Trash code to insert "pragma once" instead of the code below
        cpp_include.add_includes('a.h')

        cpp_include.add_includes('smart_converter_core.h')
        temp_host_component_header_file = '{}.hpp'
        host_component_header_file = temp_host_component_header_file.format(self.__component_name)
        cpp_include.add_includes(host_component_header_file)
        cpp_include.add_includes('host_component_service.h')
        self.__cpp_class.add_includes(cpp_include)

    def generate_header_impl_files(self):
        # base_host_component
        #self.__cpp_class.render_to_string_declaration(self.__cpp_header_file) 
        self.__cpp_class.render_to_string_declaration(self.__cpp_header_file_path)
        self.__cpp_class.render_to_string_implementation(self.__cpp_impl_file, self.__cpp_header_file)
       
    def __add_header_specific_lines(self):
        service_name = self.__class_name
        return 'DECLARE_SERVICE_CLASS(' + service_name + ')'

    def __add_impl_specific_lines(self):
        service_name = self.__class_name
        return 'IMPLEMENT_SERVICE_CLASS(' + service_name + ')'


# This script creats the cpp file with data mapping.
import logging
import sys
import json
import os


class JsonGenClass():
    def __init__(self):
        self.name: str = ""
        self.base_name: str = ""
        self.template_args: dict = []
        self.functions: list = []
        self.includes: list = []


class JsonGenFunction():
    def __init__(self):
        self.name = ""
        self.body = ""
        self.args_types_param = {}
        self.return_type = ""


class JsonDataMapFuncGen():
    def __init__(self):
        # Initialize the logging
        logging.basicConfig(level=logging.INFO,
                            format='%(asctime)s %(levelname)s %(threadName)s %(name)s %(message)s',
                            datefmt='%d-%b-%y %H:%M:%S')

    def get_unique_sources(self, func_list):
        param_list = {}
        check_list = []
        for item in func_list:
            source = item['from']
            if source not in check_list:
                check_list.append(source)
                # not good idea as class name can be in lower case alredy.
                param_list[source] = source.lower()
        return param_list

    def make_func_body(self, mapping_list, return_type):
        body = []
        function_body = []
        for map_item in mapping_list:
            body.append(f'{return_type.lower()}->' + map_item['dst_var_name'] +
                        " = " + map_item['from'].lower() + '->' + map_item['assigne'] + ";\n")
        formated_boy = "\t".join(body)
        function_body.append(f'{formated_boy}')
        return "".join(function_body)

    def get_classes_from(self, json_path: str = None) -> list:
        logging.info('Extracting the mapping and forming the function body.')
        if not os.path.isfile(json_path):
            logging.error("File does not exists: {0} ".format(json_path))
            sys.exit(-1)

        logging.info(f'Generating the mapper in C++ for {json_path}')
        with open(json_path) as f:
            main_data = json.load(f)

        direction_keys = list(main_data.keys())
        cpp_class_list = []
        for direction in direction_keys:  # iterate over the directions
            logging.info(
                "Generating the function for the {0}".format(direction))

            data = main_data[direction]
            base_class = direction
            function_name = data['function']
            calss_initial = data['class_initial']
            childs = data['data_map']
            include_list = data['includes']
            return_keys = list(childs.keys())

            for return_type in return_keys:
                # TODO move below hard code to json file later.
                set_memory_func = JsonGenFunction()
                set_memory_func.name = "GetInternalMemory"
                set_memory_func.args_types_param = {"MemoryMan*": "memory"}
                set_memory_func.return_type = "bool"
                mem_assign_format = "{0} = {1}->getMem<{2}*>();\n"

                cpp_function = JsonGenFunction()
                cpp_class = JsonGenClass()
                cpp_class.includes = list(include_list)

                cpp_class.name = calss_initial + return_type
                cpp_class.base_name = base_class

                logging.info(f'Extract the converter types for {return_type}')
                mapping_list = childs[return_type]
                cpp_function.name = function_name
                params = self.get_unique_sources(mapping_list)
                cpp_function.args_types_param = {"void": ""}
                cpp_function.return_type = "void"
                cpp_function.body = self.make_func_body(
                    mapping_list, return_type)
                cpp_class.template_args.extend(list(params.keys()))
                cpp_class.template_args.append(return_type)

                #     TODO add the function body with return types.
                setmem_body = []
                setmem_body.append("bool result = true;\n")
                memptr = list(set_memory_func.args_types_param.values())[0]
                for type_t, var_name in params.items():
                    memassign = mem_assign_format.format(
                        var_name, memptr, type_t)
                    setmem_body.append(memassign)
                setmem_body.append(
                    mem_assign_format.format(return_type.lower(), memptr, return_type))
                setmem_body.append("return result;")
                set_memory_func.body = "\t".join(setmem_body)

                cpp_class.functions.append(cpp_function)
                cpp_class.functions.append(set_memory_func)

                cpp_class_list.append(cpp_class)

        logging.info('Done making functions.')
        return cpp_class_list


def main():
    print("hello world! APMS")
    abc = JsonDataMapFuncGen()
    data = abc.get_classes_from("../../../../pfsp_config/json/HostApplication1_Smart_Converter.json")
    print(data)


if __name__ == "__main__":
    main()

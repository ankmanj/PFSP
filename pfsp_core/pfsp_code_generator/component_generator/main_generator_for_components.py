import os
import sys
import argparse
import glob
import json

#from cpp_generator
from cpp_generator import * 
from code_generator import *
from lib.data_map_gen.json_function_gen import *
from generation_coordinator import GenerationCordinator
from pprint import PrettyPrinter

pp = PrettyPrinter(indent=2, compact=True).pprint

defaultInputDir = '../../pfsp_config/json'
#inputFilePatternForSmartConverter = 'smart_converter_host_component[0-9].json'
inputFilePatternForSmartConverter = 'smart_converter_[0-9]_*.json'
inputFileForHostComponent = '../../pfsp_config/json/pfsp_main_config.json'

defaultBaseComponentOutputBaseDir = '../../pfsp_component_plugin/component_generated'
baseComponentHeaderFilename = 'base_host_component.h'
baseComponentCppFilename = 'base_host_component.cpp'

def main(argv):
  parser = argparse.ArgumentParser(description='generate base component')
  parser.add_argument(
    '--input-dir', action='store', nargs='?', required=False,
    dest='input_dir', type=str, default= defaultInputDir,
    help='Specify the directiory with ' + inputFilePatternForSmartConverter
  )
  parser.add_argument(
    '--output-header-base-dir', action='store', nargs='?', required=False,
    dest='output_header_base_dir', type=str, default=defaultBaseComponentOutputBaseDir,
    help='Specify the directory for saving ' + baseComponentHeaderFilename
  )
  parser.add_argument(
    '--output-cpp-base-dir', action='store', nargs='?', required=False,
    dest='output_cpp_base_dir', type=str, default=defaultBaseComponentOutputBaseDir,
    help='Specify the directory for saving ' + baseComponentCppFilename
  )

  args = parser.parse_args()
  if not os.path.isdir(args.input_dir):
    raise Exception('No directory for reading JSON fies')
  if not os.path.isdir(args.output_header_base_dir):
    raise Exception('No directory for saving baseComponentHeaderFilename.h')

  base_component_dict_from_smart_converter = dict()
  host_component_dict = dict()
  
  input_filenames = glob.glob(args.input_dir + '/' + inputFilePatternForSmartConverter)
  print('input_filenames:', input_filenames)
  for filename in input_filenames:
    print('input filename:', filename)
    with open(filename, 'r') as f:
      
      json_data = json.load(f)
      base_component_id = 0
      external_input_compoint_id = 0

      for k,v in json_data.items():
        base_component_id = int(v['smart_converter_component_id'])
        flag_set_break = 0;        
        for class_name,mappings in v['data_map'].items():
          flag_set_break = 0;        
          for connection in mappings:
            external_input_compoint_id = int(connection['scomponent_id'])
            flag_set_break = 1
            break
          if flag_set_break == 1:
            break
        if flag_set_break == 1:
          break

      base_component_dict_from_smart_converter[base_component_id] = {
        'ExternalInputComponentID': external_input_compoint_id
      }

  json_data = None
  number_of_components = 0

  with open(inputFileForHostComponent, 'r') as f:
    json_data = json.load(f)

    for k,v in json_data.items():
      for service_key,mappings in v['serviceTable'].items():
            
        host_component_id = 0;
        host_component_name = ""
        host_component_class_name = ""

        for connection in mappings:
          input_host_component_name = connection['name']
          input_host_component_id = int(connection['id'])
          input_host_component_class_name = connection['class']

          if input_host_component_id >= 4:
            host_component_id = input_host_component_id - 4
            host_component_name = input_host_component_name.replace("/","")
            host_component_class_name = input_host_component_class_name
            external_input_compoint_id = base_component_dict_from_smart_converter[host_component_id]['ExternalInputComponentID']

            #component_index = "component_" + str(host_component_id)
            # component_index = host_component_id
            host_component_dict[host_component_id] = {
              'ComponentName': host_component_name,
              'ComponentClassName': host_component_class_name,
              'ExternalInputComponentID': external_input_compoint_id
            }
            number_of_components = host_component_id

  # ===============================================================================================
  # ====================== Start the code for generation of each components =======================
  # ===============================================================================================
  base_component_coordinator = GenerationCordinator()
  host_component_coordinator = GenerationCordinator()
  mw_starter_header_coordinator = GenerationCordinator()
  main_shell_script_coordinator = GenerationCordinator()
  sub_shell_script_coordinator = GenerationCordinator()

  cmake_file_name = 'CMakeLists.txt'
  base_components_dirname = '../../pfsp_component_plugin/component_generated'
  host_components_dirname = '../../../host_components'
  mw_starter_header_dirname = '../../pfsp_middleware_start/include'
  mw_starter_header_name = 'pfsp_mw_start.h'
  main_shell_script_dirname = '../../'
  main_shell_script_file_name = 'run_pfsp_second.sh'
  sub_shell_script_dirname = '../../../'
  sub_shell_script_file_name = 'simple_run.sh'

  with open(os.path.join(base_components_dirname, cmake_file_name), 'w') as f:
    f.close()
  with open(os.path.join(host_components_dirname, cmake_file_name), 'w') as f:
    f.close()
  with open(os.path.join(mw_starter_header_dirname, mw_starter_header_name), 'w') as f:
    f.close()
  with open(os.path.join(main_shell_script_dirname, main_shell_script_file_name), 'w') as f:
    f.close()
  with open(os.path.join(sub_shell_script_dirname, sub_shell_script_file_name), 'w') as f:
    f.close()

  for component_id_key,component_info_values in host_component_dict.items():
    component_name = host_component_dict[component_id_key]['ComponentName']
    component_class_name = host_component_dict[component_id_key]['ComponentClassName']
    external_input_component_id = host_component_dict[component_id_key]['ExternalInputComponentID']
    print('component_name:', component_name)
    print('component_class_name:', component_class_name)
    print('external_component_id:', external_input_component_id)
    print('\n===========================\n')

    # ===============================================================================================
    # ========================= Part for generation of pfsp_component_plugin =========================
    # ===============================================================================================
    base_component_coordinator.set_component_information(int(component_id_key), external_input_component_id, component_name, component_class_name)

    dest_dirname = '../../pfsp_component_plugin/component_generated/base_host_component_{}'
    header_file = 'include/base_host_component.h'
    cpp_file = 'src/base_host_component.cpp'
    header_file_name = 'base_host_component.h'

    #dest_path = dest_dirname.format(component_name)
    dest_path = dest_dirname.format(int(component_id_key))
    header_file_path = dest_path + '/' + header_file
    cpp_file_path = dest_path + '/' + cpp_file
    cmake_file_path = dest_path + '/' + cmake_file_name

    component_dirname = (dest_path)
    if not os.path.isdir(component_dirname):
      os.makedirs(component_dirname)

    header_dirname = (dest_path + '/' + 'include')
    if not os.path.isdir(header_dirname):
      os.makedirs(header_dirname)

    cpp_dirname = (dest_path + '/' + 'src')
    if not os.path.isdir(cpp_dirname):
      os.makedirs(cpp_dirname)

    base_component_coordinator.set_component_files_information(dest_path, header_file_path, cpp_file_path)
    base_component_coordinator.generate_base_component()

    base_component_coordinator.set_base_component_cmake_information(component_dirname, cmake_file_name)
    base_component_coordinator.generate_base_component_cmake_file()

    with open(os.path.join(header_dirname, header_file_name), 'r+') as f:
      f.write( '#pragma once\n\n')
      f.close()

    component_id = int(component_id_key)
    with open(os.path.join(base_components_dirname, cmake_file_name), 'a+') as f:
      f.write(f'add_subdirectory(base_host_component_{component_id})\n')
      f.close()

    # ==========================================================================================
    # ========================= Part for generation of host_components =========================
    # ==========================================================================================
    real_host_component_class_name = component_class_name.replace("Base","")
    host_component_coordinator.set_component_information(int(component_id_key), external_input_component_id, component_name, real_host_component_class_name)

    host_component_dest_dirname = '../../../host_components/{}'
    host_component_header_file = 'include/{}.hpp'
    host_component_cpp_file = 'src/{}.cpp'
    temp_header_file_name = '{}.hpp'
    host_component_header_file_name = temp_header_file_name.format(component_name)
    temp_cpp_file_name = '{}.cpp'
    host_component_cpp_file_name = temp_cpp_file_name.format(component_name)

    host_component_dest_path = host_component_dest_dirname.format(component_name)
    host_component_header_file_path = host_component_dest_path + '/' + host_component_header_file.format(component_name)
    host_component_cpp_file_path = host_component_dest_path + '/' + host_component_cpp_file.format(component_name)

    host_component_dirname = (host_component_dest_path)
    if not os.path.isdir(host_component_dirname):
      os.makedirs(host_component_dirname)

    host_component_header_dirname = (host_component_dest_path + '/' + 'include')
    if not os.path.isdir(host_component_header_dirname):
      os.makedirs(host_component_header_dirname)

    host_component_cpp_dirname = (host_component_dest_path + '/' + 'src')
    if not os.path.isdir(host_component_cpp_dirname):
      os.makedirs(host_component_cpp_dirname)

    host_component_coordinator.set_component_files_information(host_component_dest_path, host_component_header_file_path, host_component_cpp_file_path)
    host_component_coordinator.generate_host_component(host_component_header_file_name)

    host_component_coordinator.set_host_component_cmake_information(host_component_dirname, cmake_file_name)
    host_component_coordinator.generate_host_component_cmake_file()

    with open(os.path.join(host_component_cpp_dirname, host_component_cpp_file_name), 'r+') as f:
      f.write(f'#include "{host_component_header_file_name}" \n\n')
      f.write(f'{real_host_component_class_name}::{real_host_component_class_name}()\n')
      f.write('{\n')
      f.write('}\n\n')
      f.write(f'{real_host_component_class_name}::~{real_host_component_class_name}()\n')
      f.write('{\n')
      f.write('}\n\n')
      f.write(f'bool {real_host_component_class_name}::HostCompInit(const std::string component_name, HostComponentStatusT& host_message)\n')
      f.write('{\n')
      f.write('    bool result = true;\n\n')
      f.write(f'    internal_data_ = dynamic_cast<HostComponent{component_id_key}InternalDataT*>(this->GetInternalDataMemory(component_name));\n\n')
      f.close()

    with open(os.path.join(host_component_header_dirname, host_component_header_file_name), 'r+') as f:
      f.write( '#pragma once\n\n')
      f.close()

    component_id = int(component_id_key)
    with open(os.path.join(host_components_dirname, cmake_file_name), 'a+') as f:
      f.write(f'add_subdirectory({component_name})\n')
      f.close()

    # ==========================================================================================
    # ==================== Part for generation of Middleware Starter Header ====================
    # ==========================================================================================
    mw_starter_header_coordinator.set_component_information(int(component_id_key), external_input_component_id, component_name, component_class_name)

    mw_starter_header_dest_dirname = mw_starter_header_dirname
    mw_starter_header_file_name = mw_starter_header_name

    mw_starter_header_coordinator.set_mw_starter_header_information(mw_starter_header_dest_dirname, mw_starter_header_file_name)
    mw_starter_header_coordinator.generate_mw_starter_header_file(number_of_components)

    # ==========================================================================================
    # =========================== Part for generation of run_pfsp_second.sh ===========================
    # ==========================================================================================
    main_shell_script_coordinator.set_component_information(int(component_id_key), external_input_component_id, component_name, component_class_name)

    shell_script_dest_dirname = main_shell_script_dirname
    shell_script_name = main_shell_script_file_name

    main_shell_script_coordinator.set_main_shell_script_information(shell_script_dest_dirname, shell_script_name)
    main_shell_script_coordinator.generate_main_shell_script_file(number_of_components)

    # ==========================================================================================
    # =========================== Part for generation of simple_run.sh ===========================
    # ==========================================================================================
    sub_shell_script_coordinator.set_component_information(int(component_id_key), external_input_component_id, component_name, component_class_name)

    shell_script_dest_dirname = sub_shell_script_dirname
    shell_script_name = sub_shell_script_file_name

    sub_shell_script_coordinator.set_sub_shell_script_information(shell_script_dest_dirname, shell_script_name)
    sub_shell_script_coordinator.generate_sub_shell_script_file(number_of_components)

if __name__ == '__main__':
  main(sys.argv)
#!/bin/bash

current_Path=$(pwd)

build_path="build"
cd $current_Path/pfsp_core/$build_path
make

cd $current_Path/pfsp_core/$build_path/pfsp_component_plugin/error_handler
cp -rf libPFSP_plugin_error_handler.so $current_Path/pfsp_core/pfsp_lib/libs

cd $current_Path/pfsp_core/$build_path/pfsp_component_plugin/mode_manager
cp -rf libPFSP_plugin_mode_manager.so $current_Path/pfsp_core/pfsp_lib/libs

cd $current_Path/pfsp_core/$build_path/pfsp_component_plugin/component_generated/base_host_component_0
cp -rf libPFSP_base_host_component_0.so $current_Path/pfsp_core/pfsp_lib/libs

cd $current_Path/pfsp_core/$build_path/$build_path/component1
cp -rf libPFSP_component1.so $current_Path/pfsp_core/pfsp_lib/libs

cd $current_Path/pfsp_core/$build_path/pfsp_component_plugin/component_generated/base_host_component_1
cp -rf libPFSP_base_host_component_1.so $current_Path/pfsp_core/pfsp_lib/libs

cd $current_Path/pfsp_core/$build_path/$build_path/component2
cp -rf libPFSP_component2.so $current_Path/pfsp_core/pfsp_lib/libs

cd $current_Path/pfsp_core/$build_path/pfsp_component_plugin/component_generated/base_host_component_2
cp -rf libPFSP_base_host_component_2.so $current_Path/pfsp_core/pfsp_lib/libs

cd $current_Path/pfsp_core/$build_path/$build_path/component3
cp -rf libPFSP_component3.so $current_Path/pfsp_core/pfsp_lib/libs

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$current_Path/pfsp_core/pfsp_lib/libs
cd $current_Path/pfsp_core/$build_path/pfsp_main
sudo ./PFSP_main

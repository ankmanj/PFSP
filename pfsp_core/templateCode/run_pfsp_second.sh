#!/bin/bash

current_Path=$(pwd)

cd $current_Path

build_path="build"
if [ -d $build_path ]; then
    sudo rm -rf $build_path
    mkdir $build_path
else
    mkdir $build_path
fi

cd $current_Path/$build_path
cmake ..
make

cd $current_Path/$build_path/pfsp_component_plugin/error_handler
cp -rf libPFSP_plugin_error_handler.so $current_Path/pfsp_lib

cd $current_Path/$build_path/pfsp_component_plugin/mode_manager
cp -rf libPFSP_plugin_mode_manager.so $current_Path/pfsp_lib

cd $current_Path/$build_path/pfsp_component_plugin/component_generated/base_host_component_1
cp -rf libPFSP_base_host_component_0.so $current_Path/pfsp_lib

cd $current_Path/$build_path/host_components/host_application_1
cp -rf libHOST_app_component_1.so $current_Path/pfsp_lib

cd $current_Path/$build_path/pfsp_component_plugin/component_generated/base_host_component_2
cp -rf libPFSP_base_host_component_1.so $current_Path/pfsp_lib

cd $current_Path/$build_path/host_components/host_application_2
cp -rf libHOST_app_component_2.so $current_Path/pfsp_lib

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$current_Path/pfsp_lib
cd $current_Path/$build_path/pfsp_main
sudo ./PFSP_main

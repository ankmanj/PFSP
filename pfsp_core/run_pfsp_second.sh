#!/bin/bash

current_Path=$(pwd)

cd $current_Path/pfsp_lib
lib_path="libs"
if [ -d $lib_path ]; then
    sudo rm -rf $lib_path
    mkdir $lib_path
else
    mkdir $lib_path
fi

cd $current_Path/pfsp_lib/PFSPMiddleware
cp -rf libPFSP_MW.so $current_Path/pfsp_lib/libs

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
cp -rf libPFSP_plugin_error_handler.so $current_Path/pfsp_lib/libs

cd $current_Path/$build_path/pfsp_component_plugin/mode_manager
cp -rf libPFSP_plugin_mode_manager.so $current_Path/pfsp_lib/libs

cd $current_Path/$build_path/pfsp_component_plugin/component_generated/base_host_component_0
cp -rf libPFSP_base_host_component_0.so $current_Path/pfsp_lib/libs

cd $current_Path/$build_path/$build_path/component1
cp -rf libPFSP_component1.so $current_Path/pfsp_lib/libs

cd $current_Path/$build_path/pfsp_component_plugin/component_generated/base_host_component_1
cp -rf libPFSP_base_host_component_1.so $current_Path/pfsp_lib/libs

cd $current_Path/$build_path/$build_path/component2
cp -rf libPFSP_component2.so $current_Path/pfsp_lib/libs

cd $current_Path/$build_path/pfsp_component_plugin/component_generated/base_host_component_2
cp -rf libPFSP_base_host_component_2.so $current_Path/pfsp_lib/libs

cd $current_Path/$build_path/$build_path/component3
cp -rf libPFSP_component3.so $current_Path/pfsp_lib/libs

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$current_Path/pfsp_lib/libs
cd $current_Path/$build_path/pfsp_main
sudo ./PFSP_main

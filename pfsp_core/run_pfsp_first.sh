#!/bin/bash

current_Path=$(pwd)

echo -e "==================\n"$current_Path"\n==================\n"

cd $current_Path/pfsp_component_plugin
component_generated_path="component_generated"
if [ -d $component_generated_path ]; then
    sudo rm -rf $component_generated_path
    mkdir $component_generated_path
else
    mkdir $component_generated_path
fi
mkdir $current_Path/pfsp_component_plugin/component_generated/external_interface

cd $current_Path
cd ../
sudo rm -rf host_components
mkdir host_components

cd $current_Path/pfsp_code_generator/component_generator
python3 main_generator_for_components.py

cd $current_Path/pfsp_code_generator/smart_converter_generator
python3 main_generator_for_smart_converter.py

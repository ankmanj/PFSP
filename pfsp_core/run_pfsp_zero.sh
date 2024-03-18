#!/bin/bash

current_Path=$(pwd)

echo -e "==================\n"$current_Path"\n==================\n"

cd $current_Path/pfsp_config
config_xml_path="xml"
if [ -d $config_xml_path ]; then
    sudo rm -rf $config_xml_path
    mkdir $config_xml_path
else
    mkdir $config_xml_path
fi

config_json_path="json"
if [ -d $config_json_path ]; then
    sudo rm -rf $config_json_path
    mkdir $config_json_path
else
    mkdir $config_json_path
fi

#pragma once

#include <cassert>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <memory>

template<typename TExternalInput, typename TComponentInternal, typename TExternalOutput>
class SmartConverterCoreMemoryManager
{
public:
    SmartConverterCoreMemoryManager(void) {}
    virtual ~SmartConverterCoreMemoryManager(void) {}

    // =============================================== External Input ================================================ //

    void SetExternalInputMemory(std::string component_name, TExternalInput* external_input_ptr)
    {
        external_input_map_.insert(std::make_pair(component_name, external_input_ptr));
        //std::cout << "\n SmartConverterCoreMemoryManager    :: SetExternalInputMemory: count" << count << std::endl;
        count++;
    }

    TExternalInput* GetExternalInputMemory(std::string component_name)
    {
        TExternalInput* return_ptr {};

        for( auto& external_input : external_input_map_ )
        {
            std::string name = external_input.first;

            if( !strcmp(name.c_str(), component_name.c_str()) )
            {
                return_ptr = external_input.second;
            }
        }

        return return_ptr;
    }

    // =============================================== Internal Data ================================================ //

    //void SetInternalDataMemory(std::string component_name, std::shared_ptr<TComponentInternal> internal_data_ptr)
    void SetInternalDataMemory(std::string component_name, TComponentInternal* internal_data_ptr)
    {
        //internal_data_map_.insert(std::pair<std::string, std::shared_ptr<TComponentInternal>>(component_name, internal_data_ptr));
        internal_data_map_.insert(std::make_pair(component_name, internal_data_ptr));
        //std::cout << "\n SmartConverterCoreMemoryManager    :: SetInternalDataMemory: count" << count << std::endl;
        count++;
    }

    TComponentInternal* GetInternalDataMemory(std::string component_name)
    {
        TComponentInternal* return_ptr {};
        for( auto& internal_data : internal_data_map_ )
        {
            std::string name = internal_data.first;

            if( !strcmp(name.c_str(), component_name.c_str()) )
            {
                return_ptr = internal_data.second;
            }
        }

        return return_ptr;
    }

    // =============================================== External Output ================================================ //

    void SetExternalOutputMemory(std::string component_name, TExternalOutput* external_output_ptr)
    {
        external_output_map_.insert(std::make_pair(component_name, external_output_ptr));
        //std::cout << "\n SmartConverterCoreMemoryManager    :: SetExternalOutputMemory: count" << count << std::endl;

        count++;
    }

    TExternalOutput* GetExternalOutputMemory(std::string component_name)
    {
        TExternalOutput* return_ptr {};
        for( auto& external_output : external_output_map_ )
            {
                std::string name = external_output.first;

                if( !strcmp(name.c_str(), component_name.c_str()) )
                {
                    return_ptr = external_output.second;
                }
            }

        return return_ptr;
    }

private:
    std::unordered_map< std::string, TExternalInput* > external_input_map_{};
    std::unordered_map< std::string, TExternalOutput* > external_output_map_{};
    std::unordered_map< std::string, TComponentInternal* > internal_data_map_{};
    //std::unordered_map< std::string, std::shared_ptr<TComponentInternal> > internal_data_map_{};

    std::uint32_t count{}; 
};


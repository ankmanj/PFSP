#ifndef PFSP_HOSTAPP_MEMMANAGER_H_
#define PFSP_HOSTAPP_MEMMANAGER_H_

#include "../../pfsp_include/pfsp_enum.h"

template<typename TInternalData>
class PfspHostComponentMemoryManager
{
public:
    PfspHostComponentMemoryManager(void) {}
    virtual ~PfspHostComponentMemoryManager(void) {}

    void SetInternalDataMemory(std::string component_name, TInternalData* internal_data_ptr)
    {
        internal_data_map_.insert(std::make_pair(component_name, internal_data_ptr));
        //std::cout << "\n PfspHostComponentMemoryManager::SetInternalDataMemory: count" << count << std::endl;
        count++;
    }

    TInternalData* GetInternalDataMemory(std::string component_name)
    {
        TInternalData* return_ptr {};
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

private:
    std::unordered_map< std::string, TInternalData* > internal_data_map_{};
    std::uint32_t count{}; 
};

#endif /* PFSP_HOSTAPP_MEMMANAGER_H_ */

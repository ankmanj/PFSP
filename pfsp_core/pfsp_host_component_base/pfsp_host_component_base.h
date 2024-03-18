#ifndef PFSP_HOSTAPP_INTERFACE_H_
#define PFSP_HOSTAPP_INTERFACE_H_

#include "../pfsp_component_core/smart_converter/component_internal_data_memmanager.h"

template<typename TInternalData>
class PfspHostComponentBase: virtual public PfspHostComponentMemoryManager<TInternalData>
{
public:
    PfspHostComponentBase(void) {}
    virtual ~PfspHostComponentBase(void) {}

    virtual bool HostCompInit(std::string component_name, HostComponentStatusT &host_message) = 0;

    virtual bool HostCompRun(HostComponentStatusT &host_message) = 0;

    virtual bool HostCompStop(HostComponentStatusT &host_message) = 0;

    virtual bool HostCompShutdown(HostComponentStatusT &host_message) = 0;

    //virtual bool HostCompResetData(void) = 0;

    //virtual bool HostCompGetMessageFromPlugin(void) = 0;

    //virtual bool HostCompGetInternalDataMem(HostComponentStatusT &host_message) = 0;
    
    //virtual bool HostCompSendMessageToPlugin(const PfspHostMessageTypeE output_message_type, const unsigned int output_message_value) = 0;

protected:

private:

};
#endif /* PFSP_HOSTAPP_INTERFACE_H_ */

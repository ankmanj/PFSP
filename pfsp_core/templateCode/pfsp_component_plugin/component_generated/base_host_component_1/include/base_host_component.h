#pragma once

#include "smart_converter_core.h"
#include "host_component_service.h"
#include "host_app_component_1.h"

class CHostApplication1Service : public AService, public INotify
{
    DECLARE_SERVICE_CLASS(CHostApplication1Service)
public:
    CHostApplication1Service(void){

    }

    virtual ~CHostApplication1Service(void){};

    virtual int16_t MwCompInitialize(std::string component_name, uint32_t component_id, uint32_t number_of_components);
    virtual int16_t MwCompStart(void);
    virtual int16_t MwCompStop(void);
    virtual int16_t MwCompFinalize(void);
    virtual int16_t MwCompMessage(AServiceMessage *poMessage);
    virtual int16_t MwCompCommand(int32_t nArgc, char_t *pszArgv[]);
    virtual void MwCompNotify(void *pvArg);

private:
    HostAppComponent1 component; 

#if defined(FEATURE_USE_SMART_PTR)
    std::shared_ptr< SmartConverterCoreBase<HostComponent0ExternalInputT, HostComponent0InternalDataT, HostComponent0ExternalOutputT> > \
                    smart_converter_core_ = std::make_shared<SmartConverterCore0>("HostComponent0");
#else
    SmartConverterCore0 smart_converter_core_{"HostComponent0"};
#endif

    // Key: <HostComponent0ExternalInputT, HostComponent0InternalDataT, HostComponent0ExternalOutputT> : 
    //      Those parameters are default. That means if this component ID is "2", the parameters should be "<HostComponent1ExternalInputT, >"
    CHostApplicationService<HostComponent0ExternalInputT, HostComponent0InternalDataT, HostComponent0ExternalOutputT> 
                    application_service_{ "HostComponent0", "HostComponent1", 
                                            eHostApplicationComponentOffset0, 
                                            this, 
                                            &component, 
#if defined(FEATURE_USE_SMART_PTR)
                                            smart_converter_core_ };
#else
                                            &smart_converter_core_ };
#endif
};

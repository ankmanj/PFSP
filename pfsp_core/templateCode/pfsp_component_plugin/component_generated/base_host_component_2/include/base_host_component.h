#pragma once

#include "smart_converter_core.h"
#include "host_app_component_2.h"
#include "host_component_service.h"

class CHostApplication2Service : public AService, public INotify
{
    DECLARE_SERVICE_CLASS(CHostApplication2Service)
public:      
    CHostApplication2Service(void){

    }    

    virtual ~CHostApplication2Service(void){};

    virtual int16_t MwCompInitialize(std::string component_name, uint32_t component_id, uint32_t number_of_components);
    virtual int16_t MwCompStart(void);
    virtual int16_t MwCompStop(void);
    virtual int16_t MwCompFinalize(void);
    virtual int16_t MwCompMessage(AServiceMessage *poMessage);
    virtual int16_t MwCompCommand(int32_t nArgc, char_t *pszArgv[]);
    virtual void MwCompNotify(void *pvArg);

private:
    HostAppComponent2 component;
#if defined(FEATURE_USE_SMART_PTR)
    std::shared_ptr< SmartConverterCoreBase<HostComponent1ExternalInputT, HostComponent1InternalDataT, HostComponent1ExternalOutputT> > \
                    smart_converter_core_ = std::make_shared<SmartConverterCore1>("HostComponent1");
#else
    SmartConverterCore1 smart_converter_core_{"HostComponent1"};
#endif


    CHostApplicationService<HostComponent1ExternalInputT, HostComponent1InternalDataT, HostComponent1ExternalOutputT> application_service_\
                                                                                                    {   "HostComponent1", \
                                                                                                        "HostComponent0", \
                                                                                                        eHostApplicationComponentOffset1, \
                                                                                                        this, \
                                                                                                        &component,
#if defined(FEATURE_USE_SMART_PTR)
                                                                                                        smart_converter_core_   };
#else
                                                                                                        &smart_converter_core_   };
#endif
};

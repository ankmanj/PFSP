#pragma once


#include "smart_converter_core.h"
#include "component2.hpp"
#include "host_component_service.h"


class CService2Base  : public AService, INotify
{
	DECLARE_SERVICE_CLASS(CService2Base)
	
public:
	 CService2Base();
	
	virtual  ~CService2Base();
	
	virtual int16_t MwCompInitialize(std::string component_name, uint32_t component_id, uint32_t number_of_components);
	
	virtual int16_t MwCompStart(void);
	
	virtual int16_t MwCompStop(void);
	
	virtual int16_t MwCompFinalize(void);
	
	virtual int16_t MwCompMessage(AServiceMessage *poMessage);
	
	virtual int16_t MwCompCommand(int32_t nArgc, char_t *pszArgv[]);
	
	virtual void MwCompNotify(void *pvArg);
	
	
private:
	CService2 component;
	
	std::shared_ptr< SmartConverterCoreBase<HostComponent1ExternalInputT, HostComponent1InternalDataT, HostComponent1ExternalOutputT> > smart_converter_core_ = std::make_shared<SmartConverterCore1>("HostComponent1");
	
	CHostApplicationService<HostComponent1ExternalInputT, HostComponent1InternalDataT, HostComponent1ExternalOutputT> application_service_{ "HostComponent1", "HostComponent1", eHostApplicationComponentOffset1, this, &component, smart_converter_core_ };
	
};

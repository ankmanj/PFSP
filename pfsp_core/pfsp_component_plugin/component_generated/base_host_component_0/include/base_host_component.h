#pragma once


#include "smart_converter_core.h"
#include "component1.hpp"
#include "host_component_service.h"


class CService1Base  : public AService, INotify
{
	DECLARE_SERVICE_CLASS(CService1Base)
	
public:
	 CService1Base();
	
	virtual  ~CService1Base();
	
	virtual int16_t MwCompInitialize(std::string component_name, uint32_t component_id, uint32_t number_of_components);
	
	virtual int16_t MwCompStart(void);
	
	virtual int16_t MwCompStop(void);
	
	virtual int16_t MwCompFinalize(void);
	
	virtual int16_t MwCompMessage(AServiceMessage *poMessage);
	
	virtual int16_t MwCompCommand(int32_t nArgc, char_t *pszArgv[]);
	
	virtual void MwCompNotify(void *pvArg);
	
	
private:
	CService1 component;
	
	std::shared_ptr< SmartConverterCoreBase<HostComponent0ExternalInputT, HostComponent0InternalDataT, HostComponent0ExternalOutputT> > smart_converter_core_ = std::make_shared<SmartConverterCore0>("HostComponent0");
	
	CHostApplicationService<HostComponent0ExternalInputT, HostComponent0InternalDataT, HostComponent0ExternalOutputT> application_service_{ "HostComponent0", "HostComponent0", eHostApplicationComponentOffset0, this, &component, smart_converter_core_ };
	
};

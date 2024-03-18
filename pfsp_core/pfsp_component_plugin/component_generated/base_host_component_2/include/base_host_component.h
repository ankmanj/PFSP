#pragma once


#include "smart_converter_core.h"
#include "component3.hpp"
#include "host_component_service.h"


class CService3Base  : public AService, INotify
{
	DECLARE_SERVICE_CLASS(CService3Base)
	
public:
	 CService3Base();
	
	virtual  ~CService3Base();
	
	virtual int16_t MwCompInitialize(std::string component_name, uint32_t component_id, uint32_t number_of_components);
	
	virtual int16_t MwCompStart(void);
	
	virtual int16_t MwCompStop(void);
	
	virtual int16_t MwCompFinalize(void);
	
	virtual int16_t MwCompMessage(AServiceMessage *poMessage);
	
	virtual int16_t MwCompCommand(int32_t nArgc, char_t *pszArgv[]);
	
	virtual void MwCompNotify(void *pvArg);
	
	
private:
	CService3 component;
	
	std::shared_ptr< SmartConverterCoreBase<HostComponent2ExternalInputT, HostComponent2InternalDataT, HostComponent2ExternalOutputT> > smart_converter_core_ = std::make_shared<SmartConverterCore2>("HostComponent2");
	
	CHostApplicationService<HostComponent2ExternalInputT, HostComponent2InternalDataT, HostComponent2ExternalOutputT> application_service_{ "HostComponent2", "HostComponent2", eHostApplicationComponentOffset2, this, &component, smart_converter_core_ };
	
};

#pragma once


#include "component2_internal_data.h"
#include "../../../pfsp_core/pfsp_host_component_base/pfsp_host_component_base.h"


class CService2  : public PfspHostComponentBase<HostComponent1InternalDataT>
{
public:
	 CService2();
	
	virtual  ~CService2();
	
	virtual bool HostCompInit(const std::string component_name, HostComponentStatusT& host_message);
	
	virtual bool HostCompRun(HostComponentStatusT& host_message);
	
	virtual bool HostCompStop(HostComponentStatusT& host_message);
	
	virtual bool HostCompShutdown(HostComponentStatusT& host_message);
	
	
private:
	HostComponent1InternalDataT* internal_data_;
	
};

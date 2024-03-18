#pragma once


#include "FirstComponent_internal_data.h"
#include "../../../pfsp_core/pfsp_host_component_base/pfsp_host_component_base.h"


class FirstClass  : public PfspHostComponentBase<HostComponent0InternalDataT>
{
public:
	 FirstClass();
	
	virtual  ~FirstClass();
	
	virtual bool HostCompInit(const std::string component_name, HostComponentStatusT& host_message);
	
	virtual bool HostCompRun(HostComponentStatusT& host_message);
	
	virtual bool HostCompStop(HostComponentStatusT& host_message);
	
	virtual bool HostCompShutdown(HostComponentStatusT& host_message);
	
	
private:
	HostComponent0InternalDataT* internal_data_;
	
};

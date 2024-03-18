#pragma once


#include "component3_internal_data.h"
#include "../../../pfsp_core/pfsp_host_component_base/pfsp_host_component_base.h"


class CService3  : public PfspHostComponentBase<HostComponent2InternalDataT>
{
public:
	 CService3();
	
	virtual  ~CService3();
	
	virtual bool HostCompInit(const std::string component_name, HostComponentStatusT& host_message);
	
	virtual bool HostCompRun(HostComponentStatusT& host_message);
	
	virtual bool HostCompStop(HostComponentStatusT& host_message);
	
	virtual bool HostCompShutdown(HostComponentStatusT& host_message);
	
	
private:
	HostComponent2InternalDataT* internal_data_;
	
};

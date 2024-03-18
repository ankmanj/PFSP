#pragma once

#include "TestApp2_internal_data.h"
#include "../../external_interface/smart_converter_external_interface.h"
#include "smart_converter_core_base.h"

class SmartConverterCore1: public SmartConverterCoreBase<HostComponent1ExternalInputT, HostComponent1InternalDataT, HostComponent1ExternalOutputT>
{
public:
    SmartConverterCore1(std::string component_name)
	{
		component_name_ = component_name;
    }

    virtual ~SmartConverterCore1(void) = default;

	// Generated code
	void PreRun(void)
	{
		external_input = dynamic_cast<HostComponent1ExternalInputT*>(this->GetExternalInputMemory(component_name_)) ;
		internal_data = dynamic_cast<HostComponent1InternalDataT*>(this->GetInternalDataMemory(component_name_)) ;

		internal_data->internalobject.time = external_input->priorityobject.time;
		internal_data->internalobject.type = external_input->priorityobject.type;
		internal_data->internalobject.risk_level = external_input->priorityobject.priority;
	}

	// Generated code
	void PostRun(void)
	{
		external_output = dynamic_cast<HostComponent1ExternalOutputT*>(this->GetExternalOutputMemory(component_name_)) ;
		internal_data = dynamic_cast<HostComponent1InternalDataT*>(this->GetInternalDataMemory(component_name_)) ;

		external_output->ofobject.stamp = internal_data->internalobject.time;
		external_output->ofobject.obj_type = internal_data->internalobject.type;
		external_output->ofobject.vehicle_loc = internal_data->internalobject.fused_obj_loc;

		external_output->fusedroad.stamp = 10;
		external_output->fusedroad.detected_lane_num = 20;
		external_output->fusedroad.lane_color = 30;
	}

private:
	std::string component_name_ {};

	HostComponent1ExternalInputT* external_input {};
	HostComponent1InternalDataT* internal_data {};
	HostComponent1ExternalOutputT* external_output {};
};

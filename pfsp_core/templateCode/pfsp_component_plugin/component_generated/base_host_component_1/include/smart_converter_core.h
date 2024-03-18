#pragma once

#include "TestApp1_internal_data.h"
#include "../../external_interface/smart_converter_external_interface.h"
#include "smart_converter_core_base.h"

class SmartConverterCore0: public SmartConverterCoreBase<HostComponent0ExternalInputT, HostComponent0InternalDataT, HostComponent0ExternalOutputT>
{
public:
    SmartConverterCore0(std::string component_name)
	{
		component_name_ = component_name;
    }

    virtual ~SmartConverterCore0(void) = default;

	// Generated code
	void PreRun(void)
	{
		external_input = dynamic_cast<HostComponent0ExternalInputT*>(this->GetExternalInputMemory(component_name_)) ;
		internal_data = dynamic_cast<HostComponent0InternalDataT*>(this->GetInternalDataMemory(component_name_)) ;

		internal_data->internalobject.time = external_input->ofobject.stamp;
		internal_data->internalobject.type = external_input->ofobject.obj_type;
		internal_data->internalobject.fused_obj_loc = external_input->ofobject.vehicle_loc;

		internal_data->roadinfo.time = external_input->fusedroad.stamp;
		internal_data->roadinfo.lane_num = external_input->fusedroad.detected_lane_num;
		internal_data->roadinfo.lane_type = external_input->fusedroad.lane_color;
	}

	// Generated code
	void PostRun(void)
	{
		external_output = dynamic_cast<HostComponent0ExternalOutputT*>(this->GetExternalOutputMemory(component_name_)) ;
		internal_data = dynamic_cast<HostComponent0InternalDataT*>(this->GetInternalDataMemory(component_name_)) ;
		
		external_output->drivablepath.time = internal_data->roadinfo.time;
		external_output->drivablepath.lane_num = internal_data->roadinfo.lane_num;
		external_output->drivablepath.driving_window = internal_data->roadinfo.safe_time;

		external_output->drivablepath.time = 100;
		external_output->drivablepath.lane_num = 200;
		external_output->drivablepath.driving_window = 300;

		internal_data->internalobject.time = 5;
		internal_data->internalobject.type = 15;
		internal_data->internalobject.risk_level = 20;
		//internal_data->internalsomething.some = "test";

        //std::cout << "\n PostRun()    :: internal_data->internalsomething.some: [" << internal_data->internalsomething.some << "]" << std::endl;

		external_output->priorityobject.time = internal_data->internalobject.time;
		external_output->priorityobject.type = internal_data->internalobject.type;
		external_output->priorityobject.priority = internal_data->internalobject.risk_level;
		external_output->priorityobject.name = internal_data->internalsomething.some;
	}


private:
	std::string component_name_ {};

	HostComponent0ExternalInputT* external_input {};
	HostComponent0InternalDataT* internal_data {};
	HostComponent0ExternalOutputT* external_output {};
};

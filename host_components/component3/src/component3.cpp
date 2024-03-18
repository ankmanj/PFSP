#include "component3.hpp" 

CService3::CService3()
{
}

CService3::~CService3()
{
}

bool CService3::HostCompInit(const std::string component_name, HostComponentStatusT& host_message)
{
    bool result = true;

    internal_data_ = dynamic_cast<HostComponent2InternalDataT*>(this->GetInternalDataMemory(component_name));

	host_message.state = SWComponentStateE::eSWComponentStateInitialized;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}

bool CService3::HostCompRun(HostComponentStatusT& host_message)
{
	bool result = true;

	host_message.state = SWComponentStateE::eSWComponentStateRun;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}

bool CService3::HostCompStop(HostComponentStatusT& host_message)
{
	bool result = true;

	host_message.state = SWComponentStateE::eSWComponentStateStopDone;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}

bool CService3::HostCompShutdown(HostComponentStatusT& host_message)
{
	bool result = true;

	host_message.state = SWComponentStateE::eSWComponentStateShutdownDone;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}


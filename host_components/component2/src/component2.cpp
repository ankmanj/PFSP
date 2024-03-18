#include "component2.hpp" 

CService2::CService2()
{
}

CService2::~CService2()
{
}

bool CService2::HostCompInit(const std::string component_name, HostComponentStatusT& host_message)
{
    bool result = true;

    internal_data_ = dynamic_cast<HostComponent1InternalDataT*>(this->GetInternalDataMemory(component_name));

	host_message.state = SWComponentStateE::eSWComponentStateInitialized;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}

bool CService2::HostCompRun(HostComponentStatusT& host_message)
{
	bool result = true;

	host_message.state = SWComponentStateE::eSWComponentStateRun;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}

bool CService2::HostCompStop(HostComponentStatusT& host_message)
{
	bool result = true;

	host_message.state = SWComponentStateE::eSWComponentStateStopDone;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}

bool CService2::HostCompShutdown(HostComponentStatusT& host_message)
{
	bool result = true;

	host_message.state = SWComponentStateE::eSWComponentStateShutdownDone;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}


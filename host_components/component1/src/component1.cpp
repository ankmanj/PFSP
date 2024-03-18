#include "component1.hpp" 

CService1::CService1()
{
}

CService1::~CService1()
{
}

bool CService1::HostCompInit(const std::string component_name, HostComponentStatusT& host_message)
{
    bool result = true;

    internal_data_ = dynamic_cast<HostComponent0InternalDataT*>(this->GetInternalDataMemory(component_name));

	host_message.state = SWComponentStateE::eSWComponentStateInitialized;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}

bool CService1::HostCompRun(HostComponentStatusT& host_message)
{
	bool result = true;

	host_message.state = SWComponentStateE::eSWComponentStateRun;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}

bool CService1::HostCompStop(HostComponentStatusT& host_message)
{
	bool result = true;

	host_message.state = SWComponentStateE::eSWComponentStateStopDone;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}

bool CService1::HostCompShutdown(HostComponentStatusT& host_message)
{
	bool result = true;

	host_message.state = SWComponentStateE::eSWComponentStateShutdownDone;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}


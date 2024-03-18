#include "SecondComponent.hpp" 

SecondClass::SecondClass()
{
}

SecondClass::~SecondClass()
{
}

bool SecondClass::HostCompInit(const std::string component_name, HostComponentStatusT& host_message)
{
    bool result = true;

    internal_data_ = dynamic_cast<HostComponent1InternalDataT*>(this->GetInternalDataMemory(component_name));

	host_message.state = SWComponentStateE::eSWComponentStateInitialized;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}

bool SecondClass::HostCompRun(HostComponentStatusT& host_message)
{
	bool result = true;

    std::cout << "[SecondClass]: internal_data_->secondclassinternaldata.expect_20: " << internal_data_->secondclassinternaldata.expect_20 << std::endl;
    std::cout << "[SecondClass]: internal_data_->secondclassinternaldata.expect_200: " << internal_data_->secondclassinternaldata.expect_200 << std::endl;

	internal_data_->secondclassinternaldata.expect_20 = 10;
	internal_data_->secondclassinternaldata.expect_200 = 100;

	host_message.state = SWComponentStateE::eSWComponentStateRun;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}

bool SecondClass::HostCompStop(HostComponentStatusT& host_message)
{
	bool result = true;

	host_message.state = SWComponentStateE::eSWComponentStateStopDone;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}

bool SecondClass::HostCompShutdown(HostComponentStatusT& host_message)
{
	bool result = true;

	host_message.state = SWComponentStateE::eSWComponentStateShutdownDone;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}


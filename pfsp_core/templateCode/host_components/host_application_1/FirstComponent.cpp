#include "FirstComponent.hpp" 

FirstClass::FirstClass()
{
}

FirstClass::~FirstClass()
{
}

bool FirstClass::HostCompInit(const std::string component_name, HostComponentStatusT& host_message)
{
    bool result = true;
	frame_count = 0;
    internal_data_ = dynamic_cast<HostComponent0InternalDataT*>(this->GetInternalDataMemory(component_name));

	host_message.state = SWComponentStateE::eSWComponentStateInitialized;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

    frame_count++;
	return result;
}

bool FirstClass::HostCompRun(HostComponentStatusT& host_message)
{
	bool result = true;

    //std::cout << "[FirstClass]: internal_data_->firstclassinternaldata.expect_10: " << internal_data_->firstclassinternaldata.expect_10 << std::endl;
    //std::cout << "[FirstClass]: internal_data_->firstclassinternaldata.expect_100: " << internal_data_->firstclassinternaldata.expect_100 << std::endl;

	internal_data_->firstclassinternaldata.expect_10 = internal_data_->firstclassinternaldata.expect_10 + 10;
	internal_data_->firstclassinternaldata.expect_100 = internal_data_->firstclassinternaldata.expect_100 + 100;



    if(frame_count > 20 && frame_count < 27)
    {
        host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorInvalidDataValue;
        host_message.error_name = "InvalidDataValue";
    }
    else if(frame_count > 40)
    {
        host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;
        host_message.error_name = "";
    }
    else
    {
        host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;
        host_message.error_name = "";
    }

    frame_count++;
    



	host_message.state = SWComponentStateE::eSWComponentStateRun;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	//host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}

bool FirstClass::HostCompStop(HostComponentStatusT& host_message)
{
	bool result = true;

	host_message.state = SWComponentStateE::eSWComponentStateStopDone;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}

bool FirstClass::HostCompShutdown(HostComponentStatusT& host_message)
{
	bool result = true;

	host_message.state = SWComponentStateE::eSWComponentStateShutdownDone;
	host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
	host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;

	return result;
}


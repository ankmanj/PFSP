#include "host_app_component_2.h"

HostAppComponent2::HostAppComponent2(void)
{
    
}

bool HostAppComponent2::HostCompInit(std::string component_name, HostComponentStatusT &host_message)
{
    bool result = true;

    frame_count = 0;
    
    //std::cout << "\n=========================================" << std::endl;
    std::cout << "HostAppComponent_2::PfspHostCompInit()" << std::endl;
    //std::cout << "=========================================\n" << std::endl;


    std::cout << "[FirstClass]: internal_data_->secondclassinternaldata.expect_20: " << internal_data_->secondclassinternaldata.expect_20 << std::endl;
    std::cout << "[FirstClass]: internal_data_->secondclassinternaldata.expect_200: " << internal_data_->secondclassinternaldata.expect_200 << std::endl;

	internal_data_->secondclassinternaldata.expect_20 = internal_data_->secondclassinternaldata.expect_20 - 10;
	internal_data_->secondclassinternaldata.expect_200 = internal_data_->secondclassinternaldata.expect_200 - 100;




	internal_data_ = dynamic_cast<HostComponent1InternalDataT*>(this->GetInternalDataMemory(component_name)) ;

    host_message.state = SWComponentStateE::eSWComponentStateInitialized;
    host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
    host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;
    host_message.error_name = "test";

    frame_count++;
    return result;
}

bool HostAppComponent2::HostCompRun(HostComponentStatusT &host_message)
{
    bool result = true;

    //std::cout << "\n=========================================" << std::endl;
    std::cout << "HostAppComponent_2::PfspHostCompRun()" << std::endl;
    //std::cout << "=========================================\n" << std::endl;

    host_message.state = SWComponentStateE::eSWComponentStateRun;
    host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
    host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;
    host_message.error_name = "test";

    //std::cout << "[HostAppComponent2]: internal_data_->internalobject.time: " << internal_data_->internalobject.time << std::endl;
    //std::cout << "[HostAppComponent2]: internal_data_->internalobject.type: " << internal_data_->internalobject.type << std::endl;
    //std::cout << "[HostAppComponent2]: internal_data_->internalobject.fused_obj_loc: " << internal_data_->internalobject.fused_obj_loc << std::endl;

    if(frame_count > 110 && frame_count < 140)
    {
        host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorInvalidDataValue;
        host_message.error_name = "InvalidDataValue";
    }
    else if(frame_count > 150)
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
    
    return result;
}

bool HostAppComponent2::HostCompStop(HostComponentStatusT &host_message)
{
    bool result = true;

    //std::cout << "\n=========================================" << std::endl;
    std::cout << "HostAppComponent_2::PfspHostCompStop()" << std::endl;
    //std::cout << "=========================================\n" << std::endl;

    host_message.state = SWComponentStateE::eSWComponentStateStopDone;
    host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
    host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;
    host_message.error_name = "test";

    return result;
}

bool HostAppComponent2::HostCompShutdown(HostComponentStatusT &host_message)
{
    bool result = true;

    std::cout << "\n=========================================" << std::endl;
    std::cout << "HostAppComponent_2::HostCompShutdown()" << std::endl;
    std::cout << "=========================================\n" << std::endl;

    host_message.state = SWComponentStateE::eSWComponentStateShutdownDone;
    host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
    host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;
    host_message.error_name = "test";

    return result;
}

bool HostAppComponent2::HostCompResetData(void)
{
    bool result = true;

    std::cout << "\n=========================================" << std::endl;
    std::cout << "HostAppComponent_2::PfspHostCompResetData()" << std::endl;
    std::cout << "=========================================\n" << std::endl;

    return result;
}

bool HostAppComponent2::HostCompGetMessageFromPlugin(void)
{
    bool result = true;

    std::cout << "\n=========================================" << std::endl;
    std::cout << "HostAppComponent_2::PfspHostCompGetEventFromPlugin()" << std::endl;
    std::cout << "=========================================\n" << std::endl;
    
    return result;
}

#if 0
bool HostAppComponent2::HostCompSendMessageToPlugin(const PfspHostMessageTypeE output_message_type, const unsigned int output_message_value)
{
    bool result = true;
    std::cout << "HostAppComponent_2::HostCompSendMessageToPlugin()" << std::endl;
    return result;
}
#endif
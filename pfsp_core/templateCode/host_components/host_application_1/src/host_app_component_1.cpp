#include "host_app_component_1.h"

HostAppComponent1::HostAppComponent1(void)
{
    
}

bool HostAppComponent1::HostCompInit(std::string component_name, HostComponentStatusT &host_message)
{
    bool result = true;

    //std::cout << "\n=========================================" << std::endl;
    std::cout << "HostAppComponent_1::PfspHostCompInit()" << std::endl;
    //std::cout << "=========================================\n" << std::endl;

	internal_data_ = dynamic_cast<HostComponent0InternalDataT*>(this->GetInternalDataMemory(component_name)) ;

    host_message.state = SWComponentStateE::eSWComponentStateInitialized;
    host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
    host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;
    host_message.error_name = "test";

    frame_count++;
    return result;
}

bool HostAppComponent1::HostCompRun(HostComponentStatusT &host_message)
{
    bool result = true;

    //std::cout << "\n=========================================" << std::endl;
    std::cout << "HostAppComponent_1::PfspHostCompRun()" << std::endl;
    //std::cout << "=========================================\n" << std::endl;

    host_message.state = SWComponentStateE::eSWComponentStateRun;
    host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
    host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;
    host_message.error_name = "test";

    //std::cout << "[HostAppComponent1]: internal_data_->internalobject.time: " << internal_data_->internalobject.time << std::endl;
    //std::cout << "[HostAppComponent1]: internal_data_->internalobject.type: " << internal_data_->internalobject.type << std::endl;
    //std::cout << "[HostAppComponent1]: internal_data_->internalobject.fused_obj_loc: " << internal_data_->internalobject.fused_obj_loc << std::endl;
    //std::cout << "[HostAppComponent1]: internal_data_->roadinfo.time: " << internal_data_->roadinfo.time << std::endl;
    //std::cout << "[HostAppComponent1]: internal_data_->roadinfo.lane_num: " << internal_data_->roadinfo.lane_num << std::endl;
    //std::cout << "[HostAppComponent1]: internal_data_->roadinfo.lane_type: " << internal_data_->roadinfo.lane_type << std::endl;


    std::cout << "[FirstClass]: internal_data_->firstclassinternaldata.expect_10: " << internal_data_->firstclassinternaldata.expect_10 << std::endl;
    std::cout << "[FirstClass]: internal_data_->firstclassinternaldata.expect_100: " << internal_data_->firstclassinternaldata.expect_100 << std::endl;

	internal_data_->firstclassinternaldata.expect_10 = internal_data_->firstclassinternaldata.expect_10 + 10;
	internal_data_->firstclassinternaldata.expect_100 = internal_data_->firstclassinternaldata.expect_100 + 100;



	internal_data_->internalsomething.some = "HostAppComponent1_Test";

    if(frame_count > 20 && frame_count < 27)
    {
        host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorMemoryReadWriteFailure;
        host_message.error_name = "MemoryReadWriteFailure";
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
    
    return result;
}

bool HostAppComponent1::HostCompStop(HostComponentStatusT &host_message)
{
    bool result = true;

    //std::cout << "\n=========================================" << std::endl;
    std::cout << "HostAppComponent_1::PfspHostCompStop()" << std::endl;
    //std::cout << "=========================================\n" << std::endl;

    host_message.state = SWComponentStateE::eSWComponentStateStopDone;
    host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
    host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;
    host_message.error_name = "test";

    return result;
}

bool HostAppComponent1::HostCompShutdown(HostComponentStatusT &host_message)
{
    bool result = true;

    std::cout << "\n=========================================" << std::endl;
    std::cout << "HostAppComponent_1::HostCompShutdown()" << std::endl;
    std::cout << "=========================================\n" << std::endl;

    host_message.state = SWComponentStateE::eSWComponentStateShutdownDone;
    host_message.operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
    host_message.error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;
    host_message.error_name = "test";

    return result;
}

bool HostAppComponent1::HostCompResetData(void)
{
    bool result = true;

    std::cout << "\n=========================================" << std::endl;
    std::cout << "HostAppComponent_1::PfspHostCompResetData()" << std::endl;
    std::cout << "=========================================\n" << std::endl;

    return result;
}

bool HostAppComponent1::HostCompGetMessageFromPlugin(void)
{
    bool result = true;

    std::cout << "\n=========================================" << std::endl;
    std::cout << "HostAppComponent_1::PfspHostCompGetEventFromPlugin()" << std::endl;
    std::cout << "=========================================\n" << std::endl;

    return result;
}

#if 0
bool HostAppComponent1::HostCompSendMessageToPlugin(const PfspHostMessageTypeE output_message_type, const unsigned int output_message_value)
{
    bool result = true;
    std::cout << "HostAppComponent_1::HostCompSendMessageToPlugin()" << std::endl;

    return result;
}
#endif
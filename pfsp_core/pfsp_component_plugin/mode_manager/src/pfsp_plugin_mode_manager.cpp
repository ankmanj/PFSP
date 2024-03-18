#include <cstddef>
#include <math.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <dlfcn.h>

#include "osal/Debug.h"
#include "osal/Memory.h"
#include "osal/File.h"

#include "osac/CMessageQueue.h"
#include "osac/CSemaphore.h"
#include "osac/CFile.h"
#include "osac/CSharedMemory.h"

#include "utility/htoi.h"
#include "factory/CPortMap.h"

#include "pfsp_plugin_mode_manager.h"
#include "pfsp_plugin_mode_manager_message.h"
#include "host_application_topic.h"

IMPLEMENT_SERVICE_CLASS(PfspPluginModeManager)

PfspPluginModeManager::PfspPluginModeManager(void)
{

}

int16_t PfspPluginModeManager::MwCompInitialize(std::string component_name, uint32_t component_id, uint32_t number_of_components)
{
    FunctionResult result {};

    access_sync_.Create(0);
    real_number_of_components_ = number_of_components - static_cast<uint32_t>(DEFAULT_NUMBER_OF_COMPONENTS);

    std::cout << "\n[=== PfspPluginModeManager::MwCompInitialize ===]" << std::endl;
    //std::cout << "[number_of_components : " << real_number_of_components_ << "]\n";

    RegisterToScribeTopics();

    component_id_ = component_id;
    runner_priority_ = static_cast<uint32_t>(BASE_RUNNER_PRIORITY) + component_id;

    core_mode_manager_ = std::make_shared<PfspCoreModeManager>();
    if( nullptr == core_mode_manager_ )
    {
        return -1;
    }

    service_publication_message_ = std::make_shared<PfspPluginModeManagerMessage>(GetName(), GetName());
    if( nullptr != service_publication_message_ )
    {
        result = GetPointerOfPublicationTopics();
        if ( eResultSuccess == result )
        {
            components_reaction_output_->component.resize(number_of_components - static_cast<uint32_t>(DEFAULT_NUMBER_OF_COMPONENTS));
            core_mode_manager_->Initialize(number_of_components, components_reaction_output_);
            
            ReadParametersForComponentRunner("PlayType", "ThreadFrequency");
        }
        else
        {
            return -1;
        }

        mode_manager_runner_class_ = std::make_shared<PfspPluginModeManagerRunner>(service_publication_message_, core_mode_manager_, components_reaction_output_, &access_sync_);
        mode_manager_runner_class_->SetParametersForRunner(component_id_, play_type_, thread_frequency_);
    }
    else
    {
        return -1;
    }

    return 0;
}

int16_t PfspPluginModeManager::MwCompStart(void)
{
    std::cout << "\n[=== PfspPluginModeManager::MwCompStart ===]" << std::endl;

    runner_class_ = std::make_shared<CWorker>("/ModeManager_Runner", mode_manager_runner_class_.get(), runner_priority_, 50, eNONE_TRIGGERED);
    if(runner_class_->GetBusy() == 0)
    {
        runner_class_->DoWork(nullptr, 0);
    }
    else
    {
        ;
    }

    return 0;
}

int16_t PfspPluginModeManager::MwCompStop(void)
{
    return 0;
}

void PfspPluginModeManager::MwCompNotify(void *pvArg)
{
}

int16_t PfspPluginModeManager::MwCompMessage( AServiceMessage *poMessage )
{
    int16_t ret_val = poMessage->GetUserMsgType();

    if(static_cast<int16_t>(ePluginModeManagerComponentOffset) == ret_val)
    {
        access_sync_.Lock();
        CTopicMessage<ErrorHandlerReactionReportT> oTopicMessage(poMessage);
        ErrorHandlerReactionReportT *reported_reaction = oTopicMessage.GetContent();
        if ( nullptr != reported_reaction )
        {
            core_mode_manager_->ReportErrorHandlerReactionStateMode(reported_reaction);
        }
        access_sync_.Unlock();
    }
    else
    {
        int32_t all_mode_info_topic {};
        int32_t mode_state_report_topic {};
        int32_t base_position = static_cast<int32_t>(ePluginModeManagerComponentOffset) + 1;
        for(int32_t count = 0 ; count < static_cast<int32_t>(real_number_of_components_) ; count++)
        {
            all_mode_info_topic = base_position + 2*count;
            mode_state_report_topic = base_position + 2*count + 1;

            if(static_cast<int16_t>(all_mode_info_topic) == ret_val)
            {
                access_sync_.Lock();
                CTopicMessage<pfsp_topics::HostComponentAllModeInfoT> oTopicMessage(poMessage);
                pfsp_topics::HostComponentAllModeInfoT *all_mode_info = oTopicMessage.GetContent();
                if ( nullptr != all_mode_info )
                {   
                    core_mode_manager_->MakeModeManagementTable(all_mode_info);
                }
                access_sync_.Unlock();
                break;
            }
            else if(static_cast<int16_t>(mode_state_report_topic) == ret_val)
            {
                access_sync_.Lock();
                CTopicMessage<HostComponentModeStateReportT> oTopicMessage(poMessage);
                HostComponentModeStateReportT *reported_state_mode = oTopicMessage.GetContent();
                if ( nullptr != reported_state_mode )
                {
                    core_mode_manager_->ReportComponentStateMode(reported_state_mode);
                    mode_manager_runner_class_->SetComponentPubSubStatus(ComponentPubSubStatusE::eReceivedExternalInput);
                }
                access_sync_.Unlock();
                break;
            }
            else
            {
                ;
            }
        }
    }
    
    return 0;
}

int16_t PfspPluginModeManager::MwCompCommand( int32_t nArgc, char_t *pszArgv[] )
{
    return -1;
}

int16_t PfspPluginModeManager::MwCompFinalize(void)
{
    std::cout << "\n==> [PfspPluginModeManager]. PfspPluginModeManager::MwCompFinalize [The Start ~ ]" << std::endl;
    (void)access_sync_.Close();
    (void)access_sync_.Destroy();
    
    std::cout << "==> [PfspPluginModeManager]. PfspPluginModeManager::MwCompFinalize [ 111111111111 ]" << std::endl;
    mode_manager_runner_class_.reset();
    service_publication_message_.reset();

    std::cout << "==> [PfspPluginModeManager]. PfspPluginModeManager::MwCompFinalize [ 222222222222 ]" << std::endl;
    core_mode_manager_->Terminate();
    core_mode_manager_.reset();
    
    std::cout << "==> [PfspPluginModeManager]. PfspPluginModeManager::MwCompFinalize [ 333333333333 ]" << std::endl;
    runner_class_->DoFinalize();
    runner_class_->Join();

    std::cout << "==> [PfspPluginModeManager]. PfspPluginModeManager::MwCompFinalize [ 444444444444 ]" << std::endl;
    runner_class_.reset();
    
    std::cout << "==> [PfspPluginModeManager]. PfspPluginModeManager::MwCompFinalize [~ The End]\n" << std::endl;
    return 0;
}

void PfspPluginModeManager::RegisterToScribeTopics(void)
{
    CTopicMessage<Subscribe_t> subscription_topic_message {};
    subscription_topic_message.Subscribe("ReactionReportTopic", ePluginModeManagerComponentOffset);

    char_t temp_char[100] {};
    int32_t base_position = static_cast<int32_t>(ePluginModeManagerComponentOffset) + 1;
    for(int32_t count = 0 ; count < static_cast<int32_t>(real_number_of_components_) ; count++)
    {
        //sprintf(temp_char, "HostComponent%dAllModeInfoTopic", (count + 1));
        sprintf(temp_char, "HostComponent%dAllModeInfoTopic", count);
        subscription_topic_message.Subscribe(temp_char, base_position + 2*count);

        //sprintf(temp_char, "HostComponent%dModeStateReportTopic", (count + 1));
        sprintf(temp_char, "HostComponent%dModeStateReportTopic", count);
        subscription_topic_message.Subscribe(temp_char, base_position + 2*count + 1);
    }
}

FunctionResult PfspPluginModeManager::GetPointerOfPublicationTopics(void)
{
    FunctionResult result {eResultSuccess};
    
    components_reaction_output_ = service_publication_message_->GetPointerOfComponentsReaction();
    if(nullptr == components_reaction_output_)
    {
        result = eResultFailure;
    }

    return result;
}

void PfspPluginModeManager::ReadParametersForComponentRunner(const char_t *play_type, const char_t *thread_frequency)
{
    char_t *enumeration_char {};

    enumeration_char = const_cast<char_t *>(GetParameterValue(play_type));
    //if( strstr(enumeration_char, "Once") )
    if(0 == strcmp(enumeration_char, "Once"))
    {
        play_type_ = eOnce;
    }
    else
    {
        play_type_ = eRepeat;
    }

    thread_frequency_ = atoi(GetParameterValue(thread_frequency));
}

/*
int16_t PfspPluginModeManager::OnReceive(AServiceMessage* poMessage)
{
    switch(poMessage->GetFrameworkMessageHeader()->m_unType)
    {
        case eMSGTYPE_FRAME_SUBSCRIBE:
            break;
                
        case eMSGTYPE_FRAME_UNSUBSCRIBE:
            break;
                
        case eMSGTYPE_FRAME_USERMSG:
            break;

        case eMSGTYPE_FRAME_FINALIZE:
            //(void) MwCompFinalize();
            //return SERVICE_FINALIZE_RETURN;
			break;
   
        default:
            break;
	}

	return 0;
}
*/
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

#include "pfsp_plugin_error_handler.h"
#include "pfsp_plugin_error_handler_message.h"
#include "host_application_topic.h"

IMPLEMENT_SERVICE_CLASS(PfspPluginErrorHandler)

PfspPluginErrorHandler::PfspPluginErrorHandler(void){
}

int16_t PfspPluginErrorHandler::MwCompInitialize(std::string component_name, uint32_t component_id, uint32_t number_of_components)
{
    FunctionResult result {};

    access_sync_.Create(0);
    real_number_of_components_ = number_of_components - static_cast<uint32_t>(DEFAULT_NUMBER_OF_COMPONENTS);

    std::cout << "\n[=== PfspPluginErrorHandler::MwCompInitialize ===]" << std::endl;
    //std::cout << "[number_of_components : " << real_number_of_components_ << "]\n";

    RegisterToScribeTopics();

    component_id_ = component_id;
    runner_priority_ = static_cast<uint32_t>(BASE_RUNNER_PRIORITY) + component_id;

    core_error_handler_ = std::make_shared<PfspCoreErrorHandler>();
    if( nullptr == core_error_handler_ )
    {
        return -1;
    }

    service_publication_message_ = std::make_shared<PfspPluginErrorHandlerMessage>(GetName(), GetName());
    if( nullptr != service_publication_message_ )
    {
        result = GetPointerOfPublicationTopics();
        if ( eResultSuccess == result )
        {
            reaction_report_output_->next_reaction.resize(number_of_components - static_cast<uint32_t>(DEFAULT_NUMBER_OF_COMPONENTS));
            core_error_handler_->Initialize(number_of_components, reaction_report_output_);

            ReadParametersForComponentRunner("PlayType", "ThreadFrequency");
        }
        else
        {
            return -1;
        }

        error_handler_runner_class_ = std::make_shared<PfspPluginErrorHandlerRunner>(service_publication_message_, core_error_handler_, reaction_report_output_, &access_sync_);
        error_handler_runner_class_->SetParametersForRunner(real_number_of_components_, play_type_, thread_frequency_);
    }
    else
    {
        return -1;
    }

    return 0;
}

int16_t PfspPluginErrorHandler::MwCompStart(void)
{
    std::cout << "\n[=== PfspPluginErrorHandler::MwCompStart ===]" << std::endl;

    runner_class_ = std::make_shared<CWorker>("/ErrorHandler_Runner", error_handler_runner_class_.get(), runner_priority_, 50, eNONE_TRIGGERED);
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

int16_t PfspPluginErrorHandler::MwCompStop(void)
{
    return 0;
}

void PfspPluginErrorHandler::MwCompNotify(void *pvArg)
{
}

int16_t PfspPluginErrorHandler::MwCompMessage( AServiceMessage *poMessage )
{
    int16_t all_errors_info_topic {};
    int16_t error_report_topic {};
    int16_t base_position = static_cast<int16_t>(ePluginErrorHandlerComponentOffset);
    
    access_sync_.Lock();
    int16_t ret_val = poMessage->GetUserMsgType();
    for(int16_t count = 0 ; count < static_cast<int16_t>(real_number_of_components_) ; count++)
    {
        all_errors_info_topic = base_position + 2*count;
        error_report_topic = base_position + 2*count + 1;

        if(all_errors_info_topic == ret_val)
        {
            CTopicMessage<pfsp_topics::HostComponentAllErrorsInfoT> oTopicMessage(poMessage);
            pfsp_topics::HostComponentAllErrorsInfoT *all_errors_info = oTopicMessage.GetContent();
            if ( nullptr != all_errors_info )
            {
                //std::cout << "============= [Error Hander Receive::eApplication  All Errors Info], [component_id: " << count << "]\n";
                core_error_handler_->MakeErrorHandlingTable(all_errors_info);
            }
            break;
        }
        else if(error_report_topic == ret_val)
        {
            CTopicMessage<HostComponentErrorReportT> oTopicMessage(poMessage);
            HostComponentErrorReportT *reported_error = oTopicMessage.GetContent();
            if ( nullptr != reported_error )
            {
                //std::cout << "============= [Error Hander Receive::eApplication  Error Message], [component_id: " << count << "]\n";
                core_error_handler_->ReportError(reported_error);
            }
            break;
        }
        else
        {
            ;
        }
    }
    access_sync_.Unlock();

    return 0;
}

int16_t PfspPluginErrorHandler::MwCompCommand( int32_t nArgc, char_t *pszArgv[] )
{
    return -1;
}

int16_t PfspPluginErrorHandler::MwCompFinalize(void)
{
    std::cout << "\n==> [PfspPluginErrorHandler]. PfspPluginErrorHandler::MwCompFinalize [The Start ~ ]" << std::endl; 
    (void)access_sync_.Close();
    (void)access_sync_.Destroy();
    
    std::cout << "==> [PfspPluginErrorHandler]. PfspPluginErrorHandler::MwCompFinalize [ 111111111111 ]" << std::endl;  
    error_handler_runner_class_.reset();
    service_publication_message_.reset();

    std::cout << "==> [PfspPluginErrorHandler]. PfspPluginErrorHandler::MwCompFinalize [ 222222222222 ]" << std::endl; 
    core_error_handler_->Terminate();
    core_error_handler_.reset();

    std::cout << "==> [PfspPluginErrorHandler]. PfspPluginErrorHandler::MwCompFinalize [ 333333333333  ]" << std::endl;
    runner_class_->DoFinalize();
    runner_class_->Join();

    std::cout << "==> [PfspPluginErrorHandler]. PfspPluginErrorHandler::MwCompFinalize [ 444444444444 ]" << std::endl;
    runner_class_.reset();

    std::cout << "==> [PfspPluginErrorHandler]. PfspPluginErrorHandler::MwCompFinalize [ ~ The End ]\n" << std::endl;
    return 0;
}

void PfspPluginErrorHandler::RegisterToScribeTopics(void)
{
    CTopicMessage<Subscribe_t> subscription_topic_message {};
    char_t temp_char[100] {};
    int16_t base_position = static_cast<int16_t>(ePluginErrorHandlerComponentOffset);
    for(int16_t count = 0 ; count < static_cast<int16_t>(real_number_of_components_) ; count++)
    {
        //sprintf(temp_char, "HostComponent%dAllErrorsInfoTopic", (count + 1));
        sprintf(temp_char, "HostComponent%dAllErrorsInfoTopic", count);
        subscription_topic_message.Subscribe(temp_char, base_position + 2*count);

        //sprintf(temp_char, "HostComponent%dErrorReportTopic", (count + 1));
        sprintf(temp_char, "HostComponent%dErrorReportTopic", count);
        subscription_topic_message.Subscribe(temp_char, base_position + 2*count + 1);
    }
}

FunctionResult PfspPluginErrorHandler::GetPointerOfPublicationTopics(void)
{
    FunctionResult result {eResultSuccess};

    reaction_report_output_ = service_publication_message_->GetPointerOfErrorHandlerReactionReportT();
    if(nullptr == reaction_report_output_)
    {
        result = eResultFailure;
    }

    return result;
}

void PfspPluginErrorHandler::ReadParametersForComponentRunner(const char_t *play_type, const char_t *thread_frequency)
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
int16_t PfspPluginErrorHandler::OnReceive(AServiceMessage* poMessage)
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
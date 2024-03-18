#pragma once

#include "worker/CWorker.h"
#include "servicemessage/CTimeServiceMessage.h"

#include "host_component_message.h"

using namespace std;

template<typename TExternalInput, typename TComponentInternal, typename TExternalOutput>
class CHostApplicationRunner : public IWork
{
public:
    CHostApplicationRunner(std::shared_ptr<CHostApplicationMessage<TExternalOutput>> publication_message, \
                            HostComponentCommonTopicPtrT &common_topics_ptr, \
                            TComponentInternal *component_internal_data, \
                            TExternalOutput *output_topics_ptr, \
                            PfspHostComponentBase<TComponentInternal> *host_app_component,
                            OSAC::CMutex *access_sync,
#if defined(FEATURE_USE_SMART_PTR)
                            std::shared_ptr< SmartConverterCoreBase<TExternalInput, TComponentInternal, TExternalOutput> > smart_converter)
#else
                            SmartConverterCoreBase<TExternalInput, TComponentInternal, TExternalOutput> *smart_converter)
#endif
    {
        service_publication_message_ = publication_message;
        component_internal_data_ = component_internal_data;
        component_output_ = output_topics_ptr;
        publication_common_topics_ = &common_topics_ptr;
        host_app_component_ = host_app_component;
        smart_converter_ = smart_converter;
        access_sync_ = access_sync;
        start_frame_info_.frame_count = 0U;

        request_statemode_.state = SWComponentStateE::eSWComponentStateCreation;
        flagFinalized = false;
    }

    virtual ~CHostApplicationRunner(void) = default;

    virtual void OnInitialize(void){};

    virtual void OnProcess(const char_t *pvBuffer, const int32_t nLen)
    {
        if (flagFinalized == false)
        {
            GetFrameTimeStamp(start_frame_info_);

            access_sync_->Lock();
            temp_frame_count_ = start_frame_info_.frame_count;

            if(SWComponentStateE::eSWComponentStateCreation != request_statemode_.state)
            {
                uint32_t temp_frequency_1 = component_basic_info_.thread_frequency/10 * 2;
                TASK_Delay(temp_frequency_1, 0);
    
                if(SWComponentStateE::eSWComponentStateTriggerInit == request_statemode_.state || 
                    SWComponentStateE::eSWComponentStateRun == request_statemode_.state || 
                    SWComponentStateE::eSWComponentStateStop == request_statemode_.state || 
                    SWComponentStateE::eSWComponentStateTriggerShutdown == request_statemode_.state)
                {
                    std::cout << "\ncomponentRun():ID[ " << component_basic_info_.component_id << 
                                                    "], Name[\"" << real_component_name_ << 
                                                    "\"], ThreadFrequency[" << elapse_millisec << "ms" << 
                                                    "], FrameCount[" << temp_frame_count_ << "]" << std::endl;
                }

                FunctionResult result = ProcessHostAPI();
                if ( eResultFailure == result )
                {
                    std::cout << "\n Error:: ==> CHostApplicationRunner::OnProcess" << std::endl;
                }
                service_publication_message_->PublishComponentOutputData();
                pubsub_status_ = ComponentPubSubStatusE::ePublishedExternalOutput;
            }

            uint32_t temp_frequency_2 = component_basic_info_.thread_frequency/10 * 2;
            TASK_Delay(temp_frequency_2, 0);

            Common_MakeErrorReport(start_frame_info_, component_basic_info_, host_message_, publication_common_topics_->error_report_);
            service_publication_message_->PublishErrorReportT();

            Common_MakeModeStateReport(start_frame_info_, component_basic_info_, host_message_, publication_common_topics_->mode_state_report_);
            service_publication_message_->PublishModeStateReportT();
            access_sync_->Unlock();

            uint32_t temp_frequency_3 = component_basic_info_.thread_frequency/10 * 6;
            TASK_Delay(temp_frequency_3, 0);

            GetFrameTimeStamp(end_frame_info_);
            elapse_millisec = GetElapseTimeByReference(end_frame_info_.frame_time_stamp, start_frame_info_.frame_time_stamp);
        }
    }

    virtual void OnFinalize(void)
    {
        std::cout << "==> [CHostApplicationRunner]. Application::OnFinalize, Name[\"" << real_component_name_ << "\"]" << std::endl;
        //service_publication_message_.reset();
    }

    virtual void RequestFinalize(void)
    {
        std::cout << "==> [CHostApplicationRunner]. Application::RequestFinalize, Name[\"" << real_component_name_ << "\"]" << std::endl;
        flagFinalized = true;
    }

    void SetParametersForRunner(std::string component_name, std::string real_component_name, uint32_t component_id, PlayTypeE play_type, uint32_t thread_frequency)
    {
        component_name_ = component_name;
        real_component_name_ = real_component_name;
        component_basic_info_.component_id = component_id;
        component_basic_info_.play_type = play_type;
        component_basic_info_.thread_frequency = thread_frequency; 
    }

    void SetNextStateAndMode(SWComponentStateE state, SWComponentOperationModeE operation_mode)
    {
        request_statemode_.state = state;
        request_statemode_.operation_mode = operation_mode;
    }

    void SetComponentPubSubStatus(ComponentPubSubStatusE pubsub_status)
    {
        pubsub_status_ = pubsub_status;
    }
private:

    void Common_MakeErrorReport(pfsp_topics::ComponentFrameInfo &frame_info, pfsp_topics::ComponentBasicInfo &component_basic_info, 
                                HostComponentStatusT &host_message, HostComponentErrorReportT *error_report)
    {
        error_report->component_id = component_basic_info.component_id;
        error_report->frame_count = temp_frame_count_;
        error_report->frame_time_stamp.usec = frame_info.frame_time_stamp.usec;
        error_report->frame_time_stamp.sec = frame_info.frame_time_stamp.sec;
		
        strcpy(error_report->error_name, host_message.error_name.c_str());

        if(SWComponentStateE::eSWComponentStateCreation == request_statemode_.state)
        {
            error_report->error_type = SWComponentErrorCategoryE::eSWComponentErrorNone;
        }
        else
        {
            error_report->error_type = host_message.error_type;
        }
    }

    void Common_MakeModeStateReport(pfsp_topics::ComponentFrameInfo &frame_info, pfsp_topics::ComponentBasicInfo &component_basic_info, 
                                    HostComponentStatusT &host_message, HostComponentModeStateReportT *mode_state_report)
    {
        mode_state_report->component_id = component_basic_info.component_id;
        mode_state_report->play_type = component_basic_info.play_type;
        mode_state_report->frame_count = temp_frame_count_;
        mode_state_report->current_info.operation_mode = host_message.operation_mode;

        if(SWComponentStateE::eSWComponentStateCreation == request_statemode_.state)
        {
            mode_state_report->current_info.state = SWComponentStateE::eSWComponentStateTriggerInit;
        }
        else if(SWComponentStateE::eSWComponentStateNone == request_statemode_.state)
        {
            mode_state_report->current_info.state = SWComponentStateE::eSWComponentStateNone;
        }
        else
        {
            mode_state_report->current_info.state = host_message.state;
        }
    }

    FunctionResult ProcessHostAPI(void)
    {
        FunctionResult result {eResultSuccess};
        switch( request_statemode_.state )
        {
            case SWComponentStateE::eSWComponentStateTriggerInit:
            {
                start_frame_info_.frame_count = 0U;

                service_publication_message_->PublishAllErrorsInfoT();
                service_publication_message_->PublishAllModeInfoT();

                bool host_result = host_app_component_->HostCompInit(component_name_, host_message_);
                if( false == host_result )
                {
                    std::cout << "\n Error:: ==> HostCompInit( )" << std::endl;
                    result = eResultFailure;
                }
                else
                {
                    start_frame_info_.frame_count++;
                }                
                break;
            }
            
            case SWComponentStateE::eSWComponentStateRun:
            {
                //uint32_t temp_frequency = component_basic_info_.thread_frequency/10 * 1;
                //TASK_Delay(temp_frequency, 100);
                //if( pubsub_status_ == ComponentPubSubStatusE::eReceivedExternalInput )
                {
                    bool host_result = host_app_component_->HostCompRun(host_message_);
                    if ( false == host_result )
                    {
                        std::cout << "\n Error:: ==> HostCompRun( )" << std::endl;
                        result = eResultFailure;
                    }
                    else
                    {
                        smart_converter_->PostRun();
                        start_frame_info_.frame_count++;
                    }
                }
                
                if(pubsub_status_ != ComponentPubSubStatusE::eReceivedExternalInput)
                {
                    //std::cout << "===================================== [Problem] =====================================" << std::endl;
                }

                pubsub_status_ = ComponentPubSubStatusE::eComponentRun;
                break;
            }

            case SWComponentStateE::eSWComponentStateStop:
            {
                bool host_result = host_app_component_->HostCompStop(host_message_);
                if(false == host_result)
                {
                    std::cout << "\n Error:: ==> HostCompStop( )" << std::endl;
                    result = eResultFailure;
                }
                else
                {
                    start_frame_info_.frame_count++;
                }
                break;
            }
            
            case SWComponentStateE::eSWComponentStateTriggerShutdown:
            {
                bool host_result = host_app_component_->HostCompShutdown(host_message_);
                if(false == host_result)
                {
                    std::cout << "\n Error:: ==> HostCompShutdown( )" << std::endl;
                    result = eResultFailure;
                }
                else
                {
                    start_frame_info_.frame_count++;
                }
                break;
            }
            
            default:
                host_message_.state = request_statemode_.state;
                host_message_.operation_mode = request_statemode_.operation_mode;
                break;
        }

        return result;
    }

    bool flagFinalized{false};
    uint32_t temp_frame_count_ {};
    int64_t elapse_millisec{};
    std::string component_name_ {};
    std::string real_component_name_{};
    HostComponentStatusT host_message_ {};
    ComponentPubSubStatusE pubsub_status_ = ComponentPubSubStatusE::ePubSubNone;

    pfsp_topics::ComponentBasicInfo component_basic_info_ {};
    pfsp_topics::ComponentFrameInfo start_frame_info_ {};
    pfsp_topics::ComponentFrameInfo end_frame_info_ {};
    HostComponentModeManagementInfoT request_statemode_ {};

    TComponentInternal *component_internal_data_ {};
    TExternalOutput *component_output_ {};
    HostComponentCommonTopicPtrT *publication_common_topics_ {};

    PfspHostComponentBase<TComponentInternal> *host_app_component_ {};
#if defined(FEATURE_USE_SMART_PTR)
    std::shared_ptr< SmartConverterCoreBase<TExternalInput, TComponentInternal, TExternalOutput> > smart_converter_{};
#else
    SmartConverterCoreBase<TExternalInput, TComponentInternal, TExternalOutput> *smart_converter_{};
#endif
    std::shared_ptr< CHostApplicationMessage<TExternalOutput> > service_publication_message_ {};
    OSAC::CMutex *access_sync_;
};

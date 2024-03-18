#pragma once

#include "osal/Debug.h"
#include "osal/Memory.h"
#include "osal/File.h"
#include "osac/CFile.h"
#include "osac/CSemaphore.h"
#include "osac/CMutex.h"
#include "osac/CSharedMemory.h"

#include "factory/CPortMap.h"

#include "interface/INotify.h"
#include "service/AService.h"

#include "host_component_runner.h"

template<typename TExternalInput, typename TComponentInternal, typename TExternalOutput>
class CHostApplicationService
{
public:
    CHostApplicationService(std::string component_name, 
                            std::string external_input_component_name, 
                            int16_t offset, 
                            AService *service_class, 
                            PfspHostComponentBase<TComponentInternal> *host_app_component,
#if defined(FEATURE_USE_SMART_PTR)
                            std::shared_ptr< SmartConverterCoreBase<TExternalInput, TComponentInternal, TExternalOutput> > smart_converter)
#else
                            SmartConverterCoreBase<TExternalInput, TComponentInternal, TExternalOutput> *smart_converter)
#endif
                            :   component_name_{component_name}, \
                                external_input_component_name_{external_input_component_name}, \
                                host_appln_offset_{offset}, \
                                service_class_{service_class}, \
                                host_app_component_{host_app_component}, \
                                smart_converter_{smart_converter} {
        
    }

    virtual ~CHostApplicationService(void) = default;

    int16_t Initialize(std::string component_name, uint32_t component_id, uint32_t number_of_components)
    {
        FunctionResult result {};

        component_name.erase(0,1);

        access_sync_.Create(0);
        RegisterToScribeTopics();

        real_component_name_ = component_name;
        component_id_ = component_id - static_cast<uint32_t>(DEFAULT_NUMBER_OF_COMPONENTS);
        runner_priority_ = static_cast<uint32_t>(BASE_RUNNER_PRIORITY) + component_id;

        std::cout << "\n[=== CHostComponent::MwCompInitialize ===]" << std::endl;
        std::cout << "[component_name : " << real_component_name_ << "]\n";
        std::cout << "[component_id : " << component_id_ << "]\n"; 
        std::cout << "[number_of_components : " << number_of_components - static_cast<uint32_t>(DEFAULT_NUMBER_OF_COMPONENTS) << "]\n";
        
/*
        result = Common_LoadPluginHostComponent(HOST_APPLICATION_LIB_NAME);
        if( eResultFailure == result )
        {
            return -1;
        }
*/
        service_publication_message_ = std::make_shared<CHostApplicationMessage<TExternalOutput>>\
                                                        (service_class_->GetName(), service_class_->GetName(), component_name_);
        if( nullptr != service_publication_message_ )
        {
            result = GetPointerOfPublicationTopics();
            if ( eResultSuccess == result )
            {
                Common_ReadParametersForErrorHandling(common_topics_ptr_.all_errors_info_);
                Common_ReadParametersForModeManagement(common_topics_ptr_.all_mode_info_);
                Common_ReadParametersForComponentRunner("PlayType", "ThreadFrequency");
            }
            else
            {
                return -1;
            }

            smart_converter_->SetExternalOutputMemory(component_name_, component_output_);
            smart_converter_->SetInternalDataMemory(component_name_, &component_internal_data_);
            host_app_component_->SetInternalDataMemory(component_name_, &component_internal_data_);

            host_component_runner_class_ = std::make_shared<CHostApplicationRunner<TExternalInput, TComponentInternal, TExternalOutput>>\
                                                                (service_publication_message_, \
                                                                    common_topics_ptr_, \
                                                                    &component_internal_data_, \
                                                                    component_output_, \
                                                                    host_app_component_, \
                                                                    &access_sync_, \
                                                                    smart_converter_);
            host_component_runner_class_->SetParametersForRunner(component_name_, real_component_name_, component_id_, play_type_, thread_frequency_);

            const SWComponentStateE state = SWComponentStateE::eSWComponentStateCreation;
            const SWComponentOperationModeE operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
            host_component_runner_class_->SetNextStateAndMode(state, operation_mode);
        }
        else
        {
            return -1;            
        }

        return 0;
    }

    int16_t Start(void)
    {
        std::string temp{"/" + component_name_ + "_Runner"};
        runner_class_ = std::make_shared<CWorker>(temp.c_str(), host_component_runner_class_.get(), runner_priority_, 50, eNONE_TRIGGERED);
        if( runner_class_->GetBusy() == 0 )
        {
            runner_class_->DoWork(nullptr, 0);
        }
        else
        {
            ;
        }

        return 0;
    }

    int16_t Stop(void)
    {
        return 0;
    }

    int16_t Finalize(void)
    {
        std::cout << "\n==> [CHostApplicationService]. Application::MwCompFinalize [ The Start ~ ]" << std::endl;
        std::cout << "component ID[ " << component_id_ << "], Name[\"" << real_component_name_ << "\"]" << std::endl;

        std::cout << "==> [CHostApplicationService]. Application::MwCompFinalize [ 11111111111 ]" << std::endl;
        (void)access_sync_.Close();
        (void)access_sync_.Destroy();
        
        std::cout << "==> [CHostApplicationService]. Application::MwCompFinalize [ 22222222222 ]" << std::endl;
        host_component_runner_class_.reset();
        service_publication_message_.reset();

        std::cout << "==> [CHostApplicationService]. Application::MwCompFinalize [ 333333333333 ]" << std::endl;
        component_errors_info_.clear();
        std::map<std::string, HostComponentErrorInfoT>().swap(component_errors_info_);
        component_modes_info_.clear();
        std::map<std::string, SWComponentOperationModeE>().swap(component_modes_info_);

        std::cout << "==> [CHostApplicationService]. Application::MwCompFinalize [ 444444444444 ]\n" << std::endl;
        runner_class_->DoFinalize();
        runner_class_->Join();
                
        std::cout << "==> [CHostApplicationService]. Application::MwCompFinalize [ ~ The End ]" << std::endl;
        runner_class_.reset();
        
        return 0;
    }

    int16_t Message(AServiceMessage *poMessage)
    {
        int16_t ret_val = poMessage->GetUserMsgType();
        if( ret_val == host_appln_offset_)
        {
            //Blank if statement                
        }
        else if(ret_val == host_appln_offset_ + 1)
        {
            access_sync_.Lock();
            CTopicMessage<TExternalInput> oTopicMessage(poMessage);
            TExternalInput *external_input = oTopicMessage.GetContent();

            smart_converter_->SetExternalInputMemory(component_name_, external_input);
            smart_converter_->PreRun();
            host_component_runner_class_->SetComponentPubSubStatus(ComponentPubSubStatusE::eReceivedExternalInput);
            access_sync_.Unlock();
        } 
        else if(ret_val == host_appln_offset_ + 2)
        {
            access_sync_.Lock();
            CTopicMessage<ModeManagerComponentsReactionT> oTopicMessage(poMessage);

            ModeManagerComponentsReactionT *reaction = oTopicMessage.GetContent();

            if ( nullptr != reaction )
            {
                HostComponentModeManagementInfoT *component_next_reaction = &reaction->component[static_cast<uint32_t>(component_id_)];
                if( DataPublicationStateE::ePublishedData == component_next_reaction->publication_state )
                {
                    host_component_runner_class_->SetNextStateAndMode(component_next_reaction->state, component_next_reaction->operation_mode);
                    component_next_reaction->publication_state = DataPublicationStateE::eNotPublishedData;
                }
            }
            access_sync_.Unlock();
        }
        return 0;
    }    

    int16_t Command(int32_t nArgc, char_t *pszArgv[])
    {
        return -1;
    }

protected:
    void Notify(void *pvArg);

private:
    std::string component_name_ {};
    std::string real_component_name_ {};
    std::string external_input_component_name_ {};
    int16_t host_appln_offset_ {};
    uint32_t thread_frequency_ {};
    uint32_t runner_priority_ {};
    uint32_t component_id_ {};
    PlayTypeE play_type_ {};
    AService *service_class_ {};

    TComponentInternal component_internal_data_ {};
    TExternalOutput *component_output_ {};
    HostComponentCommonTopicPtrT common_topics_ptr_ {};

    PfspHostComponentBase<TComponentInternal> *host_app_component_ {};
#if defined(FEATURE_USE_SMART_PTR)
    std::shared_ptr< SmartConverterCoreBase<TExternalInput, TComponentInternal, TExternalOutput> > smart_converter_ {};
#else
    SmartConverterCoreBase<TExternalInput, TComponentInternal, TExternalOutput> *smart_converter_ {};
#endif
    std::shared_ptr<CWorker> runner_class_ {};
    std::shared_ptr<CHostApplicationMessage<TExternalOutput>> service_publication_message_ {};
    std::shared_ptr<CHostApplicationRunner<TExternalInput, TComponentInternal, TExternalOutput>> host_component_runner_class_ {};

    OSAC::CMutex access_sync_;

    std::map<std::string, HostComponentErrorInfoT> component_errors_info_ {};
    std::map<std::string, SWComponentOperationModeE> component_modes_info_ {};
/*
	FunctionResult Common_LoadPluginHostComponent(const char_t *component_lib_name)
    {
        char_t *host_app_lib_name {};
        host_app_lib_name = const_cast<char_t *>(service_class_->GetParameterValue(component_lib_name));
        void* lib_handle = dlopen(host_app_lib_name, RTLD_NOW | RTLD_GLOBAL);
        if ( nullptr == lib_handle )
        {
            std::cout << "error openning " << host_app_lib_name << std::endl;
            return eResultFailure;
        }

        PfspHostComponentBase* (*creator)() = (PfspHostComponentBase*(*)())dlsym(lib_handle, "create");
        host_app_component_ = (*creator)();
        if ( nullptr != host_app_component_ )
        {
            return eResultSuccess;
        }
        else
        {
            return eResultFailure;
        }
    }
*/
	FunctionResult GetPointerOfPublicationTopics(void)
    {
        FunctionResult result {eResultSuccess};

        component_output_ = service_publication_message_->GetPointerOfOutputData();
        if(nullptr == component_output_)
        {
            result = eResultFailure;
        }

        common_topics_ptr_.all_errors_info_ = service_publication_message_->GetPointerOfAllErrorsInfoT();
        if(nullptr == common_topics_ptr_.all_errors_info_)
        {
            result = eResultFailure;
        }

        common_topics_ptr_.all_mode_info_ = service_publication_message_->GetPointerOfAllModeInfoT();
        if(nullptr == common_topics_ptr_.all_mode_info_)
        {
            result = eResultFailure;
        }

        common_topics_ptr_.error_report_ = service_publication_message_->GetPointerOfErrorReportT();
        if(nullptr == common_topics_ptr_.error_report_)
        {
            result = eResultFailure;
        }

        common_topics_ptr_.mode_state_report_ = service_publication_message_->GetPointerOfModeStateReportT();
        if(nullptr == common_topics_ptr_.mode_state_report_)
        {
            result = eResultFailure;
        }

        return result;
    }

    void RegisterToScribeTopics(void)
    {
        CTopicMessage<Subscribe_t> subscription_topic_message;

        std::string temp = external_input_component_name_ + "OutputTopic";
        subscription_topic_message.Subscribe(temp.c_str(), host_appln_offset_ + 1);

        //TODO: eModeManagerMessage replaced with  host_appln_offset_+ 2 for now
        subscription_topic_message.Subscribe("ComponentsReactionTopic", host_appln_offset_ + 2);
    }

    void Common_ReadParametersForErrorHandling(pfsp_topics::HostComponentAllErrorsInfoT *all_errors_info)
    {
        if(nullptr != all_errors_info)
        {
            all_errors_info->errors_info = &component_errors_info_;
            all_errors_info->component_id = component_id_;
            
            for(uint32_t error_id = 0 ; error_id < MAX_NUMBER_OF_ERRORS ; error_id++)
            {
                char_t temp_char[100] {};
                char_t *enumeration_char {};
                HostComponentErrorInfoT error_info {};

                sprintf(temp_char, "Error%d_Use", error_id);
                enumeration_char = const_cast<char_t *>(service_class_->GetParameterValue(temp_char));
                if ( nullptr != enumeration_char )
                {
                    //if ( strstr(enumeration_char, "eOn") )
                    if(0 == strcmp(enumeration_char, "eOn"))
                    {
                        Common_SaveAllErrorsInfo(error_id, error_info);
                        component_errors_info_.insert(std::pair<std::string, HostComponentErrorInfoT>(error_info.error_name, error_info));
                    }
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            std::cout << "\n Error::    ReadParametersForErrorHandling() -> [all_errors_info_ == nullptr] " << std::endl;
        }
    }

    void Common_SaveAllErrorsInfo(uint32_t count, HostComponentErrorInfoT &error_info)
    {
        char_t temp_char[100] {};
        char_t *enumeration_char {};
        uint32_t threshold {};

        error_info.status_of_usage = eOn;

        sprintf(temp_char, "Error%d", count);
        enumeration_char = const_cast<char_t *>(service_class_->GetParameterValue(temp_char));
        error_info.error_name.assign(enumeration_char);

        sprintf(temp_char, "Error%d_DebouncingCriteria", count);
        enumeration_char = const_cast<char_t *>(service_class_->GetParameterValue(temp_char));
        //if( strstr(enumeration_char, "eTime") )
        if(0 == strcmp(enumeration_char, "eTime"))
        {
            error_info.debouncing_criteria = FailureDebouncingCriteriaE::eTime;
        }
        else
        {
            error_info.debouncing_criteria = FailureDebouncingCriteriaE::eCount;
        }

        sprintf(temp_char, "Error%d_ThresholdToTriggerFailure", count);
        threshold = atoi(service_class_->GetParameterValue(temp_char));
        error_info.threshold_to_trigger_failure = threshold;

        sprintf(temp_char, "Error%d_ThresholdToResetFailure", count);
        threshold = atoi(service_class_->GetParameterValue(temp_char));
        error_info.threshold_to_reset_failure = threshold;

        sprintf(temp_char, "Error%d_ReactionMode", count);
        enumeration_char = const_cast<char_t *>(service_class_->GetParameterValue(temp_char));
        //if( strstr(enumeration_char, "eSWComponentOperationModeNormal") )
        if(0 == strcmp(enumeration_char, "eSWComponentOperationModeNormal"))
        {
            error_info.required_reaction_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;
        }
        //else if( strstr(enumeration_char, "eSWComponentOperationModeDegradation") )
        else if(0 == strcmp(enumeration_char, "eSWComponentOperationModeDegradation"))
        {
            error_info.required_reaction_mode = SWComponentOperationModeE::eSWComponentOperationModeDegradation;
        }
        //else if( strstr(enumeration_char, "eSWComponentOperationModeDiagnostics") )
        else if(0 == strcmp(enumeration_char, "eSWComponentOperationModeDiagnostics"))
        {
            error_info.required_reaction_mode = SWComponentOperationModeE::eSWComponentOperationModeDiagnostics;
        }
        else
        {
            error_info.required_reaction_mode = SWComponentOperationModeE::eSWComponentOperationModeEOL;
        }

        sprintf(temp_char, "Error%d_ReactionState", count);
        enumeration_char = const_cast<char_t *>(service_class_->GetParameterValue(temp_char));
        if(0 == strcmp(enumeration_char, "eSWComponentStateCreation"))
        {
            error_info.required_reaction_state = SWComponentStateE::eSWComponentStateCreation;
        }
        // else if( strstr(enumeration_char, "eSWComponentStateTriggerInit") )
        else if(0 == strcmp(enumeration_char, "eSWComponentStateTriggerInit"))
        {
            error_info.required_reaction_state = SWComponentStateE::eSWComponentStateTriggerInit;
        }                                
        //else if( strstr(enumeration_char, "eSWComponentStateInitialized") )
        else if(0 == strcmp(enumeration_char, "eSWComponentStateInitialized"))
        {
            error_info.required_reaction_state = SWComponentStateE::eSWComponentStateInitialized;
        }
        //else if( strstr(enumeration_char, "eSWComponentStateRun") )
        else if(0 == strcmp(enumeration_char, "eSWComponentStateRun"))
        {
            error_info.required_reaction_state = SWComponentStateE::eSWComponentStateRun;
        }
        //else if( strstr(enumeration_char, "eSWComponentStateStop") )
        else if(0 == strcmp(enumeration_char, "eSWComponentStateStop"))
        {
            error_info.required_reaction_state = SWComponentStateE::eSWComponentStateStop;
        }
        //else if( strstr(enumeration_char, "eSWComponentStateStopDone") )
        else if(0 == strcmp(enumeration_char, "eSWComponentStateStopDone"))
        {
            error_info.required_reaction_state = SWComponentStateE::eSWComponentStateStopDone;
        }
        //else if( strstr(enumeration_char, "eSWComponentStateTriggerShutdown") )
        else if(0 == strcmp(enumeration_char, "eSWComponentStateTriggerShutdown"))
        {
            error_info.required_reaction_state = SWComponentStateE::eSWComponentStateTriggerShutdown;
        }
        //else if( strstr(enumeration_char, "eSWComponentStateReadyToShutdown") )
        else if(0 == strcmp(enumeration_char, "eSWComponentStateReadyToShutdown"))
        {
            error_info.required_reaction_state = SWComponentStateE::eSWComponentStateReadyToShutdown;
        }
        else if(0 == strcmp(enumeration_char, "eSWComponentStateShutdownDone"))
        {
            error_info.required_reaction_state = SWComponentStateE::eSWComponentStateShutdownDone;
        }
        else if(0 == strcmp(enumeration_char, "eSWComponentStateCurrent"))
        {
            error_info.required_reaction_state = SWComponentStateE::eSWComponentStateCurrent;
        }
        else if(0 == strcmp(enumeration_char, "eSWComponentStateNone"))
        {
            error_info.required_reaction_state = SWComponentStateE::eSWComponentStateNone;
        }
        else
        {
            error_info.required_reaction_state = SWComponentStateE::eSWComponentStateMax;
        }
    }

    void Common_ReadParametersForModeManagement(pfsp_topics::HostComponentAllModeInfoT *all_mode_info)
    {
        if(nullptr != all_mode_info)
        {
            all_mode_info->modes_info = &component_modes_info_;
            all_mode_info->component_id = component_id_;

            char_t *enumeration_char {};
            std::string temp_mode_name {};

            enumeration_char = const_cast<char_t *>(service_class_->GetParameterValue("eSWComponentOperationModeNormal"));
            //temp_mode_name = enumeration_char;
            temp_mode_name.assign(enumeration_char);
            //if( strstr(enumeration_char, "eOn") )
            if(0 == strcmp(enumeration_char, "eOn"))
            {
                component_modes_info_.insert(std::pair<std::string, SWComponentOperationModeE>(temp_mode_name, SWComponentOperationModeE::eSWComponentOperationModeNormal));
            }

            enumeration_char = const_cast<char_t *>(service_class_->GetParameterValue("eSWComponentOperationModeDegradation"));
            //temp_mode_name = enumeration_char;
            temp_mode_name.assign(enumeration_char);
            //if( strstr(enumeration_char, "eOn") )
            if(0 == strcmp(enumeration_char, "eOn"))
            {
                component_modes_info_.insert(std::pair<std::string, SWComponentOperationModeE>(temp_mode_name, SWComponentOperationModeE::eSWComponentOperationModeDegradation));
            }

            enumeration_char = const_cast<char_t *>(service_class_->GetParameterValue("eSWComponentOperationModeDiagnostics"));
            //temp_mode_name = enumeration_char;
            temp_mode_name.assign(enumeration_char);
            //if( strstr(enumeration_char, "eOn") )
            if(0 == strcmp(enumeration_char, "eOn"))
            {
                component_modes_info_.insert(std::pair<std::string, SWComponentOperationModeE>(temp_mode_name, SWComponentOperationModeE::eSWComponentOperationModeDiagnostics));
            }
        }
    }

    void Common_ReadParametersForComponentRunner(const char_t *play_type, const char_t *thread_frequency)
    {
        char_t *enumeration_char {};

        enumeration_char = const_cast<char_t *>(service_class_->GetParameterValue(play_type));
        //if( strstr(enumeration_char, "Once") )
        if(0 == strcmp(enumeration_char, "Once"))
        {
            play_type_ = eOnce;
        }
        else
        {
            play_type_ = eRepeat;
        }

        thread_frequency_ = atoi(service_class_->GetParameterValue(thread_frequency));
    }
};


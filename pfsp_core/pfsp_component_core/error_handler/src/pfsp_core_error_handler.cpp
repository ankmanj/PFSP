#include <cstring>
#include <iterator>
#include "pfsp_core_error_handler.h"

PfspCoreErrorHandler::PfspCoreErrorHandler(void){
}

void PfspCoreErrorHandler::Initialize(uint32_t number_of_components, ErrorHandlerReactionReportT *reaction_report_output)
{
    real_number_of_components_ = number_of_components - static_cast<uint32_t>(DEFAULT_NUMBER_OF_COMPONENTS);
    
    components_error_handling_table_.resize(real_number_of_components_);
	components_error_tracking_table_.resize(real_number_of_components_);
    normal_state_tracking_table_.resize(real_number_of_components_);
    components_status_.resize(real_number_of_components_);

    last_frame_count_.assign(real_number_of_components_, 1000U);
    current_frame_count_.resize(real_number_of_components_);
    last_input_timestamp_.resize(real_number_of_components_);
    current_input_timestamp_.resize(real_number_of_components_);

    reaction_report_output_ = reaction_report_output;
    synchronization_ = false;
}

void PfspCoreErrorHandler::MakeErrorHandlingTable(pfsp_topics::HostComponentAllErrorsInfoT* component_errors_info) noexcept
{
    std::map<std::string, HostComponentErrorInfoT> *component_error_info_map_input = component_errors_info->errors_info;
    std::unordered_map<std::string, HostComponentErrorInfoT> *error_map = 
                                                        &components_error_handling_table_.at(component_errors_info->component_id);
    std::unordered_map<std::string, ErrorTrackingInfoT> *error_tracking_map = 
                                                        &components_error_tracking_table_.at(component_errors_info->component_id);

    for(auto error_input : (*component_error_info_map_input))
    {
        std::string component_error_name_on_table = error_input.first;
        auto& component_error_criteria = error_input.second;

        (*error_map).insert(std::pair<std::string, HostComponentErrorInfoT>(component_error_name_on_table, component_error_criteria));

        ErrorTrackingInfoT temp_error_tracking_info{};
        (*error_tracking_map).insert(std::pair<std::string, ErrorTrackingInfoT>(component_error_name_on_table, temp_error_tracking_info));
    }

#if 0
    std::unordered_map<std::string, HostComponentErrorInfoT> *component_error_info_map_2 = &components_error_handling_table_.at(1);

    std::cout << "\n[ Component ID: Always 1111111 ]\n";
    for(auto& error : (*component_error_info_map_2))
    {
        std::string component_error_name_on_table = error.first;
        auto& component_error_criteria = error.second;

        std::cout << "[Second_error name: " << component_error_name_on_table << "]\n";
        std::cout << "[Second_threshold_to_trigger_failure: " << component_error_criteria.threshold_to_trigger_failure << "]\n";
        std::cout << "[Second_threshold_to_reset_failure: " << component_error_criteria.threshold_to_reset_failure << "]\n";
    }
#endif
}

void PfspCoreErrorHandler::ReportError(const HostComponentErrorReportT *reported_error) noexcept 
{
    TimeInfoT *current_input_timestamp = &current_input_timestamp_[reported_error->component_id];
    TimeInfoT *last_input_timestamp = &last_input_timestamp_[reported_error->component_id];

    current_input_timestamp->sec = reported_error->frame_time_stamp.sec;
    current_input_timestamp->usec = reported_error->frame_time_stamp.usec;
    //int64_t elapse_millisec = GetElapseTimeByPointer(current_input_timestamp, last_input_timestamp);
    //std::cout << "============= [Error Handler:: Error_Report_Subscription_Frequency  " << elapse_millisec << "]\n\n";

    uint32_t *current_frame_count = &current_frame_count_[reported_error->component_id];
    uint32_t *last_frame_count = &last_frame_count_[reported_error->component_id];

    *current_frame_count = reported_error->frame_count;
    //int32_t diff_of_frame_count = static_cast<int32_t>(*current_frame_count) - static_cast<int32_t>(*last_frame_count);

    if(SWComponentErrorCategoryE::eSWComponentErrorNone == reported_error->error_type)
    {
        UpdateNormalStateTrackingTable(reported_error);
        CoreProcessingForResetFailure(reported_error);
    }
    else
    {
        std::cout << "\n========================= [CoreErrorHandler: ReportError] =========================\n";
        std::cout << "[Component ID: " << reported_error->component_id << "], " << "[Frame Count: " << reported_error->frame_count << "], " << "[Error Name: " << reported_error->error_name << "]\n";
        std::cout << "========================= [CoreErrorHandler: ReportError] =========================\n\n";

        UpdateErrorStateTrackingTable(reported_error);
        CoreProcessingForFailureDetection(reported_error);
    }

    last_input_timestamp->usec = current_input_timestamp->usec;
    last_input_timestamp->sec = current_input_timestamp->sec;

    *last_frame_count = *current_frame_count;

    synchronization_ = true;
}

void PfspCoreErrorHandler::UpdateNormalStateTrackingTable(const HostComponentErrorReportT *reported_error) noexcept
{
    pfsp_topics::NormalStateTrackingInfoT *normal_state_info_tracking = &normal_state_tracking_table_.at(reported_error->component_id);

    if((normal_state_info_tracking->frame_count + 1) == reported_error->frame_count)
    {
        normal_state_info_tracking->count_to_reset_failure++;
        normal_state_info_tracking->normal_consecutiveness = true;
    }
    else
    {
        normal_state_info_tracking->count_to_reset_failure = 1;
        normal_state_info_tracking->normal_consecutiveness = false;
        normal_state_info_tracking->first_normal_time.sec = reported_error->frame_time_stamp.sec;
        normal_state_info_tracking->first_normal_time.usec = reported_error->frame_time_stamp.usec;
    }
    normal_state_info_tracking->frame_count = reported_error->frame_count;
    normal_state_info_tracking->last_normal_time.sec = reported_error->frame_time_stamp.sec;
    normal_state_info_tracking->last_normal_time.usec = reported_error->frame_time_stamp.usec;
}

void PfspCoreErrorHandler::CoreProcessingForResetFailure(const HostComponentErrorReportT *reported_error) noexcept
{
    pfsp_topics::NormalStateTrackingInfoT *tracked_normal_state_info = &normal_state_tracking_table_.at(reported_error->component_id);
    HostComponentModeManagementInfoT *component_next_reaction = &reaction_report_output_->next_reaction[reported_error->component_id];
    ComponentFailureInfoT *component_status = &components_status_.at(reported_error->component_id);

    ComponentFailureInfoT current_component_status {};
    current_component_status.failed = component_status->failed;
    current_component_status.error_name = component_status->error_name;
    current_component_status.debouncing_criteria = component_status->debouncing_criteria;
    current_component_status.threshold_to_reset_failure = component_status->threshold_to_reset_failure;

    if(true == component_status->failed)
    {
        if(FailureDebouncingCriteriaE::eCount == current_component_status.debouncing_criteria)
        {
            if(current_component_status.threshold_to_reset_failure <= tracked_normal_state_info->count_to_reset_failure)
            {
                std::cout << "*************** [Reset Fault] ***************\n";
                std::cout << "[Component ID: " << reported_error->component_id << "]\n";
                std::cout << "[Threshold To Reset Fault: " << current_component_status.threshold_to_reset_failure << "]\n";
                std::cout << "[Count To Reset Fault: " << tracked_normal_state_info->count_to_reset_failure << "]\n";
                std::cout << "*************** [Reset Fault] ***************\n";

                /* Finally, reset the component failure. The component is no more failure status*/
                // And we can discussion the reaction strategy in this case for the variables "keep_current_state_mode" and "state"
                // component_next_reaction->keep_current_state_mode = NextStateE::eChangeCurrentState;
                // component_next_reaction->state = SWComponentStateE::eSWComponentStateTriggerInit;
                component_next_reaction->keep_current_state_mode = NextStateE::eKeepCurrentState;
                component_next_reaction->state = SWComponentStateE::eSWComponentStateCurrent;
                component_next_reaction->operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;

                component_status->failed = false;
                component_status->error_name = "";
            }
            else
            {
                /* Finally, reset the component failure. The component is no more failure status*/
                component_next_reaction->keep_current_state_mode = NextStateE::eKeepCurrentState;
                component_next_reaction->state = SWComponentStateE::eSWComponentStateCurrent;
                component_next_reaction->operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNone;

                // The component fault setting "component_status->failed = true" should be done in the only "[Trigger Fault]".
                // component_status->failed = true;
            }
        }
        else    /* ( FailureDebouncingCriteriaE::eTime == error_info.debouncing_criteria ) */
        {
            /* need to check that time stamp is working properly */
            int64_t normal_status_elapse_sec = tracked_normal_state_info->last_normal_time.sec - 
                                                tracked_normal_state_info->first_normal_time.sec;
            int64_t normal_status_elapse_usec = tracked_normal_state_info->last_normal_time.usec - 
                                                tracked_normal_state_info->first_normal_time.usec;
            int64_t normal_status_elapse_millisec = (normal_status_elapse_sec*1000 + normal_status_elapse_usec/1000);
            //int64_t normal_status_elapse_millisec = GetElapseTimeByReference(tracked_normal_state_info->last_normal_time, 
            //                                                                    tracked_normal_state_info->first_normal_time);

            if(current_component_status.threshold_to_reset_failure <= normal_status_elapse_millisec && \
                    tracked_normal_state_info->normal_consecutiveness == true)
            {
                std::cout << "\n*************** [Reset Fault] ***************\n";
                std::cout << "[Component ID: " << reported_error->component_id << "]\n";
                std::cout << "[Threshold To Reset Fault: " << current_component_status.threshold_to_reset_failure << "]\n";
                std::cout << "[Normal Status Elapse Millisec: " << normal_status_elapse_millisec << "]\n";
                std::cout << "*************** [Reset Fault] ***************\n";

                /* Finally, reset the component failure. The component is no more failure status*/
                // And we can discussion the reaction strategy in this case for the variables "keep_current_state_mode" and "state"
                // component_next_reaction->keep_current_state_mode = NextStateE::eChangeCurrentState;
                // component_next_reaction->state = SWComponentStateE::eSWComponentStateTriggerInit;
                component_next_reaction->keep_current_state_mode = NextStateE::eKeepCurrentState;
                component_next_reaction->state = SWComponentStateE::eSWComponentStateCurrent;
                component_next_reaction->operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNormal;

                component_status->failed = false;
                component_status->error_name = "";
            }
            else
            {
                /* Finally, reset the component failure. The component is no more failure status*/
                component_next_reaction->keep_current_state_mode = NextStateE::eKeepCurrentState;
                component_next_reaction->state = SWComponentStateE::eSWComponentStateCurrent;
                component_next_reaction->operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNone;

                // The component fault setting "component_status->failed = true" should be done in the only "[Trigger Fault]".
                // component_status->failed = true;
            }
        }
    }
    else 
    {
        component_next_reaction->keep_current_state_mode = NextStateE::eKeepCurrentState;
        component_next_reaction->state = SWComponentStateE::eSWComponentStateCurrent;
        component_next_reaction->operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNone;
    }
}

void PfspCoreErrorHandler::UpdateErrorStateTrackingTable(const HostComponentErrorReportT *reported_error) noexcept 
{
    std::unordered_map<std::string, ErrorTrackingInfoT> *component_error_tracking_map = 
                                                            &components_error_tracking_table_.at(reported_error->component_id);
    for(auto& error_tracking_info : (*component_error_tracking_map))
    {
        std::string component_error_name_on_table = error_tracking_info.first;
        //if(strstr(component_error_name_on_table.c_str(), reported_error->error_name.c_str()))
        if(0 == component_error_name_on_table.compare(reported_error->error_name))
        {
            auto& tracking_info = error_tracking_info.second;
            
            if( (tracking_info.frame_count + 1) == reported_error->frame_count )
            {
                tracking_info.count_to_trigger_failure++;
                tracking_info.error_consecutiveness = true;
            }
            else
            {
                tracking_info.count_to_trigger_failure = 1;
                tracking_info.error_consecutiveness = false;
                tracking_info.first_error_timestamp.sec = reported_error->frame_time_stamp.sec;
                tracking_info.first_error_timestamp.usec = reported_error->frame_time_stamp.usec;
            }
            tracking_info.frame_count = reported_error->frame_count;
            tracking_info.last_error_timestamp.sec = reported_error->frame_time_stamp.sec;
            tracking_info.last_error_timestamp.usec = reported_error->frame_time_stamp.usec;
        }
    }
}

void PfspCoreErrorHandler::CoreProcessingForFailureDetection(const HostComponentErrorReportT *reported_error) noexcept
{
    std::unordered_map<std::string, HostComponentErrorInfoT> *component_error_info_map = 
                                                            &components_error_handling_table_.at(reported_error->component_id);
    std::unordered_map<std::string, ErrorTrackingInfoT> *component_error_tracking_map = 
                                                            &components_error_tracking_table_.at(reported_error->component_id);
    HostComponentModeManagementInfoT *component_next_reaction = &reaction_report_output_->next_reaction[reported_error->component_id];
    ComponentFailureInfoT *component_status = &components_status_.at(reported_error->component_id);

    for(auto& error : (*component_error_info_map))
    {
        std::string component_error_name_on_table = error.first;
        //if(strstr(component_error_name_on_table.c_str(), reported_error->error_name))
        if(0 == component_error_name_on_table.compare(reported_error->error_name))
        {
            auto& tracking_info = (*component_error_tracking_map).find(component_error_name_on_table)->second;
            auto& component_error_criteria = error.second;
            if(FailureDebouncingCriteriaE::eCount == component_error_criteria.debouncing_criteria)
            {
                if(component_error_criteria.threshold_to_trigger_failure <= tracking_info.count_to_trigger_failure)
                {
                    /* Finally, give the value of derived "reaction state" and "reaction operation mode" 
                                to external output of error handler */
                    component_next_reaction->keep_current_state_mode = NextStateE::eChangeCurrentState;
                    component_next_reaction->state = component_error_criteria.required_reaction_state;
                    component_next_reaction->operation_mode = component_error_criteria.required_reaction_mode;

                    component_status->failed = true;
                    component_status->error_name = component_error_name_on_table;
                    component_status->debouncing_criteria = component_error_criteria.debouncing_criteria;
                    component_status->threshold_to_reset_failure = component_error_criteria.threshold_to_reset_failure;

                    std::cout << "=============== [Trigger Fault] ===============\n";
                    std::cout << "[component_id: " << reported_error->component_id << "]\n";
                    std::cout << "[Threshold To Trigger Fault: " << component_error_criteria.threshold_to_trigger_failure << "]\n";
                    std::cout << "[Count To Trigger Fault: " << tracking_info.count_to_trigger_failure << "]\n";
                    std::cout << "=============== [Trigger Fault] ===============\n";
                }
                else
                {
                    /* Need to keep the current "reaction state" and "reaction operation mode" */
                    component_next_reaction->keep_current_state_mode = NextStateE::eKeepCurrentState;
                    component_next_reaction->state = SWComponentStateE::eSWComponentStateCurrent;
                    component_next_reaction->operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNone;

                    // The component fault setting "component_status->failed = false" should be done in the only "[Reset Fault]".
                    // component_status->failed = false;
                    component_status->error_name = component_error_name_on_table;
                    component_status->debouncing_criteria = component_error_criteria.debouncing_criteria;
                    component_status->threshold_to_reset_failure = component_error_criteria.threshold_to_reset_failure;
                }

                if(true == component_status->failed)
                {
                    tracking_info.count_to_trigger_failure = 0;
                }
            }
            else    /* ( FailureDebouncingCriteriaE::eTime == error_info.debouncing_criteria ) */
            {
                /* need to check that time stamp is working properly */
                int64_t error_elapse_sec = tracking_info.last_error_timestamp.sec - tracking_info.first_error_timestamp.sec;
                int64_t error_elapse_usec = tracking_info.last_error_timestamp.usec - tracking_info.first_error_timestamp.usec;
                int64_t error_elapse_millisec = (error_elapse_sec*1000 + error_elapse_usec/1000);
                // int64_t error_elapse_millisec = GetElapseTimeByReference(tracking_info.last_error_timestamp, 
                //                                                             tracking_info.first_error_timestamp);

                std::cout << "[Error Name: " << component_error_name_on_table << "]\n";
                std::cout << "[Error Elapse Millisec: " << error_elapse_millisec  << "]\n";
                std::cout << "[Threshold To Trigger Fault: " << component_error_criteria.threshold_to_trigger_failure << "]\n\n";

                if(component_error_criteria.threshold_to_trigger_failure <= error_elapse_millisec && \
                        tracking_info.error_consecutiveness == true)
                {
                    std::cout << "=============== [Trigger Fault] ===============\n";
                    std::cout << "[component_id: " << reported_error->component_id << "]\n";
                    std::cout << "[Threshold To Trigger Fault: " << component_error_criteria.threshold_to_trigger_failure << "]\n";
                    std::cout << "[Error Elapse Millisec: " << error_elapse_millisec << "]\n";
                    std::cout << "=============== [Trigger Fault] ===============\n";

                    /* Finally, give the value of derived "reaction state" and "reaction operation mode" 
                                to external output of error handler */
                    component_next_reaction->keep_current_state_mode = NextStateE::eChangeCurrentState;
                    component_next_reaction->state = component_error_criteria.required_reaction_state;
                    component_next_reaction->operation_mode = component_error_criteria.required_reaction_mode;

                    component_status->failed = true;
                    component_status->error_name = component_error_name_on_table;
                    component_status->debouncing_criteria = component_error_criteria.debouncing_criteria;
                    component_status->threshold_to_reset_failure = component_error_criteria.threshold_to_reset_failure;
                }
                else
                {
                    /* Need to keep the current "reaction state" and "reaction operation mode" */
                    component_next_reaction->keep_current_state_mode = NextStateE::eKeepCurrentState;
                    component_next_reaction->state = SWComponentStateE::eSWComponentStateCurrent;
                    component_next_reaction->operation_mode = SWComponentOperationModeE::eSWComponentOperationModeNone;

                    // The component fault setting "component_status->failed = false" should be done in the only "[Reset Fault]".
                    // component_status->failed = false;
                    component_status->error_name = component_error_name_on_table;
                    component_status->debouncing_criteria = component_error_criteria.debouncing_criteria;
                    component_status->threshold_to_reset_failure = component_error_criteria.threshold_to_reset_failure;
                }

                if(true == component_status->failed)
                {
                    tracking_info.first_error_timestamp.sec = reported_error->frame_time_stamp.sec;
                    tracking_info.first_error_timestamp.usec = reported_error->frame_time_stamp.usec;
                }
            }

            if(NextStateE::eChangeCurrentState == component_next_reaction->keep_current_state_mode &&
                SWComponentStateE::eSWComponentStateTriggerInit == component_next_reaction->state &&
                true == component_status->failed)
            {
                component_status->failed = false;
                component_status->error_name = "";
            }
        }
    }
}

bool PfspCoreErrorHandler::SyncUpOutputOfComponentsReaction() noexcept
{
    bool result = synchronization_;

    synchronization_ = false;
    
    return result;
}

void PfspCoreErrorHandler::Terminate(void) noexcept 
{
    components_status_.clear();
    std::vector<ComponentFailureInfoT>().swap(components_status_);

    normal_state_tracking_table_.clear();
    std::vector<pfsp_topics::NormalStateTrackingInfoT>().swap(normal_state_tracking_table_);

    for(auto& map_data : components_error_handling_table_)
    {
        map_data.clear();
        std::unordered_map<std::string, HostComponentErrorInfoT>().swap(map_data);
    }    
    std::vector<std::unordered_map<std::string, HostComponentErrorInfoT>>().swap(components_error_handling_table_);

    for(auto& map_data : components_error_tracking_table_)
    {
        map_data.clear();
        std::unordered_map<std::string, ErrorTrackingInfoT>().swap(map_data);
    }    
    std::vector<std::unordered_map<std::string, ErrorTrackingInfoT>>().swap(components_error_tracking_table_);

    last_frame_count_.clear();
    std::vector<uint32_t>().swap(last_frame_count_);

    current_frame_count_.clear();
    std::vector<uint32_t>().swap(current_frame_count_);

    last_input_timestamp_.clear();
    std::vector<TimeInfoT>().swap(last_input_timestamp_);

    current_input_timestamp_.clear();
    std::vector<TimeInfoT>().swap(current_input_timestamp_);
}


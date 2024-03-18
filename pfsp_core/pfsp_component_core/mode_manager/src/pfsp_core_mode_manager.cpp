#include <bits/stdint-uintn.h>
#include "pfsp_core_mode_manager.h"

PfspCoreModeManager::PfspCoreModeManager(void){
}

void PfspCoreModeManager::Initialize(uint32_t number_of_components, ModeManagerComponentsReactionT *reaction_report_output)
{
    real_number_of_components_ = number_of_components - static_cast<uint32_t>(DEFAULT_NUMBER_OF_COMPONENTS);

    components_mode_management_table_.resize(real_number_of_components_);
    components_current_state_mode_table_.resize(real_number_of_components_);
    error_handler_reaction_report_.resize(real_number_of_components_);

    reaction_output_ = reaction_report_output;
    synchronization_ = false;
}

void PfspCoreModeManager::MakeModeManagementTable(pfsp_topics::HostComponentAllModeInfoT *component_modes_info) noexcept 
{
    std::map<std::string, SWComponentOperationModeE> *component_mode_info_map_input = component_modes_info->modes_info;
    std::unordered_map<std::string, SWComponentOperationModeE> *mode_info_map = 
                                            &components_mode_management_table_.at(component_modes_info->component_id);

    for(auto mode_info_input : (*component_mode_info_map_input))
    {
        std::string component_mode_name_on_table = mode_info_input.first;
        auto& component_mode_info = mode_info_input.second;

        (*mode_info_map).insert(std::pair<std::string, SWComponentOperationModeE>(component_mode_name_on_table, component_mode_info));
    }
}

void PfspCoreModeManager::ReportErrorHandlerReactionStateMode(ErrorHandlerReactionReportT *reported_reaction) noexcept
{
    uint32_t count {};
    for(auto &input : reported_reaction->next_reaction)
    {
        //std::cout << "ReportErrorHandlerReactionStateMode [component_id: " << count << "]\n";
        //std::cout << "ReportErrorHandlerReactionStateMode [keep_current_state_mode: " << input.keep_current_state_mode << "]\n";
        //std::cout << "ReportErrorHandlerReactionStateMode [state: " << static_cast<uint32_t>(input.state) << "]\n";
        //std::cout << "ReportErrorHandlerReactionStateMode [operation_mode: " << static_cast<uint32_t>(input.operation_mode) << "]\n\n";
        HostComponentModeManagementInfoT* error_handler_reaction = &error_handler_reaction_report_.at(count);
        error_handler_reaction->keep_current_state_mode = input.keep_current_state_mode;
        error_handler_reaction->operation_mode = input.operation_mode;
        error_handler_reaction->state = input.state;
        count++;
    }
}

void PfspCoreModeManager::ReportComponentStateMode(HostComponentModeStateReportT *reported_state_mode) noexcept 
{
    UpdateComponentCurrentStateModeTable(reported_state_mode);
    CoreProcessingForModeManagement(reported_state_mode);
}

void PfspCoreModeManager::UpdateComponentCurrentStateModeTable(HostComponentModeStateReportT *reported_state_mode) noexcept
{
    HostComponentModeStateReportT* (*current_mode_info_map) = &components_current_state_mode_table_.at(reported_state_mode->component_id);
    (*current_mode_info_map) = reported_state_mode;
}

void PfspCoreModeManager::CoreProcessingForModeManagement(HostComponentModeStateReportT *reported_state_mode) noexcept
{
    /* Mode manager output */
    HostComponentModeManagementInfoT* output_next_reaction = &reaction_output_->component.at(reported_state_mode->component_id);

    /* Input(component current state and mode) from each host component */
    HostComponentModeStateReportT* input_component_state_mode = components_current_state_mode_table_.at(reported_state_mode->component_id);

    /* Input(next reaction) from error handler */
    HostComponentModeManagementInfoT* input_error_handler_next_reaction = &error_handler_reaction_report_.at(reported_state_mode->component_id);

    //std::cout << "\nCoreProcessingForModeManagement [component_id: " << reported_state_mode->component_id << "]\n";
    //std::cout << "input_error_handler_next_reaction [state: " << static_cast<uint32_t>(input_error_handler_next_reaction->state) << "]\n";
    //std::cout << "input_error_handler_next_reaction [operation_mode: " << static_cast<uint32_t>(input_error_handler_next_reaction->operation_mode) << "]\n\n";

    output_next_reaction->publication_state = DataPublicationStateE::ePublishedData;
    if ( eOnce == input_component_state_mode->play_type )
    {
        //std::cout << "\nif ( eOnce == input_component_state_mode->play_type ) " << "]\n";
        if (SWComponentStateE::eSWComponentStateTriggerInit == input_component_state_mode->current_info.state)
        {
            output_next_reaction->state = SWComponentStateE::eSWComponentStateTriggerInit;
            output_next_reaction->operation_mode = input_component_state_mode->current_info.operation_mode;
        }
        else if (SWComponentStateE::eSWComponentStateInitialized == input_component_state_mode->current_info.state)
        {
            output_next_reaction->state = SWComponentStateE::eSWComponentStateRun;
            output_next_reaction->operation_mode = input_component_state_mode->current_info.operation_mode;
        }
        else if (SWComponentStateE::eSWComponentStateRun == input_component_state_mode->current_info.state)
        {
            output_next_reaction->state = SWComponentStateE::eSWComponentStateStop;
            output_next_reaction->operation_mode = input_component_state_mode->current_info.operation_mode;
        }
        else
        {
            output_next_reaction->state = input_component_state_mode->current_info.state;
            output_next_reaction->operation_mode = input_component_state_mode->current_info.operation_mode;
        }
    }
    else
    {
        //std::cout << "\nif ( eRepeat == input_component_state_mode->play_type ) " << "]\n";
        if(NextStateE::eNone == input_error_handler_next_reaction->keep_current_state_mode)
        {
            if (SWComponentStateE::eSWComponentStateTriggerInit == input_component_state_mode->current_info.state)
            {
                output_next_reaction->state = SWComponentStateE::eSWComponentStateTriggerInit;
                output_next_reaction->operation_mode = input_component_state_mode->current_info.operation_mode;
            }
            else if (SWComponentStateE::eSWComponentStateInitialized == input_component_state_mode->current_info.state)
            {
                output_next_reaction->state = SWComponentStateE::eSWComponentStateRun;
                output_next_reaction->operation_mode = input_component_state_mode->current_info.operation_mode;
            }
            else
            {
                output_next_reaction->state = SWComponentStateE::eSWComponentStateTriggerInit;
                output_next_reaction->operation_mode = input_error_handler_next_reaction->operation_mode;
                output_next_reaction->publication_state = DataPublicationStateE::eNotPublishedData;
            }        
        }
        else if (NextStateE::eKeepCurrentState == input_error_handler_next_reaction->keep_current_state_mode)
        {
            if (SWComponentStateE::eSWComponentStateInitialized == input_component_state_mode->current_info.state)
            {
                output_next_reaction->state = SWComponentStateE::eSWComponentStateRun;
                output_next_reaction->operation_mode = input_component_state_mode->current_info.operation_mode;
                //output_next_reaction->keep_current_state_mode = NextStateE::eChangeCurrentState;
            }
            else if (SWComponentStateE::eSWComponentStateStopDone == input_component_state_mode->current_info.state)
            {
                output_next_reaction->state = SWComponentStateE::eSWComponentStateNone;
                output_next_reaction->operation_mode = input_component_state_mode->current_info.operation_mode;
            }
            else
            {
                output_next_reaction->state = input_component_state_mode->current_info.state;
                output_next_reaction->operation_mode = input_component_state_mode->current_info.operation_mode;
            }
        }
        else
        {
            std::cout << "\n=============== ModeManager: Publish Changed Reaction ===============\n";
            std::cout << "[Component ID: " << reported_state_mode->component_id << "]\n";
            std::cout << "[Reaction State: " << static_cast<uint32_t>(input_error_handler_next_reaction->state) << "]\n";
            std::cout << "[Reaction Operation Mode: " << static_cast<uint32_t>(input_error_handler_next_reaction->operation_mode) << "]\n";
            std::cout << "=============== ModeManager: Publish Changed Reaction ===============\n";
            
            if( SWComponentStateE::eSWComponentStateCurrent == input_error_handler_next_reaction->state )
            {
                output_next_reaction->state = input_component_state_mode->current_info.state;
                output_next_reaction->operation_mode = input_component_state_mode->current_info.operation_mode;
            }
            else
            {
                output_next_reaction->state = input_error_handler_next_reaction->state;
                output_next_reaction->operation_mode = input_error_handler_next_reaction->operation_mode;
            }
        }
    }
    synchronization_ = true;
}

bool PfspCoreModeManager::SyncUpOutputOfComponentsReaction() noexcept
{
    bool result = synchronization_;

    synchronization_ = false;
    
    return result;
}

void PfspCoreModeManager::Terminate(void) noexcept 
{
    error_handler_reaction_report_.clear();
    std::vector<HostComponentModeManagementInfoT>().swap(error_handler_reaction_report_);

    for(auto& map_data : components_mode_management_table_)
    {
        map_data.clear();
        std::unordered_map<std::string, SWComponentOperationModeE>().swap(map_data);
    }
    std::vector<std::unordered_map<std::string, SWComponentOperationModeE>>().swap(components_mode_management_table_);
}

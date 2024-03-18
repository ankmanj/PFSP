#ifndef INCLUDE_PFSP_CORE_MODE_MANAGER__H_
#define INCLUDE_PFSP_CORE_MODE_MANAGER__H_

#include "host_application_topic.h"
#include "pfsp_typedef.h"

class PfspCoreModeManager
{
public:
    PfspCoreModeManager(void);
    virtual ~PfspCoreModeManager(void) = default;

    void Initialize(uint32_t number_of_components, ModeManagerComponentsReactionT *reaction_report_output);
    void Terminate(void) noexcept;
    void ReportComponentStateMode(HostComponentModeStateReportT *reported_state_mode) noexcept;
    void ReportErrorHandlerReactionStateMode(ErrorHandlerReactionReportT *reported_reaction) noexcept;
    void MakeModeManagementTable(pfsp_topics::HostComponentAllModeInfoT *component_modes_info) noexcept;
    bool SyncUpOutputOfComponentsReaction(void) noexcept;
    
protected:

private:
    void UpdateComponentCurrentStateModeTable(HostComponentModeStateReportT *reported_state_mode) noexcept;
    void CoreProcessingForModeManagement(HostComponentModeStateReportT *reported_state_mode) noexcept;

    bool synchronization_ {};
    uint32_t real_number_of_components_ {};

    ModeManagerComponentsReactionT *reaction_output_ {};                               // output
    std::vector<HostComponentModeStateReportT *> components_current_state_mode_table_ {};    // input from each component
    std::vector<HostComponentModeManagementInfoT> error_handler_reaction_report_ {};   // input from error handler
    std::vector<std::unordered_map<std::string, SWComponentOperationModeE>> components_mode_management_table_ {};
};

#endif // INCLUDE_PFSP_CORE_MODE_MANAGER__H_
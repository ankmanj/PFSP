#ifndef INCLUDE_PFSP_CORE_ERROR_HANDLER__H_
#define INCLUDE_PFSP_CORE_ERROR_HANDLER__H_

#include "host_application_topic.h"
#include "pfsp_typedef.h"

class PfspCoreErrorHandler
{
public:
    PfspCoreErrorHandler(void);
    virtual ~PfspCoreErrorHandler(void) = default;

    void Initialize(uint32_t number_of_components, ErrorHandlerReactionReportT *reaction_report_output);
    void Terminate(void) noexcept;
    void MakeErrorHandlingTable(pfsp_topics::HostComponentAllErrorsInfoT* component_errors_info) noexcept;
    void ReportError(const HostComponentErrorReportT *reported_error) noexcept;
    bool SyncUpOutputOfComponentsReaction(void) noexcept;
protected:

private:
    void UpdateNormalStateTrackingTable(const HostComponentErrorReportT *reported_error) noexcept;
    void UpdateErrorStateTrackingTable(const HostComponentErrorReportT *reported_error) noexcept;
    void CoreProcessingForResetFailure(const HostComponentErrorReportT *reported_error) noexcept;
    void CoreProcessingForFailureDetection(const HostComponentErrorReportT *reported_error) noexcept;

    uint32_t real_number_of_components_ {};

    // This is for the output from the component "PfspCoreErrorHandler".
    ErrorHandlerReactionReportT *reaction_report_output_ {};

    // "components_status_" is mainly used in the function "CoreProcessingForResetFailure()".
    std::vector<ComponentFailureInfoT> components_status_ {};
    
    // "components_status_" is mainly used in the function "CoreProcessingForResetFailure()".
    std::vector<pfsp_topics::NormalStateTrackingInfoT> normal_state_tracking_table_ {};

    // "components_status_" is mainly used in the function "CoreProcessingForFailureDetection()".
    std::vector<std::unordered_map<std::string, HostComponentErrorInfoT>> components_error_handling_table_ {};
    std::vector<std::unordered_map<std::string, ErrorTrackingInfoT>> components_error_tracking_table_ {};

    bool synchronization_ {};
    //std::vector<bool> first_time_check_ {};
    
    std::vector<uint32_t> last_frame_count_ {};
    std::vector<uint32_t> current_frame_count_ {};
    std::vector<TimeInfoT> last_input_timestamp_ {};
    std::vector<TimeInfoT> current_input_timestamp_ {};
};

#endif // INCLUDE_PFSP_CORE_ERROR_HANDLER__H_

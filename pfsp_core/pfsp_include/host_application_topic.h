#ifndef INCLUDE_PFSP_HOST_APPLICATION_TOPIC_H_
#define INCLUDE_PFSP_HOST_APPLICATION_TOPIC_H_

#include "pfsp_typedef.h"
#include "../pfsp_host_component_base/pfsp_host_component_base.h"

#define HOST_APPLICATION_LIB_NAME "HostAppLibName"
#define BUFFER_SIZE (20)

// =======================================  ErrorHandler  ==================================== //

class ErrorHandlerReactionReportT
{
public:
    ErrorHandlerReactionReportT() = default;
    virtual ~ErrorHandlerReactionReportT() = default;

    std::vector<HostComponentModeManagementInfoT> next_reaction;
    //HostComponentModeManagementInfoT next_reaction[static_cast<uint32_t>(MAX_NUMBER_OF_COMPONENTS)];
};

// =======================================  ModeManager  ==================================== //
class ModeManagerComponentsReactionT
{
public:
    ModeManagerComponentsReactionT() = default;
    virtual ~ModeManagerComponentsReactionT() = default;

    std::vector<HostComponentModeManagementInfoT> component;
    //HostComponentModeManagementInfoT component[static_cast<uint32_t>(MAX_NUMBER_OF_COMPONENTS)];
};

// =============================== Common - For Every Host Components =========================== //
class HostComponentCommonTopicPtrT
{
public:
    HostComponentCommonTopicPtrT() = default;
    virtual ~HostComponentCommonTopicPtrT() = default;

    /* Default Publication Topics */
    pfsp_topics::HostComponentAllErrorsInfoT *all_errors_info_;
    pfsp_topics::HostComponentAllModeInfoT *all_mode_info_;
    HostComponentErrorReportT *error_report_;
    HostComponentModeStateReportT *mode_state_report_;
};

#endif /* INCLUDE_PFSP_HOST_APPLICATION_TOPIC_H_ */

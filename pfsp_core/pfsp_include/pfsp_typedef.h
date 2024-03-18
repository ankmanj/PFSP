#ifndef INCLUDE_PFSP_TYPEDEF_H_
#define INCLUDE_PFSP_TYPEDEF_H_

#include <sys/time.h>
#include "pfsp_enum.h"

typedef struct TimeInfo
{
	int64_t usec;
	int64_t sec;
} TimeInfoT;

typedef struct HostComponentErrorReport
{
    char_t error_name[100];
    uint32_t component_id;
    uint32_t frame_count;
    TimeInfoT frame_time_stamp;
    SWComponentErrorCategoryE error_type;
}HostComponentErrorReportT;

typedef struct ErrorTrackingInfo
{
    TimeInfoT first_error_timestamp;
    TimeInfoT last_error_timestamp;
    uint32_t frame_count;
    uint32_t count_to_trigger_failure;
    bool error_consecutiveness;
    SWComponentErrorCategoryE error_type;
} ErrorTrackingInfoT;

typedef struct HostComponentErrorInfo
{
    std::string error_name;
    ToggleSwitchE status_of_usage;
    FailureDebouncingCriteriaE debouncing_criteria;
    uint32_t threshold_to_trigger_failure;
    uint32_t threshold_to_reset_failure;
    SWComponentOperationModeE required_reaction_mode;
    SWComponentStateE required_reaction_state;

    //ErrorTrackingInfoT tracking_info;
}HostComponentErrorInfoT;

typedef struct HostComponentModeManagementInfo
{
    SWComponentStateE state;
    SWComponentOperationModeE operation_mode;
    NextStateE keep_current_state_mode;
    DataPublicationStateE publication_state;
}HostComponentModeManagementInfoT;

typedef struct HostComponentModeInfo
{
    SWComponentOperationModeE operation_mode;
    ToggleSwitchE status_of_usage;
}HostComponentModeInfoT;

typedef struct ComponentStatus
{
    uint32_t component;
    ToggleSwitchE status;
}ComponentStatusT;


class HostComponentModeStateReportT
{
public:
    HostComponentModeStateReportT() = default;
    virtual ~HostComponentModeStateReportT() = default;

    uint32_t component_id;
    uint32_t frame_count;
    PlayTypeE play_type;
    HostComponentModeManagementInfoT current_info;
};


class ComponentFailureInfoT
{
public:
    ComponentFailureInfoT() = default;
    virtual ~ComponentFailureInfoT() = default;

    bool failed {};
    std::string error_name {};
    FailureDebouncingCriteriaE debouncing_criteria;
    uint32_t threshold_to_reset_failure;
};

class ComponentsStatusInfo
{
public:
    ComponentsStatusInfo() = default;
    virtual ~ComponentsStatusInfo() = default;

    std::vector<ComponentFailureInfoT> status;
};


namespace pfsp_topics
{
    class HostComponentAllErrorsInfoT
    {
    public:
        HostComponentAllErrorsInfoT() = default;
        virtual ~HostComponentAllErrorsInfoT() = default;

        uint32_t component_id;
        std::map<std::string, HostComponentErrorInfoT> *errors_info {};
        //std::map<std::string, std::vector<HostComponentErrorInfoT>> *errors_info {};  // jhlee use vector
                    //std::vector<HostComponentErrorInfoT> errors_info;
    };

    class NormalStateTrackingInfoT
    {
    public:
        NormalStateTrackingInfoT() = default;
        virtual ~NormalStateTrackingInfoT() = default;

        bool normal_consecutiveness{};
        TimeInfoT first_normal_time {};
        TimeInfoT last_normal_time {};
        uint32_t count_to_reset_failure {};
        uint32_t frame_count {};
    };

    class ComponentBasicInfo
    {
    public:
        ComponentBasicInfo() = default;
        virtual ~ComponentBasicInfo() = default;

        uint32_t component_id {};
        PlayTypeE play_type {};
        uint32_t thread_frequency {};
    };
    
    class ComponentFrameInfo
    {
    public:
        ComponentFrameInfo() = default;
        virtual ~ComponentFrameInfo() = default;

        uint32_t frame_count {};
        TimeInfoT frame_time_stamp {};
    };

    class HostComponentAllModeInfoT
    {
    public:
        HostComponentAllModeInfoT() = default;
        virtual ~HostComponentAllModeInfoT() = default;

        uint32_t component_id {};
        std::map<std::string, SWComponentOperationModeE> *modes_info {};
        //std::vector<HostComponentModeInfoT> *modes_info {};
    };

};

inline int64_t GetElapseTimeByReference(TimeInfoT &current_time, TimeInfoT &last_time)
{
    int64_t elapse_sec = current_time.sec - last_time.sec;
    int64_t elapse_usec = current_time.usec - last_time.usec;
    int64_t elapse_millisec = (elapse_sec*1000 + elapse_usec/1000);

    return elapse_millisec;
}

inline int64_t GetElapseTimeByPointer(TimeInfoT *current_time, TimeInfoT *last_time)
{
    int64_t elapse_sec = current_time->sec - last_time->sec;
    int64_t elapse_usec = current_time->usec - last_time->usec;
    int64_t elapse_millisec = (elapse_sec*1000 + elapse_usec/1000);

    return elapse_millisec;
}

inline void GetFrameTimeStamp(pfsp_topics::ComponentFrameInfo &frame_info)
{
    struct timeval time{};
        
    gettimeofday(&time, NULL);

    frame_info.frame_time_stamp.usec = time.tv_usec;
    frame_info.frame_time_stamp.sec = time.tv_sec;
}
#endif /* INCLUDE_PFSP_TYPEDEF_H_ */

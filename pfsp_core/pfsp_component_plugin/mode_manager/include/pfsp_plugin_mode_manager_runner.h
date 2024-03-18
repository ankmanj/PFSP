#ifndef INCLUDE_PFSP_PLUGIN_MODE_MANAGER_RUNNER_H_
#define INCLUDE_PFSP_PLUGIN_MODE_MANAGER_RUNNER_H_

#include "osac/CMutex.h"
#include "service/AService.h"
#include "worker/CWorker.h"

#include "pfsp_plugin_mode_manager_message.h"

class PfspPluginModeManagerRunner : public IWork
{
public:
    PfspPluginModeManagerRunner(std::shared_ptr<PfspPluginModeManagerMessage> publication_message, std::shared_ptr<PfspCoreModeManager> core_mode_manager, ModeManagerComponentsReactionT *publication_data, OSAC::CMutex *access_sync);
    virtual ~PfspPluginModeManagerRunner(void);

    virtual void OnInitialize(void){};
    virtual void OnProcess(const char_t *pvBuffer, const int32_t nLen);
    virtual void OnFinalize(void);
    virtual void RequestFinalize(void);

    void SetParametersForRunner(uint32_t component_id, PlayTypeE play_type, uint32_t thread_frequency);
    void SetComponentPubSubStatus(ComponentPubSubStatusE pubsub_status);

private:
    bool flagFinalized{false};

    ComponentPubSubStatusE pubsub_status_ = ComponentPubSubStatusE::ePubSubNone;
    
    uint32_t component_id_{};
    PlayTypeE play_type_{};
    uint32_t thread_frequency_{};

    std::shared_ptr<PfspCoreModeManager> core_mode_manager_ {};
    std::shared_ptr<PfspPluginModeManagerMessage> service_publication_message_ {};
	ModeManagerComponentsReactionT *components_reaction_output_ {};
    OSAC::CMutex *access_sync_;
};

#endif /* INCLUDE_PFSP_PLUGIN_MODE_MANAGER_RUNNER_H_ */
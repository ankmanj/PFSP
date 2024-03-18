#include "osac/CFile.h"
#include "servicemessage/CTopicMessage.h"
#include "pfsp_plugin_mode_manager_runner.h"

PfspPluginModeManagerRunner::PfspPluginModeManagerRunner(std::shared_ptr<PfspPluginModeManagerMessage> publication_message, 
                                                            std::shared_ptr<PfspCoreModeManager> core_mode_manager, 
                                                            ModeManagerComponentsReactionT *publication_data,
                                                            OSAC::CMutex *access_sync)
{
    service_publication_message_ = publication_message;
    core_mode_manager_ = core_mode_manager;
    components_reaction_output_ = publication_data;
    access_sync_ = access_sync;
    flagFinalized = false;
}

PfspPluginModeManagerRunner::~PfspPluginModeManagerRunner(void)
{
}

void PfspPluginModeManagerRunner::RequestFinalize(void)
{
    std::cout << "==> [1]. PfspPluginModeManagerRunner::RequestFinalize" << std::endl;
    flagFinalized = true;
}

void PfspPluginModeManagerRunner::OnProcess(const char_t *pvBuffer, const int32_t nLen)
{
    if (flagFinalized == false)
    {
        //uint32_t temp_frequency_1 = thread_frequency_/2;
        //TASK_Delay(temp_frequency_1, 0);

        access_sync_->Lock();
        //bool synchronized = core_mode_manager_->SyncUpOutputOfComponentsReaction();
        //if ( true == synchronized )
        {
            service_publication_message_->PublishComponentsReaction();
            pubsub_status_ = ComponentPubSubStatusE::ePublishedExternalOutput;
        }
        access_sync_->Unlock();

        uint32_t temp_frequency_2 = thread_frequency_;
        TASK_Delay(temp_frequency_2, 0);
    }
}

void PfspPluginModeManagerRunner::OnFinalize(void)
{
    std::cout << "==> [PfspPluginModeManagerRunner]. PfspPluginModeManagerRunner::OnFinalize" << std::endl;
    //service_publication_message_.reset();
}

void PfspPluginModeManagerRunner::SetParametersForRunner(uint32_t component_id, PlayTypeE play_type, uint32_t thread_frequency)
{
    component_id_ = component_id;
    play_type_ = play_type;
    thread_frequency_ = thread_frequency;
}

void PfspPluginModeManagerRunner::SetComponentPubSubStatus(ComponentPubSubStatusE pubsub_status)
{
    pubsub_status_ = pubsub_status;
}
#include "pfsp_plugin_mode_manager_message.h"

PfspPluginModeManagerMessage::PfspPluginModeManagerMessage(const char_t *pszSenderName, const char_t *pszReceiverName ) 
                                : CServiceMessage(pszSenderName, pszReceiverName, sizeof(PfspPluginModeManagerMessage))
{
    InitPublicationTopics();
}

PfspPluginModeManagerMessage::~PfspPluginModeManagerMessage(void)
{
    TerminatePublicationTopics();
}

PfspPluginModeManagerMessage::PfspPluginModeManagerMessage(AServiceMessage *poMessage) : CServiceMessage(poMessage)
{
    InitPublicationTopics();
}

ModeManagerComponentsReactionT *PfspPluginModeManagerMessage::GetPointerOfComponentsReaction(void)
{
    components_reaction_data_ = components_reaction_topic_->GetContent();
    return components_reaction_data_;
}

void PfspPluginModeManagerMessage::PublishComponentsReaction()
{
    components_reaction_topic_->Publish("ComponentsReactionTopic");
}

void PfspPluginModeManagerMessage::InitPublicationTopics(void)
{
    components_reaction_topic_ = std::make_shared<CTopicMessage<ModeManagerComponentsReactionT>>();
    components_reaction_data_ = components_reaction_topic_->GetContent();
}

void PfspPluginModeManagerMessage::TerminatePublicationTopics(void)
{
    if ( 0 < components_reaction_topic_.use_count() )
    {
        components_reaction_topic_.reset();
    }
}
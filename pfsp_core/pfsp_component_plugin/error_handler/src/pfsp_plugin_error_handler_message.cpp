#include "pfsp_plugin_error_handler_message.h"

PfspPluginErrorHandlerMessage::PfspPluginErrorHandlerMessage(const char_t *pszSenderName, const char_t *pszReceiverName ) 
                                    : CServiceMessage(pszSenderName, pszReceiverName, sizeof(PfspPluginErrorHandlerMessage))
{
    InitPublicationTopics();
}

PfspPluginErrorHandlerMessage::~PfspPluginErrorHandlerMessage(void)
{
    TerminatePublicationTopics();
}

PfspPluginErrorHandlerMessage::PfspPluginErrorHandlerMessage(AServiceMessage *poMessage) : CServiceMessage(poMessage)
{
    InitPublicationTopics();
}
 
ErrorHandlerReactionReportT *PfspPluginErrorHandlerMessage::GetPointerOfErrorHandlerReactionReportT(void)
{
    reaction_report_data_ = reaction_report_topic_->GetContent();
    return reaction_report_data_;
}

void PfspPluginErrorHandlerMessage::PublishErrorHandlerReactionReportT()
{
    reaction_report_topic_->Publish("ReactionReportTopic");
}

void PfspPluginErrorHandlerMessage::InitPublicationTopics(void)
{
    reaction_report_topic_ = std::make_shared<CTopicMessage<ErrorHandlerReactionReportT>>();
    reaction_report_data_ = reaction_report_topic_->GetContent();
}

void PfspPluginErrorHandlerMessage::TerminatePublicationTopics(void)
{
    if ( 0 < reaction_report_topic_.use_count() )
    {
        reaction_report_topic_.reset();
    }
}
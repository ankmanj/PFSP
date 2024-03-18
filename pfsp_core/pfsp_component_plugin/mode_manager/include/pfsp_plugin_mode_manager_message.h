#ifndef INCLUDE_PFSP_PLUGIN_MODE_MANAGER_MESSAGE_H_
#define INCLUDE_PFSP_PLUGIN_MODE_MANAGER_MESSAGE_H_

#include <memory>
#include "service/AService.h"
#include "servicemessage/AServiceMessage.h"
#include "servicemessage/CTopicMessage.h"
#include "servicemessage/CTimeServiceMessage.h"

#include "host_application_topic.h"
#include "pfsp_core_mode_manager.h"

class PfspCoreModeManager;

class PfspPluginModeManagerMessage : public CServiceMessage
{
public:
	PfspPluginModeManagerMessage(const char_t *pszSenderName, const char_t *pszReceiverName);
	explicit PfspPluginModeManagerMessage(AServiceMessage *poMessage);
	virtual ~PfspPluginModeManagerMessage(void);

	ModeManagerComponentsReactionT *GetPointerOfComponentsReaction(void);
	void PublishComponentsReaction();

private:

	void InitPublicationTopics(void);
	void TerminatePublicationTopics(void);

	std::shared_ptr<CTopicMessage<ModeManagerComponentsReactionT>> components_reaction_topic_ {};
	ModeManagerComponentsReactionT *components_reaction_data_;
};

#endif /* INCLUDE_PFSP_PLUGIN_MODE_MANAGER_MESSAGE_H_ */

#ifndef INCLUDE_PFSP_PLUGIN_ERROR_HANDLER_MESSAGE_H_
#define INCLUDE_PFSP_PLUGIN_ERROR_HANDLER_MESSAGE_H_
#include <memory>
#include "service/AService.h"
#include "servicemessage/AServiceMessage.h"
#include "servicemessage/CTopicMessage.h"
#include "servicemessage/CTimeServiceMessage.h"

#include "host_application_topic.h"
#include "pfsp_core_error_handler.h"

class PfspCoreErrorHandler;

class PfspPluginErrorHandlerMessage : public CServiceMessage
{
public:
	PfspPluginErrorHandlerMessage(const char_t *pszSenderName, const char_t *pszReceiverName);
	explicit PfspPluginErrorHandlerMessage(AServiceMessage *poMessage);
	virtual ~PfspPluginErrorHandlerMessage(void);

	ErrorHandlerReactionReportT *GetPointerOfErrorHandlerReactionReportT(void);
	void PublishErrorHandlerReactionReportT(void);

private:

	void InitPublicationTopics(void);
	void TerminatePublicationTopics(void);

	std::shared_ptr<CTopicMessage<ErrorHandlerReactionReportT>> reaction_report_topic_ {};
	ErrorHandlerReactionReportT *reaction_report_data_ {};
};

#endif /* INCLUDE_PFSP_PLUGIN_ERROR_HANDLER_MESSAGE_H_ */

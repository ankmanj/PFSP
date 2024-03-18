#ifndef INCLUDE_PFSP_PLUGIN_ERROR_HANDLER_H_
#define INCLUDE_PFSP_PLUGIN_ERROR_HANDLER_H_

#include "osac/CThread.h"
#include "osac/CMessageQueue.h"
#include "osac/CMutex.h"
#include "interface/INotify.h"
#include "service/AService.h"
#include "worker/CWorker.h"

#include "pfsp_plugin_error_handler_runner.h"

class PfspPluginErrorHandlerMessage;

class PfspPluginErrorHandler : public AService, public INotify
{
    DECLARE_SERVICE_CLASS(PfspPluginErrorHandler)
public:
    PfspPluginErrorHandler(void);
    virtual ~PfspPluginErrorHandler(void){};

    virtual int16_t MwCompInitialize(std::string component_name, uint32_t component_id, uint32_t number_of_components);
    virtual int16_t MwCompStart(void);
    virtual int16_t MwCompStop(void);
    virtual int16_t MwCompFinalize(void);
    virtual int16_t MwCompMessage(AServiceMessage* poMessage);
	//virtual int16_t OnReceive(AServiceMessage* poMessage);
    virtual int16_t MwCompCommand(int32_t nArgc, char_t *pszArgv[]);
    virtual void MwCompNotify(void *pvArg);

private:
    PlayTypeE play_type_ {};
    uint32_t thread_frequency_ {};

    uint32_t runner_priority_ {};
    uint32_t component_id_ {};
    uint32_t real_number_of_components_ {};

	FunctionResult GetPointerOfPublicationTopics(void);
    void RegisterToScribeTopics(void);
    void ReadParametersForComponentRunner(const char_t *play_type, const char_t *thread_frequency);

    std::shared_ptr<CWorker> runner_class_ {};
    std::shared_ptr<PfspPluginErrorHandlerMessage> service_publication_message_ {};
    std::shared_ptr<PfspPluginErrorHandlerRunner> error_handler_runner_class_ {};
    std::shared_ptr<PfspCoreErrorHandler> core_error_handler_ {};

    ErrorHandlerReactionReportT *reaction_report_output_ {};
    
    OSAC::CMutex access_sync_;
};

#endif /* INCLUDE_PFSP_PLUGIN_ERROR_HANDLER_H_ */

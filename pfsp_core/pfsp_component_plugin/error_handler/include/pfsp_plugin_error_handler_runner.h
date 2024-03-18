
#ifndef INCLUDE_PFSP_PLUGIN_ERROR_HANDLER_RUNNER_H_
#define INCLUDE_PFSP_PLUGIN_ERROR_HANDLER_RUNNER_H_

#include "osac/CMutex.h"
#include "service/AService.h"
#include "worker/CWorker.h"

#include "pfsp_plugin_error_handler_message.h"

class PfspPluginErrorHandlerRunner : public IWork
{
public:
    PfspPluginErrorHandlerRunner(std::shared_ptr<PfspPluginErrorHandlerMessage> publication_message, std::shared_ptr<PfspCoreErrorHandler> core_error_handler, ErrorHandlerReactionReportT *publication_data, OSAC::CMutex *access_sync);
    virtual ~PfspPluginErrorHandlerRunner(void);

    virtual void OnInitialize(void){};
    virtual void OnProcess(const char_t *pvBuffer, const int32_t nLen);
    virtual void OnFinalize(void);
    virtual void RequestFinalize(void);

    void SetParametersForRunner(uint32_t real_number_of_components, PlayTypeE play_type, uint32_t thread_frequency);
private:
    bool flagFinalized{false};
    uint32_t real_number_of_components_{};
    PlayTypeE play_type_{};
    uint32_t thread_frequency_{};
    
    std::shared_ptr<PfspCoreErrorHandler> core_error_handler_ {};
    std::shared_ptr<PfspPluginErrorHandlerMessage> service_publication_message_ {};
    ErrorHandlerReactionReportT *reaction_report_output_ {};
    OSAC::CMutex *access_sync_;
};

#endif /* INCLUDE_PFSP_PLUGIN_ERROR_HANDLER_RUNNER_H_ */
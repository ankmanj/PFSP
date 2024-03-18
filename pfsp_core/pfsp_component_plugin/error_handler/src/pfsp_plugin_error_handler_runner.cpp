#include "osac/CFile.h"
#include "servicemessage/CTopicMessage.h"
#include "pfsp_plugin_error_handler_runner.h"

PfspPluginErrorHandlerRunner::PfspPluginErrorHandlerRunner(std::shared_ptr<PfspPluginErrorHandlerMessage> publication_message, 
                                                            std::shared_ptr<PfspCoreErrorHandler> core_error_handler, 
                                                            ErrorHandlerReactionReportT *publication_data,
                                                            OSAC::CMutex *access_sync)
{
    service_publication_message_ = publication_message;
    core_error_handler_ = core_error_handler;
    reaction_report_output_ = publication_data;
    access_sync_ = access_sync;
    flagFinalized = false;
}

PfspPluginErrorHandlerRunner::~PfspPluginErrorHandlerRunner(void)
{
}

void PfspPluginErrorHandlerRunner::RequestFinalize(void)
{
    std::cout << "==> [PfspPluginErrorHandlerRunner]. PfspPluginErrorHandlerRunner::RequestFinalize" << std::endl;
    flagFinalized = true;
}

void PfspPluginErrorHandlerRunner::OnProcess(const char_t *pvBuffer, const int32_t nLen)
{
    if (flagFinalized == false)
    {
        //uint32_t temp_frequency_1 = thread_frequency_/2;
        //TASK_Delay(temp_frequency_1, 0);

        access_sync_->Lock();
        //bool synchronized = core_error_handler_->SyncUpOutputOfComponentsReaction();
        //if ( true == synchronized )
        {
            //std::cout << "  ==================  OnProcess    Publish     OnProcess    Publish \n";
            service_publication_message_->PublishErrorHandlerReactionReportT();
        }
        access_sync_->Unlock();
        
        uint32_t temp_frequency_2 = thread_frequency_;
        TASK_Delay(temp_frequency_2, 0);
    }
}

void PfspPluginErrorHandlerRunner::OnFinalize(void)
{
    std::cout << "==> [PfspPluginErrorHandlerRunner]. PfspPluginErrorHandlerRunner::OnFinalize" << std::endl;
    //service_publication_message_.reset();
}

void PfspPluginErrorHandlerRunner::SetParametersForRunner(uint32_t real_number_of_components, PlayTypeE play_type, uint32_t thread_frequency)
{
    real_number_of_components_ = real_number_of_components;
    play_type_ = play_type;
    thread_frequency_ = thread_frequency;    
}

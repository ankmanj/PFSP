#pragma once

#include <string>
#include <cstddef>
#include <math.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <dlfcn.h>
#include <memory>

#include "osac/CMessageQueue.h"
#include "servicemessage/AServiceMessage.h"
#include "servicemessage/CTopicMessage.h"

#include "host_application_topic.h"
#include "smart_converter_core_base.h"

#define FEATURE_USE_SMART_PTR

template<typename TExternalOutput>
class CHostApplicationMessage : public CServiceMessage
{
public:
	CHostApplicationMessage(const char_t *pszSenderName, const char_t *pszReceiverName, std::string component_name) \
                            :   component_name_{component_name}, \
                                CServiceMessage(pszSenderName, pszReceiverName, sizeof(CHostApplicationMessage<TExternalOutput>))
    {
        InitPublicationTopics();
    }
	
    explicit CHostApplicationMessage(AServiceMessage *poMessage) : CServiceMessage(poMessage)
    {
        InitPublicationTopics();
    }

	virtual ~CHostApplicationMessage(void)
    {
        TerminatePublicationTopics();
    }

	/* Those are default functions for publication topic */
    pfsp_topics::HostComponentAllErrorsInfoT *GetPointerOfAllErrorsInfoT(void)
    {
        all_errors_info_data_ = all_errors_info_topic_->GetContent();
        return all_errors_info_data_;
    }

	void PublishAllErrorsInfoT(void)
    {
        std::string temp = component_name_ + "AllErrorsInfoTopic";
        all_errors_info_topic_->Publish(temp.c_str());
    }

    pfsp_topics::HostComponentAllModeInfoT *GetPointerOfAllModeInfoT(void)
    {
        all_mode_info_data_ = all_mode_info_topic_->GetContent();
        return all_mode_info_data_;
    }

	void PublishAllModeInfoT(void)
    {
        std::string temp = component_name_ + "AllModeInfoTopic";
        all_mode_info_topic_->Publish(temp.c_str());
    }

    HostComponentErrorReportT *GetPointerOfErrorReportT(void)
    {
        error_report_data_ = error_report_topic_->GetContent();
        return error_report_data_;
    }

	void PublishErrorReportT(void)
    {
        std::string temp = component_name_ + "ErrorReportTopic";
        error_report_topic_->Publish(temp.c_str());
    }
	
    HostComponentModeStateReportT *GetPointerOfModeStateReportT(void)
    {
        mode_state_report_data_ = mode_state_report_topic_->GetContent();
        return mode_state_report_data_;
    }

	void PublishModeStateReportT(void)
    {
        std::string temp = component_name_ + "ModeStateReportTopic";
        mode_state_report_topic_->Publish(temp.c_str());
    }

    /* Can be removed that code below ~ */
	/* Those are pair for one publication topic : Optional */
	TExternalOutput *GetPointerOfOutputData(void)
    {
        output_data_ = output_topic_->GetContent();
        return output_data_;
    }

	void PublishComponentOutputData(void)
    {
        std::string temp = component_name_ + "OutputTopic";
        output_topic_->Publish(temp.c_str());
    }
    /* ~ Can be removed that code above */

private:

	void InitPublicationTopics(void)
    {
        all_errors_info_topic_ = std::make_shared<CTopicMessage<pfsp_topics::HostComponentAllErrorsInfoT>>();
        all_errors_info_data_ = all_errors_info_topic_->GetContent();

        all_mode_info_topic_ = std::make_shared<CTopicMessage<pfsp_topics::HostComponentAllModeInfoT>>();
        all_mode_info_data_ = all_mode_info_topic_->GetContent();

        error_report_topic_ = std::make_shared<CTopicMessage<HostComponentErrorReportT>>();
        error_report_data_ = error_report_topic_->GetContent();

        mode_state_report_topic_ = std::make_shared<CTopicMessage<HostComponentModeStateReportT>>();
        mode_state_report_data_ = mode_state_report_topic_->GetContent();

        /* Can be removed that code below ~ */
        output_topic_ = std::make_shared<CTopicMessage<TExternalOutput>>();
        output_data_ = output_topic_->GetContent();
        /* ~ Can be removed that code above */
    }

	void TerminatePublicationTopics(void)
    {
        all_errors_info_topic_.reset();
        all_mode_info_topic_.reset();
        error_report_topic_.reset();
        mode_state_report_topic_.reset();
    
        /* Can be removed that code below */
        output_topic_.reset();
    }

	/* Those are default parameters for publication topic */
	std::shared_ptr<CTopicMessage<pfsp_topics::HostComponentAllErrorsInfoT>> all_errors_info_topic_ {};
	pfsp_topics::HostComponentAllErrorsInfoT *all_errors_info_data_ {};

	std::shared_ptr<CTopicMessage<pfsp_topics::HostComponentAllModeInfoT>> all_mode_info_topic_ {};
	pfsp_topics::HostComponentAllModeInfoT *all_mode_info_data_ {};

	std::shared_ptr<CTopicMessage<HostComponentErrorReportT>> error_report_topic_ {};
	HostComponentErrorReportT *error_report_data_ {};
	
	std::shared_ptr<CTopicMessage<HostComponentModeStateReportT>> mode_state_report_topic_ {};
	HostComponentModeStateReportT *mode_state_report_data_ {};

	/* Those are pair for one publication topic */
	std::shared_ptr<CTopicMessage<TExternalOutput>> output_topic_ {};
	TExternalOutput *output_data_ {};

    std::string component_name_ {};
};


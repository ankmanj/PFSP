#pragma once

#include "service/AService.h"

class CTestServiceMessage;

class CTestService : public AService
{
    DECLARE_SERVICE_CLASS(CTestService)

public:
	CTestService(void) {}
	virtual ~CTestService(void) {}

	virtual int16_t MwCompInitialize(std::string component_name, uint32_t component_id, uint32_t number_of_components);
	virtual int16_t MwCompStart(void);
	virtual int16_t MwCompStop(void);
	virtual int16_t OnReset(void);
	virtual int16_t MwCompFinalize(void);
	virtual int16_t MwCompMessage(AServiceMessage* poMessage);
	virtual int16_t MwCompCommand(int32_t nArgCount, char_t** nArgValue);

protected:
	CTestServiceMessage* m_poPublisher;
	CTestServiceMessage* m_poTestMessage;
};


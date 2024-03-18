#pragma once

#include "servicemessage/AServiceMessage.h"

struct TestBigUserData_t
{
	uint32_t unParam1;
	uint32_t unParam2;
	char_t   arData[1024*1024];
};

class CTestServiceBigMessage : public AServiceMessage
{
public:
	CTestServiceBigMessage(const char* pszSenderName, const char* pszReceiverName);
	virtual ~CTestServiceBigMessage(void){};

	int32_t SendTestBigMsg(int32_t nServiceID);

private:
	TestBigUserData_t* m_psTestBigUserData;
};

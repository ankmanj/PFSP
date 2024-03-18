#pragma once

#include "servicemessage/AServiceMessage.h"

typedef struct
{
	int32_t nType;
	int32_t nParam1;
	int32_t nParam2;
	int32_t nParam3;
} TestTopic_t;

struct TestUserData_t
{
	uint32_t unParam1;
	uint32_t unParam2;
};

enum ServiceID_t
{
	eTESTSERVICE_EVENT			= 0x0000, // Self service
	eTESTSERVICE_TEST1			= 0x0001,
	eTESTSERVICE_TEST2			= 0x0002,
	eTESTSERVICE_TEST3			= 0x0003,
	eTESTSERVICE_TEST_BIG		= 0x0004,
	eTESTSERVICE_TEST_TOPIC		= 0x0005,
	eTESTSERVICE_TESTMICOM_TOPIC = 0x0006,
	eTESTSERVICE_TIMER_MICOM_3  = 0x0007,
	eTESTSERVICE_TIMER_MICOM_5  = 0x0008,
	eTESTSERVICE_TIMER_MICOM_10 = 0x0009,
	eTESTSERVICE_TIMER_VISION_3 = 0x0000A,
	eTESTSERVICE_TIMER_VISION_5 = 0x000B,
	eTESTSERVICE_TIMER_VISION_10 = 0x000C,
	eTESTSERVICE_TIMER_PC		= 0x000D,
	eTESTSERVICE_TIMER_START	= 0x000E,
	eTESTSERVICE_TIMER_STOP		= 0x000F,
	eTESTSERVICE_TIMER_RESET	= 0x0010,
	eTESTSERVICE_SYNCCALL		= 0x0011,
	eTESTSERVICE_TIMER_FINALIZE	= 0x0012,

	eTESTSERVICE_TEST_BOARD_TOPIC = 0x0013
};

class CTestServiceMessage : public AServiceMessage
{
public:
	CTestServiceMessage(const char* pszSenderName, const char* pszReceiverName);
	virtual ~CTestServiceMessage(void){};

	int32_t SendTest1Msg(int32_t nServiceID);
	int32_t SendTest2Msg(int32_t nServiceID);
	int32_t SendTest3Msg(int32_t nServiceID);

	int32_t SendSyncCall(int32_t nParam, int32_t nTimeout);

	int32_t PublishTestTopic(uint32_t nParam1);
	int32_t PublishTestMicomTopic(uint32_t nParam1);

private:
	TestUserData_t* m_psTestUserData;
};

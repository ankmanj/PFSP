#ifndef CTIMESERVICEMESSAGE_H_
#define CTIMESERVICEMESSAGE_H_

#include <string>

#include "base/CTypedefs.h"
#include "AServiceMessage.h"

enum MessageTimeService_t
{
	eTIMESERVICE_REQUEST_INITIALIZE_MSG = eTIMESERVICE_OFFSET,
	eTIMESERVICE_REQUEST_FINALIZE_MSG,
	eTIMESERVICE_TIME_MSG,
	eTIMESERVICE_SERVICE_READY,
	eTIMESERVICE_TIMER_TICK,
	eTIMESERVICE_TIMER_PUBLISH,
	eTIMESERVICE_TIMER_RESET,
	eTIMESERVICE_TIMER_PAUSE,
	eTIMESERVICE_SET_DATETIME
};

class CTimeServiceMessage : public AServiceMessage
{
public:
	static const uint32_t CTIMESERVICEMESSAGE_CYCLICCOUNTS_INFINITE = static_cast<uint32_t>(-1);
	static const uint32_t CTIMESERVICEMESSAGE_CYCLICCOUNTS_ONCE = static_cast<uint32_t>(1);

public:
	CTimeServiceMessage(const char_t* pszSenderName, const char_t* pszReceiverName = "/time");
	virtual ~CTimeServiceMessage() {}

	void SetTimer(const char_t* pszServiceName, uint32_t unTicksPeriod, uint32_t nCyclicCount, uint32_t nReturnMessageId);

	void Start(void);

	void Stop(void);

	void Reset(void);

	void Pause(void);	

	void SetDateTime(uint16_t unYear, uint16_t unMonth, uint16_t unDay, uint16_t unHour, uint16_t unMin, uint16_t unSec);

	const char_t* GetDateTime(void);

	void UpdateTimerTick(void);

	void PublishTimer(uint32_t nPublishServiceId, uint32_t nReturnMessageId);

protected:

private:	
	std::string m_strServiceName;
	uint32_t m_unTicksPeriod;
	int32_t m_nCyclicCount;
	int32_t m_nReturnMessageId;
};

#endif	/* CTIMESERVICEMESSAGE_H_ */
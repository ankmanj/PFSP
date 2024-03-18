#ifndef CTIMESERVICETIMER_H_
#define CTIMESERVICETIMER_H_

#include <list>

#include "base/CTypedefs.h"
#include "servicemessage/CTimeServiceMessage.h"

class CServiceTimerSlot {
public:
	CServiceTimerSlot(int32_t nServiceId, int32_t nPeriod, int32_t nCyclicCount, int32_t nParameter)
		: m_nServiceId(nServiceId), m_nTicks(nPeriod) , m_nPeriod(nPeriod), m_nCyclicCount(nCyclicCount), m_nParameter(nParameter)
	{	
	}
	
	~CServiceTimerSlot(void)
	{		
	}

	int32_t m_nServiceId;
	int32_t m_nTicks;
	int32_t m_nPeriod;
	int32_t m_nCyclicCount;
	int32_t m_nParameter;
};

class CTimeServiceTimer {
public:
	CTimeServiceTimer(void) {}
	~CTimeServiceTimer(void) {
		RemoveAllService();
	}

	void InsertService(CServiceTimerSlot* poNewTimerSlot);

	CServiceTimerSlot* ExtractService(int32_t nServiceId, int32_t nReturnId);
		
	void AddService(int32_t nServiceId, int32_t nPeriod, int32_t nCyclicCount, int32_t nReturnId);

	int32_t ResetService(int32_t nServiceId, int32_t nPeriod, int32_t nCyclicCount, int32_t nReturnId);

	int32_t PauseService(int32_t nServiceId, int32_t nPeriod, int32_t nCyclicCount, int32_t nReturnId);
		
	void UpdateService(void);

	void RemoveService(int32_t nServiceId, int32_t nReturnId);
	
	void RemoveAllService(void);

	CServiceTimerSlot* FindService(int32_t nServiceId, int32_t nReturnId);
	
	CServiceTimerSlot* FindPausedService(int32_t nServiceId, int32_t nReturnId);
	
	void InsertPausedService(CServiceTimerSlot* poServiceTimerSlot);

private:

	std::list<CServiceTimerSlot*> m_oTimerServiceList;
	std::list<CServiceTimerSlot*> m_oTimerPausedServiceList;
};

#endif	/* CTIMESERVICETIMER_H_ */
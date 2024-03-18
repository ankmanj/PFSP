#ifndef CTIMER_H_
#define CTIMER_H_

#include "osal/Timer.h"
#include "osal/Task.h"

namespace OSAC {

class CTimer {
public:
	static const int32_t CTIMR_ONESHOT_FLAG = 0;
	static const int32_t CTIMR_CYCLIC_FLAG = 1;

	CTimer(void);
	virtual ~CTimer(void);

	int32_t Create(const char_t* pcName, void* pvArgs, int32_t nPeriodMs, int32_t nCycleFlag = CTIMR_CYCLIC_FLAG);

	int32_t Close(void);
	int32_t Destroy(void);

	int32_t Start(void);
	int32_t Stop(void);

	static TASK_RETURN TimerHandler(TASK_ARGS);

protected:
	virtual int32_t OnTime(void* pvArgs) = 0;

private:
	class CTimerArgs {
	public:
		CTimerArgs(CTimer* pcTimer, void* pvArgs) : m_pcTimer(pcTimer), m_pvArgs(pvArgs) {}
		~CTimerArgs(void) {};

		CTimer* m_pcTimer;
		void*	m_pvArgs;
	private:
		CTimerArgs(void);
	};

	CTimerArgs* m_pvTimerArgs;

	TIMR_T m_cContent;
	char_t*  m_pcName;
};

}

#endif	/* CTIMER_H_ */
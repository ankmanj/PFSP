#ifndef TIMER_H_
#define TIMER_H_

#include "osal/port/Port_Timer.h"
#include "osal/Task.h"
#include "osal/Mutex.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct 
{
		int32_t				nPeriodMs;
		int32_t             nCyclicFlag;
		int32_t				nDestroyFlag;
	TIMER_CALLBACK_TYPE pFncCallback;
#if defined(SYS_BIOS6)
	void*				pvArgs;
#else
	TASK_ARGS			pvArgs;
#endif
	TASK_T				cTimerTask;
		int32_t				nIsStop;
	MUTX_T				cMutex;
} timr_t;

typedef timr_t* TIMR_T;

#if defined(SYS_BIOS6)
	TIMR_T TIMR_Create(const char_t* pcTimerName, TIMER_CALLBACK_TYPE Timer_Callback, void* pvArgs, int32_t nPeriodMs, int32_t nStartFlag);
#else
	TIMR_T TIMR_Create(const char_t* pcTimerName, TIMER_CALLBACK_TYPE Timer_Callback, TASK_ARGS pvArgs, int32_t nPeriodMs, int32_t nCyclicFlag);
#endif

	int32_t    TIMR_Start(TIMR_T cTimer);
	int32_t    TIMR_Stop(TIMR_T cTimer);

	int32_t    TIMR_Close(TIMR_T cTimer);
	int32_t	   TIMR_Destroy(const char_t* pcTimerName);

#ifdef __cplusplus
}
#endif

#endif // TIMER_H_


#ifndef EVENTFLAG_H_
#define EVENTFLAG_H_

//#define EVTF_IMPLEMENTED_BY_MUTX
#define EVTF_IMPLEMENTED_BY_SEMA

#define EVTF_WAIT_INFINITE	(-1)
#define EVTF_FAILURE		((EVTF_T)-1)

#if defined(STM32F2XX)

#include <RTOS.h>

typedef struct {
    unsigned int unFlags;
    OS_EVENT oEvent;
} etvf_t;

#else

#ifdef EVTF_IMPLEMENTED_BY_SEMA
#include "Semaphore.h"
#else
#include "osal/Mutex.h"
#endif // EVTF_IMPLEMENTED_BY_SEMA
#include "SharedMemory.h"

typedef struct {
	uint32_t* punFlags;
	uint32_t  unFlagOwner;
#ifdef EVTF_IMPLEMENTED_BY_SEMA
	SEMA_T		  cSema;
#else	
	MUTX_T		  cMutex;
#endif
	SHRM_T		  cSharedFlag;
} etvf_t;

#endif //STM32F2XX

typedef etvf_t* EVTF_T;


#ifdef __cplusplus
extern "C" {
#endif

EVTF_T EVTF_Create(const char_t* pcEventFName);
EVTF_T EVTF_Open(const char_t* pcEventFName);

int32_t    EVTF_Set(EVTF_T cEventFId, uint32_t unFlags);
int32_t    EVTF_Wait(EVTF_T cEventFId, uint32_t unFlags, int32_t nMsWait);

int32_t    EVTF_Clear(EVTF_T cEventFId, uint32_t unFlags);
int32_t	   EVTF_GetFlags(EVTF_T cEventFId);

int32_t    EVTF_Close(EVTF_T cEventFId);
int32_t    EVTF_Destroy(const char_t* pcEventFName);

#ifdef __cplusplus
}
#endif

#endif // EVENTFLAG_H__


#ifndef PORT_SEMAPHORE_H_
#define PORT_SEMAPHORE_H_

#include "base/CTypedefs.h"

#define SEMA_WAIT_INFINITE	(-1)
#define SEMA_FAILURE		((SEMA_T)-1)
#define SEMA_TIMEOUT		(-1)

#if defined(PTHREAD)

	#include <semaphore.h>
	typedef sem_t* SEMA_T;

#elif defined(WINCE) || defined(WIN32)

	#include <windows.h>

	#define SemaphorePost(a)		ReleaseSemaphore(a, 1, NULL)
	#define SemaphoreWait(a,b)		WaitForSingleObject(a, b)
	#define CloseSemaphore(a)		CloseHandle(a)

#ifndef COROUTINE
	typedef HANDLE SEMA_T;
#else
	#define MAX_SEMAPHORE (500)
	#define MAX_SEMAPHORENAME_LENGTH (100)
	typedef struct
	{
		char strSemaphoreName[MAX_SEMAPHORENAME_LENGTH];
		int nCurrentCount;
	}SEMA_T_;
	typedef SEMA_T_* SEMA_T;
#endif
#elif defined(DSP_BIOS5)

	#include <std.h>
	#include <sys.h>
	#include <sem.h>

	typedef SEM_Handle	SEMA_T;

#elif defined(SYS_BIOS6)

	#include "ti/sysbios/knl/Semaphore.h"

	typedef Semaphore_Handle	SEMA_T;

#elif defined(STM32F2XX)
    
    #include <RTOS.h>
    typedef OS_CSEMA*    SEMA_T;
	
#endif

#ifdef __cplusplus
extern "C" {
#endif

SEMA_T Port_SEMA_Create(const char_t* pcSemaphoreName, int32_t nInit);
SEMA_T Port_SEMA_Open(const char_t* pcSemaphoreName);

int32_t	Port_SEMA_Post(SEMA_T SemaphoreId);
int32_t Port_SEMA_Wait(SEMA_T SemaphoreId, int32_t nMsWait);

int32_t Port_SEMA_GetCount(SEMA_T SemaphoreId);

int32_t Port_SEMA_Close(SEMA_T SemaphoreId);
int32_t Port_SEMA_Destroy(const char_t* pcSemaphoreName);

#ifdef __cplusplus
}
#endif

#endif // PORT_SEMAPHORE_H__


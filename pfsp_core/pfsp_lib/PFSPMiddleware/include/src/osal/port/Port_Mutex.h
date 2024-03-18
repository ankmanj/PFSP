#ifndef PORT_MUTEX_H_
#define PORT_MUTEX_H_

#include "base/CTypedefs.h"

#define MUTX_FAILURE	((MUTX_T)-1)

#if defined(PTHREAD)

	#include <pthread.h>
	
	typedef struct {		
		pthread_mutex_t* pcMutex;
		int              nFd;		
	} mutx_t;
	
	typedef mutx_t*	MUTX_T;

#elif defined(WINCE) || defined(WIN32)

	#include <windows.h>
	
#if 1 
#include "osal/port/win32/Port_Win32.h"
    #define MutexLock(a)			WaitForSingleObject(a, IPC_WAIT_INFINITE)
    #define SemaphoreLock(a)    	WaitForSingleObject(a, IPC_WAIT_INFINITE)
#else
	#define MutexLock(a)			WaitForSingleObject(a, INFINITE)
	#define SemaphoreLock(a)    	WaitForSingleObject(a, INFINITE)
#endif
	#define MutexTryLock(a)			WaitForSingleObject(a, 0)
	#define MutexRelease(a)			ReleaseMutex(a)
	#define SemaphoreRelease(a) 	ReleaseSemaphore(a, 1, NULL)
	#define CloseMutex(a)			CloseHandle(a)
	#define CloseSemaphore(a)   	CloseHandle(a)
	#define CloseFileMapping(a)		CloseHandle(a)

#ifndef COROUTINE
	typedef struct {		
		HANDLE      cMutex;
		char_t*	    pvBaseAddress;                
		HANDLE	    cSharedMemory;
		int32_t         nRecursiveFlag;
	} mutx_t;
	typedef mutx_t*	MUTX_T;
#else
	#include "osal/port/Port_Semaphore.h"
	typedef SEMA_T MUTX_T;
#endif

#elif defined(DSP_BIOS5)

	#include <std.h>
	#include <sys.h>
	#include <sem.h>
	
	typedef SEM_Handle	MUTX_T;
	
#elif defined(SYS_BIOS6)

//#define __SEMAPHORE_MUTEX__
#ifndef __SEMAPHORE_MUTEX__
	#include "ti/sysbios/knl/Semaphore.h"
	#include "ti/ipc/GateMP.h"

	typedef enum{BINSEM_MUTEX = 0, GATEMP_MUTEX}MutexType_t;

	typedef struct{
		union{
			GateMP_Handle 		oGateMPMutex;
			Semaphore_Handle	oSemMutex;
		}stMutex;

		int				nKey;
		MutexType_t		eType;
	}stMutex;

	typedef stMutex* MUTX_T;

	#define MUTEX_REGION_ID	(0)
#else
	#include "ti/sysbios/knl/Semaphore.h"

	typedef Semaphore_Handle MUTX_T;

#endif
#elif defined(STM32F2XX)
    
    #include <RTOS.h>
    typedef OS_RSEMA*    MUTX_T;
	
#endif


#ifdef __cplusplus
extern "C" {
#endif

	MUTX_T Port_MUTX_Create(const char_t* pcMtxName, int32_t nRecursizeFlag);
	MUTX_T Port_MUTX_Open(const char_t* pcMtxName);

	int32_t    Port_MUTX_Lock(MUTX_T MtxId);
	int32_t    Port_MUTX_TryLock(MUTX_T MtxId);

	int32_t    Port_MUTX_Unlock(MUTX_T MtxId);

	int32_t    Port_MUTX_Close(MUTX_T MtxId);
	int32_t    Port_MUTX_Destroy(const char_t* pcMtxName);

#ifdef __cplusplus
}
#endif

#endif // PORT_MUTEX_H_


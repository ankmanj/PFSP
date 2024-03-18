#ifndef PORT_SHAREDMEMORY_H_
#define PORT_SHAREDMEMORY_H_

#include "base/CTypedefs.h"

#define MAX_SHAREDMEMORY_NAME	(64)

#define SHRM_FAILURE		((SHRM_T)-1)

#if defined(PTHREAD)
	#include <mqueue.h>
	#include <semaphore.h>

	typedef struct {
		int    nFd;
		void*  pvBaseAddress;
		int	   nSize;
		sem_t* cSemaphore;
		char   acSharedMemoryName[MAX_SHAREDMEMORY_NAME];
	} shm_t;

#elif defined(WINCE) || defined(WIN32)

	#include <windows.h>

	#define SemaphoreRelease(a)		ReleaseSemaphore(a, 1, NULL)
	#define SemaphoreWait(a,b)		WaitForSingleObject(a, b)
	#define CloseSemaphore(a)		CloseHandle(a)

	#define CloseFileMapping(a)		CloseHandle(a)

	typedef struct {		
		char_t*	pvBaseAddress;
		int32_t	nSize;
		HANDLE	cSemaphore;
		HANDLE	cSharedMemory;
	} shm_t;

#elif defined(SYS_BIOS6)
	#include <xdc/runtime/Memory.h>

	#include <ti/ipc/SharedRegion.h>
	#include <ti/ipc/HeapMemMP.h>
	#include <ti/sysbios/knl/Semaphore.h>

	#include "Port_Mutex.h"

	#define SHARED_CTRL_REGION_ID		(1)
	#define SHARED_MEM_REGION_ID		(1)
	#define SHARED_BLOCK_NUM			(1)
	#define SHARED_ALIGN				(128)
	#define SHARED_NAME_LEN				(HEAP_NAME_LEN)
	#define SHARED_BLOCK_NUM			(1)

	#define HEAP_NAME_LEN				(64)
	#define	POSTFIX_LEN					(5)

	/* Round the value 'a' up by 'b', a power of two */
	#define ROUNDUP_SIZE(a, b) (uint32_t)((((uint32_t)(a)) + (((uint32_t)(b)) - 1)) & ~((uint32_t)(b) - 1))

	typedef struct{
		HeapMemMP_Handle 	pHeapMem_Handle;
		MUTX_T				pMutex;
		void*				pvBaseAddress;
	}shm_t;


#elif defined(STM32F2XX)
    
    #include <RTOS.h>
    
	typedef struct {
		void* pvBaseAddress;
		unsigned int nSize;
		OS_CSEMA oSemaphore;
	} shm_t;
#endif

typedef shm_t* SHRM_T;

#ifdef __cplusplus
extern "C" {
#endif

	SHRM_T Port_SHRM_Create(const char_t* pcSharedMName, int32_t nSize);
	SHRM_T Port_SHRM_Open(const char_t* pcSharedMName, int32_t* pnSize);

	int32_t    Port_SHRM_Read(SHRM_T SharedMId, void* pvAddr, int32_t nOffset, int32_t nSize);
	int32_t    Port_SHRM_Write(SHRM_T SharedMId, void* pvAddr, int32_t nOffset, int32_t nSize);

	char_t*  Port_SHRM_GetAddress(SHRM_T SharedMId);
	int32_t    Port_SHRM_Lock(SHRM_T SharedMId);
	int32_t    Port_SHRM_Unlock(SHRM_T SharedMId);

	int32_t    Port_SHRM_Close(SHRM_T SharedMId);
	int32_t    Port_SHRM_Destroy(const char_t* pcSharedMName);

	void*  Port_SHRM_Alloc(int32_t nHeapId, int32_t nSize, int32_t nAlign);
	void   Port_SHRM_Free(int32_t nHeapId, void* pHeap, int32_t nSize);

#ifdef __cplusplus
}
#endif

#endif // PORT_SHAREDMEMORY_H_


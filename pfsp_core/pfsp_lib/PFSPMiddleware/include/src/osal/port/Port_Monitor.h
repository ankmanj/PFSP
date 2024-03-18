#ifndef PORT_MONITOR_H_
#define PORT_MONITOR_H_

#include "base/CTypedefs.h"

#define MAX_MONITOR_NAME	(32)

#if defined(PTHREAD)

	typedef struct {
		int    nFd;
		void*  pvBaseAddress;
		int	   nSize;
		char   acMonitorName[MAX_MONITOR_NAME];
	} mnt_t;
	typedef mnt_t*		MONT_T;

	typedef mnt_t*	MONT_T;

	typedef void*	TASKSTATUS_T;

	typedef void*	TASKLOAD_T;

	typedef void* 	HEAPUSAGE_T;


#elif defined(WINCE) || defined(WIN32)

	#include <windows.h>

	#define CloseFileMapping(a)		CloseHandle(a)

	typedef struct {		
		char_t*	pvBaseAddress;
		int32_t	nSize;
		HANDLE	cSharedMemory;
		char_t  acSharedMemoryName[MAX_MONITOR_NAME];
	} mnt_t;

	typedef mnt_t*	MONT_T;

	typedef void*	TASKSTATUS_T;

	typedef void*	TASKLOAD_T;

	typedef void* 	HEAPUSAGE_T;


#elif defined(SYS_BIOS6)
#define MAX_TASK_NUM			16		// Need to be defined in XML
#define MAX_DURATION_COUNT

typedef void*		MONT_T;
typedef void*		TASKSTATUS_T;

typedef struct{
	double unTaskTime;
	double unTotalTime;
	double unStackUsedSize;
	double unStackSize;
	double unPeakStackSize;
	double unHeapSize;
	char* pcTaskName;
}TASKLOAD_T;

typedef struct{
	double unHeapSize;
	double unHeapUsed;
}HEAPUSAGE_T;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if 0
MONT_T Port_MONT_Create(const char* pcMonitorName, int nSize);
MONT_T Port_MONT_Open(const char* pcMonitorName, int* pnSize);

char*  Port_MONT_GetAddress(MONT_T MonitorId);

int    Port_MONT_Close(MONT_T MonitorId);
int    Port_MONT_Destroy(const char* pcMonitorName);
#endif

// for profiling
void* 	Port_MONT_InitProfile();
void  	Port_MONT_UpdateState();
int32_t Port_MONT_TaskLoad(void* pvStats, TASKLOAD_T* pTaskLoad, int32_t nCount);
void*	Port_MONT_GetFirstTask();
void*	Port_MONT_GetNextTask(void* pTask);
int32_t Port_MONT_CPULoad();
int32_t Port_MONT_HeapUseage(void* pvStats, HEAPUSAGE_T* pHeapUsage);
void 	Port_MONT_Terminate();
char* 	Port_MONT_GetTaskName(void* pTask);
int32_t Port_MONT_GetTaskMode(void* pTask);

// for hooking
void 	Port_MONT_SoftwareBKT();
void 	Port_MONT_SethookFxn(void* fxn, void* hookFxn);
void	Port_MONT_DumpInfo();


#ifdef __cplusplus
}
#endif

#endif // PORT_MONITOR_H_


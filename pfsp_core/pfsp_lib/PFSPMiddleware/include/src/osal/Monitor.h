#ifndef MONITOR_H_
#define MONITOR_H_

#include "osal/port/Port_Monitor.h"
#include "base/CTypedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

MONT_T MONT_Create(const char_t* pcMonitorName, int32_t iSize);
MONT_T MONT_Open(const char_t* pcMonitorName, int32_t* piSize);

char_t* MONT_GetAddress(MONT_T MonitorId);

int32_t	MONT_Close(MONT_T MonitorId);
int32_t MONT_Destroy(const char_t* pcMonitorName);


// For Profiling
void* 	MONT_Init_Profile();
int32_t MONT_GetHeapUsage(void* pvStats, HEAPUSAGE_T* pHeapUsage);
void 	MONT_RefreshProfile();
void 	MONT_Terminate();
int32_t MONT_GetTaskLoad(void* pvStats, TASKLOAD_T* pTaskLoad, int32_t nCount);
void* 	MONT_GetFirstTask();
void* 	MONT_GetNextTask( void* pTask );
int32_t MONT_GetCPULoad();
char_t*	MONT_GetTaskName( void* pTask );
int32_t	MONT_GetTaskMode( void* pTask );

#ifdef __cplusplus
}
#endif

#endif // MONITOR_H_


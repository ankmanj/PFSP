#ifndef OSAL_TASK_H_
#define OSAL_TASK_H_

#include "osal/port/Port_Task.h"

#ifdef __cplusplus
extern "C" {
#endif

	TASK_T TASK_Create(const char_t* pcTaskName, TASK_HANDLE Task_Handle, void* pvArgs, int32_t nPriority, int32_t nStackSize, int32_t nCoreId);

	TASK_T TASK_CreateEX(const char_t* pcTaskName, TASK_HANDLE Task_Handle, void* pvArgs, int32_t nPriority, int32_t nStackSize, int32_t nCoreId);

	int32_t    TASK_Join(TASK_T TaskId);
	int32_t    TASK_Delete(TASK_T TaskId);

	int32_t	   TASK_GetId(void);

	int32_t	   TASK_Delay(int32_t nMsWait, int32_t nUsWait);

	int32_t    TASK_SetPriority(TASK_T TaskId, int32_t nPriority);
	int32_t    TASK_GetPriority(TASK_T TaskId, int32_t* pnPriority);

void   TASK_PrintTaskInfo(void);

	int32_t    TASK_Local(int32_t* pnLocal);

#ifdef __cplusplus
}
#endif

#endif // OSAL_TASK_H_


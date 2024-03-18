#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "osal/port/Port_Semaphore.h"

#ifdef __cplusplus
extern "C" {
#endif
	
SEMA_T SEMA_Create(const char_t* pcSemaphoreName, int32_t nInit);
SEMA_T SEMA_Open(const char_t* pcSemaphoreName);

int32_t    SEMA_Post(SEMA_T SemaphoreId);
int32_t    SEMA_Wait(SEMA_T SemaphoreId, int32_t nMsWait);

int32_t    SEMA_GetCount(SEMA_T SemaphoreId);

int32_t    SEMA_Close(SEMA_T SemaphoreId);
int32_t    SEMA_Destroy(const char_t* pcSemaphoreName);

#ifdef __cplusplus
}
#endif

#endif // SEMAPHORE_H_


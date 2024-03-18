#ifndef MUTEX_H_
#define MUTEX_H_

#include "osal/port/Port_Mutex.h"
#include "base/CTypedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

MUTX_T MUTX_Create(const char_t* pcMtxName, int32_t nRecursive);
MUTX_T MUTX_Open(const char_t* pcMtxName);

int32_t    MUTX_Lock(MUTX_T MtxId);
int32_t    MUTX_TryLock(MUTX_T MtxId);

int32_t    MUTX_Unlock(MUTX_T MtxId);

int32_t    MUTX_Close(MUTX_T MtxId);
int32_t    MUTX_Destroy(const char_t* pcMtxName);

#ifdef __cplusplus
}
#endif

#endif // MUTEX_H_


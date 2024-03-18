#ifndef SHAREDMEMORY_H_
#define SHAREDMEMORY_H_

#include "osal/port/Port_SharedMemory.h"

#ifdef __cplusplus
extern "C" {
#endif

	SHRM_T	SHRM_Create(const char_t* pcSharedMName, int32_t nSize);
	SHRM_T	SHRM_Open(const char_t* pcSharedMName, int32_t* pnSize);

	int32_t SHRM_Read(SHRM_T SharedMId, void* pvAddr, int32_t nOffset, int32_t nSize);
	int32_t	SHRM_Write(SHRM_T SharedMId, void* pvAddr, int32_t nOffset, int32_t nSize);

	char_t* SHRM_GetAddress(SHRM_T SharedMId);
	int32_t SHRM_Lock(SHRM_T SharedMId);
	int32_t SHRM_Unlock(SHRM_T SharedMId);

	int32_t	SHRM_Close(SHRM_T SharedMId);
	int32_t SHRM_Destroy(const char_t* pcSharedMName);

#ifdef __cplusplus
}
#endif

#endif // SHAREDMEMORY_H_


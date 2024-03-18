#ifndef __PORT_OBJECTMANAGER_H__
#define __PORT_OBJECTMANAGER_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    eOMTYPE_SEMAPHORE = 0x01,
    eOMTYPE_MUTEX,
    eOMTYPE_MESSAGEQUEUE,
    eOMTYPE_SHAREDMEMORY,
    eOMTYPE_EVENTFLAG,
    eOMTYPE_THREAD,
} ObjectManagerType_t;

int OM_RegisterObject(void* pObj, int nObjType, const char* szObjName);
void* OM_FindObject(ObjectManagerType_t nObjType, const char* szObjName, unsigned int nIncreaseReference);
int OM_ReduceReferenceObjectInfoFromName(ObjectManagerType_t nObjType, const char* szObjName);
int OM_ReduceReferenceObjectInfoFromHandle(void* pObj);
int OM_RemovewObjectInfoFromName(ObjectManagerType_t nObjType, const char* szObjName);
int OM_RemovewObjectInfoFromHandle(void* pObj);
void OM_ClearObjInfo(void);
void* OM_CheckObject(int nObjType, unsigned int unNextStep);

#ifdef __cplusplus
}
#endif

#endif //__PORT_OBJECTMANAGER_H__

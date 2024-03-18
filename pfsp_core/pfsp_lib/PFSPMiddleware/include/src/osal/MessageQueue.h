#ifndef MESSAGEQUEUE_H_
#define MESSAGEQUEUE_H_

#include "osal/port/Port_MessageQueue.h"
#include "base/CTypedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

MSGQ_T     MSGQ_Create(const char_t* pcMbxName, int32_t nMaxQueue, int32_t nMsgLength);
MSGQ_T	   MSGQ_Open(const char_t* pcMbxName, int32_t* nMaxQueue, int32_t* nMsgLendth);

int32_t    MSGQ_Send(MSGQ_T MsgQId, char_t* pcBuf, uint32_t unSize, int32_t nMsWait);
int32_t    MSGQ_Receive(MSGQ_T MsgQId, char_t* pcBuf, int32_t nMsWait);

int32_t    MSGQ_GetCount(MSGQ_T MsgQId);
int32_t    MSGQ_Flush(MSGQ_T MsgQId);

int32_t    MSGQ_Close(MSGQ_T MsgQId);
int32_t    MSGQ_Destroy(const char_t* pcMbxName);

#ifdef __cplusplus
}
#endif

#endif // MESSAGEQUEUE_H_


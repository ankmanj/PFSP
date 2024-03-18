#ifndef MAILBOX_H_
#define MAILBOX_H_

#define MAIL_USED_SEMAPHORE
#define MAIL_USED_SEMAPHORE_FOR_SEND

#define MAIL_WAIT_INFINITE	(-1)
#define MAIL_FAILURE		((MAIL_T)-1)

#include "osal/Mutex.h"
#if defined(MAIL_USED_SEMAPHORE)
#include "Semaphore.h"
#else
#include "osal/MessageQueue.h"
#endif
#include "SharedMemory.h"

typedef void* MAIL_T;

#ifdef __cplusplus
extern "C" {
#endif

MAIL_T MAIL_Create(const char_t* pcMailboxName, int32_t nMaxMailCount, int32_t nMaxMailSize, int32_t nTotalMailboxSize);
MAIL_T MAIL_Open(const char_t* pcMailboxName, int32_t* pnMaxMailSize);

int32_t    MAIL_Send(MAIL_T MailboxIdParam, char_t* pcBuf, uint32_t unSize, int32_t nMsWait);
int32_t    MAIL_Receive(MAIL_T MailboxIdParam, char_t* pcBuf, uint32_t unSize, int32_t nMsWait);

int32_t    MAIL_GetCount(MAIL_T MailboxIdParam);
int32_t    MAIL_Flush(MAIL_T MailboxIdParam);

int32_t    MAIL_Close(MAIL_T MailboxIdParam);
int32_t    MAIL_Destroy(const char_t* pcMailboxName);

int32_t	   MAIL_GetMailSize(MAIL_T MailboxIdParam);
int32_t	   MAIL_GetMailboxSize(MAIL_T MailboxIdParam);

int32_t    MAIL_PrintStatus(MAIL_T MailboxId);

#ifdef __cplusplus
}
#endif

#endif // MAILBOX_H_


#ifndef PORT_MESSAGEQUEUE_H_
#define PORT_MESSAGEQUEUE_H_

#include "base/CTypedefs.h"

#if defined(PTHREAD)
    #include <mqueue.h>
#endif

#if defined(WINCE) || defined(WIN32)
	#include <windows.h>
	#include "osal/utils/squeue.h"

#elif defined(DSP_BIOS5)
    #include <std.h>
    #include <sys.h>
    #include <mbx.h>

#elif defined(SYS_BIOS6)
    #include <xdc/runtime/System.h>
    #include <xdc/runtime/IHeap.h>
    #include <ti/ipc/MessageQ.h>
    #include <ti/ipc/HeapBufMP.h>

#elif defined(STM32F2XX)
    #include <RTOS.h>

#endif  // defined(WINCE) || defined(WIN32)

#define MSGQ_WAIT_INFINITE	~(0)
#define MSGQ_FAILURE		((MSGQ_T)-1)


typedef enum{
	SIZE_64_BYTE 	= 64,
	SIZE_512_BYTE	= 512,
	SIZE_1_K 		= 1024,
	SIZE_4_K 		= 4*1024,
	SIZE_32_K 		= 32*1024,
	SIZE_128_K 		= 128*1024,
	SIZE_512_K 		= 512*1024,
	SIZE_1024_K 	= 1024*1024
}MSGSIZE_T;

#if defined(PTHREAD)

	typedef mqd_t	MSGQ_T;

#elif defined(WINCE) || defined(WIN32)

	typedef struct  {
		HANDLE    cSharedMemory;
		HANDLE    cSemaphoreForReceive;
		HANDLE	  cSemaphoreForSend;
		HANDLE    cMutex;
		squeue_t* pstSqueue;
	} msq_t;

	typedef msq_t* MSGQ_T;
	typedef MSGQ_T MSGQID_T;
	typedef void*	MSG_T;	


#elif defined(DSP_BIOS5)

	typedef MBX_Handle MSGQ_T;	
	
#elif defined(SYS_BIOS6)

	#define MAX_MSG_COUNT	4

#define __USE_HEAPMEM_ALLOC__

#ifndef __USE_HEAPMEM_ALLOC__
	#define MSGQ_HEAP_REGION_ID		(0)
#else
	#define MSGQ_HEAP_REGION_ID		(1)
#endif //__USE_HEAPMEM_ALLOC__
	#define MSGQ_HEAP_ID			(0)
	#define MSGQ_HEAP_BUF_ID		(1)

	#define HEAP_NAME_LEN			(64)
	#define MSGQ_NAME_LEN			(HEAP_NAME_LEN)

	#define ALIGN_SIZE 				(128)

	#define MAX_MSG_SIZE			(32)
	#define MSGQ_ID_FAILURE			MessageQ_E_NOTFOUND

	#define MSG_HEAP_ALLOC

//#define __USE_HEAPMEM_ALLOC__


	typedef MessageQ_QueueId MSGQID_T;

#ifndef __USE_HEAPMEM_ALLOC__
	typedef struct
	{
		MessageQ_MsgHeader	msgHdr;
		int					msgSize;
#if defined(MSG_HEAP_ALLOC)
		char*				msgBuf;
#else
		char				msgBuf[MAX_MSG_SIZE];
#endif //MSG_HEAP_ALLOC
	}msg_t;
	typedef msg_t	MSG_T;

	typedef struct{
		MessageQ_Handle		msgQ;
		MSGQID_T			msgQId;
		MSG_T*				msg;
	}msgq_t;
	typedef msgq_t*	MSGQ_T;
#else
	typedef struct{
		MessageQ_MsgHeader	stHeader;
		int 	nSize;
		char*	pnBuffer;
	} MsgHeader_t;

	typedef struct {
		MessageQ_Handle stMessageQueueHandle;
		MessageQ_QueueId stMessageQueueId;
		HeapBufMP_Handle stHeadBufMPHandler;
	} msg_t;

	typedef msg_t* MSGQ_T;

	#define MSG_FAILURE			(MSGQ_T)-1;
#endif

#elif defined(STM32F2XX)
    
    typedef struct
    {
        OS_MAILBOX oMailbox;
        void* pBuf;
    }msgq_t;
    
    typedef msgq_t* MSGQ_T;	

#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

MSGQ_T				Port_MSGQ_Create(const char_t* pcMbxName, int32_t nMaxQueue, int32_t nMsgLength);
MSGQ_T 				Port_MSGQ_Open(const char_t* pcMbxName, int32_t* pnMaxQueue, int32_t* pnMsgLength);
int32_t 			Port_MSGQ_Send(MSGQ_T cMsgQId, char_t* pcBuf, uint32_t unSize, int32_t nMsWait);
int32_t 			Port_MSGQ_Receive(MSGQ_T cMsgQId, char_t* pcBuf, int32_t nMsWait);
int32_t				Port_MSGQ_GetCount(MSGQ_T cMsgQId);
int32_t				Port_MSGQ_Flush(MSGQ_T cMsgQId);
int32_t				Port_MSGQ_Close(MSGQ_T cMsgQId);
int32_t				Port_MSGQ_Destroy(const char_t* pcMbxName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // PORT_MESSAGEQUEUE_H_


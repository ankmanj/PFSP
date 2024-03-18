#ifndef PORT_TASK_H_
#define PORT_TASK_H_

#include "base/CTypedefs.h"

#include <stdlib.h>
#if defined(PTHREAD)

	#include <pthread.h>
    #include <unistd.h>

	typedef pthread_t*	TASK_T;

	#define TASK_ARGS void*
	#define TASK_RETURN void*
	#define TASK_EXIT
	
	#define TASK_Self(a) {a = alloca(sizeof(pthread_t)); \
	                      *a = pthread_self();}
		
#elif defined(WINCE) || defined(WIN32)

	#include <windows.h>

	typedef HANDLE TASK_T;
	typedef TASK_T pid_t;
#ifndef COROUTINE
	#define TASK_ARGS void*
	#define TASK_RETURN void*
	#define TASK_EXIT
#else
	#define MAX_FIBER (100)
	typedef struct 
	{
		LPVOID arFiber[MAX_FIBER];
		int* arpAService[MAX_FIBER];
		unsigned int nFiberQueueHeadIndex;
		unsigned int nFiberExecuteIndex;
		unsigned int arbIsDead[MAX_FIBER];
		unsigned int arnDelay[MAX_FIBER];
	} FIBER_T;
	
	#define TASK_NO_RETURN
	#define TASK_ARGS void*
	#define TASK_RETURN void
	#define TASK_EXIT TASK_Delete(0);
#endif
#elif defined(DSP_BIOS5)
	
	#include <std.h>
	#include <sys.h>
	#include <tsk.h>
	
	typedef TSK_Handle TASK_T;
	#define TASK_ARGS Arg
	#define TASK_RETURN void
	#define TASK_NO_RETURN
	#define TASK_EXIT

#elif defined(SYS_BIOS6)

	#include "xdc/runtime/Types.h"
	#include "ti/sysbios/knl/Task.h"

	typedef Task_Handle TASK_T;
	
	#define TASK_ARGS UArg pvArgs0, UArg
	#define TASK_RETURN void
	#define TASK_NO_RETURN
	#define TASK_EXIT	Task_exit()
	#define TASK_MODE_READY 	Task_Mode_READY
	#define TASK_MODE_BLOCKED	Task_Mode_BLOCKED
	#define TASK_MODE_RUNNING	Task_Mode_RUNNING

    typedef TASK_T pid_t;
    
#elif defined(STM32F2XX)
    
    #include <RTOS.h>
    
    typedef struct
    {
        int nID;
        void* pArgs;
        void* pReturn;
        void* pRoutine;
        int nStackSize;
        OS_TASK oTask;
        char* pTLSRegion;
        OS_STACKPTR char* pStack;
    }task_t;
    
	typedef task_t* TASK_T;
    typedef TASK_T pid_t;

    #define TASK_ARGS void*
	#define TASK_RETURN void*
	#define TASK_EXIT

#endif

typedef struct
{
	pid_t pid;
	char* strNameProc;
}PROCESS_T;
	
PROCESS_T TASK_CreateProcess(char* strNameProcess,char* argv,TASK_RETURN (*func)(TASK_ARGS));

#define CREATE_PROCESS(a) TASK_CreateProcess(""#a"",argv[0],a); g_nProcess++; 

typedef TASK_RETURN (*TASK_HANDLE)(TASK_ARGS);

#ifdef __cplusplus
extern "C" {
#endif

	TASK_T Port_TASK_Create(const char_t* pcTaskName, TASK_HANDLE Task_Handle, void* pvArgs, int32_t nPriority, int32_t nStackSize, int32_t nCoreId);
	TASK_T Port_TASK_CreateEx(const char_t* pcTaskName, TASK_HANDLE Task_Handle, void* pvArgs, int32_t nPriority, int32_t nStackSize, int32_t nCoreId);

	int32_t    Port_TASK_Join(TASK_T TaskId);
	int32_t    Port_TASK_Delete(TASK_T TaskId);

	int32_t Port_TASK_GetId(void);

	int32_t Port_TASK_Delay(int32_t nMsWait, int32_t nUsWait);

	int32_t    Port_TASK_SetPriority(TASK_T TaskId, int32_t nPriority);
	int32_t    Port_TASK_GetPriority(TASK_T TaskId, int32_t* pnPriority);

	int32_t    Port_TASK_Local(int32_t* pnLocal);

void   Port_TASK_PrintTaskInfo(void);
#ifdef __cplusplus
}
#endif

#endif // PORT_TASK_H_


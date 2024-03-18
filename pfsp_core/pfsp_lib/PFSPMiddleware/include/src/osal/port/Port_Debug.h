#ifndef PORT_DEBUG_H_
#define PORT_DEBUG_H_

#include "osal/OSAL_Config.h"
#include "osal/SharedMemory.h"
#include "base/CTypedefs.h"

#if defined(LINUX)
    #include <sys/time.h>
    #include <sys/resource.h>
    #include <signal.h>
    #include <execinfo.h>
    #include <unistd.h>
    #include <time.h>
    #include <sched.h>

#elif defined(WINCE) || defined(WIN32)
	#include <time.h>
	#include <windows.h>

#elif defined(DSP_BIOS5)

#elif defined(__TI_COMPILER_VERSION__)
    #include "xdc/runtime/System.h"
    #include "xdc/runtime/Types.h"
    #include "ti/sysbios/utils/Load.h"

#elif defined(STM32F2XX)
    #include <RTOS.h>
#endif  // defined(LINUX)

#ifndef TYPE_DATE_T
#define TYPE_DATE_T
typedef struct
{
    struct {
#if defined(OSAL_LITTLE_ENDIAN)
		uint32_t usec;
		uint32_t sec;
#else
		uint32_t sec;
		uint32_t usec;
#endif
    } word;
} DATE_T;
#endif

typedef enum{
	DIRECT = 0,
	INDIRECT
}HOOKMOD_T;

typedef struct{
	char* pcFuncName;
	char* pcFuncLoc;
	addr_t ulFuncLoc;
}DEBG_FUNC_INFO_T;

typedef void(*FAULT_HANDLER_T)( int32_t );

/********************* For Performance monitoring ***********************/
#define MAX_LOG_BUF_SIZE			(1024)
#define MAX_LOGBUF_NAME_LEN			(128)
#define MAX_LOG_NAME_LEN			(16)
#define WRITER_OFFSET				(0)
#define READER_OFFSET				(1)
#define PERFCOUNT_OFFSET			(2)
#define PRIORITY_OFFSET				(4)
#define PERFSTART_OFFSET			(6)
#define READ_READER_FLAG(a)			( *((a) + READER_OFFSET) )
#define WRITE_READER_FLAG(a, b)		( *((a) + READER_OFFSET) = (b) )
#define READ_PERFCOUNT_FLAG(a)		( *((a) + PERFCOUNT_OFFSET) )
#define READ_PRIORITY(a)			( *((a) + PRIORITY_OFFSET) )
#define WRITE_PERF_COUNT(a, b)		( *((a) + PERFCOUNT_OFFSET) = (b) )
#define WRITE_PRIORITY(a, b)		( *((a) + PRIORITY_OFFSET) = (b) )

typedef struct{
	int32_t 		nIdx;
	int32_t			ulCore;
	uint32_t 		nPriority;
	char_t 			arName[MAX_LOG_NAME_LEN];
	DATE_T 			stSUserTime;
	DATE_T 			stFUserTime;
	DATE_T 			stSSysTime;
	DATE_T 			stFSysTime;
}PERF_T;

typedef struct{
	PERF_T stPerf;
	SHRM_T stSharedMem;
}PerfLogHandle_T;


#if defined(LINUX)

	typedef struct {
		struct timeval stUserTime;
		struct timeval stSystemTime;
	} TIME_T;
	
	typedef enum{
		FALL = 0,
		FABRT = SIGABRT,
		FSEGV = SIGSEGV,
		FBUS  = SIGBUS,
		FKILL = SIGKILL,
        FINTK = SIGINT
	}FAULT_T;

#elif defined(WINCE) || defined(WIN32)
	typedef LARGE_INTEGER  TIME_T;

	typedef int32_t FAULT_T;

#elif defined(DSP_BIOS5)

	typedef unsigned int TIME_T;

	typedef int FAULT_T;
    
#elif defined(__TI_COMPILER_VERSION__)
	typedef Types_Timestamp64 TIME_T;
	
	typedef int FAULT_T;

#elif defined(STM32F2XX)
    typedef OS_TIMING   TIME_T;	

	typedef int FAULT_T;
#endif

#ifdef __cplusplus
extern "C" {
#endif

// jhlee - add callback
typedef void (*callback_event_function)(FAULT_T callback_event);

void Port_DEBG_TimeDate(DATE_T* pDate);
void Port_DEBG_ExecuteTimeDate(DATE_T* pDate);
void Port_DEBG_TimeMeasureStart(TIME_T* pTimerId);
void Port_DEBG_TimeMeasureGet(TIME_T* pTimerId, uint32_t* unMs, uint32_t* unUs);
void Port_DEBG_ExecuteTimeMeasureStart(TIME_T* pTimerId);
void Port_DEBG_ExecuteTimeMeasureGet(TIME_T* pTimerId, uint32_t* unMs, uint32_t* unUs);

// for hooking
void 	Port_DEBG_Terminate(int32_t* pRegAddr);
void 	Port_DEBG_RegisterFaultView(FAULT_T nFaultType, FAULT_HANDLER_T handler, callback_event_function callback_func);	// jhlee - add callback
void	Port_DEBG_BackTrace();
void	Port_DEBG_ShowDeadCode();
void 	Port_DEBG_SoftwareBKT();

int32_t 	Port_DEBG_SetPerfLog(PerfLogHandle_T* pstPerfLog, const char_t* pszServiceName, int32_t nSizeProfileBuffer);
void 	Port_DEBG_PerfLog_Start(PerfLogHandle_T* pstPerfLog);
void 	Port_DEBG_PerfLog_Finish(PerfLogHandle_T* pstPerfLog, const char_t* pszName);


void 	Port_DEBG_SethookFxn(void* fxn, void* hookFxn, HOOKMOD_T mode);
void	Port_DEBG_DumpInfo(int32_t* pRegAddr);
void 	Exception_HookHandler(uint32_t *excStack, uint32_t pc);

int32_t		Port_DEBG_CPULoad(void);


#if defined(STM32F2XX)    
void Port_SetFlag(unsigned short unFlag);
#endif

#ifdef __cplusplus
}
#endif

#endif // PORT_DEBUG_H_


#ifndef DEBUG_H_
#define DEBUG_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <utility/strnlen.h>

#include "osal/port/Port_Debug.h"
#include "hal/Uart.h"

#if defined(WIN32)
#pragma warning(disable : 4127)
#endif

#if defined(_DEBUG)
#define DEBUG_MSG_LEVEL DEBG_LEVEL_INFO
#else
#define DEBUG_MSG_LEVEL DEBG_LEVEL_ERRO
#endif

#ifdef __cplusplus
extern "C" {
#endif
	
enum { DEBG_LEVEL_INFO=0, DEBG_LEVEL_WARN, DEBG_LEVEL_SPOT, DEBG_LEVEL_ERRO, DEBG_LEVEL_NONE };

extern int32_t debug_level;

#if defined(SYS_BIOS6)
#include <xdc/runtime/Assert.h>
	#ifdef DEBUG_UART_PRINTF
	#define DEBG_Print(...) DEBG_PrintUart(__VA_ARGS__)
	#else
	#define DEBG_Print(...) printf(__VA_ARGS__)
	#endif
#else
	#define DEBG_Print(...) printf(__VA_ARGS__)
#endif

#define DEBG_I_Print(...) if(debug_level <= DEBG_LEVEL_INFO) { (void)DEBG_Print(__VA_ARGS__); }
#define DEBG_W_Print(...) if(debug_level <= DEBG_LEVEL_WARN) { (void)DEBG_Print(__VA_ARGS__); }
#define DEBG_E_Print(...) if(debug_level <= DEBG_LEVEL_ERRO) { (void)DEBG_Print(__VA_ARGS__); }
#define DEBG_S_Print(...) if(debug_level <= DEBG_LEVEL_SPOT) { (void)DEBG_Print(__VA_ARGS__); }

#define PERROR(a) fprintf(stderr, "%s, %d:%s\n",__FILE__,__LINE__,#a)

#ifdef _DEBUG
#ifdef WINCE
#define DEBG_Assert(a)	{if(!(a)) {DEBG_E_Print("DEBG_Assert(%s):%s, %d\n",#a, __FILE__, __LINE__); assert(a);}}
#elif defined(STM32F2XX)
#define DEBG_Assert(a)	{if(!(a)) {DEBG_E_Print("DEBG_Assert(%s):%s, %d\n",#a, __FILE__, __LINE__); Port_SetFlag(1);}}
#elif defined(__TI_COMPILER_VERSION__)
#define DEBG_Assert(a)	{if(!(a)) {DEBG_E_Print("DEBG_Assert(%s):%s, %d\n",#a, __FILE__, __LINE__); Port_DEBG_SoftwareBKT();}}
#elif defined(WIN32)
#define DEBG_Assert(a)	{if(!(a)) {DEBG_E_Print("DEBG_Assert(%s):%s, %d\n",#a, __FILE__, __LINE__);  __debugbreak(); assert(a);}}
#elif defined(__linux)
#define DEBG_Assert(a)	{if(!(a)) {DEBG_E_Print("DEBG_Assert(%s):%s, %d\n",#a, __FILE__, __LINE__); Port_DEBG_SoftwareBKT(); }}
#else
#define DEBG_Assert(a)	{if(!(a)) {DEBG_E_Print("DEBG_Assert(%s):%s, %d\n",#a, __FILE__, __LINE__); Port_DEBG_SoftwareBKT();}}
#endif
#else
#define DEBG_Assert(a)
#endif


#define DEBG_Errno() (errno == 0 ? "None" : strerror(errno))
#define DEBG_E_Printf(M, ...) if(debug_level <= DEBG_LEVEL_ERRO)  fprintf(stderr, "ErrorNo: %s, " M "\n", DEBG_Errno(), ##__VA_ARGS__)

//@ DEBG_Assertf [7/7/2015 yeonchool.park]
#ifdef _DEBUG
#ifdef WINCE
#define DEBG_Assertf(a,m,...)	{if(!(a)) {DEBG_E_Print("DEBG_Assertf(%s):%s, %d\n",#a, __FILE__, __LINE__); DEBG_E_Printf(m, ##__VA_ARGS__); assert(a);}}
#elif defined(STM32F2XX)
#define DEBG_Assertf(a,m,...)	{if(!(a)) {DEBG_E_Print("DEBG_Assertf(%s):%s, %d\n",#a, __FILE__, __LINE__); DEBG_E_Printf(m, ##__VA_ARGS__); Port_SetFlag(1);}}
#elif defined(__TI_COMPILER_VERSION__)
#define DEBG_Assertf(a,m,...)	{if(!(a)) {DEBG_E_Print("DEBG_Assertf(%s):%s, %d\n",#a, __FILE__, __LINE__); DEBG_E_Printf(m, ##__VA_ARGS__); Port_DEBG_SoftwareBKT();}}
#elif defined(WIN32)
#define DEBG_Assertf(a,m,...)	{if(!(a)) {DEBG_E_Print("DEBG_Assertf(%s):%s, %d\n",#a, __FILE__, __LINE__); DEBG_E_Printf(m, ##__VA_ARGS__);  __debugbreak(); assert(a);}}
#elif defined(__linux)
#define DEBG_Assertf(a,m,...)	{if(!(a)) {DEBG_E_Print("DEBG_Assertf(%s):%s, %d\n",#a, __FILE__, __LINE__); DEBG_E_Printf(m, ##__VA_ARGS__); Port_DEBG_SoftwareBKT(); }}
#else
#define DEBG_Assertf(a,m,...)	{if(!(a)) {DEBG_E_Print("DEBG_Assertf(%s):%s, %d\n",#a, __FILE__, __LINE__); DEBG_E_Printf(m, ##__VA_ARGS__); Port_DEBG_SoftwareBKT();}}
#endif
#else
#define DEBG_Assertf(a,m,...)	{;}
#endif

#define DBG_PERF_LOG						(0)
#define DBG_PERF_LOG_INITIALIZE(a, b, c) 	if(DBG_PERF_LOG != NULL) { (void)Port_DEBG_SetPerfLog(a, b, c);}
#define DBG_PERF_LOG_START(a) 				if(DBG_PERF_LOG != NULL) { (void)Port_DEBG_PerfLog_Start( a );}
#define DBG_PERF_LOG_FINISH(a, b) 			if(DBG_PERF_LOG != NULL) { (void)Port_DEBG_PerfLog_Finish( a, b );}


typedef struct
{
	uint16_t unYear;  // 1970~2100
	uint8_t unMonth;  // 1~12
	uint8_t unDate;   // 0~31
	uint8_t unWeekday;// 0~6 (0 : sun. ~ 6 : sat.)

	uint8_t unHour;   // 0~23
	uint8_t unMinute; // 0~59
	uint8_t unSecond; // 0~59
} DATEINFO_T;

void DEBG_SetLevel(int32_t msg_level);

void DEBG_TimeDate(DATE_T* pDate);
int32_t DEBG_GetDateInfo(DATE_T* pIn, DATEINFO_T* pOut);

void DEBG_Terminate(int32_t* pnArg);

uint32_t DEBG_GetMsTimeDate(DATE_T* pData);

DATE_T DEBG_MaxDate(DATE_T dest, DATE_T src);
DATE_T DEBG_AvgDate(DATE_T dest, DATE_T src);
DATE_T DEBG_MinusDate(DATE_T dest, DATE_T src);
DATE_T DEBG_PlusDate(DATE_T dest, DATE_T src);

void DEBG_TimeMeasureStart(TIME_T* pTimerId);
void DEBG_TimeMeasureGet(TIME_T* pTimerId, uint32_t* ulMs, uint32_t* ulUs);
void DEBG_ExecuteTimeMeasureStart(TIME_T* pTimerId);
void DEBG_ExecuteTimeMeasureGet(TIME_T* pTimerId, uint32_t* pulMs, uint32_t* pulUs);

#if defined(DEBUG_UART_PRINTF)
extern int (*DEBG_PutCharUart) (char);
extern int (*DEBG_GetCharUart) (char *);

void DEBG_DM8148_Init();
void DEBG_PrintUart_Init();
void DEBG_PrintUart_Deinit();
int DEBG_PrintUart(char *fmt, ...);
int DEBG_GetInputUart(char *ch);

int DEBG_Open(const char *path, unsigned int flags, int llv_fd);
int DEBG_Close(int dev_fd);
int DEBG_Write(int dev_fd, const char *buf, unsigned int count);
int DEBG_Read(int dev_fd, char* buf, unsigned int count);
long DEBG_Lseek(int dev_fd, long offset, int origin);
int DEBG_Unlink(const char* path);
int DEBG_Rename(const char* old_name, const char* new_name);
#endif


#ifndef STM32F2XX
void DEBG_SetFileHandle(FILE *pFile);
void DEBG_SetFile(const char_t* filename);
int32_t DEBG_PrintFile(char_t* fmt, ...);

void DEBG_FinalizeFileHandle(void);
#endif //STM32F2XX

#ifdef __cplusplus
}
#endif

#endif // DEBUG_H_

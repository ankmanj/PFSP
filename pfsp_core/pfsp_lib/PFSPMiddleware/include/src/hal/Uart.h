#ifndef HAL_UART_H_
#define HAL_UART_H_

/*
* Include files
*/
#include "base/CTypedefs.h"


#if defined(TI_DM8148)
	typedef struct {
		void *pUartRxHandle;
		void *pUartTxHandle;
	} uart_t;
	typedef uart_t* UART_T;
#else
	#define UART_T	void*
#endif


/*
* Public Definition
*/
#define UART_TIMEOUT_INFINITE (int16_t)(-1)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    eUART_CONTROL_SET_ENABLE = 0,
	eUART_CONTROL_SET_DISABLE,
	eUART_CONTROL_SET_BAUDRATE,
} UARTControlEnum_t;

/*
* Public Member Functions
*/
void UART_DM8148_Init(void);
int16_t UART_Open(UART_T* poDevice, const int8_t* pszPortName, int32_t nBaudrate);
int16_t UART_Close(UART_T cUartId);
int16_t UART_Read(UART_T cUartId, int8_t* pcData, const int16_t nSize, const int16_t nTimeOutMs);

int16_t UART_Write(UART_T cUartId, const int8_t* pcData, const int16_t nSize, const int16_t nMsTimeOut);

int16_t UART_Control(UART_T oDevice, const int16_t nControl, int8_t* pData);

int16_t UART_Wait(UART_T oDevice, const int16_t nCondition, const int16_t nMsTimeout);

int32_t UART_Putc(char_t c);

int32_t UART_Getc(char_t *c);

int32_t UART_Debug(void);

#ifdef __cplusplus
}
#endif

#endif //HAL_UART_H_


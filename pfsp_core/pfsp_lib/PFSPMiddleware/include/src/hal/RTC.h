#ifndef HAL_RTC_H_
#define HAL_RTC_H_

#include "base/CTypedefs.h"

#define RTC_T	void*

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	eRTC_CONTROL_SET_ALARM_A = 0,
	eRTC_CONTROL_SET_ALARM_B,
    eRTC_CONTROL_DISABLE_ALARM_A,
    eRTC_CONTROL_DISABLE_ALARM_B,
    eRTC_CONTROL_CHECK_ALARM_A_ENABLE,
    eRTC_CONTROL_CHECK_ALARM_B_ENABLE,
} RtcControlEnum_t;

typedef struct 
{
	uint8_t unWeekDay;
    uint8_t unMonth;
    uint8_t unDate;
    uint8_t unYear;
	
	uint8_t unHours;
    uint8_t unMinutes;
    uint8_t unSeconds;
	uint8_t unReserved;
} RtcTimeDate_t;

typedef struct 
{
	uint8_t unHours;
    uint8_t unMinutes;
    uint8_t unSeconds;
	uint8_t unAmPm;
	uint8_t unDateOrWeekDaySel;
	uint8_t unDateOrWeekDay;
    uint16_t unReserved;
} RtcAlarm_t;

/*
* Public Member Functions
*/
int16_t RTC_Open(RTC_T* poDevice, const int8_t* pcDeviceName);

int16_t RTC_Close(RTC_T oDevice);

int16_t RTC_Read(RTC_T oDevice, int8_t* pBuffer, const int16_t nMaxReadSize, const int16_t nMsTimeOut);

int16_t RTC_Write(RTC_T oDevice, const int8_t* pBuffer, const int16_t nMaxWriteSize, const int16_t nMsTimeOut);

int16_t RTC_Control(RTC_T oDevice, const int16_t nControl, int8_t* pData);

int16_t RTC_Wait(RTC_T oDevice, const int16_t nCondition, const int16_t nMsTimeout);

#ifdef __cplusplus
}
#endif

#endif //HAL_RTC_H_

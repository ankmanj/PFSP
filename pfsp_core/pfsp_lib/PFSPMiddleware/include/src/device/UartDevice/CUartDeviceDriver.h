#ifndef CUARTDEVICEDRIVER_H_
#define CUARTDEVICEDRIVER_H_


#include "interface/IDeviceDriver.h"

#include "hal/Uart.h"

class CUartDeviceDriver : public IDeviceDriver {
public:
	// Control Code
	static const int32_t CUARTDEVICEDRIVER_SET_BAUDRATE;
	static const int32_t CUARTDEVICEDRIVER_TIMEOUT_INFINIT = UART_TIMEOUT_INFINITE;

public:
	CUartDeviceDriver(void);
	virtual ~CUartDeviceDriver(void);

	virtual int32_t Open(const char_t* pcName, void* pvOpen = NULL);
	virtual int32_t Close(void);

	virtual int32_t Read(char_t* pcBuf, uint32_t unSize, int32_t nTimeOut = CUARTDEVICEDRIVER_TIMEOUT_INFINIT);
	virtual int32_t Write(char_t* pcBuf, uint32_t unSize, int32_t nTimeOut = CUARTDEVICEDRIVER_TIMEOUT_INFINIT);

	virtual int32_t Control(uint32_t unCode, void* pvControl);
	virtual int32_t Wait(int32_t nTimeOut = 0);

private:
	UART_T	m_oTargetDevice;
};

#endif	/* CUARTDEVICEDRIVER_H_ */
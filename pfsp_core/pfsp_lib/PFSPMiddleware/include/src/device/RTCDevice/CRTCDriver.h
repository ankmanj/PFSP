#ifndef CRTCDRIVER_H_
#define CRTCDRIVER_H_


#include "interface/IDeviceDriver.h"

#include "hal/RTC.h"

class CRTCDriver : public IDeviceDriver {
public:

public:
	CRTCDriver(void);
	virtual ~CRTCDriver(void);

	virtual int32_t Open(const char_t* pcName, void* pvOpen = NULL);
	virtual int32_t Close(void);

	virtual int32_t Read(char_t* pcBuf,uint32_t unSize, int32_t nTimeOut = 0);
	virtual int32_t Write(char_t* pcBuf,uint32_t unSize, int32_t nTimeOut = 0);

	virtual int32_t Control(uint32_t unCode, void* pvControl);
	virtual int32_t Wait(int32_t nTimeOut = 0);

private:
    RTC_T	m_cTargetDev;
};

#endif	/* CRTCDRIVER_H_ */
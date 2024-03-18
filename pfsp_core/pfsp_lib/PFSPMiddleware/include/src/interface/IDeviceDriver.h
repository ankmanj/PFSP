#ifndef IDEVICEDRIVER_H_
#define IDEVICEDRIVER_H_

#define DECLARE_DEVICE_CLASS DECLARE_CLASS
#define IMPLEMENT_DEVICE_CLASS IMPLEMENT_CLASS

#define DEVICE_FOR_NAME(pszDeviceName) reinterpret_cast<IDeviceDriver*>(CClassEntry::GetClassForName(pszDeviceName))

class IDeviceDriver {
public:
	virtual ~IDeviceDriver(void){}
	virtual int32_t Open(const char_t* pcName, void* pvOpen) = 0;
	virtual int32_t Close(void) = 0;
	
	virtual int32_t Read(char_t* pcBuf, uint32_t unSize, int32_t nTimeOut = 0) = 0;
	virtual int32_t Write(char_t* pcBuf, uint32_t unSize, int32_t nTimeOut = 0) = 0;
	
	virtual int32_t Control(uint32_t unCode, void* pvControl) = 0;
	virtual int32_t Wait(int32_t nTimeOut = 0) = 0;

};

#endif	/* IDEVICEDRIVER_H_ */
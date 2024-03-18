#ifndef IPORT_H_
#define IPORT_H_

#include "base/CTypedefs.h"

#define DECLARE_PORT_CLASS DECLARE_CLASS
#define IMPLEMENT_PORT_CLASS IMPLEMENT_CLASS

#define PORT_FOR_NAME(pszPortName) reinterpret_cast<IPort*>(CClassEntry::GetClassForName(pszPortName))

class IPort {
public:
	static const int32_t IPORT_INFINITE_TIMEOUT;
	static const int32_t IPORT_NONBLOCK_TIMEOUT;

public:
	IPort(void) {}
	virtual ~IPort(void) {}

	virtual int32_t Create(const char_t* pszName) = 0;

	virtual int32_t Open(const char_t* pszName) = 0;
	virtual int32_t Close(void) = 0;

	virtual int32_t Send(void* pvData, int32_t nLength, int32_t nTimeOutMs = IPORT_INFINITE_TIMEOUT) = 0;
    virtual int32_t Receive(void* pvData, int32_t nLength, int32_t nTimOutMs = IPORT_INFINITE_TIMEOUT) = 0;

    // MISRA_JEROME_20160418_RULE_10_3_1
	virtual int32_t WaitSyncCall(void) = 0;
	virtual int32_t SignalSyncCall(int32_t nReturn) = 0;

	virtual const char_t* GetName(void) = 0;
};

#endif	/* IPORT_H_ */
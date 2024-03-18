#ifndef CUARTCONNECT_H_
#define CUARTCONNECT_H_

#include <string>
#include "osac/CSemaphore.h"
#include "factory/ClassForName.h"

#include "interface/IPort.h"
#include "interface/IConnect.h"
#include "device/UartDevice/CUartDeviceDriver.h"

class CUartConnect : public IConnect 
{
	DECLARE_CONNECT_CLASS(CUartConnect)

public:
	virtual int32_t Create(const char_t* pszName);

	virtual int32_t Open(const char_t* pszName);
	virtual int32_t Close(void);

    virtual int32_t Send(void* pvData, int32_t nLength, int32_t nTimoutMs = IPORT_INFINITE_TIMEOUT);    
    virtual int32_t Receive(void* pvData, int32_t nLength, int32_t nTimoutMs = IPORT_INFINITE_TIMEOUT);

	virtual int32_t Connect(void);
	virtual int32_t Disconnect(void);
	virtual int32_t Reconnect(void) {return 0;}

	virtual const char_t* GetName(void) {
		return m_strName.c_str();
	}

	// jerome_20160521: for MISRA_JEROME_20160418_RULE_10_3_1
	virtual int32_t WaitSyncCall(void) {return 0;}
	virtual int32_t SignalSyncCall(int32_t nReturn) {return nReturn;}

private:
    static char_t ms_arPreeamble[4];
	CUartDeviceDriver* m_poUartDevice;

	std::string m_strName;
	int32_t m_nServerFlag;

	static const uint8_t m_unPreamble; 
	static const uint8_t m_unPacketType; 
	static const uint8_t m_unNoAckPacketType;
	static const uint8_t m_unAcktype;

	OSAC::CSemaphore m_oSendSync;
	OSAC::CSemaphore m_oAckSync;
	int32_t m_unAckWait;
	uint32_t m_unAckSequence;

	uint16_t m_unProcessedSession;
	uint16_t m_nTimeOutCount;
	uint32_t m_unMaxPacketLength;

	int32_t m_nMaxTimeoutCount;
	int32_t m_nSendTimeoutMs;

	
	uint16_t m_unSession;

	uint32_t MakeCrc(char_t* punHeader, uint32_t unHeaderLength, char_t* punData, uint32_t unLength);
	int32_t SendAckPacket(uint16_t unSession, uint16_t unSequence);
	int32_t SendDummyPacket(void);
};

#endif  /* CUARTCONNECT_H_ */

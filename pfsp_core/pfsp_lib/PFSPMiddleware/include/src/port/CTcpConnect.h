#ifndef CTCPCONNECT_H_
#define CTCPCONNECT_H_

#include <string>

#include "factory/ClassForName.h"
#include "osac/CSocket.h"

#include "interface/IConnect.h"

class CTcpConnect : public IConnect 
{
	DECLARE_PORT_CLASS(CTcpConnect)

public:
	CTcpConnect(void) {}
	virtual ~CTcpConnect(void) {}

	virtual int32_t Create(const char_t* pszName);

	virtual int32_t Open(const char_t* pszName);
	virtual int32_t Close(void);

    virtual int32_t Send(void* pvData, int32_t nLength, int32_t nTimoutMs = IPORT_INFINITE_TIMEOUT);
    virtual int32_t Receive(void* pvData, int32_t nLength, int32_t nTimoutMs = IPORT_INFINITE_TIMEOUT);

	virtual int32_t Connect(void);
	virtual int32_t Disconnect(void);
	virtual int32_t Reconnect(void);

	virtual const char_t* GetName(void) {
		return m_strName.c_str();
	}

	virtual int32_t WaitSyncCall(void) {return 0;}
	virtual int32_t SignalSyncCall(int32_t nReturn) {return nReturn;}

private:
	OSAC::CSocket* m_poSocket;
	OSAC::CSocket m_oConnectedSocket;
	int32_t m_nConnected;

	std::string m_strName;
	std::string m_strIpAddress;
	int32_t m_nPortNumber;

	int32_t m_nServerFlag;
};

#endif  /* CTCPCONNECT_H_ */

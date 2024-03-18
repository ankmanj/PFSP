#ifndef CUDPCONNECT_H_
#define CUDPCONNECT_H_

#include "osac/CSocket.h"

#include "factory/CClassEntry.h"
#include "interface/IPort.h"
#include "interface/IConnect.h"
#include <string>

class CUdpConnect : public IConnect
{
	DECLARE_PORT_CLASS(CUdpConnect)

public:
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

private:
	OSAC::CSocket* m_poSocket;

	std::string m_strName;
	int32_t m_nServerFlag;

	SOCKADDR_IN m_stAddress;
	SOCKADDR_IN m_stConnectedAddress;
	int32_t m_nClientConnected;
};

#endif  /* CUDPCONNECT_H_ */
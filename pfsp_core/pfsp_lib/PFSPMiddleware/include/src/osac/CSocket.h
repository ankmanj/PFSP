#ifndef CSOCKET_H_
#define CSOCKET_H_

#include "osal/Socket.h"


namespace OSAC {

class CSocket {

public:
	CSocket(void);
	CSocket(SOCK_T oSock);
	virtual ~CSocket(void);

	virtual int32_t		CleanUp(void);
	virtual int32_t		Bind(int32_t PORT);
	virtual int32_t		Close(void);
	virtual int32_t 	Create(SOCK_PROTOCOL_TYPE type, int32_t MaxTransferSize);
	virtual int32_t		StartUp(void);
	virtual int32_t		Listen(int32_t nSOMAX );
	virtual int32_t		Accept(CSocket* poSocket, SOCKADDR* pSockaddr, int32_t* pnSize);
	virtual int32_t		GetError(void);

	virtual int32_t		Connect(SOCKADDR* pSockaddr, int32_t nSize);
	virtual int32_t		Receive(char_t* pcBuf, int32_t nLen, int32_t nFlag);
	virtual int32_t		Send(char_t* pcBuf, int32_t nLen, int32_t nFlag);
	virtual int32_t		SendTo(char_t* pcBuf, int32_t nLen, int32_t nFlags , SOCKADDR* pSockAddr, int32_t nSize);
	virtual int32_t		ReceiveFrom(char_t* pcBuf, int32_t nLen, int32_t nFlags, SOCKADDR* pSockAddr, int32_t* pnSize);

	virtual int32_t		CancelBlockingCall(void);
	virtual int32_t		SetSendBufferSizetoMax(int32_t MaxBufsize);
	virtual int32_t		SetRecvBufferSizetoMax(int32_t MaxBufsize);

	virtual int32_t		SetReceiveTimeout(uint32_t nMiliSec);
	virtual int32_t		SetSendTimeout(uint32_t nMiliSec);

	virtual int32_t		SetBlockingEnabled(int32_t nBlocking);
	virtual int32_t		SetSendNoDelay(void);

	virtual int32_t		IsDisconnected(void);

	static int32_t		GetCurrentIPAddress(char_t* pcBuf, int32_t nLen);

protected:
	SOCK_T m_Sock;
};

}

#endif	/* CSOCKET_H_ */

#ifndef CMAILBOXPORT_H_
#define CMAILBOXPORT_H_

#include "osal/Debug.h"
#include "osac/CMailbox.h"
#include "osac/CEventFlag.h"

#include "interface/IPort.h"
#include "factory/ClassForName.h"

class CMailboxPort : public IPort {

	DECLARE_PORT_CLASS(CMailboxPort)

public:
	CMailboxPort(void) : IPort(), m_poMailbox(0), m_pszName(0) {}
	virtual ~CMailboxPort(void) {}

	virtual int32_t Create(const char_t* pszName);

	virtual int32_t Open(const char_t* pszName);
	virtual int32_t Close(void);

    virtual int32_t Send(void* pvData, int32_t nLength, int32_t nTimoutMs = IPORT_INFINITE_TIMEOUT);
    virtual int32_t Receive(void* pvData, int32_t nLength, int32_t nTimoutMs = IPORT_INFINITE_TIMEOUT);

	virtual int32_t WaitSyncCall(void);
	virtual int32_t SignalSyncCall(int32_t nReturn);

	virtual const char_t* GetName(void) {
		return m_pszName;
	}

private:
	OSAC::CMailbox* m_poMailbox;
	OSAC::CEventFlag m_pSynchCallEventFlag;

	char_t* m_pszName;
};

#endif	/* CMAILBOXPORT_H_ */
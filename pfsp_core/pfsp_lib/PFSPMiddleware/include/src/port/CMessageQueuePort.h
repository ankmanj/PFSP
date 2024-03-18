#ifndef CMESSAGEQUEUEPORT_H_
#define CMESSAGEQUEUEPORT_H_

#include <string>

#include "osal/Debug.h"
#include "osac/CMessageQueue.h"

#include "interface/IPort.h"
#include "factory/ClassForName.h"

class CMessageQueuePort : public IPort {
	
	DECLARE_PORT_CLASS(CMessageQueuePort)
	
public:	
	virtual int32_t Create(const char_t* pszName);

	virtual int32_t Open(const char_t* pszName);
	virtual int32_t Close(void);
    
    virtual int32_t Send(void* pvData, int32_t nLength, int32_t nTimoutMs = IPORT_INFINITE_TIMEOUT);
    virtual int32_t Receive(void* pvData, int32_t nLength, int32_t nTimoutMs = IPORT_INFINITE_TIMEOUT);

	virtual int32_t WaitSyncCall(void) {return 0;}
	virtual int32_t SignalSyncCall(int32_t nReturn) {return nReturn;}

	virtual const char_t* GetName(void) {
		return m_strName.c_str();
	}
    
private:
    OSAC::CMessageQueue* m_poMessageQueue;
    
	std::string m_strName;    
};

#endif	/* CMESSAGEQUEUEPORT_H_ */
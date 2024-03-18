#ifndef CMESSAGEQUEUE_H_
#define CMESSAGEQUEUE_H_

#include "osal/MessageQueue.h"

namespace OSAC {

class CMessageQueue {
public:
	static const int32_t CMSGQ_WAIT_INFINITE = MSGQ_WAIT_INFINITE;

public:
	CMessageQueue(void);
	~CMessageQueue(void);

	int32_t Create(const char_t* pcName, const uint32_t unMaxQueue, const uint32_t unMsgLength);
	int32_t Destroy(void);

	int32_t Open(const char_t* pcName);
	int32_t Close(void);

	int32_t Send(char_t* pcBuf, uint32_t unSize, int32_t nMsWait = CMSGQ_WAIT_INFINITE);
	int32_t Receive(char_t* pcBuf, int32_t nMsWait = CMSGQ_WAIT_INFINITE);

	int32_t GetCount(void) const;
	int32_t Flush(void);

private:
	MSGQ_T m_cContent;
	char_t*  m_pcName;
};

}

#endif	/* CMESSAGEQUEUE_H_ */
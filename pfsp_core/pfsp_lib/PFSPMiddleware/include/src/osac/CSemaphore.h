#ifndef CSEMAPHORE_H_
#define CSEMAPHORE_H_

#include "osal/Semaphore.h"

namespace OSAC {

class CSemaphore {
public:
	static const int32_t CSEMA_WAIT_INFINITE = SEMA_WAIT_INFINITE;

public:
	CSemaphore(void);
	~CSemaphore(void);

	int32_t Create(const char_t* pcName, const uint32_t unCounter = 1);
	int32_t Destroy(void);

	int32_t Open(const char_t* pcName);
	int32_t Close(void);

	int32_t Wait(const int32_t nMsWait = CSEMA_WAIT_INFINITE);
	int32_t Post(void);

	int32_t GetCount(void) const;
	const char_t* GetName(void) const { return m_pcName;}

private:
	SEMA_T m_cContent;
	char_t*  m_pcName;
	uint32_t m_unCounter;
};

}

#endif	/* CSEMAPHORE_H_ */

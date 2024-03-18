#ifndef CEVENTFLAG_H_
#define CEVENTFLAG_H_

#include "osal/EventFlag.h"

namespace OSAC {

class CEventFlag {
public:
	static const int32_t CEVTF_WAIT_INFINITE = EVTF_WAIT_INFINITE;

public:
	CEventFlag(void);
	~CEventFlag(void);

	int32_t Create(const char_t* pcName);
	int32_t Destroy(void);

	int32_t Open(const char_t* pcName);
	int32_t Close(void);

	int32_t Set(uint32_t unFlags);
	int32_t Wait(uint32_t unFlags, int32_t nMsWait = CEVTF_WAIT_INFINITE);

	uint32_t GetFlags(void) const;
	int32_t Clear(uint32_t unFlags);

private:
	EVTF_T m_cContent;
	char_t*  m_pcName;
};

}

#endif	/* CEVENTFLAG_H_ */
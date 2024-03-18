#ifndef CSTOPWATCH_H_
#define CSTOPWATCH_H_

#include "osal/Debug.h"

namespace OSAC {

class CTime {
public:
	CTime(void) : m_ulMs(0), m_ulUs(0) {};
	~CTime(void) {};
	
	uint32_t m_ulMs;
	uint32_t m_ulUs;
};

class CStopwatch {
public:
	CStopwatch(void);
	~CStopwatch(void);

	void  Start(void);
	CTime GetTime(void);

private:
	TIME_T m_cContent;
};

}

#endif	/* CSTOPWATCH_H_ */
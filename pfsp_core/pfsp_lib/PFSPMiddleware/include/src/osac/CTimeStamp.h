#ifndef CTIMESTAMP_H_
#define CTIMESTAMP_H_

#include "osal/Debug.h"

namespace OSAC {

class CTimeStamp {
public:
	CTimeStamp(uint32_t ulSec = 0, uint32_t ulUSec = 0);
	~CTimeStamp(void);

	CTimeStamp operator+(const CTimeStamp& T) const;
	CTimeStamp operator-(const CTimeStamp& T) const;
	CTimeStamp& operator=(const CTimeStamp& T);

	int32_t operator>(const CTimeStamp& T) const;
	int32_t operator<(const CTimeStamp& T) const;
	int32_t operator==(const CTimeStamp& T) const;

	static CTimeStamp GetDateTime(void);
	int32_t GetDateInfo(DATEINFO_T* pDataInfo);

public:
#if 1	
	uint32_t m_ulSec;
	uint32_t m_ulUSec;
#else
	int32_t m_ulSec;
	int32_t m_ulUSec;
#endif
};

}

#endif	/* CTIMESTAMP_H_ */

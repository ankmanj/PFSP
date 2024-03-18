#ifndef CSHAREDMEMORY_H_
#define CSHAREDMEMORY_H_

#include "osal/SharedMemory.h"

namespace OSAC {

class CSharedMemory {
public:
	CSharedMemory(void);
	~CSharedMemory(void);

	int32_t Create(const char_t* pcName, const uint32_t unSize);
	int32_t Destroy(void);

	int32_t Open(const char_t* pcName);
	int32_t Close(void);

	int32_t Lock(void);
	int32_t Unlock(void);

	int32_t Read(void* pvDest, const uint32_t nOffset, const uint32_t nSize);
	int32_t Write(void* pvSrc, const uint32_t nOffset, const uint32_t nSize);

	void* GetAddress(void) const;
	uint32_t GetSize(void) const;

private:
	SHRM_T m_cContent;
	char_t*  m_pcName;
	uint32_t m_nSize;
};

}

#endif	/* CSHAREDMEMORY_H_ */

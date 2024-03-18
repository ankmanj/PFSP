#ifndef CMETEX_H_
#define CMETEX_H_

#include "osal/Mutex.h"

namespace OSAC {

class CMutex {
public:
	static const int32_t CMUTEX_NONRECURSIVE_FLAG = 0;
	static const int32_t CMUTEX_RECURSIVE_FLAG = 1;

public:
	CMutex(void);
	~CMutex(void);

	int32_t Create(const char_t* pcName, const int32_t nRecursive = CMUTEX_RECURSIVE_FLAG);
	int32_t Destroy(void);

	int32_t Open(const char_t* pcName);
	int32_t Close(void);

	int32_t Lock(void);
	int32_t TryLock(void);

	int32_t Unlock(void);

private:
	MUTX_T m_cContent;
	char_t*  m_pcName;

	bool flagCreatedSuccessfully{};
};

}

#endif	/* CMETEX_H_ */

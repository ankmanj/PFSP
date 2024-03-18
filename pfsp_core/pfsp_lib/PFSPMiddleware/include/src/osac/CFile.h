#ifndef CFILE_H_
#define CFILE_H_

#include "osal/File.h"
#include "base/CTypedefs.h"

namespace OSAC {

class CFile {
public:
#if defined(WIN32)
	CFile(void){m_oContent = NULL;};
#else
	CFile(void){m_oContent = 0;};
#endif
	~CFile(void){};
	int32_t Open(const char_t* pcFileName, int32_t nOption);

	int32_t Read(char_t* pcData, uint32_t unSize);
	int32_t Write(const char_t* pcData, uint32_t unSize);

	int32_t Seek(int32_t nOffset, int32_t nOrigin);
	int32_t Tell(void);

	int32_t Flush(void);

	int32_t Close(void);

private:
	FILE_T m_oContent;
};

}

#endif	/* CFILE_H_ */

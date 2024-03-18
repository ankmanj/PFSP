#ifndef LOGANTASKFILE_H_
#define LOGANTASKFILE_H_

#include <stdio.h>
#include "osac/CFile.h"
#include "logger/ILogAn.h"

namespace LOGGER {

class CLogAnTaskFile : public ILogAn
{
public:
	CLogAnTaskFile(int32_t nServiceName,char_t** ppszServiceName);
	virtual ~CLogAnTaskFile();

	virtual int32_t MwCompMessage(AServiceMessage* poMessage);
	virtual int32_t OnLog(const char_t* pszServiceName, const char_t* pszLogMsgName,int8_t* pData,int32_t nSize,OSAC::CTimeStamp *pTimeStamp);
	virtual int32_t OnUpdate();
	virtual int32_t MwCompFinalize(void);

	int32_t SetLogFile(OSAC::CFile *pFileLog);
	int32_t SetLogFileName(const char_t* pszFileName);
	virtual int32_t HasOnLog(void) {return 1;}
	virtual int32_t HasOnMessage(void)  {return 0;}

private:
	int32_t m_nCount;
	char_t** m_ppszServiceName;
	int32_t m_nServiceName;
	char_t* m_pszLogFileName;
	OSAC::CFile* m_pFileLog;
};

}	/* namespace LOGGER */

#endif	/* LOGANTASKFILE_H_ */
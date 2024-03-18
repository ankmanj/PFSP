#ifndef LOGANFILE_H_
#define LOGANFILE_H_

#include <stdio.h>
#include "osac/CFile.h"
#include "logger/ILogAn.h"

namespace LOGGER {

class CLogAnFile : public ILogAn
{
public:
	CLogAnFile(int32_t nServiceName,char_t** ppszServiceName);
	virtual ~CLogAnFile();

	virtual int32_t MwCompMessage(AServiceMessage* poMessage);
	virtual int32_t OnLog(const char_t* pszServiceName, const char_t* pszLogMsgName,int8_t* pData,int32_t nSize,OSAC::CTimeStamp *pTimeStamp);
	virtual int32_t OnUpdate();
	virtual int32_t MwCompFinalize(void);

	int32_t SetLogFile(OSAC::CFile *pFileLog);
	int32_t SetPrintFile(OSAC::CFile *pFilePrint);
	int32_t SetLogFileName(const char_t* pszFileName);
	int32_t SetPrintFileName(const char_t* pszFileName);
	int32_t WriteLogFileHeader(OSAC::CFile* pFileLog);
	virtual int32_t HasOnLog(void) {return 1;}
	virtual int32_t HasOnMessage(void)  {return 1;}

private:
	char_t** m_ppszServiceName;
	int32_t m_nServiceName;	
	char_t* m_pszLogFileName;
	char_t* m_pszPrintFileName;
	OSAC::CFile* m_pFileLog;
	OSAC::CFile* m_pFilePrint;
};

}	/* namespace LOGGER */

#endif	/* LOGANFILE_H_ */
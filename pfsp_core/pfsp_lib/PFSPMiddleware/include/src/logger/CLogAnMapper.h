#ifndef CDEBUGANMAPPER_H_
#define CDEBUGANMAPPER_H_

#include <list>
#include "logger/CDebugType.h"

namespace LOGGER {

class ILogAn;

class CLogAnServiceMap
{
public:
	CLogAnServiceMap(const char_t* pszServiceName);
	virtual ~CLogAnServiceMap();

	int32_t AddLogAn(ILogAn * pLogAn);
	int32_t RemoveLogAn(ILogAn* pLogAn);
	int32_t PushLog(DebugLogHeader_t* pstLogHeader, int8_t* pLogData);
	int32_t	Flush(void);
	uint32_t GetLogAnCount(void);
	int32_t GetOnLogCount(void);
	int32_t GetOnMessageCount(void);

	const char_t* GetName(void);

private:
	char_t* m_pszServiceName;
	std::list<void*> m_listLogAn;
};

class CLogAnMapper
{
public:
	CLogAnMapper(const char_t* pszLoggerName);	
	virtual ~CLogAnMapper(void);
	
	int32_t AddLogAn(ILogAn * pLogAn, const char_t* pszServiceName,int32_t nBufferSize);
	int32_t RemoveLogAn(ILogAn* pLogAn);
	int32_t PushLog(DebugLogHeader_t* pstLogHeader, int8_t* pLogData);
	int32_t	Flush(void);

	std::list<void*>* GetLogAnList(void);
	

protected:


private:	
	char_t* m_pszLoggerName;
	std::list<void*> m_listLogAnMapService;
	std::list<void*> m_listLogAn;
};

}	

#endif	/* CDEBUGANMAPPER_H_ */
#ifndef CDEBUGLOGGER_H_
#define CDEBUGLOGGER_H_

#include "osac/CThread.h"

#include "interface/IService.h"

#include "logger/CDebugType.h"
#include "logger/ILogAn.h"
#include "factory/ClassForName.h"

namespace LOGGER {

class CDebugMessage;
class CLogAnMapper;

class CDebugLogger:public IService, public OSAC::CThread
{
	DECLARE_SERVICE_CLASS(CDebugLogger)

public:
	CDebugLogger(void);	
	virtual ~CDebugLogger(void);
	
	virtual int16_t Initialize(void);
	virtual int16_t Start(std::string component_name, uint32_t component_id, uint32_t number_of_components);

	virtual int16_t Stop(void);
	virtual int16_t Finalize(void);

	virtual void SetName(const char_t* pszName);
	virtual const char* GetName(void);

	virtual void SetPriority(const int32_t nPriority);
	virtual int32_t GetPriority(void);

	virtual void SetCore(const int32_t nCore);
	virtual int32_t GetCore(void);

	virtual void SetNice(const int32_t nNice);
	virtual int32_t GetNice(void);

	virtual int16_t OnReceive(AServiceMessage* poMessage);

	virtual void SetParameterList(CParameterList* poParameterList);
	virtual const char_t* GetParameterValue(const char_t* pszParameter, int32_t nIndex = 0);
	virtual int32_t GetParameterCount(const char_t* pszParameter);

	int32_t MwCompFinalize(void);

protected:
	virtual void* Run(void* pvArgs);

private:
	CParameterList* m_poParameterList;
	char* m_pszServiceName;
	int32_t m_nPriority;
	int32_t m_nCore;
	int32_t m_nNice;
	CLogAnMapper* m_poLogAnMapper;
	int32_t LogDataDeAssemble(int8_t* pData,int32_t nSize);
	ILogAn *m_pOutputLogAn;

	int32_t m_nReceivedDataSize;

	int32_t m_nFinalizeRequest;

	void Print(const char_t* fmt,...);



	//CPortMap *m_pPortMap;
	//CDebugMessage* m_poDebugMessagetoCollector;
};

}	/* namespace LOGGER */

#endif	/* CDEBUGLOGGER_H_ */

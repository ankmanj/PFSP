#ifndef CDEBUGCOLLECTOR_H_
#define CDEBUGCOLLECTOR_H_

#include <list>
#include "CDebugType.h"

#include "osac/CThread.h"

#include "interface/IService.h"
#include "factory/ClassForName.h"
#include "utility/CParameterList.h"

namespace LOGGER {

class CDebugMessage;
class CDebugBuffer;

class CDebugCollector:public IService, public OSAC::CThread
{
	DECLARE_SERVICE_CLASS(CDebugCollector)

public:
	CDebugCollector();	
	virtual ~CDebugCollector(void);
	
	virtual int16_t Initialize(void);
	virtual int16_t Start(std::string component_name, uint32_t component_id, uint32_t number_of_components);

	virtual int16_t Stop(void);
	virtual int16_t Finalize(void);

	virtual void SetName(const char_t* pszName);
	virtual const char_t* GetName(void);

	virtual void SetPriority(const int32_t nPriority);
	virtual int32_t GetPriority(void);

	virtual void SetCore(const int32_t nCore);
	virtual int32_t GetCore(void);

	virtual void SetNice(const int32_t nNice);
	virtual int32_t GetNice(void);

	virtual void SetParameterList(CParameterList* poParameterList);
	virtual const char_t* GetParameterValue(const char_t* pszParameter, int32_t nIndex = 0);
	virtual int32_t GetParameterCount(const char_t* pszParameter);

	virtual int16_t OnReceive(AServiceMessage* poMessage);

	virtual int16_t MwCompFinalize();

protected:	
	virtual void* Run(void* pvArgs);


private:

	CParameterList* m_poParameterList;
	int32_t m_nHarvestPeriod;

	char_t* m_pszServiceName;
	int32_t m_nPriority;
	int32_t m_nCore;
	int32_t m_nNice;


	std::list<CDebugBuffer*> m_listDebugBuffer;
	//UTILS::CList* m_pListDebugBuffer;
	int32_t m_nFinalizeRequest;
	//CPortMap *m_pPortMap;
	int8_t* m_pDebugMessageBuffer;
	int32_t m_nMaxDebugBufferSize;
	CDebugMessage* m_poDebugMessagetoLogger;
	void DoDebugHarvest(void);
	void AddDebugBuffer(const char* pszNameDebugBuffer);
	void DeleteDebugBuffer(const char* pszNameDebugBuffer);

	char_t* m_pszLoggerName;

};

}	/* namespace LOGGER */

#endif	/* CDEBUGCOLLECTOR_H_ */
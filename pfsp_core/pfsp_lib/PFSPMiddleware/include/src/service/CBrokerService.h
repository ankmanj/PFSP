#ifndef CBROKERSERVICE_H_
#define CBROKERSERVICE_H_


#include "interface/IService.h"
#include "osac/CMutex.h"
#include "worker/CWorker.h"

class IPort;

class CConnectWork : public IWork
{
public:
	CConnectWork(const char_t* pszName, IConnect* poConnect, uint32_t unRxBufferSize) 
		: IWork(), m_poConnect(poConnect), m_unRxBufferSize(unRxBufferSize), m_nRequestedFinalize(0), m_poOwnerService(0) {
		m_strName = pszName;
		
		(void) m_oSync.Create(pszName, 0);
		(void) m_oSync.Lock();	
	}
	virtual ~CConnectWork(void) {
		(void) m_oSync.Close();
		(void) m_oSync.Destroy();
	}

	inline void SetService(IService* poService) { m_poOwnerService = poService; }
	inline const char_t* GetParameterValue(const char_t* pszParameter) { return m_poOwnerService->GetParameterValue(pszParameter); }
	
	virtual void OnInitialize(void){};

	virtual void OnProcess(const char_t* pvBuffer, const int32_t nLength);

	virtual void OnFinalize(void) {

	}

	virtual void RequestFinalize(void) {
		m_nRequestedFinalize = 1;
	}

	inline void WaitSync(void) { (void) m_oSync.Lock(); }
	inline void PostSync(void) { (void) m_oSync.Unlock(); }

	inline const char_t* GetName(void) {
		return m_strName.c_str();
	}

private:
	IConnect* m_poConnect;
	uint32_t m_unRxBufferSize;
	int32_t m_nRequestedFinalize;

	std::string m_strName;

	OSAC::CMutex m_oSync;

	IService* m_poOwnerService;
};


class CBrokerService : public IService
{
	DECLARE_SERVICE_CLASS(CBrokerService)
		
public:
	CBrokerService(void);
	virtual ~CBrokerService(void);

	virtual int16_t Initialize(void);
	virtual int16_t Start(std::string component_name, uint32_t component_id, uint32_t number_of_components);
	
	virtual int16_t Stop(void);
	virtual int16_t Finalize(void);

	virtual int16_t OnReceive(AServiceMessage* poMessage);

	virtual void SetName(const char_t* pszName);
	virtual const char* GetName(void);

	virtual void SetPriority(const int32_t nPriority);
	virtual int32_t GetPriority(void);

	virtual void SetCore(const int32_t nCore);
	virtual int32_t GetCore(void);

	virtual void SetNice(const int32_t nNice);
	virtual int32_t GetNice(void);

	virtual void SetParameterList(CParameterList* poParameterList);
	virtual const char_t* GetParameterValue(const char_t* pszParameter, int32_t nIndex = 0);
	virtual int32_t GetParameterCount(const char_t* pszParameter);

	int32_t IsTargetNode(const char_t* pszTargetNodeName) {
		std::list<std::string*>::iterator iter;
		for(iter = m_oTargetNodeList.begin(); iter != m_oTargetNodeList.end(); iter++)
		{
			std::string* pstrNodeName = *iter;
			if(*pstrNodeName == pszTargetNodeName)
			{
				return 1;
			}
		}

		return 0;
	}

	int32_t AddTargetNode(const char_t* pszTargetNodeName) {
		std::string* pstrTargetNode = new std::string(pszTargetNodeName);

		m_oTargetNodeList.push_front(pstrTargetNode);
		return 1;
	}

private:
	int32_t m_nForceWorkerFinalize;
	
	char_t* m_pszServiceName;
	int32_t m_nPriority;
	int32_t m_nCore;
	int32_t m_nNice;

	std::list<std::string*> m_oTargetNodeList;

	CParameterList* m_poParameterList;

	std::list<CWorker*> m_oConnectList;
};

#endif	/* CBROKERSERVICE_H_ */
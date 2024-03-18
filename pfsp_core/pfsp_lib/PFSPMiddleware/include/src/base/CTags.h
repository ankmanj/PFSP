#ifndef CTAGS_H_
#define CTAGS_H_

#include <list>
#include <map>
#include <string>

#include "base/CTypedefs.h"
#include "utility/CParameterList.h"

#include "interface/IPort.h"
#include "interface/IConnect.h"

#include "osal/Debug.h"
#include "osal/Memory.h"
#include "osac/CSharedMemory.h"

template<typename T1, typename T2> 
T1* GetTagMap(std::map<T2, T1*>* poMap, T2 key) 
{
	T1 *pRet = NULL;
	
	if(poMap == NULL)
    {
    	pRet = NULL;
    }
    else
    {
    	typename std::map<T2, T1*>::iterator iteratorMap;
		iteratorMap = poMap->find(key);
    	pRet = iteratorMap == poMap->end() ? 0 : iteratorMap->second;
    }

    return pRet;
}

template<typename T1, typename T2, typename T3>
T1* GetTagMap(std::map<T2, T1*>* poMap, T3 symbol) 
{
	T1 *pRet = NULL;

	if(poMap == NULL)
    {
    	pRet = NULL;
    }
    else
    {
    for(typename std::map<T2, T1*>::iterator iterator = poMap->begin(); iterator != poMap->end(); iterator++) 
	{
        if(*(iterator->second) == symbol) 
		{
				pRet = iterator->second;
				break;
        }
    }
    }

    return 	pRet;
}  

class CTag {
public:
	CTag(const char_t* pszName, const int32_t nId = 0) : m_pszName(0), m_nId(0) {
		if(pszName != NULL)
		{
			const size_t nNameLength = strnlen(pszName, __MAX_STRING_SIZE) + 1U;

			m_pszName = new char_t[nNameLength];
			(void)strncpy(m_pszName, pszName, nNameLength);

			m_nId = nId;
		}
	}
	
	virtual ~CTag(void) {	
		delete [] m_pszName;
	}

	inline const char_t* GetName(void) const { return m_pszName; }
	
	inline int32_t GetId(void) const { return m_nId; }

	inline int32_t operator==(const int32_t nId) const {
		return nId == m_nId;
	}

	inline int32_t operator==(const char_t* pszName) const {
		return strncmp(pszName, m_pszName, __MAX_STRING_SIZE) == 0 ? 1 : 0;
	}

private:
	CTag& operator=(const CTag& rValue) const {
		CTag* poTag = new CTag(rValue.GetName(), rValue.GetId());

		return *poTag;
	}

	char_t* m_pszName;	/// CTag 의 name
	int32_t m_nId;		/// CTag 의 Id
};

class CConnectTag : public CTag {
public:
	CConnectTag(const char_t* pszName, const char_t* pszPath, const char_t* pszRxBuffSize, const char_t* pszMode, const char_t* pszNodeName, const int32_t nPriority, const int32_t nCore, const char_t* pszClassName, const IConnect* poConnect);
	virtual ~CConnectTag(void);


	inline const char_t* GetPath(void) const { return m_pszPath; }

	inline const char_t* GetRxBuffSize(void) const { return m_pszRxBuffSize; }

	inline const char_t* GetMode(void) const { return m_pszMode; }

	inline const char_t* GetNodeName(void) const { return m_pszNodeName; }

	inline int32_t GetPriority(void) const { return m_nPriority; }

	inline const char_t* GetClassName(void) const { return m_pszClassName; }

	inline const IConnect* GetConnect(void) const { return m_poConnect; }

	void SetPath(const char_t* pszPath) {
		if(m_pszPath != NULL) {
			delete[] m_pszPath;			
		}
		size_t nLength = strnlen(pszPath, __MAX_STRING_SIZE);
		m_pszPath = new char_t[nLength+1];
		(void) strncpy(m_pszPath, pszPath, __MAX_STRING_SIZE);
		m_pszPath[nLength] = 0;
	}

private:
	char_t* m_pszPath;
	char_t* m_pszRxBuffSize;
	char_t* m_pszMode;

	char_t* m_pszNodeName;
	int32_t m_nPriority;
	int32_t m_nCore;

	char_t* m_pszClassName;
	const IConnect* m_poConnect;
};

class CNodeTag : public CTag {
public:
	CNodeTag(const char_t* pszNodeName, const int32_t nId) : CTag(pszNodeName, nId) {}
	virtual ~CNodeTag(void) {}
	
	inline void AddConnect(CConnectTag* poConnectTag, std::string strName)
	{
		DEBG_Assert(poConnectTag);
		m_oConnectMap.insert(std::pair<std::string, CConnectTag* >(strName, poConnectTag));
		//m_oConnectMap.at(strName) = poConnectTag;
	}

	inline void DeleteConnect(const char* pszConnectName) {
		(void) m_oConnectMap.erase(std::string(pszConnectName));
	}

	inline std::map<std::string, CConnectTag*>* GetConnectMap(void) {
		return &m_oConnectMap;
	}
	
	CConnectTag* GetConnect(const char_t* pszConnectName);
	
private:
	std::map<std::string, CConnectTag*> m_oConnectMap;
};

class CSubscriberUnit {
public:
	CSubscriberUnit(int32_t nServiceId, int32_t nReturnId, const char_t* pszSharedSubscribeName = NULL) 
	: m_nServiceId(nServiceId), m_nReturnId(nReturnId), m_poSharedSubscribeMemory(0) {
		if(pszSharedSubscribeName != NULL)
		{
			int32_t nRet;

			m_strSharedSubscribeName = std::string(pszSharedSubscribeName);
			m_poSharedSubscribeMemory = new OSAC::CSharedMemory;
			nRet = m_poSharedSubscribeMemory->Open(pszSharedSubscribeName);
			DEBG_Assert(nRet != -1);
		}
	}
	~CSubscriberUnit(void) {
	}

	void Close(void) {
		if(m_poSharedSubscribeMemory != NULL) {
			(void) m_poSharedSubscribeMemory->Close();
			delete m_poSharedSubscribeMemory; 
			m_poSharedSubscribeMemory = NULL;
		}
	}

	inline int32_t GetServiceId(void) const { return m_nServiceId; }
	inline int32_t GetReturnId(void) const { return m_nReturnId; }
	inline const char_t* GetSharedSubscribeName(void) const { return (m_strSharedSubscribeName.size() != 0) ? m_strSharedSubscribeName.c_str() : 0 ; }
	inline OSAC::CSharedMemory* GetSharedSubscribeMemory(void) { return m_poSharedSubscribeMemory; }

private:
	int32_t m_nServiceId;
	int32_t m_nReturnId;
	std::string m_strSharedSubscribeName;
	OSAC::CSharedMemory* m_poSharedSubscribeMemory;
};

class CSubscriber {
public:
	CSubscriber(const int32_t nTopicId) : m_nTopicId(nTopicId) {}
	~CSubscriber(void) {
		std::list<CSubscriberUnit>::iterator iterator;
		for( iterator = m_oServiceList.begin(); iterator != m_oServiceList.end(); iterator++)
		{
			iterator->Close();
		}
	}

	inline int32_t GetTopicId(void) const { return m_nTopicId; }

	inline std::list<CSubscriberUnit>* GetServiceList(void) { return &m_oServiceList; }

private:
	std::list<CSubscriberUnit> m_oServiceList;
	const int32_t m_nTopicId; 
};

class CServiceTag : public CTag {
public:
	static const int32_t eSERVICE_PERMISION_READ = 0x4;
	static const int32_t eSERVICE_PERMISION_WRITE = 0x2;
	static const int32_t eSERVICE_PERMISION_EXECUTE = 0x1;

public:
	CServiceTag(const char_t* pszServiceName, const int32_t nId, const CNodeTag* poNodeTag, const char_t* szClassName, const IPort* poPort,
		int32_t nPriority, int32_t nCore, int32_t nStackSize, int32_t nMailSize, int32_t nMailboxSize, const char_t* pszLogFileName, CParameterList* poParameterList);
	virtual ~CServiceTag(void);

	inline const char_t* GetLocation(void) const {	
		DEBG_Assert(m_poLocation);
		return m_poLocation->GetName();
	}

	void SetPort(const IPort* poPort) { m_poPort = poPort; }

	const IPort* SetPort(void);
	
	inline const IPort* GetPort(void) const { return m_poPort; }

	inline int32_t GetPriority(void) const { return m_nPriority; }

	inline int32_t GetCore(void) const { return m_nCore; }

	inline const char_t* GetClassName(void) const { return m_szClassName; }

	inline const char_t* GetLogFileName(void) const { return m_szLogFileName; }
	
	inline int32_t GetStackSize(void) const { return m_nStackSize; }

	inline int32_t GetMailSize(void) const { return m_nMailSize; }

	inline int32_t GetMailboxSize(void) const { return m_nMailboxSize; }

	inline int32_t GetPermission(void) const { return m_nPermision; }

	inline void SetPermission(int32_t nPermision) { m_nPermision = nPermision; }

	inline void SetParameterList(CParameterList* poParameterList) { m_poParameterList = poParameterList; }
	
	inline CParameterList* GetParameterList(void) { return m_poParameterList; }

	int32_t AddSubscriber(int32_t nTopicId, int32_t nSubscriberId, int32_t nReturnId, const char_t* pszSharedSubscribeName = NULL);
	
	int32_t RemoveSubscriber(const int32_t nTopicId, const int32_t nSubscriberId);
	
	std::list<CSubscriberUnit>* GetSubscriberList(const int32_t nTopicId);

private:
	const CNodeTag* m_poLocation;
	const IPort* m_poPort;
	std::list<CSubscriber*> m_oSubscriberList;

	char_t* m_szClassName;
	const int32_t m_nPriority;
	const int32_t m_nCore;
	const int32_t m_nStackSize;
	const int32_t m_nMailSize;
	const int32_t m_nMailboxSize;
	const char_t* m_szLogFileName;

	CParameterList* m_poParameterList;

	int32_t m_nPermision;
};

class CTopicTag : public CTag {
public:
	CTopicTag(const char_t* pszTopicName, const int32_t nId, CServiceTag* poOwnService, int32_t nAckFlag)
		: CTag(pszTopicName, nId), m_poPublisher(poOwnService), m_nAckFlag(nAckFlag) {}
	virtual ~CTopicTag(void) {}

	inline int32_t GetTopicAckFlag(void) const { return m_nAckFlag; }

	inline const char_t* GetPublisherName(void) const {
		DEBG_Assert(m_poPublisher);
		return m_poPublisher->GetName();
	}

	inline const char_t* GetPublisherName(int32_t nIdx) {		
		std::list<CServiceTag*>::iterator it = m_polPublisher.begin();
		std::advance(it, nIdx);
		CServiceTag* poServiceTag = *it;
				
		return poServiceTag->GetName();		
	}

	inline void AddPublisher(CServiceTag* poPublisher)
	{
		m_polPublisher.push_back(poPublisher);
	}

	inline int32_t GetPublisherCount(void)
	{
		return static_cast<int32_t>(m_polPublisher.size());
	}
	
	inline int32_t GetPublisherId(int32_t nIdx)
	{
		std::list<CServiceTag*>::iterator it = m_polPublisher.begin();
		std::advance(it, nIdx);
		CServiceTag* poServiceTag = *it;

		return poServiceTag->GetId();
	}

	inline int32_t GetPublisherId(void) const {
		DEBG_Assert(m_poPublisher);
		return m_poPublisher->GetId();
	}

private:
	CServiceTag* m_poPublisher;	///Publish 하는 service tag
	std::map<std::string, CServiceTag*> m_pomPublisher;
	std::list<CServiceTag*> m_polPublisher;
	int32_t m_nAckFlag;	///Topic 의 ack flag 값
};

class CBoardTag : public CTag {
public:
	CBoardTag(const char_t* pszBoardname, uint32_t unSize);
	virtual ~CBoardTag(void);

	inline uint32_t GetBoardSize(void) const { return m_unSize; }

	inline MemoryObject_t GetMemoryObject(void) const { return m_stMemoryObject; }

private:
	uint32_t m_unSize;
	MemoryObject_t m_stMemoryObject;
};

#endif	/* CTAGS_H_ */

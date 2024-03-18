#ifndef CPORTMAP_H_
#define CPORTMAP_H_


#include <string.h>

#include <string>
#include <map>
#include <list>

#include "osal/Debug.h"
#include "osac/CMutex.h"
#include "base/CTypedefs.h"
#include "base/CTags.h"
#include "interface/IPort.h"
#include "interface/IConnect.h"
#include "activity/CServiceActivity.h"

class CPortMap {
public:
	virtual ~CPortMap(void);

	const char_t* GetDebugCollectorName(void);

	inline int32_t GetLocalNodeId(void) const {
		DEBG_Assert(m_poLocalNodeTag);
		return m_poLocalNodeTag->GetId();
	}

	inline const char_t* GetLocalNodeName(void) const {
		DEBG_Assert(m_poLocalNodeTag);
		return m_poLocalNodeTag->GetName();
	}
	
	void SetLocalNodeName(const char_t* pszNodeName);

	const IPort* SetServicePort(const char_t* pszServiceName, const IPort* poPort = NULL);

	const IPort* SetServicePort(const int32_t nId);

	const IPort* GetServicePort(const char_t* pszServiceName);
	
	const IPort* GetServicePort(const int32_t nId);

	int32_t GetNodeId(const char_t* pszNodeName);
	
	const char_t* GetNodeName(const int32_t nNodeId);

	const char_t* GetLogFileName(const char_t* pszServiceName);

	const IConnect* GetConnectPort(const char_t* pszNodeName);
	
	const char_t* GetConnectName(const char_t* pszFromNode, const char_t* pszToNode);
	
	int32_t SetConnectPath(const char_t* pszNodeName, const char_t* pszConnectName, const char_t* pszPath);

	int32_t GetServiceId(const char_t* pszServiceName);
	
	const char_t* GetServiceName(const int32_t nServiceId);
	
	const char_t* GetServiceNode(const char_t* pszServiceName);
	
	const char_t* GetServiceClassName(const char_t* pszServiceName);
	
	int32_t GetServicePriority(const char_t* pszServiceName);

	int32_t GetServiceCore(const char_t* pszServiceName);

	int32_t GetServiceNice(const char_t* pszServiceName);

	int32_t GetServiceStackSize(const char_t* pszServiceName);
	
	int32_t GetServiceMailSize(const char_t* pszServiceName);

	int32_t GetServiceMailboxSize(const char_t* pszServiceName);
	
	CParameterList* GetServiceParameterList(const char_t* pszServiceName);

	int32_t GetServicePermission(const int32_t nServiceId);
	
	void SetServicePermission(const int32_t nServiceId, int32_t nPermission);

	int32_t GetTopicId(const char_t* pszTopicName);
	
	int32_t GetTopicAckFlag(const int32_t nId);
	
	const char_t* GetTopicName(const int32_t nId);
	
	const char_t* GetTopicPublisherName(const char_t* pszTopicName);
	
	int32_t GetTopicPublisherId(const char_t* pszTopicName);
	
	int32_t GetTopicPublisherId(const int32_t nId);

	const CBoardTag* GetBoard(const char_t* pszBoardName);

	const char_t* GetBoardName(const int32_t nId);

	void AddNode(const char_t* pszNodeName, const int32_t nId);
	
	void AddConnect(const char_t* pszNodeName, const char_t* pszConnectName, const char_t* pszPath, const char_t* pszRxBuffSize, const char_t* pszMode, const int32_t nPriority, const int32_t nCore, const char_t* pszClassName, const IConnect* poConnect);
	
	void DeleteConnect(const char_t* pszNodeName, const char_t* pszConnectName);

	void AddService(const char_t* pszServiceName, const int32_t nId, const char_t* pszNodeName, const char_t* pszClassName, const IPort* poPort, int32_t nPriority, int32_t nCore, int32_t nStackSize, int32_t nMailSize, int32_t nMailboxSize, const char_t* pszLogFileName, CParameterList* poParameterList);
	
	void AddTopic(const char_t* pszTopicName, const int32_t nId, const char_t* pszPublisher, const int32_t nAckFlag);

	void AddBoard(const char_t* pszBoardName, const uint32_t unSize);

	int32_t AddSubscriber(int32_t nServiceId, int32_t nTopicId, int32_t nSubscriberId, int32_t nReturnId = 0, const char_t* pszSharedSubscribeName = NULL);
	
	int32_t RemoveSubscriber(const int32_t nServiceId, const int32_t nTopicId, const int32_t nSubscriberId);

	int32_t AddServiceActivity(CServiceActivity* poServiceActivity);
	
	inline std::list<CServiceActivity*>* GetServiceActivityList(void) {
		return &m_oServiceActivityList;	
	}

	std::list<CSubscriberUnit>* GetSubscriberList(const int32_t nServiceId, const int32_t nTopicId);

	inline std::map<std::string, CNodeTag*>* GetNodeMap(void) { return &m_oNodeMap; }
	
	std::map<std::string, CConnectTag*>* GetConnectMap(void);

	inline std::map<std::string, CServiceTag*>* GetServiceMap(void) { return &m_oServiceMap; }

	inline std::map<std::string, CTopicTag*>* GetTopicMap(void) { return &m_oTopicMap; }

	inline std::map<std::string, CBoardTag*>* GetBoardMap(void) { return &m_oBoardMap; }

	void DeleteAllNode(void);
	
	void DeleteAllConnect(void);
	
	void DeleteAllService(void);
	
	void DeleteAllTopic(void);

	void DeleteAllBoard(void);

	void PrintNodeList(void);
	void PrintServiceList(void);
	void PrintTopicList(void);

	inline int32_t GetIntialized(void) const { return m_nInitialized; }
	
	inline void SetIntialized(int32_t nInitialized) { m_nInitialized = nInitialized; }

	static inline CPortMap* GetInstance(void) {
		static CPortMap s_oPortMap;
		return &s_oPortMap;
	}
	
protected:
	template<typename T1, typename T2> 
	CTag* GetTag(T1* poList, const T2 symbol) {	
		if(symbol == 0 || poList == 0) {
			return 0;
		}		
		typename T1::iterator iterator;
		for(iterator = poList->begin(); iterator != poList->end(); iterator++) {
			CTag* poTag = *iterator;
			if(*poTag == symbol)  {
				return poTag;
			}
		}
		return 0;		
	}
	

private:
	CPortMap(void);

	OSAC::CMutex m_oMutex;

	std::map<std::string, CNodeTag*> m_oNodeMap;	
	std::map<std::string, CServiceTag*> m_oServiceMap;
	std::map<std::string, CTopicTag*> m_oTopicMap; 
	std::map<std::string, CBoardTag*> m_oBoardMap;

	std::list<CServiceActivity*> m_oServiceActivityList;
	const CNodeTag* m_poLocalNodeTag;

	int32_t m_nInitialized;
};

#endif	/* CPORTMAP_H_ */
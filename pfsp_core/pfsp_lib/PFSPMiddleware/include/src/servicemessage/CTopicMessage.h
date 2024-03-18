#ifndef CTOPICMESSAGE_H_
#define CTOPICMESSAGE_H_

#include "osac/CSharedMemory.h"

#include "factory/CPortMap.h"
#include "service/AService.h"
#include "servicemessage/AServiceMessage.h"

#include "topic/SharedSubscribe.h"

template <typename T1, typename T2 = T1>
class CTopicMessage : public AServiceMessage {
public:
	inline CTopicMessage(void) : AServiceMessage(::GetService()->GetName(), 0, sizeof(T2)) {}
	inline CTopicMessage(int32_t nSize) : AServiceMessage(::GetService()->GetName(), 0, nSize) {}
	inline CTopicMessage(AServiceMessage* poMessage) : AServiceMessage(poMessage) {}
	inline virtual ~CTopicMessage(void) {}

	inline int32_t Publish( const char_t* pszTopicName ) {
		return AServiceMessage::PublishMessage(pszTopicName, CPortMap::GetInstance()->GetTopicId(pszTopicName), 0, sizeof(T2));
	}

	inline int32_t Publish( const char_t* pszTopicName , const CBoardTag* poBoardTag ) {
		uint32_t unSize = sizeof(T1);
		unSize += sizeof(ServiceMessageUserHeader_t);
		uint32_t unMessageBodySize = _MAX_(unSize, sizeof(FrameworkMessageBody_t));
		unSize = unMessageBodySize + sizeof(FrameworkMessageHeader_t) + sizeof(FrameworkPostamble_t);
		SharedTopic_t* t = reinterpret_cast<SharedTopic_t*>(AServiceMessage::GetUserBodyPtr());
		strncpy(reinterpret_cast<char*>(t->szName), poBoardTag->GetName(), MAX_SHARED_TOPIC_NAME);
		t->szName[MAX_SHARED_TOPIC_NAME-1] = 0;
		t->punStartAddress = reinterpret_cast<uint8_t*>(poBoardTag->GetMemoryObject().punVirtualAddress);
		t->punOffset = 0;
		t->unLength = unSize;
		
		return AServiceMessage::PublishMessage(pszTopicName, CPortMap::GetInstance()->GetTopicId(pszTopicName), 0, sizeof(T2), 1);
	}

	inline int32_t Subscribe( const char_t* pszTopicName, int32_t nReturnId = 0) {
		return AServiceMessage::Subscribe(::GetService()->GetName(), pszTopicName, nReturnId, 0, 0, 0);
	}

	inline int32_t Unsubscribe( const char_t* pszTopicName, int32_t nParam1 = 0) {
		return AServiceMessage::Unsubscribe(::GetService()->GetName(), pszTopicName, nParam1); 
	}

	T1* GetContent(void) {
		if(GetFrameworkMessageHeader()->m_unType == eMSGTYPE_FRAME_PUBLISH_BOARD) {
			const CBoardTag* m_poBoard = CPortMap::GetInstance()->GetBoard(reinterpret_cast<const char_t*>(AServiceMessage::GetUserBodyPtr()));
			return reinterpret_cast<T1*>(m_poBoard->GetMemoryObject().punVirtualAddress);
		} else {
			return reinterpret_cast<T1*>(AServiceMessage::GetUserBodyPtr());
		}
	}
};

template <typename T1>
class CTopicMessage<T1, SharedSubscribe_t> : public AServiceMessage {
public:
	inline CTopicMessage(const char_t* pszName, uint32_t unSize = sizeof(T1)) 
		: AServiceMessage(::GetService()->GetName(), 0, sizeof(SharedSubscribe_t)), m_poSharedMemory(0), m_poContentMessage(0) 
	{
		m_poSharedMemory = new OSAC::CSharedMemory;
		unSize += sizeof(ServiceMessageUserHeader_t);
		uint32_t unMessageBodySize = _MAX_(unSize, sizeof(FrameworkMessageBody_t));
		unSize = unMessageBodySize + sizeof(FrameworkMessageHeader_t) + sizeof(FrameworkPostamble_t);
		//if(m_poSharedMemory->Open(pszName) == -1) {
			int32_t  nRet = m_poSharedMemory->Create(pszName, unSize);
			DEBG_Assert(nRet != -1);
		//}
		SharedTopic_t* t = reinterpret_cast<SharedTopic_t*>(AServiceMessage::GetFrameworkBodyExt());
		strncpy(reinterpret_cast<char*>(t->szName), pszName, MAX_SHARED_TOPIC_NAME);
		t->szName[MAX_SHARED_TOPIC_NAME-1] = 0;
		t->punStartAddress = reinterpret_cast<uint8_t*>(m_poSharedMemory->GetAddress());
		t->punOffset = 0;
		t->unLength = unSize;

		m_poContentMessage = new AServiceMessage(::GetService()->GetName(), 0, sizeof(T1));
	}

	inline virtual ~CTopicMessage(void) {
		if(m_poSharedMemory != 0) {
			m_poSharedMemory->Close();
			delete m_poSharedMemory;
			m_poSharedMemory = 0;

			delete m_poContentMessage;
			m_poContentMessage = 0;
		}
	}

	inline TimeStamp_t GetTimeStamp(void) {
#if 0
		return m_poContentMessage->GetTimeStamp();
#else
		m_poSharedMemory->Lock();
		AServiceMessage oMessage(reinterpret_cast<int8_t*>(m_poSharedMemory->GetAddress()), m_poSharedMemory->GetSize());
		TimeStamp_t oTimeStamp;		
		oTimeStamp = oMessage.GetTimeStamp();
		m_poSharedMemory->Unlock();
		return oTimeStamp;
#endif
	}	

	inline int32_t Publish( const char_t* pszTopicName ) {
		return AServiceMessage::PublishMessage(pszTopicName, CPortMap::GetInstance()->GetTopicId(pszTopicName), 0, 0);
	}

	inline int32_t Subscribe( const char_t* pszTopicName, int32_t nReturnId = 0) {
		return AServiceMessage::Subscribe(::GetService()->GetName(), pszTopicName, nReturnId, 0, 0, 1);
	}

	inline int32_t Unsubscribe( const char_t* pszTopicName, int32_t nParam1 = 0) {
		return AServiceMessage::Unsubscribe(::GetService()->GetName(), pszTopicName, nParam1); 
	}

	int32_t Update(void) { 
		if(m_poSharedMemory)
		{
			m_poSharedMemory->Lock();
			memcpy(m_poContentMessage->GetStartAddress(), m_poSharedMemory->GetAddress(), m_poSharedMemory->GetSize());
			m_poSharedMemory->Unlock();
			return m_poSharedMemory->GetSize();
		}
		return 0; 
	}

	T1* GetContent(void) {
		if(m_poSharedMemory)
		{
			AServiceMessage oMessage(reinterpret_cast<int8_t*>(m_poSharedMemory->GetAddress()), m_poSharedMemory->GetSize());			
			return reinterpret_cast<T1*>(oMessage.GetUserBodyPtr());
			//return reinterpret_cast<T1*>(m_poContentMessage->GetUserBodyPtr());
		}
		return 0;
	}

	int32_t GetContent(T1* poT1) {
		if(m_poSharedMemory)
		{			
			m_poSharedMemory->Lock();
			AServiceMessage oMessage(reinterpret_cast<int8_t*>(m_poSharedMemory->GetAddress()), m_poSharedMemory->GetSize());			
			memcpy(poT1, oMessage.GetUserBodyPtr(), sizeof(T1));
			m_poSharedMemory->Unlock();
			return sizeof(T1);
		}
		return 0;
	}

	int32_t GetContent(void* poAddress, int32_t nSize) {
		if(m_poSharedMemory)
		{			
			m_poSharedMemory->Lock();
			AServiceMessage oMessage(reinterpret_cast<int8_t*>(m_poSharedMemory->GetAddress()), m_poSharedMemory->GetSize());			
			memcpy(poAddress, oMessage.GetUserBodyPtr(), nSize);
			m_poSharedMemory->Unlock();
			return nSize;
		}
		return 0;
	}

private:
	OSAC::CSharedMemory* m_poSharedMemory;
	AServiceMessage* m_poContentMessage;
};

typedef int32_t Subscribe_t;
typedef SharedTopic_t BoardPublish_t;
typedef int32_t AnonymousTopic_t;

#endif  /* CTOPICMESSAGE_H_ */
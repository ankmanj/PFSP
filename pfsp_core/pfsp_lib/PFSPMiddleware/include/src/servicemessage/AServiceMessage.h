#ifndef ASERVICEMESSAGE_H_
#define ASERVICEMESSAGE_H_

#include "osac/CTimeStamp.h"

#include "interface/IPort.h"
#include "interface/ISerializable.h"

#define CHECK_SERVICE_MESSAGE		(1)
#define POSTAMBLE_SERVICE_MESSAGE	(0xada5aa55U)

enum eAServiceMessage{
	eMSGTYPE_FRAME_OFFSET = 0,
	eMSGTYPE_FRAME_SUBSCRIBE,
	eMSGTYPE_FRAME_SUBSCRIBE_SHARED,
	eMSGTYPE_FRAME_UNSUBSCRIBE,
	eMSGTYPE_FRAME_PUBLISH,
	eMSGTYPE_FRAME_PUBLISH_BOARD,
	eMSGTYPE_FRAME_START,
	eMSGTYPE_FRAME_RESET,
	eMSGTYPE_FRAME_STOP,
	eMSGTYPE_FRAME_FINALIZE,
	eMSGTYPE_FRAME_USERMSG,
	eMSGTYPE_FRAME_SYNCCALL,
	eMSGTYPE_FRAME_SYNCCALL_ACK,
	eMSGTYPE_FRAME_SET_PERMISION,
	eMSGTYPE_FRAME_SET_DEBUGLEVEL,
	eMSGTYPE_FRAME_DEBUG,
	eMSGTYPE_FRAME_ONCOMMAND
};

enum FrameworkMessageOffset_t
{
	eBROKERSERVICE_OFFSET		= 0x0010,
	eTIMESERVICE_OFFSET			= 0x0020,
};

struct TimeStamp_t {
	uint32_t m_ulSec;
	uint32_t m_ulUSec;
};

struct FrameworkMessageHeader_t
{
	uint8_t m_unVersion;
	uint8_t m_unOption;
	TimeStamp_t m_stTimeStamp;
	uint16_t m_unSender;
	uint16_t m_unReceiver;
	uint16_t m_unType; 
	uint16_t m_unParam;
	uint32_t m_unTimeout;
#ifdef CHECK_SERVICE_MESSAGE
	uint32_t m_unCheckSum;
#endif
	uint32_t m_unTotalSize;
};

struct FrameworkPostamble_t
{
	uint32_t m_unPostamble;
};

struct FrameworkMessageBody_t
{
	int16_t m_nFrameMessageType;
	int16_t m_nSubscribeServiceId;
	int32_t m_nParam1;
	int32_t m_nParam2;
	int32_t m_nParam3;
	int32_t m_nParam4;
};

struct ServiceMessageUserHeader_t
{
	int16_t m_nUserMsgType;
	int32_t m_nParam;
	int32_t m_nSize;
};

class AServiceMessage : public ISerializable {
public:
	static const uint8_t m_nMessageFormatVersion = 1;

public:
	AServiceMessage(const char* szSenderName, const char* szRecieverName, int32_t nSize = 0);
	AServiceMessage(const char* szSenderName, const char* szRecieverName, int8_t* pBuffer, int32_t nSize);
	AServiceMessage(int8_t* pBuffer, int32_t nSize);
	AServiceMessage(IPort* pPort);
	AServiceMessage(AServiceMessage* poMessage);

	virtual ~AServiceMessage(void);

	inline TimeStamp_t GetTimeStamp(void) { return m_pstHeader->m_stTimeStamp; }	

	inline int8_t* GetStartAddress(void) { return m_pMsg; }

	inline uint32_t GetBufferSize(void) { return m_nBufferSize; }	
	
	inline uint32_t GetSize(void) { return m_pstHeader->m_unTotalSize; }	

	int32_t SetServicePort(IPort* pPort);

	IPort* GetServicePort(int32_t nServiceID); 
	
	inline FrameworkMessageHeader_t* GetFrameworkMessageHeader(void) {
		return m_pstHeader;
	}

	inline void* GetFrameworkBody(void) {
		return reinterpret_cast<void*>(m_pMsg + sizeof(FrameworkMessageHeader_t));
	}

	inline void* GetFrameworkBodyExt(void) {
		return reinterpret_cast<void*>(m_pMsg + sizeof(FrameworkMessageHeader_t) + sizeof(FrameworkMessageBody_t));
	}

	int32_t SendFrameworkMessage(int32_t nMessageId);

	int32_t SendUserMessage(int32_t size, int32_t nTimeOut = -1); 
	
	int32_t Publish(int32_t nMessageType, int32_t nSize, int32_t nBoardFlag = 0);

	int32_t Forward(int32_t nTimeout = -1, int32_t nForwardFlag = 1);

	int32_t Subscribe(const char* pszReceiverName, const char* pszMessageName, int32_t nParam1 = 0, int32_t nParam2 = 0, int32_t nParam3 = 0, int32_t nSharedSubscribe = 0); 

	int32_t Unsubscribe(const char* pszReceiverName, const char* pszMessageName, const int32_t nParam1 = 0, const int32_t nParam2 = 0, const int32_t nParam3 = 0); 
	
	int32_t SendSyncMessage(int32_t size, int32_t nTimeOut);

	int32_t SendAck(int32_t nReturn);

	int32_t Ack(int32_t nReturn);

	int32_t SendPermision(const int32_t nPermision);

	int32_t SendDebugLevel(const uint32_t unDebugLevel);

	int32_t Start(int32_t nReturnMessageId = 0);

	int32_t Stop(int32_t nReturnMessageId = 0); 

	int32_t Reset(int32_t nReturnMessageId = 0); 

	int32_t Finalize(int32_t nReturnMessageId = 0); 

	int32_t SendCommand(int32_t nMessageLen, int32_t nReturnMessageId = 0);

	static uint32_t GetChecksum(const uint8_t* pBuffer, int32_t nLength);
	
	virtual char_t* Serialize(int32_t* nLegnth);
	virtual void Unserialize(char_t* pszSerializedStream, int32_t nLength);

	TimeStamp_t GetMessageTimeStamp(void);

	const char_t*		GetMessageSenderName(void);

	const char_t*		GetMessageReceiverName(void);

	int16_t GetMessageSender(void);

	int16_t GetMessageReceiver(void);

	int32_t		GetMessageFrameworkSize(void);

	int32_t		GetMessageHeaderSize(void);
	
	const char_t* GetTopicName(void);

	int32_t GetTopicId(void);

	int8_t GetMessageVersion(void);

	inline void* GetUserBodyPtr(void) {
		m_pstUserHeader = reinterpret_cast<ServiceMessageUserHeader_t*>(GetFrameworkBody());
		return  reinterpret_cast<void*>(reinterpret_cast<int8_t*>(m_pstUserHeader) + sizeof(ServiceMessageUserHeader_t));
	}

	inline int16_t GetUserMsgType() {
		return m_pstUserHeader->m_nUserMsgType;
	}
	
	inline int32_t GetUserMsgParam(){
		return m_pstUserHeader->m_nParam;
	}
	
	inline int32_t GetUserMsgSize() {
		return m_pstUserHeader->m_nSize;
	}

	inline int32_t SendMessage(uint16_t unUserMsgType, int32_t nParam, int32_t nSize, int32_t nTimeOut = -1) {
		m_pstUserHeader->m_nUserMsgType = unUserMsgType;
		m_pstUserHeader->m_nSize = nSize;
		m_pstUserHeader->m_nParam = nParam;
		return SendUserMessage(m_pstUserHeader->m_nSize + sizeof(ServiceMessageUserHeader_t), nTimeOut);
	}
	
	inline int32_t SendMessageSync(uint16_t unUserMsgType, int32_t nParam, int32_t nSize, int32_t nTimeOut = -1) {
		m_pstUserHeader->m_nUserMsgType = unUserMsgType;
		m_pstUserHeader->m_nSize = nSize;
		m_pstUserHeader->m_nParam = nParam;
		return SendSyncMessage(m_pstUserHeader->m_nSize + sizeof(ServiceMessageUserHeader_t), nTimeOut);
	}

	int32_t PublishMessage(const char* pszSubscribeMsgType, uint16_t unUserMsgType, int32_t nParam, int32_t nSize, int32_t nBoardFlag = 0);

	void VerifyMessage(AServiceMessage* poMessage);
	
protected:
	int32_t Send(int32_t nFrameworkMsgtype, int32_t nDataSize, int32_t nTimeOut = -1);

private:	
	int32_t send(int32_t nTimeout, int32_t nForwardFlag = 0);

	FrameworkMessageHeader_t* m_pstHeader ;

	int8_t* m_pMsg;
	IPort* m_poPort;
	int8_t* m_pBuffer;
	int32_t m_nBufferSize;

	ServiceMessageUserHeader_t* m_pstUserHeader;	
};

typedef AServiceMessage CServiceMessage;

#endif	/* ASERVICEMESSAGE_H_ */
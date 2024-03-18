#ifndef CDEBUGMESSAGE_H_
#define CDEBUGMESSAGE_H_

#include <string>

#include "servicemessage/AServiceMessage.h"
#include "logger/ILogAn.h"
#include "logger/CDebugType.h"

namespace LOGGER {

enum eMessageDebug {
	eDEBUG_INITIALIZE_MSG = 0,	///< 초기화 메세지
	eDEBUG_FINALIZE_MSG, 		///< 종료 메세지
	eDEBUG_SET_DEBUGBUFFERSIZE, ///< DEBUG_LOGGER -> AService
	eDEBUG_SET_ENABLESENDMESSAGE, ///< DEBUG_LOGGER -> AService, Message 전달 Enable
	eDEBUG_SET_ENABLESENDLOG,	///< DEBUG_LOGGER -> AService, Log 전달 Enable
	eDEBUG_ADD_DEBUGBUFFER,		///< DEBUG_BUFFER -> DEBUG_COLLECTOR 
	eDEBUG_DELETE_DEBUGBUFFER,	///< DEBUG_BUFFER -> DEBUG_COLLECTOR
	eDEBUG_DO_HARVEST,			///< DEBUG_COLLECTOR -> DEBUG_COLLECTOR
	eDEBUG_LOG_DATA,			///< DEBUG_COLLECTOR -> DEBUG_LOGGER
	eDEBUG_LOG_FLUSH,			///< DEBUG_COLLECTOR -> DEBUG_LOGGER
	eDEBUG_LOG_REGISTER,		///< DEBUG_COLLECTOR -> DEBUG_LOGGER
	eDEBUG_LOG_BUFFERFULL,		///< DEBUG_COLLECTOR -> DEBUG_LOGGER
	eDEBUG_ADD_LOGAN,			///< Application -> DEBUG_LOGGER
	eDEBUG_REMOVE_LOGAN,		///< Application -> DEBUG_LOGGER
	eDEBUG_SET_OUTPUT,			///< Application -> DEBUG_LOGGER
	eDEBUG_RELEASE_OUTPUT,		///< Application -> DEBUG_LOGGER
	eDEBUG_PRINT_OUTPUT,		///< Any Service or Application -> DEBUG_COLLECTOR
	eDEBUG_SET_PLAYFILE,		///< Application -> DEBUG_PLAYER
	eDEBUG_RELEASE_PLAYFILE,	///< Application -> DEBUG_PLAYER
	eDEBUG_SET_PLAYPERIOD,		///< Application -> DEBUG_PLAYER
	eDEBUG_DO_PLAYFILE,			///< DEBUG_PLAYER Timer -> DEBUG_PLAYER
	eDEBUG_PLAYER_PLAY,			///< Application -> DEBUG_PLAYER
	eDEBUG_PLAYER_PAUSE,		///< Application -> DEBUG_PLAYER
	eDEBUG_PLAYER_REWIND,		///< Application -> DEBUG_PLAYER
	eDEBUG_PLAYER_STOP,			///< Application -> DEBUG_PLAYER
	eDEBUG_COLLECT_LOGNAME,		///< DEBUG_LOGGER -> DEBUG_COLLECTOR
};

struct DebugMessageHeader_t
{
	int32_t m_nMsgType;
	int32_t m_nParam;
	int32_t m_nSize;
};

// MISRA_JEROME_20160418_RULE_9_5_1
struct DebugMessageBody_t
{
	char_t strName[DEBUG_MAXSIZE_STRING];
};

struct DebugMessage_t
{
	DebugMessageHeader_t stHeader;
	DebugMessageBody_t unionBody;
};

class CDebugMessage : public AServiceMessage
{
public:
// 	CDebugMessage(int8_t* pBuffer, int nSize);
// 	CDebugMessage(IPort* poPort);
	CDebugMessage(const char* pszSenderName, const char* pszReceiverName, int32_t nMaxDataSize);
	CDebugMessage(AServiceMessage *poMessage);
	virtual ~CDebugMessage(void);
	
	int32_t SendDoHarvest(void);
	int32_t SendAddDebugBuffer(const char* pStrBufferName);
	int32_t SendSetSendMessage(int32_t bEnable);
	int32_t SendSetSendLog(int32_t bEnable);
	int32_t SendDeleteDebugBuffer(const char* pStrBufferName);
	int32_t SendDebugBufferData(int32_t nSize, int32_t bFull);
	int32_t SendDebugBufferFlush(int32_t nFlushedSize);
	int32_t SendSetServiceDebugBufferSize(int32_t nSize);
	int32_t SendLoggerAddLogAn(const char_t* pszServiceName,ILogAn* pLogAn,int32_t nBufferSize); //Should Use in Same Process with Logger
	int32_t SendLoggerDeleteLogAn(ILogAn* pLogAn); //Should Use in Same Process with Logger
	int32_t SendPlayerSetLogAn(ILogAn* pLogAn);	//Shoud Use in Same Process with Player;
	int32_t SendPlayerReleaseLogAn();
	int32_t SendLoggerSetOutputLogAn(ILogAn* pLogAn);	//Shoud Use in Same Process with Logger
	int32_t SendLoggerReleaseOutputLogAn();
	int32_t SendCollectorSetLoggerName(const char_t* pszLoggerName);

	int32_t SendDebugMessage(int32_t nMsgType, addr_t nParam, int32_t nSize);
	DebugMessageHeader_t* GetDebugMsgHeader(void);
	DebugMessageBody_t* GetDebugMsgBodyPtr(void);
	static int32_t GetHeaderSize(void);
	static int32_t GetMessageSize(void);
	static void GetLoggerName(std::string& strLoggerName);
	static void GetCollectorName(std::string& strCollectorName);
	static int32_t Print(char_t* fmt, ...);

protected:
private:		
	
	DebugMessageHeader_t* m_pstDebugMsgHeader;
	int32_t m_nMaxBufferSize;

};

}	/* namespace LOGGER */

#endif	/* CDEBUGMESSAGE_H_ */
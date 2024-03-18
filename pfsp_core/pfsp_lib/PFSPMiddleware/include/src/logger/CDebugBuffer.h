#ifndef CDEBUGBUFFER_H_
#define CDEBUGBUFFER_H_

class AServiceMessage;
namespace OSAC
{
	class CSharedMemory;
}

namespace LOGGER {

struct DebugBufferHeader_t
{
	int32_t nBufferSize;
	int32_t nWriteOffset;
	int32_t nReadOffset;
	int32_t bBufferFull;
};

class CDebugBuffer
{
public:
	
	/** 
	@brief	Constructor for Create CDebugBuffer Class in AService
	@param 	pszServiceName	String of Service which is going to use DebugBuffer
	@param 	nBufferSize	Buffer Size for logging Debug Data during DebugCollector's period
	@return void
	*/	
	CDebugBuffer(const char_t* pszServiceName,int32_t nBufferSize);
	
	/** 
	@brief	Constructor for Open CDebugBuffer instance in CDebugCollector
	@param 	pszServiceName	String of Service which is going to use DebugBuffer
	@return void
	*/	
	CDebugBuffer(const char_t* pszServiceName);

	/** 
	@brief	Destructor of CDebugBuffer Class
	@param 	void
	@return void
	*/
	virtual ~CDebugBuffer(void);

	int32_t LogMessageStart(AServiceMessage* poServiceMessage);
	int32_t LogMessageEnd(AServiceMessage* poServiceMessage);

	//int32_t Print(char_t *fmt, ...);
	//int32_t PrintData(int8_t* pData,int32_t nSize);
	int32_t LogData(const char_t* pszLogMsgName,int8_t* pData,int32_t nSize);
	
	DebugBufferHeader_t* GetDebugBufferHeader(void);
	int32_t GetBufferData(int8_t *pDataBuffer, int32_t* pbBufferFull);

	char_t*	GetName();
	char_t*	GetBufferName();

	int32_t SetSendMessageEnable(int32_t nEnable);
	int32_t SetSendLogEnable(int32_t nEnable);




private:

	int32_t WriteLogMessage(const char_t* pszLogMsgName, const int8_t* pData, int32_t nSize);
	

	int32_t m_bSharedCreate;
	char_t* m_pszBufferName;
	char_t* m_pszServiceName;
	DebugBufferHeader_t* m_pBufferHeader;
	int8_t* m_pBuffer;
	OSAC::CSharedMemory *m_pSharedBuffer;

	int32_t m_bEnableMessage;
	int32_t m_bEnableLog;
};

}	/* namespace LOGGER */

#endif	/* CDEBUGBUFFER_H_ */
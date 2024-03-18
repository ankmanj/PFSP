#ifndef ASERVICE_H_
#define ASERVICE_H_

#include "base/CTypedefs.h"
#include "osal/Debug.h"
#include "interface/IService.h"
#include "factory/ClassForName.h"
#include "utility/CParameterList.h"
#include "interface/ILoggable.h"

#include "logger/CDebugBuffer.h"


#define MAX_ONCOMMANDMSG_LEN			(128)
#define MAX_ONCOMMANDMSG_COUNT			(32)

typedef struct
{
	int32_t nMsgType;
	char_t* pcParamName;
	int32_t nParam;
	char_t  arMsg[MAX_ONCOMMANDMSG_LEN];
}OnCommandMessage_t;


class AService : public IService {
public:

	AService(void);

	virtual ~AService(void);

	virtual int16_t OnReceive(AServiceMessage* poMessage);

	virtual int16_t MwCompInitialize(std::string component_name, uint32_t component_id, uint32_t number_of_components) {return 0;}

	virtual int16_t MwCompStart(void) {return 0;}

	virtual int16_t MwCompStop(void) {return 0;}

	virtual int16_t OnReset(void) {return 0;}

	virtual int16_t MwCompFinalize(void) {return 0;}

	virtual int16_t MwCompCommand(int32_t nArgc, char_t* pszArgv[]) = 0;

	virtual int16_t MwCompMessage(AServiceMessage* poMessage) = 0;

	virtual int16_t Initialize(void);

	virtual int16_t Start(std::string component_name, uint32_t component_id, uint32_t number_of_components);
	
	virtual int16_t Stop(void);

	virtual int16_t Finalize(void);

	virtual void SetName(const char_t* pszName);
	
	virtual const char_t* GetName(void);

	virtual void SetPriority(const int32_t nPriority);

	virtual int32_t GetPriority(void);

	virtual int32_t GetCore(void);

	virtual void SetCore(const int32_t nCore);

	virtual void SetParameterList(CParameterList* poParameterList);
	
	virtual const char_t* GetParameterValue(const char_t* pszParameter, int32_t nIndex = 0);

	virtual int32_t GetParameterCount(const char_t* pszParameter);

	static int32_t Print(const char_t* fmt,...);
	
	static int32_t Log(const char_t* pszLogMsgName,int8_t* pData,int32_t nSize);
	static int32_t Log(ILoggable *poLoggable);

	void SetDebugBufferSize(int32_t nSizeDebugBuffer);
	LOGGER::CDebugBuffer** GetDebugBufferPtr();

private:
	char* m_pszServiceName;
	int32_t m_nPriority;
	int32_t m_nCore;
	CParameterList* m_poParameterList;

	LOGGER::CDebugBuffer* m_pDebugBuffer;

	typedef enum {eUSERSERVICE_STATE_IDLE,		
		eUSERSERVICE_STATE_RUN} ServiceState_t;

	ServiceState_t m_eServiceState;
};

#if defined(_TLS_SUPPORT_)
extern RP_THREAD_LOCAL AService* tl_poService;
extern RP_THREAD_LOCAL FILE* tl_fpServiceLog;
#endif

AService* GetService(void);
void SetService(AService* poService);

#define SVC_PRINT(...) AService::Print(__VA_ARGS__)

#define SVC_PERF_LOG_INITIALIZE(a, b) Port_DEBG_SetPerfLog(a, GetService()->GetName(), b);
#define SVC_PERF_LOG_START(a) Port_DEBG_PerfLog_Start( a );
#define SVC_PERF_LOG_FINISH(a, b, c) Port_DEBG_PerfLog_Finish( a, b, c );

#define SVC_LOG(ptrStrLogType,ptrData,nDataSize) AService::Log(ptrStrLogType,ptrData,nDataSize)
#define SVC_CLASSLOG(LoggableClass) AService::Log(&LoggableClass)

#define SVC_I_Print(...); {if(debug_level <= DEBG_LEVEL_INFO){AService::Print(__VA_ARGS__);}}
#define SVC_W_Print(...); {if(debug_level <= DEBG_LEVEL_WARN){AService::Print(__VA_ARGS__);}}
#define SVC_S_Print(...); {if(debug_level <= DEBG_LEVEL_SPOT){AService::Print(__VA_ARGS__);}}
#define SVC_E_Print(...); {if(debug_level <= DEBG_LEVEL_ERRO){AService::Print(__VA_ARGS__);}}

#endif	/* ASERVICE_H_ */

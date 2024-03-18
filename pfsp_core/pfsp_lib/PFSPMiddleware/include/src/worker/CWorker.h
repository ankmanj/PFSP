#ifndef CWORKER_H_
#define CWORKER_H_

#include "base/CTypedefs.h"
#include "osac/CThread.h"
#include "osac/CMutex.h"

class AService;

typedef enum
{
	eTRIGGERED = 0,
	eNONE_TRIGGERED
}eTriggerType_T;

struct IWork {
	IWork(void) {}
	virtual ~IWork(void) {}

	virtual void OnInitialize(void) = 0;
	virtual void OnProcess(const char_t* pvBuffer, const int32_t nLength) = 0;
	virtual void OnFinalize(void) = 0;		//Insert Finalize Code here.
	virtual void RequestFinalize(void) = 0; //if there is infinite loop in OnProcess, Insert loop escaping code here
	
};

class CWorker : public OSAC::CThread {
public:

    CWorker(const char* pszWorkerName, IWork* poWorker, int32_t nPriority, int32_t nMessageSize, int32_t nTriggerMode = eTRIGGERED);
    virtual ~CWorker(void);

	int32_t GetBusy(void);

	void DoFinalize(void);
    
    int32_t DoWork(const char_t* parMessage, const int32_t nLength);
	
	const char_t* GetParameterValue(const char_t* pszParameter, int32_t nIndex = 0);
    
protected:
    virtual void* Run(void* pvArgs);
    
private:
    IWork* m_poWork;

	int32_t m_nFinalizeRequest;
	OSAC::CMutex m_oMutex;

	int32_t m_nBusyFlag;
	int32_t m_nSkipFlag;

	int32_t m_nMessageSize;
	int32_t m_nActualMessageSize;
	char_t* m_parMessage;

	AService* m_poOwnerService;
};

#endif	/* CWORKER_H_ */

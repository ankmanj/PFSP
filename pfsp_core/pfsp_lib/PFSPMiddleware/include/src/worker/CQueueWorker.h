#ifndef CQUEUEWORK_H_
#define CQUEUEWORK_H_

#define __USE_WORKER_Q__
#ifdef __USE_WORKER_Q__
#define __USE_WORKER_Q_SEMA__
#endif

#include "base/CTypedefs.h"
#include "osac/CThread.h"
#include "osac/CMutex.h"
#include "worker/CWorker.h"
#ifdef __USE_WORKER_Q_SEMA__
#include "osac/CSemaphore.h"
#endif

#ifdef __USE_WORKER_Q__

struct WorkerQItem
{
	char_t* data;
	int32_t sz;
};

class CCriticalSection
{
public:
	CCriticalSection(OSAC::CMutex* pmutex) : m_pmutex(pmutex) {
		(void) m_pmutex->Lock();
	}
	~CCriticalSection() {
	    (void) m_pmutex->Unlock();
	}
private:
	OSAC::CMutex* m_pmutex;
};

class CWorkerQueue
{
public:
	CWorkerQueue(int32_t numItem, int32_t szItem, const char* pszWorkerName);
	~CWorkerQueue(void);

	int32_t put(const char_t* pItem, const int32_t nLen);
#ifdef __USE_WORKER_Q_SEMA__
	int32_t get(char_t** ppItem, int32_t* pnLen);
#else
	int32_t get(char_t* pItem, int32_t* pnLen);
#endif

private:
	char_t m_aWorkerName[64];
	int32_t m_nNum;
	int32_t m_nTail;
	int32_t m_nHead;
	int32_t m_nDropCnt;
	int32_t m_nNumItem;
	int32_t m_nSzItem;

	OSAC::CMutex m_oMutex;
	
	struct WorkerQItem* m_pItem;
};
#endif

class CQueueWorker : public OSAC::CThread {
public:
#ifdef __USE_WORKER_Q__
    CQueueWorker(const char* pszWorkerName, IWork* poWorker, int32_t nPriority, int32_t nMessageSize, int32_t nNumQueue = 1);
#else
    CQueueWorker(const char* pszWorkerName, IWork* poWorker, int32_t nPriority, int32_t nMessageSize);
#endif
    virtual ~CQueueWorker(void);

	int32_t GetBusy(void);

	void DoFinalize(void);
    
    int32_t DoWork(const char_t* parMessage, const int32_t nLength);
    
protected:
    virtual void* Run(void* pvArgs);
    
private:
	IWork* m_poWork;

	int32_t m_nFinalizeRequest;
	OSAC::CMutex m_oMutex;
	int32_t m_nBusyFlag; 

	int32_t m_nMessageSize; 
	int32_t m_nActualMessageSize;
	char_t* m_parMessage;

#ifdef __USE_WORKER_Q__
    CWorkerQueue* m_pWorkerQ;
#ifdef __USE_WORKER_Q_SEMA__
    OSAC::CSemaphore m_oQueueSync;
#endif
#endif

	AService* m_poOwnerService;
};

#endif	/* CQUEUEWORK_H_ */
#ifndef CTIMESERVICE_H_
#define CTIMESERVICE_H_

#include "interface/IService.h"
#include "CTimeServiceTimer.h"

#include "servicemessage/CTimeServiceMessage.h"
#include "worker/CWorker.h"

class CWorker;
class IPort;
class AServiceMessage;

class CRTCDriver;

class CTimeService : public IService
{
	DECLARE_SERVICE_CLASS(CTimeService)
		
public:
	CTimeService(void);
	virtual ~CTimeService(void);

	virtual int16_t Initialize(void);

	virtual int16_t Start(std::string component_name, uint32_t component_id, uint32_t number_of_components);
	
	virtual int16_t Stop(void);

	virtual int16_t Finalize(void);

	virtual int16_t OnReceive(AServiceMessage* poMessage);

	virtual void SetName(const char_t* pszName);
	
	virtual const char_t* GetName(void); 

	virtual void SetParameterList(CParameterList* poParameterList);
	
	virtual void SetPriority(const int32_t nPriority);
	virtual int32_t GetPriority(void);

	virtual void SetCore(const int32_t nCore);
	virtual int32_t GetCore(void);

	virtual void SetNice(const int32_t nNice);
	virtual int32_t GetNice(void);

	virtual const char_t* GetParameterValue(const char_t* pszParameter, int32_t nIndex = 0);
	virtual int32_t GetParameterCount(const char_t* pszParameter);

private:
	char_t* m_pszServiceName;
	int32_t m_nPriority;
	int32_t m_nCore;
	int32_t m_nNice;

	CParameterList* m_poParameterList; 

	CTimeServiceTimer m_oTimerServiceTimer;

	CWorker* m_poTimerWorker;

	CRTCDriver* m_poRtcDevice;
};


class CTimerWork : public IWork
{
public:
	CTimerWork(void) : IWork(), m_nRequestFinalize(0) {}
	virtual ~CTimerWork(void) {}

	virtual void OnInitialize(void) {};

	virtual void OnProcess(const char_t* pvBuffer, const int32_t nLength)
	{
		NO_ARGS_COMPILE_WARNING(nLength);

		int32_t nFrequence = *reinterpret_cast<const int32_t*>(pvBuffer);

		if(nFrequence == 0)
		{
			nFrequence = 10;
		}

		CTimeServiceMessage oTimeServiceMessage("/time");

		for(;m_nRequestFinalize == 0;)
		{
			(void) OSAC::CThread::Delay(nFrequence*100, 0);

			oTimeServiceMessage.UpdateTimerTick();
		}
	}



	virtual void OnFinalize(void)
	{

	}
	virtual void RequestFinalize(void) {m_nRequestFinalize = 1;};



private:
	int32_t m_nRequestFinalize;
};

#endif	/* CTIMESERVICE_H_ */
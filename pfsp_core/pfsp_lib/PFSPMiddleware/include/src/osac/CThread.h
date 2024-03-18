#ifndef CTHREAD_H_
#define CTHREAD_H_

#include "osal/Task.h"


#include "interface/IRunnable.h"

namespace OSAC {

class CThread : public IRunnable {
public:
	CThread(IRunnable* poRunnable = NULL);
	virtual ~CThread(void);

	int32_t Create(const char_t* pcName, void* pvArgs, int32_t nPriority, int32_t nStackSize = 0, int32_t nCoreId = 0);
	int32_t Delete(void);

	int32_t Join(void);

	int32_t GetId(void) const;

	int32_t SetPriority(const int32_t nPriority);
	int32_t GetPriority(void) const;

	const char_t* GetName(void);

	static int32_t* GetLocal(void);
	static int32_t Delay(const int32_t nMsWait, const int32_t nUsWait);
	static TASK_RETURN ThreadHandle(TASK_ARGS);

protected:
	virtual void* Run(void* pvArgs);

private:
	class CThreadArgs {
	public:
		CThreadArgs(IRunnable* pcRunnable, void* pvArgs) : m_pcRunnable(pcRunnable), m_pvArgs(pvArgs) {}
		~CThreadArgs(void) {};

		IRunnable* m_pcRunnable;
		void*	   m_pvArgs;
	private:
		CThreadArgs(void);
	};

private:
	TASK_T m_cContent;
	char_t*  m_pcApplicationName;
	IRunnable* m_poRunnable;
};

}

#endif	/* CTHREAD_H_ */
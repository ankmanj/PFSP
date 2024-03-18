#ifndef CDEBUGPLAYER_H_
#define CDEBUGPLAYER_H_

#include <list>
#include "interface/IService.h"
#include "CDebugType.h"
#include "factory/ClassForName.h"
#include "utility/CParameterList.h"

namespace LOGGER {

class CDebugMessage;

class CDebugPlayer:public IService//, public OSAC::CThread
{
	DECLARE_SERVICE_CLASS(CDebugPlayer)

public:
	CDebugPlayer(void);	
	virtual ~CDebugPlayer(void);
	
	virtual int16_t Initialize(void);
	virtual int16_t Start(std::string component_name, uint32_t component_id, uint32_t number_of_components);

	virtual int16_t Stop(void);
	virtual int16_t Finalize(void);

	virtual void SetName(const char_t* pszName);
	virtual const char* GetName(void);

	virtual void SetPriority(const int32_t nPriority);
	virtual int32_t GetPriority(void);

	virtual void SetCore(const int32_t nCore);
	virtual int32_t GetCore(void);

	virtual void SetNice(const int32_t nNice);
	virtual int32_t GetNice(void);


	virtual int16_t OnReceive(AServiceMessage* poMessage);

	virtual void SetParameterList(CParameterList* poParameterList);
	virtual const char_t* GetParameterValue(const char_t* pszParameter, int32_t nIndex = 0);	
	virtual int32_t GetParameterCount(const char_t* pszParameter);

protected:	
	//virtual void* Run(void* pvArgs);


private:
	struct TagFile
	{
		char_t* pszFileName;
		FILE* fpFile;
	};
	CParameterList* m_poParameterList;
	char_t* m_pszServiceName;
	int32_t m_nPriority;
	int32_t m_nCore;
	int32_t m_nNice;
	int8_t* m_pLogBuffer;
	int32_t m_nMaxSizeLogBuffer;

	//std::list m_listPlayFile;

	char_t* m_pszPlayFileName;
	FILE* m_fpPlayFile;
	int32_t m_bPlaying;
	int32_t m_nPlayPeriod;

	int32_t SetPlayFile(char_t* pszFileName);
	int32_t DoSingleStepPlayFile(void);
	int32_t ReleasePlayFile(void);
	int32_t StartPlayFile(int32_t nPlayPeriod_ms);
	int32_t SetPlayPeriod(int32_t nPlayPeriod_ms);
	int32_t PausePlayFile(void);
	int32_t RewindPlayFile(void);
	int32_t StopPlayFile(void);

	int32_t ParsePlayFileHeader();
	ILogAn *m_pPlayerLogAn;
	void Print(const char_t* fmt,...);

};

}	/* namespace LOGGER */

#endif	/* CDEBUGPLAYER_H_ */


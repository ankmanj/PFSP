#ifndef CSERVICEACTIVITY_H_
#define CSERVICEACTIVITY_H_

#include "base/CTypedefs.h"
#include <string>
#include "osac/CThread.h"

#include "interface/IService.h"

class IPort;
class CMail;

class CServiceActivity : public OSAC::CThread
{
public:
	CServiceActivity(IService* const poService, const int32_t nTaskPriority);
	virtual ~CServiceActivity(void);

	virtual int32_t Create(void);

	virtual int32_t Initialize(void);

	virtual int32_t Start(std::string component_name, uint32_t component_id, uint32_t number_of_components);

	virtual int32_t Stop(void);

	virtual int32_t Finalize(void);

	IService* GetService(void);

	void SetServicePort(IPort* piPort);

    void SetName(const char_t* szServiceName);
	void SetParameterList(CParameterList* poParameterList);
	void SetMessageSize(const int32_t nMessageSize);
	void SetStackSize(const int32_t nStackSize);

	void SetPriority(const int32_t nPriority);
	void SetCore(const int32_t nCore);
	int32_t GetPriority(void) const;
	int32_t GetCore(void);

	void RequestFinalize(void);

protected:
	virtual void* Run(void* pvArgs);

private:
	IPort* m_poPort;

	int32_t m_nFinalizeRequest;

	IService* m_poService;	

	int32_t m_nTaskPriority; 
	int32_t m_nTaskCore;
	int32_t m_nMessageSize;
	int32_t m_nStackSize;

	std::string m_strName; 
	AServiceMessage* m_poMessage;

	std::string component_name_;
	uint32_t component_id_;
	uint32_t number_of_components_;
};

#endif	/* CSERVICEACTIVITY_H_ */

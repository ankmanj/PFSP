#ifndef ISERVICE_H_
#define ISERVICE_H_


#include "base/CTypedefs.h"
#include "utility/CParameterList.h"

class AServiceMessage;

#define MAX_SERVICE_NAME_LENGTH (128)

#define SERVICE_FINALIZE_RETURN (-1)

#define DECLARE_SERVICE_CLASS DECLARE_CLASS
#define IMPLEMENT_SERVICE_CLASS IMPLEMENT_CLASS

#define SERVICE_FOR_NAME(pszServiceName) reinterpret_cast<IService*>(CClassEntry::GetClassForName(pszServiceName))

class IService {	

public:
	IService(void) {}
	virtual ~IService(void) {}

	virtual int16_t Initialize(void) = 0;
	virtual int16_t Start(std::string component_name, uint32_t component_id, uint32_t number_of_components) = 0;
	virtual int16_t Stop(void) = 0;
	virtual int16_t Finalize(void) = 0;

	virtual int16_t OnReceive(AServiceMessage* poMessage) = 0;

    virtual void SetName(const char_t* szName) = 0;
	virtual const char* GetName(void) = 0;

	virtual void SetPriority(const int32_t nPriority) = 0;
	virtual int32_t GetPriority(void) = 0;

	virtual void SetCore(const int32_t nCore) = 0;
	virtual int32_t GetCore(void) = 0;

	virtual void SetParameterList(CParameterList* oParameterList) = 0;
	virtual const char_t* GetParameterValue(const char_t* pszParameter, int32_t nIndex = 0) = 0;

	virtual int32_t GetParameterCount(const char_t* pszParameter) = 0;
};

#endif	/* ISERVICE_H_ */

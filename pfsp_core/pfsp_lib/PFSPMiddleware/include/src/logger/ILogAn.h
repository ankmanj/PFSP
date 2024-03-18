#ifndef ILOGAN_H_
#define ILOGAN_H_

#include "base/CTypedefs.h"
#include "osac/CTimeStamp.h"


#define _HasMember(a, b) \
	template<class T> struct has_member_##b;         \
	template<class R, class C>     \
	class has_member_##b<R C::*> {        \
	private:    \
		template<R C::*> struct helper;              \
		template<class T> static char check(helper<&T::##b>*);    \
		template<class T> static char (& check(...))[2];     \
	public:     \
		static const bool value = (sizeof(check<C>(0)) == sizeof(char));          \
	};              \
	\
	template<typename T>          \
	bool has_member_function_##b(T o)  \
	{                \
	return has_member_##b<int32_t (T::*)(const char* pszServiceName, const char* pszLogMsgName,int8_t* pData,int32_t nSize,OSAC::CTimeStamp *pTimeStamp)>::value;   \
	}                \


class AServiceMessage;

namespace LOGGER {

class ILogAn
{

public:

	virtual int32_t MwCompMessage(AServiceMessage* poMessage) = 0;
	virtual int32_t OnLog(const char_t* pszServiceName, const char_t* pszLogMsgName,int8_t* pData,int32_t nSize,OSAC::CTimeStamp *pTimeStamp) = 0;
	virtual int32_t OnUpdate() = 0;
	virtual int32_t MwCompFinalize(void) = 0;

	virtual int32_t HasOnLog(void) = 0;//{return -1;}
	virtual int32_t HasOnMessage(void)  = 0;//{return -1;}

};

}	/* namespace LOGGER */

#endif	/* ILOGAN_H_ */
#ifndef CPARAMETERLIST_H_
#define CPARAMETERLIST_H_

#include <string>
#include <map>
#include "base/CTypedefs.h"

class CParameterList {
public:
	CParameterList(void);
	~CParameterList(void) ;

	void DeleteAll(void);

	void AddParameter(const char_t* pszParameter, const char_t* pszValue);
	
	const char_t* GetValue(const char_t* pszParameter, int32_t nIndex = 0);

	int32_t GetCount(const char_t* pszParameter);
	
private:
	std::multimap<std::string, std::string> m_oParameterMap; ///Parameter map
};

#endif	/* CPARAMETERLIST_H_ */
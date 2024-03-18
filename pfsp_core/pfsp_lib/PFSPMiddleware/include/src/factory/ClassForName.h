#ifndef CLASSFORNAME_H_
#define CLASSFORNAME_H_


#include <string.h>
#include <string>
#include <map>

#include "base/CTypedefs.h"

typedef void* (*FNCLASSCREATIONMETHOD) (void);

class CClassEntry 
{
public:
	CClassEntry(const char_t* szClassName, FNCLASSCREATIONMETHOD pfnClassCreationMethod);
	~CClassEntry(void);

	static void* GetClassForName(const char_t* szServiceName);

private:
    static std::map<std::string, FNCLASSCREATIONMETHOD >* GetClassCreation(void)
    {
		static std::map<std::string, FNCLASSCREATIONMETHOD > m_oClassCreation;
		return &m_oClassCreation;
    }
};

#define DECLARE_CLASS(NAME) \
public: \
	static void* CreateForName(void) { \
		return new (NAME); \
	};

#define IMPLEMENT_CLASS(NAME) \
	static CClassEntry global##NAME( #NAME , &NAME::CreateForName ) ;

#endif	/* CLASSFORNAME_H_ */

#ifndef PARAMETERFORSERVICE_H_
#define PARAMETERFORSERVICE_H_

#include "utility/CParameterList.h"
#include "lib/xmlParser/xmlParser.h"
#include "utility/CParameterList.h"

CParameterList* ParameterForService(XMLNode oMainXmlNode, const char_t* pszServiceName);
void SetServiceIncludePath(const char* pszIncludePath);
const char* GetServiceIncludePath(void);


char_t* findCharacter(char_t* pszSource);
int32_t GetConfigFileSize(const char_t* pszFileName);
int32_t ConfigStreamForService(char_t pszStream[], const char_t* pszServiceName);
void    ParameterForStream(CParameterList* poParameterList, const char_t* pszStream);
void    AddParameterItems(const char_t* pszFileName, CParameterList* poParameterList);

#endif	/* PARAMETERFORSERVICE_H_ */

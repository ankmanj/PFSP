#ifndef RPMAIN_H_
#define RPMAIN_H_

#include <stdlib.h>

#include "base/CTypedefs.h"

#include "interface/IService.h"

#include "port/CMailboxPort.h"
#include "factory/CPortMap.h"
#include "activity/CServiceActivity.h"

#include "factory/ClassForName.h"

#include "lib/xmlParser/xmlParser.h"

enum RP_RETURN_CODE {
	RP_RETURN_NO_ERROR = 0,
	RP_RETURN_CALIBRATION_DONE = 10,
	RP_RETURN_PWR_OFF = 20,
	RP_RETURN_UPGRADE_MODE = 255
};

typedef enum
{
	RPCONTROL = 0,
	NORMAL
}eRunType_t;

typedef struct  _calc_second_t
{
	uint8_t nMark;
	uint32_t nNumber;
	struct _calc_second_t* pNext;
}Calc_Second_t;

#define MAX_RUNSCRIPT_STRLEN	(2048)

class IMainActivityNotify
{
public:
	virtual void PostInitialize() = 0;
	virtual void PostFinalize() = 0;
};
void SetReturnCode(int32_t nReturn);
int32_t GetReturnCode(void);

int32_t AddNodes(XMLNode oMainXmlNode);
extern int32_t MakePortMap(XMLNode oMainXmlNode, const char_t* szCurrentNodeName, eRunType_t eRunType, int32_t number_of_services);
int32_t DeletePortMap();

int32_t rprun(int32_t nServiceCount, char_t** ppszServiceList, CPortMap* poPortMap, IMainActivityNotify* poMainActivityNotify = NULL);
int32_t rpcontrol(char_t* pszCommand, int32_t nServiceCount, char_t** ppszServiceList, CPortMap* poPortMap);

int32_t rpmain(int32_t argc, char_t** argv, const char_t* pszConfig, IMainActivityNotify* poMainActivityNotify = NULL);
int32_t richosmain(int32_t argc, char_t** argv, IMainActivityNotify* poMainActivityNotify = NULL);

int32_t Terminate(int32_t argc, char_t** argv, IMainActivityNotify* poMainActivityNotify = NULL);
int32_t TerminatePfsp(int32_t argc,  char_t** argv, const char_t* pszConfig, IMainActivityNotify* poMainActivityNotify = NULL);
int32_t PfspTermination(int32_t nServiceCount, char_t** ppszServiceList, CPortMap* poPortMap, IMainActivityNotify* poMainActivityNotify = NULL);
#endif //RPMAIN_H_


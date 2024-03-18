#ifndef CDEBUG_H_
#define CDEBUG_H_

#include "osal/Debug.h"

#if 0
class CCallPrint {
public:
	CCallPrint(char* pcFileName) {DEBG_I_Print("%s Function Entered.\n", pcFileName);};
	~CCallPrint(void) {DEBG_I_Print("%s Function Leaved.\n", __FILE__);};
};
#endif

#endif	/* CDEBUG_H_ */
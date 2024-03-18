#ifndef FUNCTOI_H_
#define FUNCTOI_H_

#include "base/CTypedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct  _calc_t
{
	uint8_t nMark;
	uint32_t nNumber;
	struct _calc_t* pNext;
}Calc_t;

int32_t GetFunctoValue(const char_t* pstrFunction);



#ifdef __cplusplus
}
#endif

#endif // FUNCTOI_H_

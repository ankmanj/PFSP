#ifndef RANDOM_H_
#define RANDOM_H_

#include "osal/port/Port_Random.h"
#include "base/CTypedefs.h"

#ifdef __cplusplus
extern "C" {
#endif
	
uint32_t RAND_Get(uint32_t unLimit);

#ifdef __cplusplus
}
#endif

#endif // RANDOM_H_

#ifndef SHARED_SUBSCRIBE_H_
#define SHARED_SUBSCRIBE_H_

#include "base/CTypedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SHARED_TOPIC_NAME	(64)

typedef struct
{
	char_t   szName[MAX_SHARED_TOPIC_NAME];
	uint8_t* punStartAddress;
	uint8_t* punOffset;
	uint32_t unLength;
} SharedTopic_t;

typedef struct
{
	FrameworkMessageBody_t stFrameworkBody;
	SharedTopic_t stSharedTopic;
} SharedSubscribe_t;	

#ifdef __cplusplus
}
#endif

#endif // SHARED_SUBSCRIBE_H_

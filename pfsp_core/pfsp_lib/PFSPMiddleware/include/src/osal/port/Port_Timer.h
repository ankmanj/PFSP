#ifndef PORT_TIMER_H_
#define PORT_TIMER_H_

#include "osal/port/Port_Task.h"

#if defined(WIN32) || defined(LINUX)

typedef TASK_RETURN (*TIMER_CALLBACK_TYPE)(TASK_ARGS);

#elif defined(DSP_BIOS5) || defined(SYS_BIOS6)

typedef TASK_RETURN (*TIMER_CALLBACK_TYPE)(TASK_ARGS);

#endif

#endif /* PORT_TIMER_H_ */

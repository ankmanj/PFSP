#ifndef ILOGGABLE_H_
#define ILOGGABLE_H_

#include "interface/ISerializable.h"

struct ILoggable : public ISerializable
{
	virtual const char_t* GetLogMsgName(void) = 0;
};

#endif	/* ILOGGABLE_H_ */
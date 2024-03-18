#ifndef ISERIALIZABLE_H_
#define ISERIALIZABLE_H_

#include "base/CTypedefs.h"

struct ISerializable {
	virtual char_t* Serialize(int32_t* nLength) = 0;
	virtual void Unserialize(char_t* pszSerializedStream, int32_t nLength) = 0;
};

#endif	/* ISERIALIZABLE_H_ */
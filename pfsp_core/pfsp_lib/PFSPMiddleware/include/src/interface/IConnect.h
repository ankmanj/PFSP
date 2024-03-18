#ifndef ICONNECT_H_
#define ICONNECT_H_

#include "base/CTypedefs.h"

#include "interface/IPort.h"

#define DECLARE_CONNECT_CLASS DECLARE_CLASS
#define IMPLEMENT_CONNECT_CLASS IMPLEMENT_CLASS

#define CONNECT_FOR_NAME(pszConnectName) reinterpret_cast<IConnect*>(CClassEntry::GetClassForName(pszConnectName))

class IConnect : public IPort {
public:
	IConnect(void) : IPort() {}
	virtual ~IConnect(void) {}

	virtual int32_t Connect(void) = 0;
	virtual int32_t Disconnect(void) = 0;

	virtual int32_t Reconnect(void) = 0;
};

#endif	/* ICONNECT_H_ */

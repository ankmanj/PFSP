#ifndef CNEW_H_
#define CNEW_H_

#include "base/CTypedefs.h"

#if defined(WIN32) && defined(_DEBUG)

#ifndef _AFXDLL
#define _AFXDLL
#endif
#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#endif

#include <stdlib.h>
#include <crtdbg.h>
#include <afx.h>

#include <new>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DEBUG_NEW
#endif
#endif  // _DEBUG

#else
#include <stddef.h>

#if defined(MEMORY_NAMESPACE)

class pfsp {
public:
	static void* operator new(size_t bytes);
	static void operator delete(void* ptr);

	static void* operator new(size_t size, void *location, int k=0);
};

#else

void* operator new(size_t bytes);
void operator delete(void* ptr);

void* operator new(size_t size, void *location, int32_t k=0);

#endif

#endif

#endif	/* CNEW_H_ */
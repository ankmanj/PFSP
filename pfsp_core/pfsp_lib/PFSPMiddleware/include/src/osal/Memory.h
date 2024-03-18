#ifndef MEMORY_ALLOC_H_
#define MEMORY_ALLOC_H_

#include "base/CTypedefs.h"
#include "osal/port/Port_Memory.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _MEMORY_DEBUG

#if defined(WIN32) && defined(_DEBUG)

	#include <stdlib.h>
	#include <crtdbg.h>

	#define MMRY_Alloc(s) 		_malloc_dbg(s, _NORMAL_BLOCK,__FILE__,__LINE__)
	#define MMRY_Realloc(p, s)  _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
	#define MMRY_Calloc(c, s) 	_calloc_dbg(c, s, _NORMAL_BLOCK,__FILE__,__LINE__)
	#define MMRY_Free(p)  		_free_dbg(p, _NORMAL_BLOCK)


	#define MMRY_Aligned_Alloc(a, b)		_aligned_malloc_dbg(a, b, __FILE__, __LINE__)
	#define MMRY_Aligned_Free(a)			_aligned_free_dbg(a)

	#define CHECK_MEMORY_LEAK _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF);

	#define MMRY_Contiguous_Alloc(a, b)		_MMRY_Contiguous_Alloc(a, b, __FILE__, __LINE__)
	#define MMRY_Contiguous_Free(a)			_MMRY_Contiguous_Free(a, __FILE__, __LINE__)

	MemoryObject_t _MMRY_Contiguous_Alloc(const char* pcAllocName, unsigned int unSize, const char* pcFileName, const int nLineNumber);	
	void _MMRY_Contiguous_Free(MemoryObject_t stMemoryObject, const char* pcFileName, const int nLineNumber);

	void MMRY_Contiguous_Invalidate(MemoryObject_t stMemoryObject);
	//#endif //_DEBUG

#else

	#define MMRY_Alloc(a) 		_MMRY_Alloc(a, 0, __FILE__, __LINE__)
	#define MMRY_Realloc(a, b)  _MMRY_Realloc(a, b, __FILE__, __LINE__)
	#define MMRY_Calloc(a, b) 	_MMRY_Alloc(a*b, 0, __FILE__, __LINE__)
	#define MMRY_Free(a) 		_MMRY_Free(a, __FILE__, __LINE__)

	#define MMRY_Aligned_Alloc(a, b)		_MMRY_Aligned_Alloc(a, 0, b, __FILE__, __LINE__)
	#define MMRY_Aligned_Calloc(a, b, c)	_MMRY_Aligned_Alloc(a*b, 0, c, __FILE__, __LINE__)
	#define MMRY_Aligned_Free(a)			_MMRY_Aligned_Free(a, __FILE__, __LINE__)

	#define MMRY_Contiguous_Alloc(a, b)		_MMRY_Contiguous_Alloc(a, b, __FILE__, __LINE__)
	#define MMRY_Contiguous_Free(a)			_MMRY_Contiguous_Free(a, __FILE__, __LINE__)

	void* _MMRY_Alloc(uint32_t unSize, uint8_t cData, const char_t* pcFileName, const int32_t nLine);
	void* _MMRY_Realloc(void* pvSrcAddress, uint32_t unSize, const char_t* pcFileName, const int32_t nLine);
	void _MMRY_Free(void* pvAddr, const char_t* pcFileName, const int32_t nLine);

	void* _MMRY_Aligned_Alloc(uint32_t unSize, uint8_t cData, uint32_t unAlign, const char_t* pcFileName, const int32_t nLine);
	void _MMRY_Aligned_Free(void* pvAddr, const char_t* pcFileName, const int32_t nLine);

	MemoryObject_t _MMRY_Contiguous_Alloc(const char_t* pcAllocName, uint32_t unSize, const char_t* pcFileName, const int32_t nLine);
	void _MMRY_Contiguous_Free(MemoryObject_t stMemoryObject, const char_t* pcFileName, const int32_t nLine);

	void MMRY_Contiguous_Invalidate(MemoryObject_t stMemoryObject);

	void PrintMemoryNode(int32_t nMode, const char_t* pDebugText);

#endif 

#ifdef __cplusplus
}
#endif

#endif // MEMORY_ALLOC_H_


#ifndef PORT_MEMORY_ALLOC_H_
#define PORT_MEMORY_ALLOC_H_

#ifdef __cplusplus
extern "C" {
#endif

	void* Port_MMRY_Alloc(uint32_t unSize);
	void Port_MMRY_Free(void* pvAddr);

	void* Port_MMRY_Aligned_Alloc(uint32_t unSize, uint32_t unAlign);
	void Port_MMRY_Aligned_Free(void* pvAddr);

	MemoryObject_t Port_MMRY_Contiguous_Alloc(char *pszName, uint32_t unSize);
	void Port_MMRY_Contiguous_Free(MemoryObject_t stMemoryObject);

	// Added by jeihun 2014.2.3
	void Port_MMRY_Contiguous_Invalidate(MemoryObject_t stMemoryObject);

#ifdef __cplusplus
}
#endif

#endif // PORT_MEMORY_ALLOC_H_

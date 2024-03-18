#ifndef PORT_FILE_H_
#define PORT_FILE_H_

#include "base/CTypedefs.h"

#if defined(PTHREAD)
	#include <unistd.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <fcntl.h>
	#include <dirent.h>
	#define MAX_FILEFIND_PATH_LENGTH (260)
	
	typedef int FILE_T;
	typedef struct dirent* FILEFINDHANDLE_T;
	typedef struct
	{
		DIR* pDirEntry;
		char szFindPath[MAX_FILEFIND_PATH_LENGTH];
	} FILEFIND_T;

#elif defined(WINCE) || defined(WIN32)

	#include <windows.h>

	typedef HANDLE FILE_T;
	typedef HANDLE FILEFIND_T;
	typedef WIN32_FIND_DATAA FILEFINDHANDLE_T;

#elif defined(DSP_BIOS5)

	typedef void* FILE_T;
	typedef void* FILEFIND_T;
	typedef void* FILEFINDHANDLE_T;

#elif defined(SYS_BIOS6)
#define USE_MMCSD
#ifdef USE_MMCSD
	typedef int FILE_T;
#else
	typedef void* FILE_T;
#endif
	typedef void* FILEFIND_T;
	typedef void* FILEFINDHANDLE_T;

	void add_file_instance(const char* pszFileName, const char* pcStartAddress, unsigned long ulOption);

#define ADD_FILE_INSTANCE(a, b, c) \
	static __attribute__((constructor, used)) void _add_file_instance(void) \
	{ \
		add_file_instance(a, b, c); \
	}

#elif defined(STM32F2XX)

	typedef void* FILE_T;
	typedef void* FILEFIND_T;
	typedef void* FILEFINDHANDLE_T;

#endif

#ifdef __cplusplus
extern "C" {
#endif

	FILE_T     Port_FILE_Open(const char_t* pcFileName, int32_t nOption);

	int32_t    Port_FILE_Read(FILE_T FileId, char_t* pvData, uint32_t unSize);
	int32_t    Port_FILE_Write(FILE_T FileId, const char_t* pvData, uint32_t unSize);

	int32_t    Port_FILE_Seek(FILE_T FileId, uint32_t nOffset, int32_t nOrigin);
	int32_t	   Port_FILE_Seek64(FILE_T FileId, uint64_t nOffset, int32_t nOrigin);
	int32_t    Port_FILE_Tell(FILE_T FileId);

	int32_t    Port_FILE_Flush(FILE_T FileId);

	int32_t    Port_FILE_Close(FILE_T FileId);

	int32_t    Port_FILE_Control_Copy(const char_t* pcExistingFilePath, const char_t* pcNewFilePath);
	int32_t    Port_FILE_Control_Remove(const char_t* pcFilePath);
	uint32_t   Port_FILE_Control_GetAttributes(const char_t* pcPath);
	int32_t    Port_FILE_Control_MakeDirectory(const char_t* pcDirectoryPath);
	int32_t    Port_FILE_Control_RemoveDirectory(const char_t* pcDirectoryPath);


typedef struct
{
	FILEFINDHANDLE_T oFindHandle;
		char_t* pszFoundName;
		uint32_t unFoundAttributes;
		uint32_t unFoundSize;
} FILE_FIND_INFO_T;

	int32_t Port_FILE_Find_Start(FILEFIND_T* pFindHandle, const char_t* pcDirectoryPath, FILE_FIND_INFO_T* pFindResult);
	int32_t Port_FILE_Find_Next(FILEFIND_T* pFindHandle, FILE_FIND_INFO_T* pFindResult);
	int32_t Port_FILE_Find_End(FILEFIND_T* pFindHandle);
#ifdef __cplusplus
}
#endif

#endif // PORT_FILE_H_


#ifndef OSAL_FILE_H_
#define OSAL_FILE_H_

#include "osal/port/Port_File.h"
#include "base/CTypedefs.h"

#define FILE_OPTION_ACCESS_READ			0x0001
#define FILE_OPTION_ACCESS_WRITE		0x0002
#define FILE_OPTION_ACCESS_RDWR			0x0003
#define FILE_OPTION_OPEN_ALWAYS			0x0010
#define FILE_OPTION_CREATE_NEW			0x0020	// if the specific file exist, the function fails
#define FILE_OPTION_CREATE_ALWAYS		0x0030	// if the specific file exist, the function overwrites the file...

#define FILE_OPTION_SEEK_BEGIN			0x0000
#define FILE_OPTION_SEEK_CURRENT		0x0001
#define FILE_OPTION_SEEK_END			0x0002

#define FILE_OPTION_ATTRIBUTE_DIRECTORY	0x10000000
#define FILE_OPTION_ATTRIBUTE_FILE		0x01000000

#define FILE_FAILURE		((FILE_T)-1)

#define FILE_CONTROL_DIRECTORY_IS_ALREADY_EXIST (1)

#ifdef __cplusplus
extern "C" {
#endif

FILE_T FILE_Open(const char_t* pcFileName, int32_t nOption);

int32_t    FILE_Read(FILE_T FileId, char_t* pvData, uint32_t unSize);
int32_t    FILE_Write(FILE_T FileId, const char_t* pvData, uint32_t unSize);

int32_t    FILE_Seek(FILE_T FileId, uint32_t nOffset, int32_t nOrigin);
int32_t    FILE_Seek64(FILE_T FileId, uint64_t nOffset, int32_t nOrigin);
int32_t    FILE_Tell(FILE_T FileId);

int32_t    FILE_Flush(FILE_T FileId);

int32_t    FILE_Close(FILE_T FileId);

int32_t    FILE_Control_Copy(const char_t* pcExistingFilePath, const char_t* pcNewFilePath); // under 0 : can't copy file
int32_t    FILE_Control_Remove(const char_t* pcFilePath); // under 0 : can't remove file
uint32_t FILE_Control_GetAttributes(const char_t* pcPath); // 0 : file is not exist

int32_t    FILE_Control_MakeDirectory(const char_t* pcDirectoryPath);
int32_t    FILE_Control_RemoveDirectory(const char_t* pcDirectoryPath);

int32_t FILE_Find_Start(FILEFIND_T* pFindHandle, const char_t* pcDirectoryPath, FILE_FIND_INFO_T* pFindResult);
int32_t FILE_Find_Next(FILEFIND_T* pFindHandle, FILE_FIND_INFO_T* pFindResult);
int32_t FILE_Find_End(FILEFIND_T* pFindHandle);

#ifdef __cplusplus
}
#endif

#endif // OSAL_FILE_H_


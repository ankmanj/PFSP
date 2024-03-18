#ifndef CDEBUGTYPE_H_
#define CDEBUGTYPE_H_

#include "osac/CTimeStamp.h"

namespace LOGGER {

#define DEBUG_BUFFER_NAME	("_buffer")
#define DEBUG_COLLECTOR_NAME ("/collector")
#define DEBUG_LOGGER_NAME	("/logger")
#define DEBUG_PLAYER_NAME	("/player")

#define DEBUG_MAXSIZE_STRING (128)
#define DEBUG_MAXSIZE_DEBG_STRING (512)
#define DEBUG_SIZE_LOG_MSG_NAME	(32)
#define DEBUG_SIZE_LOG_SERVICE_NAME (32)
#define DEBUG_LOG_MSG_START "LOGMSGSTART"
#define DEBUG_LOG_MSG_START_ONLYHEADER "LOGMSGSTART_ONLYHEADER"
#define DEBUG_LOG_MSG_END "LOGMSGEND"
#define DEBUG_LOG_MSG_PRINT "DBGPRNT"
#define DEBUG_LOG_FILE_PREFIX "RPLG"
#define DEBUG_LOG_FILE_PREFIX_LENGTH (4)
#define DEBUG_LOG_FILE_HEADER "RPLGFILE"
#define DEBUG_LOG_FILE_HEADER_LENGTH (8)

#define DEBUG_DEFAULT_BUFFERSIZE (917504)

struct DebugLogHeader_t
{
	OSAC::CTimeStamp stTime;
	char_t pszLogServiceName[DEBUG_SIZE_LOG_SERVICE_NAME]; 
	char_t pszLogMsgName[DEBUG_SIZE_LOG_MSG_NAME];
	uint32_t nDataSize; 
};

}	

#endif	/* CDEBUGTYPE_H_ */
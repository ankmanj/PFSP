#ifndef INCLUDE_PFSP_HOSTAPP_COMMON__H_
#define INCLUDE_PFSP_HOSTAPP_COMMON__H_

#include <stdlib.h>
#include <string.h>
#include <string>

typedef enum _FunctionResult
{
    eResultFailure = 0U,
    eResultSuccess = 1U
}FunctionResult;

enum class SWComponentErrorCategoryE : unsigned int
{
    eSWComponentErrorCalibrationFailure = 0U,
    eSWComponentErrorInvalidInput = 1U,
    eSWComponentErrorInvalidOutput = 2U,
    eSWComponentErrorTimeOutInput = 3U,
    eSWComponentErrorTimeOutOutput = 4U,
    eSWComponentErrorMemoryReadWriteFailure = 5U,
    eSWComponentErrorNullPointer = 6U,
    eSWComponentErrorInvalidDataValue = 7U,
    eSWComponentErrorInvalidDataSize = 8U,
    eSWComponentErrorIndexOutOfBounds = 9U,
    eSWComponentErrorFileReadWriteOpenFailure = 10U,
    eSWComponentErrorUnknown = 11U,
    eSWComponentErrorUserDefined = 50U,
    eSWComponentErrorNone = 100U
};

enum class SWComponentStateE : unsigned int
{
    eSWComponentStateCreation = 0U,
    eSWComponentStateTriggerInit = 1U,
    eSWComponentStateInitialized = 2U,
    eSWComponentStateRun = 3U,
    eSWComponentStateStop = 4U,
    eSWComponentStateStopDone = 5U,
    eSWComponentStateTriggerShutdown = 6U,
    eSWComponentStateReadyToShutdown = 7U,
    eSWComponentStateShutdownDone = 8U,
    eSWComponentStateCurrent = 18U,
    eSWComponentStateNone = 19U,
    eSWComponentStateMax = 20U
};

enum class SWComponentOperationModeE : unsigned int
{
    eSWComponentOperationModeNormal = 0U,
    eSWComponentOperationModeDegradation = 1U,
    eSWComponentOperationModeDiagnostics = 2U,
    eSWComponentOperationModeEOL = 3U,
    eSWComponentOperationModeNone = 4U
};

typedef struct HostComponentStatus
{
    SWComponentStateE state;
    SWComponentOperationModeE operation_mode;
    SWComponentErrorCategoryE error_type;
    std::string error_name;
}HostComponentStatusT;

#endif /* INCLUDE_PFSP_HOSTAPP_COMMON__H_ */

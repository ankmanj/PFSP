#ifndef INCLUDE_PFSP_ENUM_H_
#define INCLUDE_PFSP_ENUM_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <time.h>
#include <string>
#include <unistd.h>
#include <memory>
#include <math.h>
#include <fstream>
#include <assert.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cstddef>
#include "pfsp_common.h"
#include "pfsp_hostapp_common.h"
//#include <array>

enum class FailureDebouncingCriteriaE : unsigned int
{
    eTime = 0U,
    eCount = 1U
};

typedef enum _ToggleSwitchE
{
    eOn = 0U,
    eOff = 1U
}ToggleSwitchE;

typedef enum _ServiceOffset
{
	eBaseServiceOffset						= 0x0000,
	ePluginErrorHandlerComponentOffset		= 0x0100,
	ePluginModeManagerComponentOffset		= 0x0200,
	eSmartConverterPluginComponentOffset	= 0x0300,
	eHostApplicationComponentOffset0		= 0x0400,
	eHostApplicationComponentOffset1		= 0x0500,
	eHostApplicationComponentOffset2		= 0x0600,
	eHostApplicationComponentOffset3		= 0x0700,
	eHostApplicationComponentOffset4		= 0x0800,
	eHostApplicationComponentOffset5		= 0x0900,
	eHostApplicationComponentOffset6		= 0x1000,
	eHostApplicationComponentOffset7		= 0x1100,
	eHostApplicationComponentOffset8		= 0x1200,
	eHostApplicationComponentOffset9		= 0x1300,
	eHostApplicationComponentOffset10		= 0x1400
}ServiceOffsetE;

typedef enum _PlayTypeE
{
    eOnce = 0,
    eRepeat
}PlayTypeE;

enum class ComponentPubSubStatusE : unsigned int
{
    ePubSubNone = 0U,
    eReceivedExternalInput = 1U,
    eComponentRun = 2U,
    ePublishedExternalOutput = 3U
};

enum class NextStateE : unsigned int
{
	eNone = 0U,
	eKeepCurrentState = 1U,
	eChangeCurrentState = 2U
};

//    DataPublicationStateE publication_state;
enum class DataPublicationStateE : unsigned int
{
	eNone = 0U,
	ePublishedData = 1U,
	eNotPublishedData = 2U
};

#define DEFAULT_NUMBER_OF_COMPONENTS (2U)
#define MAX_NUMBER_OF_ERRORS (100U)
#define MAX_NUMBER_OF_COMPONENT_MODES (10)
#define MAX_NUMBER_OF_COMPONENTS (50)
#define BASE_RUNNER_PRIORITY    (3U)
#endif /* INCLUDE_PFSP_ENUM_H_ */

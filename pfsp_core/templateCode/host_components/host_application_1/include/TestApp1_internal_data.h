#pragma once

#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include "../../../pfsp_include/pfsp_common.h"

// Internals of TestApp1
typedef struct InternalObject
{
	int64_t time;
	int64_t type;
	int64_t fused_obj_loc;

    int64_t risk_level; // There was no corresponding external input for the internal parameter.
} InternalObject;

typedef struct RoadInfo
{
	int64_t time;
	int64_t lane_num;
	int64_t lane_type;

	int64_t safe_time;    // There was no corresponding external input for the internal parameter.

} RoadInfo;

// There was no corresponding external input for the internal parameter.
typedef struct InternalSomething
{
	std::string some;
} InternalSomething;

class HostComponent0InternalDataT
{
public:
    HostComponent0InternalDataT() = default;
    virtual ~HostComponent0InternalDataT() = default;

    InternalObject internalobject;
	RoadInfo roadinfo;
	InternalSomething internalsomething;
};

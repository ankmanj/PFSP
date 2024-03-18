#pragma once

#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include "../../../pfsp_include/pfsp_common.h"

// Internals of TestApp2
typedef struct InternalObject
{
	int64_t time;
	int64_t type;
	int64_t fused_obj_loc;

    int64_t risk_level; // There was no corresponding external input for the internal parameter.
} InternalObject;

class HostComponent1InternalDataT
{
public:
    HostComponent1InternalDataT() = default;
    virtual ~HostComponent1InternalDataT() = default;

    InternalObject internalobject;
};

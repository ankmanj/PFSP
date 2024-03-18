#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>


// Key: Those all data structure should be generated from UI of smart converter.

typedef struct Location
{
	int64_t x;
	int64_t y;
} Location;

typedef struct Location2D
{
	int64_t x;
	int64_t y;
} Location2D;


// External Input
// External Input
// External Input
typedef struct OFObject
{
	int64_t stamp;
	int64_t obj_type;
	int64_t vehicle_loc;
} OFObject;

typedef struct FusedRoad
{
	int64_t stamp;
	int64_t detected_lane_num;
	int64_t lane_color;
} FusedRoad;

// Internal Input
// Internal Input
// Internal Input
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

// External Output
// External Output
// External Output
typedef struct PriorityObject
{
	int64_t time;
	int64_t type;
	int64_t priority;
    std::string name;
} PriorityObject;

typedef struct DrivablePath
{
	int64_t time;
	int64_t lane_num;
	int64_t driving_window;
} DrivablePath;


// ===================================== HostComponent0 ======================================= //

class HostComponent0ExternalInputT
{
public:
    HostComponent0ExternalInputT() = default;
    virtual ~HostComponent0ExternalInputT() = default;

	OFObject ofobject;
	FusedRoad fusedroad;
};

class HostComponent0InternalDataT
{
public:
    HostComponent0InternalDataT() = default;
    virtual ~HostComponent0InternalDataT() = default;

    InternalObject internalobject;
	RoadInfo roadinfo;
	InternalSomething internalsomething;
};

class HostComponent0ExternalOutputT
{
public:
    HostComponent0ExternalOutputT() = default;
    virtual ~HostComponent0ExternalOutputT() = default;

    PriorityObject priorityobject {};
	DrivablePath drivablepath {};
};

// ===================================== HostComponent1 ======================================= //
class HostComponent1ExternalInputT
{
public:
    HostComponent1ExternalInputT() = default;
    virtual ~HostComponent1ExternalInputT() = default;

    PriorityObject priorityobject {};
	DrivablePath drivablepath {};
};

class HostComponent1InternalDataT
{
public:
    HostComponent1InternalDataT() = default;
    virtual ~HostComponent1InternalDataT() = default;

    InternalObject internalobject;
};

class HostComponent1ExternalOutputT
{
public:
    HostComponent1ExternalOutputT() = default;
    virtual ~HostComponent1ExternalOutputT() = default;

	OFObject ofobject;
	FusedRoad fusedroad;
};

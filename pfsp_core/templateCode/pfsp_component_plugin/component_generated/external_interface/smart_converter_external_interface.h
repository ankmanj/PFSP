#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>

//ExternalOutputs of TestApp1
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

//ExternalOutputs of TestApp2
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

// ============ HostComponent0 for TestApp1 ============== //
class HostComponent0ExternalInputT
{
public:
    HostComponent0ExternalInputT() = default;
    virtual ~HostComponent0ExternalInputT() = default;

	OFObject ofobject;
	FusedRoad fusedroad;
};

class HostComponent0ExternalOutputT
{
public:
    HostComponent0ExternalOutputT() = default;
    virtual ~HostComponent0ExternalOutputT() = default;

    PriorityObject priorityobject {};
	DrivablePath drivablepath {};
};

// ============ HostComponent1 for TestApp2 ============== //
class HostComponent1ExternalInputT
{
public:
    HostComponent1ExternalInputT() = default;
    virtual ~HostComponent1ExternalInputT() = default;

    PriorityObject priorityobject {};
	DrivablePath drivablepath {};
};

class HostComponent1ExternalOutputT
{
public:
    HostComponent1ExternalOutputT() = default;
    virtual ~HostComponent1ExternalOutputT() = default;

	OFObject ofobject;
	FusedRoad fusedroad;
};

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>

//ExternalOutputs of component2
typedef struct ExternalOutputFromCService2 {
    int8_t type_varname;
} ExternalOutputFromCService2;

//ExternalOutputs of component3
typedef struct ExternalOutputFromCService3 {
    int8_t type_varname;
} ExternalOutputFromCService3;

//ExternalOutputs of component1
typedef struct ExternalOutputFromCService1 {
    int8_t type_varname;
} ExternalOutputFromCService1;

// ============ HostComponent1 for component2 ============== //
class HostComponent1ExternalInputT {
public:
    HostComponent1ExternalInputT() = default;
    virtual ~HostComponent1ExternalInputT() = default;

};

class HostComponent1ExternalOutputT {
public:
    HostComponent1ExternalOutputT() = default;
    virtual ~HostComponent1ExternalOutputT() = default;

    ExternalOutputFromCService2 externaloutputfromcservice2;
};

// ============ HostComponent2 for component3 ============== //
class HostComponent2ExternalInputT {
public:
    HostComponent2ExternalInputT() = default;
    virtual ~HostComponent2ExternalInputT() = default;

};

class HostComponent2ExternalOutputT {
public:
    HostComponent2ExternalOutputT() = default;
    virtual ~HostComponent2ExternalOutputT() = default;

    ExternalOutputFromCService3 externaloutputfromcservice3;
};

// ============ HostComponent0 for component1 ============== //
class HostComponent0ExternalInputT {
public:
    HostComponent0ExternalInputT() = default;
    virtual ~HostComponent0ExternalInputT() = default;

};

class HostComponent0ExternalOutputT {
public:
    HostComponent0ExternalOutputT() = default;
    virtual ~HostComponent0ExternalOutputT() = default;

    ExternalOutputFromCService1 externaloutputfromcservice1;
};


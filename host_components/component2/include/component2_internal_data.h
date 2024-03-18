#pragma once

#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include "../../../pfsp_core/pfsp_include/pfsp_common.h"

// Internals of component2
typedef struct CService2InteralData {
    int8_t type_varname;
} CService2InteralData;

class HostComponent1InternalDataT {
public:
    HostComponent1InternalDataT() = default;
    virtual ~HostComponent1InternalDataT() = default;

    CService2InteralData cservice2interaldata;
};


#pragma once

#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include "../../../pfsp_core/pfsp_include/pfsp_common.h"

// Internals of component1
typedef struct CService1InteralData {
    int8_t type_varname;
} CService1InteralData;

class HostComponent0InternalDataT {
public:
    HostComponent0InternalDataT() = default;
    virtual ~HostComponent0InternalDataT() = default;

    CService1InteralData cservice1interaldata;
};


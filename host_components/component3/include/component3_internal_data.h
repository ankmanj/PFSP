#pragma once

#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include "../../../pfsp_core/pfsp_include/pfsp_common.h"

// Internals of component3
typedef struct CService3InteralData {
    int8_t type_varname;
} CService3InteralData;

class HostComponent2InternalDataT {
public:
    HostComponent2InternalDataT() = default;
    virtual ~HostComponent2InternalDataT() = default;

    CService3InteralData cservice3interaldata;
};


#pragma once

#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include "../../../pfsp_core/pfsp_include/pfsp_common.h"

// Internals of FirstComponent
typedef struct FirstClassInternalData {
    uint32_t expect_10;
    uint32_t expect_100;
} FirstClassInternalData;

class HostComponent0InternalDataT {
public:
    HostComponent0InternalDataT() = default;
    virtual ~HostComponent0InternalDataT() = default;

    FirstClassInternalData firstclassinternaldata;
};


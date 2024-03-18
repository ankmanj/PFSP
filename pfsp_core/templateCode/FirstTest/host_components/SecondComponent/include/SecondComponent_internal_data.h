#pragma once

#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include "../../../pfsp_core/pfsp_include/pfsp_common.h"

// Internals of SecondComponent
typedef struct SecondClassInternalData {
    uint32_t expect_20;
    uint32_t expect_200;
} SecondClassInternalData;

class HostComponent1InternalDataT {
public:
    HostComponent1InternalDataT() = default;
    virtual ~HostComponent1InternalDataT() = default;

    SecondClassInternalData secondclassinternaldata;
};


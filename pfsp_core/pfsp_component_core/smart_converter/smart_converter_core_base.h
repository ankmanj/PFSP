#pragma once

#include "smart_converter_interface_memmanager.h"

template<typename TExternalInput, typename TComponentInternal, typename TExternalOutput>
class SmartConverterCoreBase: public SmartConverterCoreMemoryManager<TExternalInput, TComponentInternal, TExternalOutput>
{
public:
    SmartConverterCoreBase(void) {}
    virtual ~SmartConverterCoreBase(void) {}

    virtual void PreRun(void) = 0;
    virtual void PostRun(void) = 0;

protected:

private:

};


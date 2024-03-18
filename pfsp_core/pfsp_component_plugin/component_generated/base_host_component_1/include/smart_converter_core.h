#pragma once

#include "component2_internal_data.h"
#include "../../external_interface/smart_converter_external_interface.h"
#include "smart_converter_core_base.h"

class SmartConverterCore1:
    public SmartConverterCoreBase<HostComponent1ExternalInputT, HostComponent1InternalDataT, HostComponent1ExternalOutputT>
{
public:
    SmartConverterCore1(std::string component_name)
    {
        component_name_ = component_name;
    }
    virtual ~SmartConverterCore1(void) = default;

    void PreRun(void)
    {
    }

    void PostRun(void)
    {
        external_output = dynamic_cast<HostComponent1ExternalOutputT*>(this->GetExternalOutputMemory(component_name_));
        internal_data = dynamic_cast<HostComponent1InternalDataT*>(this->GetInternalDataMemory(component_name_));

        external_output->externaloutputfromcservice2.type_varname = internal_data->cservice2interaldata.type_varname;

    }

private:
    std::string component_name_ {};

    HostComponent1ExternalInputT* external_input {};
    HostComponent1InternalDataT* internal_data {};
    HostComponent1ExternalOutputT* external_output {};
};

#pragma once

#include "component1_internal_data.h"
#include "../../external_interface/smart_converter_external_interface.h"
#include "smart_converter_core_base.h"

class SmartConverterCore0:
    public SmartConverterCoreBase<HostComponent0ExternalInputT, HostComponent0InternalDataT, HostComponent0ExternalOutputT>
{
public:
    SmartConverterCore0(std::string component_name)
    {
        component_name_ = component_name;
    }
    virtual ~SmartConverterCore0(void) = default;

    void PreRun(void)
    {
    }

    void PostRun(void)
    {
        external_output = dynamic_cast<HostComponent0ExternalOutputT*>(this->GetExternalOutputMemory(component_name_));
        internal_data = dynamic_cast<HostComponent0InternalDataT*>(this->GetInternalDataMemory(component_name_));

        external_output->externaloutputfromcservice1.type_varname = internal_data->cservice1interaldata.type_varname;

    }

private:
    std::string component_name_ {};

    HostComponent0ExternalInputT* external_input {};
    HostComponent0InternalDataT* internal_data {};
    HostComponent0ExternalOutputT* external_output {};
};

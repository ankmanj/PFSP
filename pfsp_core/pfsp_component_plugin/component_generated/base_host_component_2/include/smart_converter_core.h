#pragma once

#include "component3_internal_data.h"
#include "../../external_interface/smart_converter_external_interface.h"
#include "smart_converter_core_base.h"

class SmartConverterCore2:
    public SmartConverterCoreBase<HostComponent2ExternalInputT, HostComponent2InternalDataT, HostComponent2ExternalOutputT>
{
public:
    SmartConverterCore2(std::string component_name)
    {
        component_name_ = component_name;
    }
    virtual ~SmartConverterCore2(void) = default;

    void PreRun(void)
    {
    }

    void PostRun(void)
    {
        external_output = dynamic_cast<HostComponent2ExternalOutputT*>(this->GetExternalOutputMemory(component_name_));
        internal_data = dynamic_cast<HostComponent2InternalDataT*>(this->GetInternalDataMemory(component_name_));

        external_output->externaloutputfromcservice3.type_varname = internal_data->cservice3interaldata.type_varname;

    }

private:
    std::string component_name_ {};

    HostComponent2ExternalInputT* external_input {};
    HostComponent2InternalDataT* internal_data {};
    HostComponent2ExternalOutputT* external_output {};
};
